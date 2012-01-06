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
#include <tizen_error.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_MODULE
 * @{
 */

/**
 * @brief Enumerations of error code for Device.
 */
typedef enum
{
    DEVICE_ERROR_NONE              = TIZEN_ERROR_NONE,                  /**< Successful */
    DEVICE_ERROR_INVALID_PARAMETER = TIZEN_ERROR_INVALID_PARAMETER,   /**< Invalid parameter */
    DEVICE_ERROR_OPERATION_FAILED  = TIZEN_ERROR_SYSTEM_CLASS | 0x12, /**< Operation failed */
} device_error_e;

/**
 * @}
*/

 /**
 * @addtogroup CAPI_SYSTEM_DEVICE_MODULE
 * @{
 */

/**
 * @brief Called when an battery charge percentage changed
 *
 * @param[out] percent       The remaining battery charge percentage (0 ~ 100) 
 * @param[in]  user_data     The user data passed from the callback registration function
 *
 */
typedef void (*device_battery_cb)(int percent, void *user_data); 

/**
 * @brief Gets the battery charge percentage.
 * @details It returns integer value from 0 to 100 that indicates remaining battery charge
 * as a percentage of the maximum level.
 * @remarks In order to be notified when the battery state changes, use system_info_set_changed_cb().
 *
 * @param[out] percent The remaining battery charge percentage (0 ~ 100) 
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_battery_is_full()
 * @see system_info_get_value_int(SYSTEM_INFO_KEY_BATTERY_PERCENTAGE, ...)
 * @see system_info_get_value_int(SYSTEM_INFO_KEY_BATTERY_CHARGE, ...)
 */
int device_battery_get_percent(int *percent);

/**
 * @brief Get charging state
 *
 * @param[out] charging The battery charging state.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 */
int device_battery_is_charging(bool *charging);

/**
 * @brief Set callback to be observing battery charge percentage.
 *
 * @param[in] callback      The callback function to set
 * @param[in] user_data     The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_battery_set_cb(device_battery_cb callback, void* user_data);

/**
 * @brief Unset battery charge percentage callback function.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_battery_unset_cb(void);

/**
 * @brief Checks whether the battery is fully charged.
 * @remarks In order to be notified when the battery state changes, use system_info_set_changed_cb().
 *
 * @param[out] full @c true when the battery is fully charged, otherwise @c false.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_battery_get_percent()
 * @see	system_info_set_changed_cb()
 * @see system_info_get_value_int(SYSTEM_INFO_KEY_BATTERY_PERCENTAGE, ...)
 * @see system_info_get_value_int(SYSTEM_INFO_KEY_BATTERY_CHARGE, ...)
 */
int device_battery_is_full(bool *full);

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
 */
int device_get_max_brightness(int display_index, int *max_brightness);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // __TIZEN_SYSTEM_DEVICE_H__

