/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <sid_api.h>
#include <sid_hal_reset_ifc.h>
#include <pal_init.h>
#include <app_ble_config.h>
#include <app_subGHz_config.h>
#include <dk_buttons_and_leds.h>
#include <zephyr/kernel.h>
#include <zephyr/smf.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app, CONFIG_LOG_DEFAULT_LEVEL);

static struct k_thread app_thread;
K_THREAD_STACK_DEFINE(app_thread_stack, CONFIG_SIDEWALK_THREAD_STACK_SIZE);

typedef enum app_events {
	APP_EVENT_SIDEWALK,
	APP_EVENT_ERROR,
	APP_EVENT_READY,
	APP_EVENT_NOT_READY,
	APP_EVENT_SEND,
	APP_EVENT_FACTORY_RESET,
	APP_EVENT_LINK_SWITCH
} app_event_t;

enum app_state {
	STATE_RUNNING,
	STATE_INIT,
	STATE_SIDEWALK_NOT_READY,
	STATE_SIDEWALK_CONNECTING,
	STATE_SIDEWALK_READY,
};

struct app_sm {
	struct smf_ctx ctx;
	struct k_msgq msgq;
	app_event_t event;
};

struct sid_ctx_s {
	struct app_sm sm;
	struct sid_handle *handle;
	struct sid_config config;
};

struct app_msg {
	struct sid_msg msg;
	struct sid_msg_desc desc;
};

static int app_event_send(app_event_t event);

static void state_running_run(void *o);
static void state_init_entry(void *o);
static void state_init_run(void *o);
static void state_ready_run(void *o);
static void state_not_ready_run(void *o);
static void state_connecting_entry(void *o);
static void state_connecting_run(void *o);

static const struct smf_state app_states[] = {
	[STATE_RUNNING] = SMF_CREATE_STATE(NULL, state_running_run, NULL, NULL),
	[STATE_INIT] = SMF_CREATE_STATE(state_init_entry, state_init_run, NULL,
					&app_states[STATE_RUNNING]),
	[STATE_SIDEWALK_READY] =
		SMF_CREATE_STATE(NULL, state_ready_run, NULL, &app_states[STATE_RUNNING]),
	[STATE_SIDEWALK_NOT_READY] =
		SMF_CREATE_STATE(NULL, state_not_ready_run, NULL, &app_states[STATE_RUNNING]),
	[STATE_SIDEWALK_CONNECTING] = SMF_CREATE_STATE(state_connecting_entry, state_connecting_run,
						       NULL, &app_states[STATE_RUNNING]),
};

static uint8_t echo_payload[255];
struct app_msg app_send_buffer;

uint8_t __aligned(4) msgq_buffer[CONFIG_SIDEWALK_THREAD_QUEUE_SIZE * sizeof(app_event_t)];

static struct sid_ctx_s sid_ctx;

static void on_sidewalk_event(bool in_isr, void *context)
{
	LOG_INF("%s", __func__);

	app_event_send(APP_EVENT_SIDEWALK);
}

static void on_sidewalk_msg_received(const struct sid_msg_desc *msg_desc, const struct sid_msg *msg,
				     void *context)
{
	LOG_INF("%s", __func__);

	LOG_DBG("received message(type: %d, link_mode: %d, id: %u size %u)", (int)msg_desc->type,
		(int)msg_desc->link_mode, msg_desc->id, msg->size);
	LOG_HEXDUMP_INF((uint8_t *)msg->data, msg->size, "Message data: ");

	memcpy(echo_payload, msg->data, msg->size);
	app_send_buffer = (struct app_msg) {
			.msg = {
				.data = echo_payload,
				.size = msg->size,
			},
			.desc = {
				.type = SID_MSG_TYPE_NOTIFY,
				.link_type = SID_LINK_TYPE_ANY,
				.link_mode = SID_LINK_MODE_CLOUD,
			},
		};
	app_event_send(APP_EVENT_SEND);
}

static void on_sidewalk_msg_sent(const struct sid_msg_desc *msg_desc, void *context)
{
	LOG_INF("%s", __func__);
	LOG_INF("sent message(type: %d, id: %u)", (int)msg_desc->type, msg_desc->id);
}

