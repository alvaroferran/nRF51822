#include "ble_analog.h"                                                             //<-------
#include "ble_srv_common.h"
#include "sdk_common.h"

//CONNECT
static void on_connect(ble_analog_t * p_analog, ble_evt_t * p_ble_evt){             //<-------
    p_analog->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;                     //<-------
}
//DISCONNECT
static void on_disconnect(ble_analog_t * p_analog, ble_evt_t * p_ble_evt){          //<-------
    UNUSED_PARAMETER(p_ble_evt);
    p_analog->conn_handle = BLE_CONN_HANDLE_INVALID;                                //<-------
}
//HANDLE BLE COMMS
void ble_analog_on_ble_evt(ble_analog_t * p_analog, ble_evt_t * p_ble_evt){         //<-------
    switch (p_ble_evt->header.evt_id){
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_analog, p_ble_evt);                                        //<-------
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_analog, p_ble_evt);                                     //<-------
            break;
        case BLE_GATTS_EVT_WRITE:
            break;
        default:
            break;
    }
}




//ADD ADC1 CHAR
static uint32_t adc1_char_add(ble_analog_t * p_analog, const ble_analog_init_t * p_analog_init){   //<-------
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.notify = 1;

    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;
    ble_uuid.type = p_analog->uuid_type;                                            //<-------

    ble_uuid.uuid = ANALOG_UUID_ADC1_CHAR;                                          //<-------

    memset(&attr_md, 0, sizeof(attr_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
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
    return sd_ble_gatts_characteristic_add(p_analog->service_handle,                //<-------
                                           &char_md,
                                           &attr_char_value,
                                           &p_analog->adc1_char_handles);          //<-------
}


//ADD ADC2 CHAR
static uint32_t adc2_char_add(ble_analog_t * p_analog, const ble_analog_init_t * p_analog_init){   //<-------
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.notify = 1;

    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;
    ble_uuid.type = p_analog->uuid_type;                                            //<-------

    ble_uuid.uuid = ANALOG_UUID_ADC2_CHAR;                                          //<-------

    memset(&attr_md, 0, sizeof(attr_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
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
    return sd_ble_gatts_characteristic_add(p_analog->service_handle,                //<-------
                                           &char_md,
                                           &attr_char_value,
                                           &p_analog->adc2_char_handles);          //<-------
}


//INIT SERVICE
uint32_t ble_analog_init(ble_analog_t * p_analog, const ble_analog_init_t * p_analog_init){    //<-------
    uint32_t   err_code;
    ble_uuid_t ble_uuid;
    // Initialize service structure.
    p_analog->conn_handle       = BLE_CONN_HANDLE_INVALID;
    // Add service.

    ble_uuid128_t base_uuid = {ANALOG_UUID_BASE};                                    //<-------

    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_analog->uuid_type);                //<-------
    VERIFY_SUCCESS(err_code);
    ble_uuid.type = p_analog->uuid_type;                                            //<-------

    ble_uuid.uuid = ANALOG_UUID_SERVICE;                                             //<-------

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_analog->service_handle); //<-------
    VERIFY_SUCCESS(err_code);

    // Add characteristics.
    err_code = adc1_char_add(p_analog, p_analog_init);                             //<-------
    VERIFY_SUCCESS(err_code);

    err_code = adc2_char_add(p_analog, p_analog_init);                             //<-------
    VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}

//NOTIFY 1
uint32_t ble_analog_adc1_notify(ble_analog_t * p_analog, uint8_t state){           //<-------
    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(state);
    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;

    params.handle = p_analog->adc1_char_handles.value_handle;                      //<-------

    params.p_data = &state;
    params.p_len = &len;
    return sd_ble_gatts_hvx(p_analog->conn_handle, &params);                       //<-------
}


//NOTIFY 2
uint32_t ble_analog_adc2_notify(ble_analog_t * p_analog, uint8_t state){           //<-------
    ble_gatts_hvx_params_t params;
    uint16_t len = sizeof(state);
    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;

    params.handle = p_analog->adc2_char_handles.value_handle;                      //<-------

    params.p_data = &state;
    params.p_len = &len;
    return sd_ble_gatts_hvx(p_analog->conn_handle, &params);                        //<-------
}



//Array of functions access each one with an index
uint32_t (*ble_analog_adcX_notify[2])(ble_analog_t *, uint8_t) = {
    ble_analog_adc1_notify,
    ble_analog_adc2_notify
};
