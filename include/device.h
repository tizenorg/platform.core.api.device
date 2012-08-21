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
    DEVICE_ERROR_NOT_SUPPORTED     = TIZEN_ERROR_SYSTEM_CLASS | 0x13, /**< Not supported in this device */
} device_error_e;

/**
 * @brief Enumerations of the battery warning status
 */
typedef enum
{
    DEVICE_BATTERY_WARN_EMPTY,      /**< The battery goes empty. Prepare for the safe termination of the application, because the device starts a shutdown process soon after entering this level. */
    DEVICE_BATTERY_WARN_CRITICAL,  /**< The battery charge is at a critical state. You may have to stop using multimedia features, because they are not guaranteed to work correctly at this battery status. */
    DEVICE_BATTERY_WARN_LOW,       /**< The battery has little charge left. */
    DEVICE_BATTERY_WARN_NORMAL,    /**< The battery status is not to be careful. */
    DEVICE_BATTERY_WARN_FULL,      /**< The battery status is full. */
} device_battery_warn_e;

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
 * @param[in] percent       The remaining battery charge percentage (0 ~ 100)
 * @param[in] user_data     The user data passed from the callback registration function
 *
 */
typedef void (*device_battery_cb)(int percent, void *user_data); 

/**
 * @brief Called when the device warn about the battery status.
 *
 * @param[in] status       The battery warning status
 * @param[in] user_data    The user data passed from the callback registration function
 *
 */
typedef void (*device_battery_warn_cb)(device_battery_warn_e status, void *user_data);

/**
 * @brief Gets the battery warning status.
 *
 * @param[out] status The battery warning status.
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_battery_status_e
 * @see device_battery_status_set_cb()
 */
int device_battery_get_warning_status(device_battery_warn_e *status);

/**
 * @brief Set callback to be observing battery warning.
 *
 * @param[in] callback      The callback function to set
 * @param[in] user_data     The user data to be passed to the callback function
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 *
 * @see device_battery_status_e
 * @see device_battery_get_status()
 */
int device_battery_warning_set_cb(device_battery_warn_cb callback, void* user_data);

/**
 * @brief Unset battery warning callback function.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 */
int device_battery_warning_unset_cb(void);

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
 * @see device_battery_get_detail()
 * @see device_battery_set_cb()
 */
int device_battery_get_percent(int *percent);

/**
 * @brief Gets the battery detail charge as a per ten thousand.
 * @details It return integer value from 0 to 10000 that indicates remaining battery charge as a per ten thousand of the maximum level.
 * @remarks this function return #DEVICE_ERROR_NOT_SUPPORTED when device can not be supported detail battery information.
 *
 * @param[out] detail   The remaining battery charge as a per ten thousand. (0 ~ 10000)
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED      Not supported device
 *
 * @see device_battery_is_full()
 * @see device_battery_get_percent()
 * @see device_battery_set_cb()
 */
int device_battery_get_detail(int *detail);

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
 * @see device_set_brightness_from_settings()
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
 */
int device_get_max_brightness(int display_index, int *max_brightness);

/**
 * @brief Sets the display brightness value that registed in settings.
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
 */
int device_set_brightness_from_settings(int display_index);

/**
 * @brief Get brightness value of LED that placed to camera flash.
 *
 * @param[out] brightness brightness value of LED (0 ~ MAX)
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_flash_get_brightness(int *brightness);

/**
 * @brief Set brightness value of LED that placed to camera flash.
 *
 * @param[in] brightness brightness value of LED (0 ~ MAX)
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_flash_set_brightness(int brightness);

/**
 * @brief Get max brightness value of LED that placed to camera flash.
 *
 * @remark
 * Brightness control does not support yet. so this functioon always return 1. \n
 * Set function can only use to switch on/off the flash. \n
 * Get function can only use to retrive on/off state of flash.
 *
 * @param[out] max_brightness max brightness value of LED
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #DEVICE_ERROR_NONE				Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED	Operation failed
 */
int device_flash_get_max_brightness(int *max_brightness);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // __TIZEN_SYSTEM_DEVICE_H__
