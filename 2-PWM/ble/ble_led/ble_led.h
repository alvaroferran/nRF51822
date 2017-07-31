/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */


#ifndef BLE_LED_H
#define BLE_LED_H

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

#define LED_UUID_BASE          {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, \
                                    0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}
                                    //0x0000xxxx-1212-efde-1523-785feabcd123
#define LED_UUID_SERVICE       0x1310
#define LED_UUID_LED1          0x1311
#define LED_UUID_LED2          0x1312

// Forward declaration of the ble_led_t type.
typedef struct ble_led_s ble_led_t;

typedef void (*ble_led_char_write_handler_t) (ble_led_t * p_led, uint8_t new_state);

/** @brief LED Button Service init structure. This structure contains all options and data needed for
 *        initialization of the service.*/
typedef struct{
    ble_led_char_write_handler_t led1_write_handler; /**< Event handler to be called when the LED1 Characteristic is written. */
    ble_led_char_write_handler_t led2_write_handler; /**< Event handler to be called when the LED2 Characteristic is written. */
} ble_led_init_t;

/**@brief LED Button Service structure. This structure contains various status information for the service. */
struct ble_led_s{
    uint16_t                     service_handle;        /**< Handle of LED Button Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t     led1_handles;          /**< Handles related to the LED1 Characteristic. */
    ble_gatts_char_handles_t     led2_handles;          /**< Handles related to the LED2 Characteristic. */
    ble_led_char_write_handler_t led1_write_handler;    /**< Event handler to be called when the LED1 Characteristic is written. */
    ble_led_char_write_handler_t led2_write_handler;    /**< Event handler to be called when the LED2 Characteristic is written. */
    uint8_t                      uuid_type;             /**< UUID type for the LED Button Service. */
    uint16_t                     conn_handle;           /**< Handle of the current connection (as provided by the BLE stack). BLE_CONN_HANDLE_INVALID if not in a connection. */
};

/**@brief Function for initializing the LED Button Service.
 *
 * @param[out] p_led      LED Button Service structure. This structure must be supplied by
 *                        the application. It is initialized by this function and will later
 *                        be used to identify this particular service instance.
 * @param[in] p_led_init  Information needed to initialize the service.
 *
 * @retval NRF_SUCCESS If the service was initialized successfully. Otherwise, an error code is returned.
 */
uint32_t ble_led_init(ble_led_t * p_led, const ble_led_init_t * p_led_init);

/**@brief Function for handling the application's BLE stack events.
 *
 * @details This function handles all events from the BLE stack that are of interest to the LED Button Service.
 *
 * @param[in] p_led      LED Button Service structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
void ble_led_on_ble_evt(ble_led_t * p_led, ble_evt_t * p_ble_evt);


#endif // BLE_led_H__

/** @} */
