/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */




#ifndef __TIZEN_SYSTEM_DEVICE_H__
#define __TIZEN_SYSTEM_DEVICE_H__

#include <stdbool.h>
#include "device-error.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_MODULE
 * @{
 */

/**
 * @brief Gets the number of display devices.
 *
 * @return The number of display devices that the device provides.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 *
 * @see	device_get_brightness()
 * @see device_set_brightness()
 * @see device_get_max_brightness()
 * @see device_set_brightness_from_settings()
 * @see device_set_brightness_to_settings()
 */
int device_get_display_numbers(int* device_number);

/**
 * @brief Gets the display brightness value.
 *
 * @param[in] display_index	The index of the display, it be greater than or equal to 0 and less than \n
 *                          the number of displays returned by device_get_display_numbers().\n
 *                          The index zero is always assigned to the main display.
 * @param[out] brightness	The current brightness value of the display
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_get_display_numbers()
 * @see device_set_brightness()
 * @see device_get_max_brightness()
 * @see device_set_brightness_from_settings()
 * @see device_set_brightness_to_settings()
 */
int device_get_brightness(int display_index, int *brightness);

/**
 * @brief Sets the display brightness value. 
 *
 * @param[in] display_index	The index of the display, it be greater than or equal to 0 and less than \n
 *                          the number of displays returned by device_get_display_numbers().\n
 *                          The index zero is always assigned to the main display.
 * @param[in] brightness	The new brightness value to set \n
 *							The maximum value can be represented by device_get_max_brightness()
 * 
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 *
 * @see device_get_display_numbers()
 * @see device_get_max_brightness()
 * @see device_get_brightness()
 * @see device_set_brightness_from_settings()
 * @see device_set_brightness_to_settings()
 */
int device_set_brightness(int display_index, int brightness);

/**
 * @brief Gets the maximum brightness value that can be set. 
 *
 * @param[in] display_index	The index of the display, it be greater than or equal to 0 and less than \n
 *                          the number of displays returned by device_get_display_numbers().\n
 *                          The index zero is always assigned to the main display.
 * @param[out] max_brightness	The maximum brightness value of the display
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_get_display_numbers()
 * @see device_set_brightness()
 * @see device_get_brightness()
 * @see device_set_brightness_from_settings()
 * @see device_set_brightness_to_settings()
 */
int device_get_max_brightness(int display_index, int *max_brightness);

/**
 * @brief Sets the display brightness value from registered in settings.
 *
 * @details
 * This function set display brightness to condition in the settings.
 * if auto brightness option is enabled in setting, display's brightness will be changed automatically.
 *
 * @param[in] display_index	The index of the display, it be greater than or equal to 0 and less than \n
 *                          the number of displays returned by device_get_display_numbers().\n
 *                          The index zero is always assigned to the main display.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_get_display_numbers()
 * @see device_get_max_brightness()
 * @see device_set_brightness()
 * @see device_get_brightness()
 * @see device_set_brightness_to_settings()
 */
int device_set_brightness_from_settings(int display_index);

/**
 * @brief Sets the display brightness value to specific display and set to variable in settings.
 *
 * @details
 * This function set given brightness value to given index of display.
 * And also brightness variable in settings will be changed to given brightness value too.
 *
 * @param[in] display_index	The index of the display, it be greater than or equal to 0 and less than \n
 *                          the number of displays returned by device_get_display_numbers().\n
 *                          The index zero is always assigned to the main display.
 * @param[in] brightness	The new brightness value to set \n
 *							The maximum value can be represented by device_get_max_brightness()
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_get_display_numbers()
 * @see device_get_max_brightness()
 * @see device_set_brightness()
 * @see device_get_brightness()
 * @see device_set_brightness_from_settings()
 */
int device_set_brightness_to_settings(int display_index, int brightness);

/**
 * @brief Get brightness value of LED that placed to camera flash.
 *
 * @remarks This API is related to the following feature: %http://tizen.org/feature/camera.back.flash
 *
 * @param[out] brightness brightness value of LED (0 ~ MAX)
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED		Not supported device
 */
int device_flash_get_brightness(int *brightness);

/**
 * @brief Set brightness value of LED that placed to camera flash.
 *
 * @remarks This API is related to the following feature: %http://tizen.org/feature/camera.back.flash
 *
 * @param[in] brightness brightness value of LED (0 ~ MAX)
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED		Not supported device
 */
int device_flash_set_brightness(int brightness);

/**
 * @brief Get max brightness value of LED that placed to camera flash.
 *
 * @remarks
 * Brightness control does not support yet. so this function always return 1. \n
 * Set function can only use to switch on/off the flash. \n
 * Get function can only use to retrieve on/off state of flash.
 * This API is related to the following feature: %http://tizen.org/feature/camera.back.flash
 *
 * @param[out] max_brightness max brightness value of LED
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED		Not supported device
 */
int device_flash_get_max_brightness(int *max_brightness);

/**
 * @brief Get total amount of physical RAM, in kilobytes
 *
 * @remarks
 *
 * @param[out] total_mem total amount of physical RAM
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_memory_get_total(unsigned int *total_mem);

/**
 * @brief Get available amount of physical RAM, in kilobytes
 *
 * @remarks
 * Available amount is defined by following formula currently.
 * available mem = MemFree+Buffers+Cached+SwapCached-Shmem
 *
 * @param[out] avail_mem available amount of physical RAM
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_memory_get_available(unsigned int *avail_mem);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // __TIZEN_SYSTEM_DEVICE_H__
