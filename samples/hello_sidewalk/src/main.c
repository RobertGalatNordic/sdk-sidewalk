/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-4-Clause
 */

#include <sid_pal_storage_kv_ifc.h>
#include <sid_pal_log_ifc.h>
#include <sid_pal_mfg_store_ifc.h>
#include <storage/flash_map.h>
#include <sid_pal_assert_ifc.h>
#include <sid_pal_timer_ifc.h>
#include <sid_pal_crypto_ifc.h>
#include <sid_pal_ble_adapter_ifc.h>

#include <sid_pal_uptime_ifc.h>
#include <dk_buttons_and_leds.h>
#include <logging/log.h>
#include <zephyr.h>

LOG_MODULE_REGISTER(app, LOG_LEVEL_DBG);

#define MFG_EMPTY_VERSION       0xFFFFFFFFUL

#define KEY_BLE_NOTIFY DK_BTN1_MSK

#if !FLASH_AREA_LABEL_EXISTS(mfg_storage)
	#error "Flash partition is not defined for the Sidewalk manufacturing storage!!"
#endif

static const sid_pal_mfg_store_region_t mfg_store_region = {
	.addr_start = (uintptr_t)(FLASH_AREA_OFFSET(mfg_storage)),
	.addr_end = (uintptr_t)(FLASH_AREA_OFFSET(mfg_storage) + FLASH_AREA_SIZE(mfg_storage)),
};

sid_pal_timer_t sidewalk_timer;
char *sidewalk_timer_str = "Hello Sidewalk timer!";

struct sid_timespec sidewalk_timer_duration = { .tv_sec = 5 };
struct sid_timespec sidewalk_timer_period = { .tv_sec = 10 };

static const sid_ble_cfg_adv_param_t adv_param = {
	.type = AMA_SERVICE,
	.fast_enabled = true,
	.slow_enabled = true,
	.fast_interval = 256,
	.fast_timeout = 3000,
	.slow_interval = 1600,
	.slow_timeout = 0,
};

static const sid_ble_config_t ble_cfg = {
	.is_adv_available = true,
	.mac_addr_type = SID_BLE_CFG_MAC_ADDRESS_TYPE_RANDOM_PRIVATE_NON_RESOLVABLE,
	.adv_param = adv_param,
	.num_profile = 0,
	.profile = NULL,
};

static sid_pal_ble_adapter_interface_t p_ble_ifc;

void sidewalk_timer_cb(void *arg, sid_pal_timer_t *originator)
{
	LOG_INF("sidewalk_timer_cb says \"%s\"\n", (char *)arg);
}

void button_handler(uint32_t button_state, uint32_t has_changed)
{
	static uint8_t buff[] = { 0x11, 0x22 };
	uint32_t button = button_state & has_changed;

	if (button & KEY_BLE_NOTIFY) {
		p_ble_ifc->send(AMA_SERVICE, buff, sizeof(buff));
	}
}

void main(void)
{
	uint32_t mfg_ver;
	uint8_t rand_buff[16];
	struct sid_timespec sid_time;
	sid_error_t erc;

	// We don't trust SW without crypto ;)
	SID_PAL_ASSERT(SID_ERROR_NONE == sid_pal_crypto_init());

	erc = sid_pal_crypto_rand(rand_buff, sizeof(rand_buff));
	if (SID_ERROR_NONE == erc) {
		SID_PAL_LOG_INFO("Random value:");
		SID_PAL_HEXDUMP(SID_PAL_LOG_SEVERITY_INFO, rand_buff, sizeof(rand_buff));
	} else {
		SID_PAL_LOG_WARNING("Cannot generate random value! [erc=%d]", erc);
	}

	LOG_INF("Hello Sidewalk World! %s\n", CONFIG_BOARD);

	if (SID_ERROR_NONE == sid_pal_storage_kv_init()) {
		LOG_INF("NVM memory is ready!\n");
	}

	SID_PAL_LOG_INFO("Hello Sidewalk Log! %d", 2022);

	sid_pal_mfg_store_init(mfg_store_region);
	mfg_ver = sid_pal_mfg_store_get_version();
	if (MFG_EMPTY_VERSION == mfg_ver) {
		SID_PAL_LOG_INFO("Mfg storage is not provisioned!");
	} else {
		uint8_t dev_id[SID_PAL_MFG_STORE_DEVID_SIZE];
		SID_PAL_LOG_INFO("Mfg storage ver: 0x%04x", mfg_ver);
		sid_pal_mfg_store_dev_id_get(dev_id);
		SID_PAL_LOG_INFO("Mfg dev ID:");
		SID_PAL_HEXDUMP(SID_PAL_LOG_SEVERITY_INFO, dev_id, sizeof(dev_id));
	}

	erc = sid_pal_timer_init(&sidewalk_timer, sidewalk_timer_cb, sidewalk_timer_str);
	if (SID_ERROR_NONE == erc) {
		erc = sid_pal_timer_arm(&sidewalk_timer, SID_PAL_TIMER_PRIO_CLASS_LOWPOWER,
					&sidewalk_timer_duration, &sidewalk_timer_period);
		if (SID_ERROR_NONE == erc) {
			SID_PAL_LOG_INFO("Timer is started and %sarmed!", sid_pal_timer_is_armed(&sidewalk_timer) ? "" : "dis");
		} else {
			SID_PAL_LOG_WARNING("Timer cannot be armed [erc=%d].", erc);
		}
	} else {
		SID_PAL_LOG_WARNING("Timer initialization error=%d.", erc);
	}

	k_sleep(K_SECONDS(3));
	erc = sid_pal_uptime_now(&sid_time);
	if (SID_ERROR_NONE == erc) {
		SID_PAL_LOG_INFO("Uptime sec: %u nsec:%u", sid_time.tv_sec, sid_time.tv_nsec);
	} else {
		SID_PAL_LOG_ERROR("Uptime fail, error code: %d", erc);
	}

	sid_pal_ble_adapter_create(&p_ble_ifc);

	p_ble_ifc->init(&ble_cfg);
	p_ble_ifc->start_adv();

	erc = dk_buttons_init(button_handler);
	if (erc) {
		SID_PAL_LOG_WARNING("Failed to initialize buttons (err %d)\n", erc);
		return;
	}

	SID_PAL_ASSERT(true);
}
