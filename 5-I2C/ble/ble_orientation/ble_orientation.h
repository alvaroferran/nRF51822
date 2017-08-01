

#ifndef BLE_ORIENTATION_H
#define BLE_ORIENTATION_H

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"

#define ORIENTATION_UUID_BASE {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, \
                               0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}
                                //0x0000xxxx-1212-efde-1523-785feabcd123 backwards in pairs
#define ORIENTATION_UUID_SERVICE      0x1210
#define ORIENTATION_UUID_ANGLE1_CHAR  0x1211
#define ORIENTATION_UUID_ANGLE2_CHAR  0x1212
#define ORIENTATION_UUID_ANGLE3_CHAR  0x1213

// Forward declaration of the ble_orientation_t type.
typedef struct ble_orientation_s ble_orientation_t;

typedef void (*ble_orientation_write_handler_t) (ble_orientation_t * p_orientation, uint8_t new_state);

typedef struct{
    ble_orientation_write_handler_t write_handler; /**< Event handler to be called when the LED Characteristic is written. */
} ble_orientation_init_t;


struct ble_orientation_s{
    ble_gatts_char_handles_t    angle1_char_handles;  /**< Handles related to Characteristic1. */
    ble_gatts_char_handles_t    angle2_char_handles;  /**< Handles related to Characteristic2. */
    ble_gatts_char_handles_t    angle3_char_handles;  /**< Handles related to Characteristic3. */
    uint16_t                    service_handle;      /**< Handle of LED Button Service (as provided by the BLE stack). */
    uint8_t                     uuid_type;           /**< UUID type for the LED Button Service. */
    uint16_t                    conn_handle;         /**< Handle of the current connection (as provided by the BLE stack). BLE_CONN_HANDLE_INVALID if not in a connection. */
};


//SERVICE INIT
uint32_t ble_orientation_init(ble_orientation_t * p_orientation, const ble_orientation_init_t * p_orientation_init);

//HANDLE BLE EVENTS
void ble_orientation_on_ble_evt(ble_orientation_t * p_orientation, ble_evt_t * p_ble_evt);

//NOTIFY
uint32_t ble_orientation_notify(ble_orientation_t * p_orientation, uint8_t button_state);

#endif

/** @} */
