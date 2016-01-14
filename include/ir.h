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
 * @return @c TRUE if IR is available,
 *         otherwise return FALSE.
 *
 */
bool device_ir_is_available();

/**
 * @brief Transmits IR command.
 *
 * @since_tizen 3.0
 * @privlevel public
 * @privilege %http://tizen.org/privilege/ir
 *
 * @param[in] carrier_frequency Carrier frequency to transmit IR command.
 * @param[in] pattern Integer array of IR command.
 * @param[in] size Size of IR command pattern.
 *
 * @return
 *
 */
void device_ir_transmit(int carrier_frequency, int *pattern, int size);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif  /* __TIZEN_SYSTEM_IR_H__ */