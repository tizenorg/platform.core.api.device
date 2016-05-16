/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_SYSTEM_BATTERY_H__
#define __TIZEN_SYSTEM_BATTERY_H__

#include <stdbool.h>
#include "device-error.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_BATTERY_MODULE
 * @{
 */

/**
 * @brief Enumeration for the battery level status.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 */
typedef enum {
    DEVICE_BATTERY_LEVEL_EMPTY = 0,  /**< The battery goes empty. Prepare for the safe termination of the application, because the device starts a shutdown process soon after entering this level. */
    DEVICE_BATTERY_LEVEL_CRITICAL,   /**< The battery charge is at a critical state. You may have to stop using multimedia features, because they are not guaranteed to work correctly at this battery status. */
    DEVICE_BATTERY_LEVEL_LOW,        /**< The battery has little charge left. */
    DEVICE_BATTERY_LEVEL_HIGH,       /**< The battery status is not to be careful. */
    DEVICE_BATTERY_LEVEL_FULL,       /**< The battery status is full. */
} device_battery_level_e;

/**
 * @brief Enumeration for battery health information.
 * @since_tizen 3.0
 */
typedef enum {
	DEVICE_BATTERY_HEALTH_GOOD,        /**< The battery health is good */
	DEVICE_BATTERY_HEALTH_COLD,        /**< The temperature of the battery is cold */
	DEVICE_BATTERY_HEALTH_DEAD,        /**< The battery is dead */
	DEVICE_BATTERY_HEALTH_OVER_HEAT,   /**< The temperature of the battery is high */
	DEVICE_BATTERY_HEALTH_OVER_VOLTAGE,/**< The battery is in over voltage state */
	DEVICE_BATTERY_HEALTH_UNKNOWN,     /**< The battery health is unknown */
	DEVICE_BATTERY_HEALTH_UNSPECIFIED_FAILURE, /**< Unspecified failure is occurred during getting the battery health information */
} device_battery_health_e;

/**
 * @brief Enumeration for power source information.
 * @since_tizen 3.0
 */
typedef enum {
	DEVICE_BATTERY_POWER_SOURCE_NONE,    /**< There is no power source */
	DEVICE_BATTERY_POWER_SOURCE_AC,      /**< AC power cable is connected */
	DEVICE_BATTERY_POWER_SOURCE_USB,     /**< USB power cable is connected */
	DEVICE_BATTERY_POWER_SOURCE_WIRELESS,/**< Power is provided by wireless manner */
} device_battery_power_source_e;

/**
 * @brief Enumeration for battery property information.
 * @since_tizen 3.0
 */
typedef enum {
	DEVICE_BATTERY_PROPERTY_CAPACITY,       /**< Battery capacity */
	DEVICE_BATTERY_PROPERTY_CURRENT_NOW,    /**< Current of battery */
	DEVICE_BATTERY_PROPERTY_CURRENT_AVERAGE,/**< Average current of battery */
} device_battery_property_e;

/**
 * @brief Enumeration for battery status information.
 * @since_tizen 3.0
 */
typedef enum {
	DEVICE_BATTERY_STATUS_CHARGING,     /**< Battery is charging */
	DEVICE_BATTERY_STATUS_DISCHARGING,  /**< Battery is discharging */
	DEVICE_BATTERY_STATUS_FULL,         /**< Battery is fully charged */
	DEVICE_BATTERY_STATUS_NOT_CHARGING, /**< Battery is not charging */
	DEVICE_BATTERY_STATUS_UNKNOWN,      /**< Battery status is unknown */
} device_battery_status_e;

/**
 * @brief Gets the battery charge percentage.
 * @details It returns an integer value from @c 0 to @c 100 that indicates remaining battery charge
 *          as a percentage of the maximum level.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @remarks In order to be notified when the battery state changes, use system_info_set_changed_cb().
 *
 * @param[out] percent The remaining battery charge percentage (@c 0 ~ @c 100)
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 */
int device_battery_get_percent(int *percent);

/**
 * @brief Gets the charging state.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[out] charging The battery charging state
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 *
 * @see device_add_callback
 * @see device_remove_callback
 * @see #DEVICE_CALLBACK_BATTERY_CHARGING
 */
int device_battery_is_charging(bool *charging);

/**
 * @brief Gets the battery level status.
 *
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 *
 * @param[out] status The battery level status
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 *
 * @see device_battery_level_e
 * @see device_add_callback
 * @see device_remove_callback
 * @see #DEVICE_CALLBACK_BATTERY_LEVEL
 */
int device_battery_get_level_status(device_battery_level_e *status);

/**
 * @brief Gets the battery health information.
 *
 * @since_tizen 3.0
 *
 * @param[out] health The battery health information
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 */
int device_battery_get_health(device_battery_health_e *health);

/**
 * @brief Gets the battery power source information.
 *
 * @since_tizen 3.0
 *
 * @param[out] source The battery power source information
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 */
int device_battery_get_power_source(device_battery_power_source_e *source);

/**
 * @brief Gets the battery properties.
 *
 * @since_tizen 3.0
 *
 * @param[in] property The property type for getting information
 * @param[out] value The battery information for the property given
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 */
int device_battery_get_property(device_battery_property_e property, int *value);

/**
 * @brief Gets the battery status information.
 *
 * @since_tizen 3.0
 *
 * @param[out] source The battery status information
 *
 * @return @c 0 on success,
 *         otherwise a negative error value
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 */
int device_battery_get_status(device_battery_status_e *status);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  // __TIZEN_SYSTEM_BATTERY_H__
