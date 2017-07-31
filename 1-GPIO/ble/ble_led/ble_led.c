/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the license.txt file.
 */

#include "ble_led.h"                                                                //<-------
#include "ble_srv_common.h"
#include "sdk_common.h"

//CONNECT
static void on_connect(ble_led_t * p_led, ble_evt_t * p_ble_evt){                   //<-------
    p_led->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;                        //<-------
}

//DISCONNECT
static void on_disconnect(ble_led_t * p_led, ble_evt_t * p_ble_evt){                //<-------
    UNUSED_PARAMETER(p_ble_evt);
    p_led->conn_handle = BLE_CONN_HANDLE_INVALID;                                   //<-------
}

//WRITE
static void on_write(ble_led_t * p_led, ble_evt_t * p_ble_evt){                     //<-------
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    //LED1
    if ((p_evt_write->handle == p_led->led1_handles.value_handle) &&                //<-------
        (p_evt_write->len == 1) &&
        (p_led->led1_write_handler != NULL))                                        //<-------
    {
        p_led->led1_write_handler(p_led, p_evt_write->data[0]);                     //<-------
    }
    //LED2
    if ((p_evt_write->handle == p_led->led2_handles.value_handle) &&                //<-------
        (p_evt_write->len == 1) &&
        (p_led->led2_write_handler != NULL))                                        //<-------
    {
        p_led->led2_write_handler(p_led, p_evt_write->data[0]);                     //<-------
    }

}



//HANDLE BLE COMMS
void ble_led_on_ble_evt(ble_led_t * p_led, ble_evt_t * p_ble_evt){                  //<-------
    switch (p_ble_evt->header.evt_id){
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_led, p_ble_evt);                                           //<-------
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_led, p_ble_evt);                                        //<-------
            break;
        case BLE_GATTS_EVT_WRITE:
            on_write(p_led, p_ble_evt);                                             //<-------
            break;
        default:
            break;
    }
}



//ADD LED1
static uint32_t led1_char_add(ble_led_t * p_led, const ble_led_init_t * p_led_init){   //<-------
    ble_gatts_char_md_t char_md;    //characteristic metadata
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;  //char properties
    char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;
    ble_uuid.type = p_led->uuid_type;                                               //<-------

    ble_uuid.uuid = LED_UUID_LED1;                                                  //<-------
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(uint8_t);
    attr_char_value.p_value      = NULL;
    return sd_ble_gatts_characteristic_add(p_led->service_handle,                   //<-------
                                           &char_md,
                                           &attr_char_value,
                                           &p_led->led1_handles);                   //<-------
}


//ADD LED2
static uint32_t led2_char_add(ble_led_t * p_led, const ble_led_init_t * p_led_init){   //<-------
    ble_gatts_char_md_t char_md;    //characteristic metadata
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;  //char properties
    char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = NULL;
    char_md.p_sccd_md         = NULL;
    ble_uuid.type = p_led->uuid_type;                                               //<-------

    ble_uuid.uuid = LED_UUID_LED2;                                                  //<-------
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    attr_md.vloc       = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth    = 0;
    attr_md.wr_auth    = 0;
    attr_md.vlen       = 0;
    memset(&attr_char_value, 0, sizeof(attr_char_value));
    attr_char_value.p_uuid       = &ble_uuid;
    attr_char_value.p_attr_md    = &attr_md;
    attr_char_value.init_len     = sizeof(uint8_t);
    attr_char_value.init_offs    = 0;
    attr_char_value.max_len      = sizeof(uint8_t);
    attr_char_value.p_value      = NULL;
    return sd_ble_gatts_characteristic_add(p_led->service_handle,                   //<-------
                                           &char_md,
                                           &attr_char_value,
                                           &p_led->led2_handles);                   //<-------
}



//INIT SERVICE
uint32_t ble_led_init(ble_led_t * p_led, const ble_led_init_t * p_led_init){        //<-------
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure.
    p_led->conn_handle       = BLE_CONN_HANDLE_INVALID;                             //<-------
    p_led->led1_write_handler = p_led_init->led1_write_handler;                     //<-------
    p_led->led2_write_handler = p_led_init->led2_write_handler;                     //<-------

    // Add service.
    ble_uuid128_t base_uuid = {LED_UUID_BASE};                                      //<-------
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_led->uuid_type);                   //<-------
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = p_led->uuid_type;
    ble_uuid.uuid = LED_UUID_SERVICE;                                               //<-------

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_led->service_handle);    //<-------
    VERIFY_SUCCESS(err_code);

    // Add characteristics.
    err_code = led1_char_add(p_led, p_led_init);                                    //<-------
    VERIFY_SUCCESS(err_code);

    err_code = led2_char_add(p_led, p_led_init);                                    //<-------
    VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}