static void on_sidewalk_send_error(sid_error_t error, const struct sid_msg_desc *msg_desc,
				   void *context)
{
	LOG_INF("%s", __func__);
	LOG_ERR("failed to send message(type: %d, id: %u), err:%d", (int)msg_desc->type,
		msg_desc->id, (int)error);
}

static void on_sidewalk_factory_reset(void *context)
{
	LOG_INF("%s", __func__);

	ARG_UNUSED(context);

	LOG_INF("factory reset notification received from sid api");
	if (sid_hal_reset(SID_HAL_RESET_NORMAL)) {
		LOG_WRN("Reboot type not supported");
	}
}

static void on_sidewalk_status_changed(const struct sid_status *status, void *context)
{
	LOG_INF("%s", __func__);

	switch (status->state) {
	case SID_STATE_READY:
	case SID_STATE_SECURE_CHANNEL_READY:
		app_event_send(APP_EVENT_READY);
		break;
	case SID_STATE_NOT_READY:
		app_event_send(APP_EVENT_NOT_READY);
		break;
	case SID_STATE_ERROR:
		app_event_send(APP_EVENT_ERROR);
		break;
	default:
		LOG_ERR("sidewalk unknow state: %d", status->state);
		break;
	}

	LOG_INF("Device %sregistered, Time Sync %s, Link status: {BLE: %s, FSK: %s, LoRa: %s}",
		(SID_STATUS_REGISTERED == status->detail.registration_status) ? "Is " : "Un",
		(SID_STATUS_TIME_SYNCED == status->detail.time_sync_status) ? "Success" : "Fail",
		(status->detail.link_status_mask & SID_LINK_TYPE_1) ? "Up" : "Down",
		(status->detail.link_status_mask & SID_LINK_TYPE_2) ? "Up" : "Down",
		(status->detail.link_status_mask & SID_LINK_TYPE_3) ? "Up" : "Down");

	for (int i = 0; i < SID_LINK_TYPE_MAX_IDX; i++) {
		enum sid_link_mode mode =
			(enum sid_link_mode)status->detail.supported_link_modes[i];

		if (mode) {
			LOG_INF("Link mode on %s = {Cloud: %s, Mobile: %s}",
				(SID_LINK_TYPE_1_IDX == i) ? "BLE" :
				(SID_LINK_TYPE_2_IDX == i) ? "FSK" :
				(SID_LINK_TYPE_3_IDX == i) ? "LoRa" :
							     "unknow",
				(mode & SID_LINK_MODE_CLOUD) ? "True" : "False",
				(mode & SID_LINK_MODE_MOBILE) ? "True" : "False");
		}
	}
}

static void state_running_run(void *o)
{
	struct sid_ctx_s *ctx = CONTAINER_OF(o, struct sid_ctx_s, sm);
	sid_error_t e = SID_ERROR_NONE;

	switch (ctx->sm.event) {
	case APP_EVENT_SIDEWALK:
		e = sid_process(ctx->handle);
		if (e) {
			LOG_ERR("sid process err %d", (int)e);
		}
		break;
	case APP_EVENT_FACTORY_RESET:
		e = sid_set_factory_reset(ctx->handle);
		if (e) {
			LOG_ERR("sid process err %d", (int)e);
		}
		break;
	case APP_EVENT_ERROR:
		LOG_ERR("sid state err %d", (int)sid_get_error(ctx->handle));
		break;
	case APP_EVENT_LINK_SWITCH:
		sid_error_t e = SID_ERROR_NONE;

		static uint32_t link_mask = SID_LINK_TYPE_1;

		switch (link_mask) {
		case SID_LINK_TYPE_1:
			link_mask = SID_LINK_TYPE_2;
			break;
		case SID_LINK_TYPE_2:
			link_mask = SID_LINK_TYPE_3;
			break;
		default:
			link_mask = SID_LINK_TYPE_1;
			break;
		}

		LOG_INF("Sidewalk link switch to 0x%x", link_mask);
		ctx->config.link_mask = link_mask;

		if (ctx->handle != NULL) {
			e = sid_deinit(ctx->handle);
			if (e) {
				LOG_ERR("sid deinit err %d", (int)e);
			}
		}

		e = sid_init(&ctx->config, &ctx->handle);
		if (e) {
			LOG_ERR("sid init err %d", (int)e);
		}

		e = sid_start(ctx->handle, link_mask);
		if (e) {
			LOG_ERR("sid start err %d", (int)e);
		}
		break;
	case APP_EVENT_READY:
	case APP_EVENT_NOT_READY:
		break;
	default:
		LOG_WRN("event: unknow %d", ctx->sm.event);
		break;
	}
}

