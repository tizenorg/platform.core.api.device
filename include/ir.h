/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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


#ifndef __TIZEN_SYSTEM_IR_H__
#define __TIZEN_SYSTEM_IR_H__

#include "device-error.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @addtogroup CAPI_SYSTEM_DEVICE_IR_MODULE
 * @{
 */

/**
 * @brief Gets the information whether IR module is available.
 *
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/ir
 *
 * @param[out] available The information whether IR is available.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value.
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED      Not supported device
 */
int device_ir_is_available(bool *available);

/**
 * @brief Transmits IR command.
 *
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/ir
 *
 * @param[in] carrier_frequency Carrier frequency to transmit IR command. (Hertz)
 * @param[in] pattern Integer array of IR command.
 * @param[in] size Size of IR command pattern.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value.
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED      Not supported device
 */
int device_ir_transmit(int carrier_frequency, int *pattern, int size);

/**
 * @brief Get available carrier frequency list.
 *
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/ir
 *
 * @param[out] carrier_frequency Available carrier frequency list. (Hertz)
 * @param[out] size Size of carrier frequency list.
 *
 * @return @c 0 on success,
 *         otherwise a negative error value.
 *
 * @retval #DEVICE_ERROR_NONE               Successful
 * @retval #DEVICE_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #DEVICE_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #DEVICE_ERROR_OPERATION_FAILED   Operation failed
 * @retval #DEVICE_ERROR_NOT_SUPPORTED      Not supported device
 */
int device_ir_get_available_frequency(int *carrier_frequency, int *size);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __TIZEN_SYSTEM_IR_H__ */
