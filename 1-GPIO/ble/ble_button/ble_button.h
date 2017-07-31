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


#ifndef BLE_BUTTON_H
#define BLE_BUTTON_H

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

#define BUTTON_UUID_BASE         {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, \
                                0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}
                                //0x0000xxxx-1212-efde-1523-785feabcd123
#define BUTTON_UUID_SERVICE      0x1210
#define BUTTON_UUID_BTN1_CHAR    0x1211
#define BUTTON_UUID_BTN2_CHAR    0x1212

// Forward declaration of the ble_button_t type.
typedef struct ble_button_s ble_button_t;

typedef void (*ble_button_write_handler_t) (ble_button_t * p_button, uint8_t new_state);

typedef struct{
    ble_button_write_handler_t write_handler; /**< Event handler to be called when the LED Characteristic is written. */
} ble_button_init_t;

struct ble_button_s{
    ble_gatts_char_handles_t    btn1_char_handles;   /**< Handles related to Characteristic1. */
    ble_gatts_char_handles_t    btn2_char_handles;   /**< Handles related to Characteristic2. */
    uint16_t                    service_handle;      /**< Handle of LED Button Service (as provided by the BLE stack). */
    uint8_t                     uuid_type;           /**< UUID type for the LED Button Service. */
    uint16_t                    conn_handle;         /**< Handle of the current connection (as provided by the BLE stack). BLE_CONN_HANDLE_INVALID if not in a connection. */
};


//SERVICE INIT
uint32_t ble_button_init(ble_button_t * p_button, const ble_button_init_t * p_button_init);

//HANDLE BLE EVENTS
void ble_button_on_ble_evt(ble_button_t * p_button, ble_evt_t * p_ble_evt);

//NOTIFY
uint32_t ble_button_btn1_notify(ble_button_t * p_button, uint8_t state);
uint32_t ble_button_btn2_notify(ble_button_t * p_button, uint8_t state);


#endif // BLE_button_H__

/** @} */