static void state_init_entry(void *o)
{
	LOG_INF("state: init (entry)");

	struct sid_ctx_s *ctx = CONTAINER_OF(o, struct sid_ctx_s, sm);
	sid_error_t e = SID_ERROR_NONE;

	e = application_pal_init();
	if (e) {
		LOG_ERR("pal init err %d", (int)e);
	}
	e = sid_init(&ctx->config, &ctx->handle);
	if (e) {
		LOG_ERR("sid init err %d", (int)e);
	}
	e = sid_start(ctx->handle, SID_LINK_TYPE_1);
	if (e) {
		LOG_ERR("sid start err %d", (int)e);
	}
}

static void state_init_run(void *o)
{
	LOG_INF("state: init");

	struct sid_ctx_s *ctx = CONTAINER_OF(o, struct sid_ctx_s, sm);

	switch (ctx->sm.event) {
	case APP_EVENT_SIDEWALK:
	case APP_EVENT_ERROR:
		break;
	case APP_EVENT_READY:
		smf_set_state(SMF_CTX(&ctx->sm), &app_states[STATE_SIDEWALK_READY]);
		break;
	case APP_EVENT_NOT_READY:
		smf_set_state(SMF_CTX(&ctx->sm), &app_states[STATE_SIDEWALK_NOT_READY]);
		break;
	default:
		LOG_WRN("event: unknow %d", ctx->sm.event);
		break;
	}
}

static void state_not_ready_run(void *o)
{
	LOG_INF("state: not ready");

	struct sid_ctx_s *ctx = CONTAINER_OF(o, struct sid_ctx_s, sm);

	switch (ctx->sm.event) {
	case APP_EVENT_SIDEWALK:
	case APP_EVENT_NOT_READY:
	case APP_EVENT_ERROR:
		break;
	case APP_EVENT_READY:
		smf_set_state(SMF_CTX(&ctx->sm), &app_states[STATE_SIDEWALK_READY]);
		break;
	case APP_EVENT_SEND:
		smf_set_state(SMF_CTX(&ctx->sm), &app_states[STATE_SIDEWALK_CONNECTING]);
		break;
	default:
		LOG_WRN("event: unknow %d", ctx->sm.event);
		break;
	}
}

static void state_connecting_entry(void *o)
{
	LOG_INF("state: connecting (entry)");

	struct sid_ctx_s *ctx = CONTAINER_OF(o, struct sid_ctx_s, sm);
	sid_error_t e = SID_ERROR_NONE;

	e = sid_ble_bcn_connection_request(ctx->handle, true);
	if (e) {
		LOG_ERR("conn req err %d", (int)e);
		smf_set_state(SMF_CTX(&ctx->sm), &app_states[STATE_SIDEWALK_NOT_READY]);
	}
}

static void state_connecting_run(void *o)
{
	LOG_INF("state: connecting");

	struct sid_ctx_s *ctx = CONTAINER_OF(o, struct sid_ctx_s, sm);

	switch (ctx->sm.event) {
	case APP_EVENT_SIDEWALK:
	case APP_EVENT_ERROR:
		break;
	case APP_EVENT_NOT_READY:
		LOG_ERR("Connecting failed");
		smf_set_state(SMF_CTX(&ctx->sm), &app_states[STATE_SIDEWALK_NOT_READY]);
		break;
	case APP_EVENT_READY:
		smf_set_state(SMF_CTX(&ctx->sm), &app_states[STATE_SIDEWALK_READY]);
		app_event_send(APP_EVENT_SEND);
		break;
	default:
		LOG_WRN("event: unknow %d", ctx->sm.event);
		break;
	}
}

