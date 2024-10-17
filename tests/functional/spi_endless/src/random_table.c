/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#include <stdint.h>
uint8_t random_table[] = {
	0x52, 0xed, 0x2a, 0xb3, 0x7c, 0xb8, 0xdb, 0xf7, 0x47, 0x1f, 0x70, 0x83, 0x7e, 0x64, 0x63,
	0xef, 0x1c, 0x0d, 0x3a, 0xb7, 0x7a, 0xf0, 0xae, 0xad, 0xad, 0x5a, 0x00, 0x5b, 0xfb, 0xe0,
	0x88, 0xec, 0x8c, 0x31, 0xdd, 0x96, 0x4b, 0xf4, 0x33, 0xfc, 0x76, 0x7d, 0x7a, 0x08, 0x56,
	0xb5, 0x64, 0xdd, 0x94, 0x65, 0xea, 0x98, 0xc2, 0xde, 0xf3, 0x4a, 0x5f, 0xa9, 0x98, 0xb9,
	0xe6, 0xbf, 0xab, 0xce, 0xd0, 0xdf, 0xff, 0x5b, 0x29, 0xa9, 0xfa, 0x24, 0xeb, 0xfc, 0xe0,
	0xb0, 0xd8, 0x1e, 0xc0, 0x11, 0x29, 0x48, 0x4e, 0xb8, 0x0d, 0xf3, 0xc7, 0x62, 0x3e, 0x1e,
	0xdb, 0x8c, 0x6c, 0x4a, 0x8a, 0xc3, 0x1b, 0x63, 0x52, 0xc3, 0xfb, 0x71, 0x7f, 0xb6, 0x4a,
	0xe8, 0x55, 0x36, 0xf2, 0x77, 0xf7, 0xe4, 0xa0, 0xf6, 0x2c, 0xa6, 0x57, 0x5e, 0x73, 0x10,
	0x7f, 0x38, 0x4b, 0x2e, 0x73, 0xa7, 0xa6, 0xaa, 0x13, 0xf1, 0x6a, 0xe2, 0x7a, 0x18, 0xd3,
	0xd0, 0x38, 0xb9, 0x55, 0x7b, 0xcd, 0x57, 0x85, 0x91, 0xcf, 0x68, 0xce, 0xb7, 0x52, 0xd4,
	0xa2, 0x82, 0xf5, 0x78, 0x5f, 0xc7, 0x37, 0x75, 0x4c, 0xfa, 0x10, 0x93, 0x6e, 0x89, 0x60,
	0x18, 0x62, 0x4a, 0xc2, 0x9c, 0x11, 0x9a, 0x20, 0x4e, 0x19, 0xbc, 0xe7, 0x12, 0xb7, 0x03,
	0x2a, 0xaf, 0x37, 0x3e, 0xf3, 0x6d, 0x6a, 0x47, 0xfb, 0x3a, 0xca, 0xe2, 0x91, 0x76, 0x85,
	0xe5, 0x79, 0x08, 0xe4, 0xe2, 0x95, 0xb5, 0x32, 0xc0, 0x15, 0xeb, 0xe1, 0xac, 0x3c, 0x8a,
	0x76, 0x08, 0x0c, 0x27, 0x2e, 0x11, 0x36, 0x83, 0x75, 0x27, 0x87, 0x81, 0x64, 0x5e, 0x3a,
	0x30, 0x97, 0x1d, 0xfb, 0x71, 0x6f, 0xae, 0xf6, 0x00, 0xab, 0xf1, 0xb5, 0x9f, 0x4c, 0xd9,
	0x3c, 0x09, 0xb0, 0x32, 0x45, 0x29, 0x73, 0x3a, 0x46, 0x9e, 0xd2, 0x82, 0x0d, 0xf3, 0xe3,
	0x78, 0x3a, 0x25, 0x5a, 0x04, 0x1c, 0xeb, 0x79, 0xd4, 0x78, 0x20, 0xff, 0x61, 0xda, 0x15,
	0x7e, 0x72, 0x30, 0xc2, 0xe4, 0x89, 0xd4, 0x22, 0x3f, 0x64, 0x97, 0xb1, 0x5a, 0xd1, 0x0c,
	0xa4, 0x1c, 0xd7, 0xdf, 0xa8, 0xae, 0x84, 0x9a, 0x49, 0x20, 0xa2, 0xe0, 0x27, 0xcc, 0xfe,
	0xe4, 0x0b, 0x48, 0x68, 0xb0, 0xdc, 0x04, 0x03, 0xd6, 0x97, 0x49, 0x63, 0x4f, 0xe7, 0xc6,
	0xd8, 0xa7, 0x85, 0xc4, 0x73, 0x0a, 0x54, 0xc5, 0xee, 0xfd, 0xb3, 0x48, 0xeb, 0x50, 0xd9,
	0xf0, 0x50, 0xb2, 0xa3, 0x9a, 0xe9, 0x89, 0x76, 0x79, 0xe4, 0x19, 0xbc, 0x98, 0xce, 0xaa,
	0x2f, 0x79, 0x48, 0x2f, 0x75, 0xb5, 0x72, 0x7d, 0xda, 0x68, 0x79, 0x7f, 0x1b, 0xff, 0x20,
	0xd6, 0x98, 0xf4, 0x5c, 0x4e, 0x80, 0x23, 0x1b, 0x2b, 0xc9, 0xe0, 0x34, 0xef, 0xb1, 0x7e,
	0x75, 0x42, 0x64, 0x38, 0x12, 0xe3, 0x7f, 0x3e, 0xbc, 0x3b, 0x43, 0x61, 0xbf, 0x7f, 0x98,
	0x0e, 0x6b, 0x4a, 0xfa, 0x49, 0x4f, 0x5e, 0x1d, 0x27, 0xae, 0xbe, 0x9d, 0xf1, 0x1d, 0xfb,
	0x79, 0x95, 0xed, 0x8c, 0x5d, 0x74, 0x8c, 0x52, 0x40, 0xa2, 0x37, 0x8e, 0x08, 0x97, 0x06,
	0x90, 0xb0, 0x66, 0x09, 0x7d, 0x31, 0x8e, 0x42, 0xef, 0xd9, 0x70, 0x97, 0x2d, 0xcc, 0x86,
	0x54, 0x78, 0x76, 0xf9, 0x72, 0x1b, 0x7f, 0xae, 0xcc, 0xa4, 0xaa, 0xf9, 0x14, 0x6b, 0x9e,
	0xc3, 0x7f, 0x28, 0xbf, 0x1c, 0xbd, 0xee, 0xd0, 0x5c, 0xe7, 0x89, 0xbf, 0x8a, 0x1d, 0xdd,
	0x77, 0x89, 0x50, 0x03, 0x21, 0x87, 0x8a, 0x73, 0x2f, 0x0e, 0x76, 0xc9, 0x01, 0x8d, 0x94,
	0xb3, 0x39, 0x18, 0x1e, 0x9e, 0xc5, 0x4a, 0xdd, 0xd2, 0x14, 0xb9, 0x12, 0xe3, 0x87, 0xa1,
	0x52, 0x70, 0x9d, 0x49, 0xda, 0xd2, 0xb3, 0x14, 0x90, 0x63, 0x40, 0x66, 0x7a, 0x1b, 0xff,
	0x9c, 0xa4, 0x94, 0xd9, 0x43, 0x82, 0x63, 0xa6, 0xaa, 0x62, 0x35, 0x4b, 0xd6, 0xf0, 0xff,
	0x2f, 0x35, 0xa8, 0x2a, 0xf0, 0x2b, 0xd2, 0x4f, 0x2e, 0x4b, 0xed, 0x0c, 0xb8, 0x16, 0x91,
	0x91, 0x76, 0xed, 0xf1, 0xa9, 0x96, 0x7c, 0x4c, 0xf3, 0x33, 0x0c, 0xe3, 0xc7, 0x86, 0xea,
	0x75, 0x4f, 0x38, 0xeb, 0xfb, 0xe6, 0x33, 0x32, 0x34, 0xc5, 0x57, 0x1f, 0x04, 0xed, 0x10,
	0xc3, 0x4e, 0x34, 0xff, 0xed, 0x3b, 0x4c, 0x7a, 0x62, 0x62, 0x4e, 0x89, 0x42, 0x5d, 0xcc,
	0x2f, 0xfa, 0x89, 0x04, 0x8c, 0x8d, 0xfa, 0xb2, 0x51, 0x5f, 0x9a, 0xc5, 0x81, 0xd7, 0x59,
	0xf2, 0x51, 0x89, 0x39, 0x11, 0x07, 0xa3, 0x57, 0xfc, 0xa0, 0x3c, 0xa3, 0xa1, 0x1a, 0x1f,
	0x34, 0x46, 0x47, 0x95, 0xaa, 0xf4, 0xc8, 0xf5, 0xc0, 0xee, 0xb9, 0x49, 0x65, 0x19, 0x5a,
	0xe4, 0x09, 0x6b, 0x86, 0x19, 0x2c, 0x85, 0x27, 0x00, 0xae, 0x3f, 0x2f, 0x08, 0x42, 0x0c,
	0x04, 0x06, 0x66, 0x9d, 0x87, 0xce, 0xab, 0xaa, 0x44, 0xd3, 0x87, 0xd5, 0x82, 0x75, 0xa1,
	0xbe, 0xe2, 0xbd, 0x6a, 0x14, 0x98, 0x08, 0x62, 0x66, 0x4d, 0xd4, 0x4a, 0x9c, 0x01, 0x6d,
	0x14, 0x56, 0x91, 0x87, 0x36, 0x3a, 0xa4, 0xf2, 0xc2, 0xdc, 0x23, 0x88, 0xfe, 0x4d, 0xa7,
	0xe5, 0xb4, 0x35, 0xc1, 0x2b, 0xcb, 0xbd, 0x02, 0x77, 0x6e, 0xe1, 0x84, 0x15, 0xd8, 0xe7,
	0x8b, 0x19, 0x4a, 0xe4, 0x81, 0xb9, 0x96, 0x0c, 0x7a, 0x83, 0x07, 0xb8, 0xa6, 0xc2, 0x56,
	0x5e, 0x05, 0x74, 0xe7, 0x5d, 0x2f, 0xc9, 0xcc, 0x90, 0x30, 0x84, 0x2c, 0xf9, 0x8d, 0x9a,
	0x09, 0xd3, 0x83, 0xa1, 0x57, 0xe4, 0x3c, 0x9a, 0x15, 0xac, 0x33, 0xd9, 0xce, 0xda, 0x5c,
	0x78, 0x4e, 0xe7, 0x7b, 0x1e, 0xe5, 0x85, 0x33, 0xf9, 0xa2, 0xad, 0xd1, 0xb4, 0xee, 0xb9,
	0x18, 0x38, 0xde, 0xc3, 0xdb, 0x52, 0x2f, 0xaf, 0xc8, 0xc3, 0xcf, 0xec, 0xc6, 0xea, 0xad,
	0xe9, 0x96, 0x6a, 0xb5, 0x79, 0x28, 0x30, 0x4a, 0x54, 0xf6, 0x2f, 0xbb, 0x02, 0x65, 0x5b,
	0xb0, 0x66, 0x15, 0xea, 0x55, 0x70, 0x34, 0x82, 0x48, 0xf4, 0x17, 0xd7, 0x7a, 0x8d, 0x4a,
	0x88, 0x41, 0x9a, 0x71, 0xc9, 0x81, 0x72, 0x8b, 0x49, 0xe9, 0xe6, 0xc1, 0x6b, 0x84, 0xcb,
	0x3d, 0xe9, 0x33, 0xf3, 0xe8, 0x0b, 0x5f, 0x44, 0x5e, 0x39, 0x38, 0x6e, 0xd5, 0x6b, 0xca,
	0xcb, 0x1f, 0x53, 0xf4, 0xc1, 0xeb, 0x74, 0x45, 0xae, 0x80, 0x9f, 0xab, 0x98, 0x55, 0x48,
	0xeb, 0x7f, 0x33, 0x89, 0x9f, 0xc8, 0x4b, 0xa0, 0x84, 0x96, 0x46, 0xd3, 0x86, 0x9a, 0xaa,
	0x99, 0x9c, 0xf8, 0xcd, 0x71, 0x72, 0x20, 0x61, 0x1a, 0x67, 0x3a, 0x84, 0xf7, 0xfc, 0x92,
	0x85, 0x48, 0x42, 0xbb, 0xd5, 0xfa, 0x3e, 0x10, 0x07, 0xa5, 0xd9, 0x39, 0xb3, 0x5b, 0x71,
	0xbe, 0xb8, 0x61, 0x4f, 0x7c, 0xa6, 0x5a, 0xfe, 0xf9, 0x04, 0xcb, 0x1c, 0x1c, 0x9b, 0x69,
	0x2e, 0xb2, 0x52, 0xbe, 0xf1, 0x9a, 0x26, 0x57, 0xef, 0x3c, 0xee, 0x6a, 0xfd, 0x26, 0xe6,
	0x46, 0xfc, 0x2a, 0xd7, 0x79, 0x8f, 0x13, 0x48, 0x42, 0x1b, 0xd7, 0x29, 0xa9, 0x16, 0x88,
	0xa0, 0x5b, 0xc8, 0xc4, 0x54, 0xe7, 0xe2, 0xba, 0x07, 0xaf, 0xcb, 0xb3, 0xdb, 0x17, 0x03,
	0x1e, 0xf6, 0x40, 0xe6, 0x97, 0x25, 0x68, 0x45, 0xa7, 0xdc, 0x1a, 0x3d, 0x4b, 0xfc, 0x8e,
	0x62, 0xdb, 0x50, 0x22, 0xd2, 0xac, 0xe6, 0xbd, 0x9d, 0xb7, 0xca, 0xcb, 0x23, 0xdb, 0xff,
	0x5d, 0x8e, 0xd6, 0x49, 0x68, 0x0a, 0x0f, 0x40, 0xf5, 0x32, 0xd7, 0xaa, 0x8c, 0x21, 0xdd,
	0xc9, 0x7b, 0xaf, 0x66, 0xe2, 0xda, 0xbf, 0xeb, 0x7a, 0x42, 0xa8, 0xb7, 0x65, 0x1e, 0xeb,
	0xd3, 0x5f, 0xb2, 0xd6
};
