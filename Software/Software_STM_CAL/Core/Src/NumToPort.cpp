/*
 * NumToPort.cpp
 *
 *  Created on: Jul 30, 2020
 *      Author: sulkith
 */
#include "main.h"
const uint16_t DISP_0 = 0;
const uint16_t DISP_1 = (HS1_Pin);
const uint16_t DISP_2 = (HS2_Pin);
const uint16_t DISP_3 = (HS1_Pin | HS2_Pin);
const uint16_t DISP_4 = (HS4_Pin);
const uint16_t DISP_5 = (HS4_Pin | HS1_Pin);
const uint16_t DISP_6 = (HS4_Pin | HS2_Pin);
const uint16_t DISP_7 = (HS4_Pin | HS2_Pin | HS1_Pin);
const uint16_t DISP_8 = (HS8_Pin);
const uint16_t DISP_9 = (HS8_Pin | HS1_Pin);
const uint16_t DISP_A = (HS8_Pin | HS2_Pin);
const uint16_t DISP_B = (HS8_Pin | HS2_Pin | HS1_Pin);
const uint16_t DISP_C = (HS8_Pin | HS4_Pin);
const uint16_t DISP_D = (HS8_Pin | HS4_Pin | HS1_Pin);
const uint16_t DISP_E = (HS8_Pin | HS4_Pin | HS2_Pin);
const uint16_t DISP_F = (HS8_Pin | HS4_Pin | HS2_Pin | HS1_Pin);
const uint16_t numToPort[] = { DISP_0, // 0
		DISP_1, // 1
		DISP_2, // 2
		DISP_3, // 3
		DISP_4, // 4
		DISP_5, // 5
		DISP_6, // 6
		DISP_7, // 7
		DISP_8, // 8
		DISP_9, // 9
		DISP_A, // A
		DISP_B, // B
		DISP_C, // C
		DISP_D, // D
		DISP_E, // E
		DISP_F // F
		};

const uint32_t mask_for_pins = (((HS8_Pin | HS4_Pin | HS2_Pin | HS1_Pin)) << 16);
const uint32_t mask_clear = (((HS8_Pin | HS4_Pin | HS2_Pin | HS1_Pin)
		| (LSH10_Pin) | (LSH01_Pin) | (LSM10_Pin) | (LSM01_Pin)) << 16);
const uint32_t Column[] = { mask_for_pins | (LSH10_Pin << 16) | (LSH01_Pin)
		| (LSM10_Pin) | (LSM01_Pin), // H10 Column
mask_for_pins | (LSH01_Pin << 16) | (LSH10_Pin) | (LSM10_Pin) | (LSM01_Pin), // H01 Column
mask_for_pins | (LSM10_Pin << 16) | (LSH10_Pin) | (LSH01_Pin) | (LSM01_Pin), // M10 Column
mask_for_pins | (LSM01_Pin << 16) | (LSH10_Pin) | (LSH01_Pin) | (LSM10_Pin) // M01 Column
};