static void state_ready_run(void *o)
{
	LOG_INF("state: ready");

	struct sid_ctx_s *ctx = CONTAINER_OF(o, struct sid_ctx_s, sm);
	sid_error_t e = SID_ERROR_NONE;

	switch (ctx->sm.event) {
	case APP_EVENT_SIDEWALK:
	case APP_EVENT_READY:
	case APP_EVENT_ERROR:
		break;
	case APP_EVENT_NOT_READY:
		smf_set_state(SMF_CTX(&ctx->sm), &app_states[STATE_SIDEWALK_NOT_READY]);
		break;
	case APP_EVENT_SEND:
		e = sid_put_msg(ctx->handle, &app_send_buffer.msg, &app_send_buffer.desc);
		if (e) {
			LOG_ERR("sid send err %d", (int)e);
		}
		LOG_INF("sid send (type: %d, id: %u)", (int)app_send_buffer.desc.type,
			app_send_buffer.desc.id);
		break;
	default:
		LOG_WRN("event: unknow %d", ctx->sm.event);
		break;
	}
}

static void button_changed(uint32_t button_state, uint32_t has_changed)
{
	uint32_t buttons = button_state & has_changed;

	if (buttons & DK_BTN1_MSK) {
		LOG_INF("button 1");

		static char payload[] = "hello";
		app_send_buffer = (struct app_msg){
			.msg = {
				.data = payload,
				.size = sizeof(payload),
			},
			.desc = {
				.type = SID_MSG_TYPE_NOTIFY,
				.link_type = SID_LINK_TYPE_ANY,
				.link_mode = SID_LINK_MODE_CLOUD,
			},
		};
		app_event_send(APP_EVENT_SEND);
	}
	if (buttons & DK_BTN2_MSK) {
		LOG_INF("button 2");
		app_event_send(APP_EVENT_LINK_SWITCH);
	}
	if (buttons & DK_BTN3_MSK) {
		LOG_INF("button 3");
	}
	if (buttons & DK_BTN4_MSK) {
		LOG_INF("button 4");
		app_event_send(APP_EVENT_FACTORY_RESET);
	}
}

static int app_event_send(app_event_t event)
{
	int e = k_msgq_put(&sid_ctx.sm.msgq, (void *)&event, K_NO_WAIT);
	if (e) {
		LOG_ERR("msg put err: %d", e);
	}
	return e;
}

static void app_thread_entry(void *context, void *unused, void *unused2)
{
	LOG_INF("App");

	ARG_UNUSED(unused);
	ARG_UNUSED(unused2);

	struct sid_ctx_s *ctx = (struct sid_ctx_s *)context;

	struct sid_event_callbacks event_callbacks = {
		.context = ctx,
		.on_event = on_sidewalk_event,
		.on_msg_received = on_sidewalk_msg_received,
		.on_msg_sent = on_sidewalk_msg_sent,
		.on_send_error = on_sidewalk_send_error,
		.on_status_changed = on_sidewalk_status_changed,
		.on_factory_reset = on_sidewalk_factory_reset,
	};

	ctx->config = (struct sid_config){
		.link_mask = SID_LINK_TYPE_1,
		.callbacks = &event_callbacks,
		.time_sync_periodicity_seconds = 7200,
		.link_config = app_get_ble_config(),
		.sub_ghz_link_config = app_get_sub_ghz_config(),
	};

	k_msgq_init(&ctx->sm.msgq, msgq_buffer, sizeof(app_event_t),
		    CONFIG_SIDEWALK_THREAD_QUEUE_SIZE);
	smf_set_initial(SMF_CTX(&ctx->sm), &app_states[STATE_INIT]);

	while (1) {
		LOG_INF("waiting for event...");
		int err = k_msgq_get(&ctx->sm.msgq, &ctx->sm.event, K_FOREVER);
		if (!err) {
			if (smf_run_state(SMF_CTX(&ctx->sm))) {
				break;
			}
		} else {
			LOG_ERR("msgq err %d", err);
		}
	}

	LOG_ERR("Application failed. You should never see this message.");
}

int main(void)
{
	LOG_INF("Main");

	if (dk_buttons_init(button_changed)) {
		LOG_ERR("Cannot init buttons");
	}

	(void)k_thread_create(&app_thread, app_thread_stack,
			      K_THREAD_STACK_SIZEOF(app_thread_stack), app_thread_entry, &sid_ctx,
			      NULL, NULL, CONFIG_SIDEWALK_THREAD_PRIORITY, 0, K_NO_WAIT);
	k_thread_name_set(&app_thread, "app");

	return 0;
}
