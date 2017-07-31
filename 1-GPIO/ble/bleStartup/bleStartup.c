
#include "bleStartup.h"

/*----------------------------------------------------------------------------*/
/*--  FUNCTIONS THAT NEED TO BE UPDATED WITH SERVICES AND CHARACTERISTICS   --*/
/*----------------------------------------------------------------------------*/


// If BT_LED_ON is 1 BT connection status shown with leds
#define BT_LED_ON 1

void led1_write_handler(ble_led_t * , uint8_t);     //<-------
void led2_write_handler(ble_led_t * , uint8_t);     //<-------


//ADVERTISING INIT
static void advertising_init(void){
    uint32_t      err_code;
    ble_advdata_t advdata;
    ble_advdata_t scanrsp;
    ble_uuid_t adv_uuids[] = {
                                {BUTTON_UUID_BASE ,BLE_UUID_TYPE_BLE},  //<-------
                                {LED_UUID_BASE    ,BLE_UUID_TYPE_BLE}   //<-------
                            };
    // Build and set advertising data
    memset(&advdata, 0, sizeof(advdata));
    advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = true;
    advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    memset(&scanrsp, 0, sizeof(scanrsp));
    scanrsp.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    scanrsp.uuids_complete.p_uuids  = adv_uuids;
    err_code = ble_advdata_set(&advdata, &scanrsp);
    APP_ERROR_CHECK(err_code);
}



//INIT SERVICES
static void services_init(void){
    uint32_t       err_code;
    ble_button_init_t button_init;
    ble_led_init_t led_init;                             //<-------
    led_init.led1_write_handler=led1_write_handler;      //<-------
    led_init.led2_write_handler=led2_write_handler;      //<-------
    err_code = ble_button_init(&m_button, &button_init); //<-------
    APP_ERROR_CHECK(err_code);
    err_code = ble_led_init(&m_led, &led_init);          //<-------
    APP_ERROR_CHECK(err_code);
}



//DISPATCH BLE EVENT TO ALL HANDLERS
static void ble_evt_dispatch(ble_evt_t * p_ble_evt){
    on_ble_evt(p_ble_evt);
    ble_conn_params_on_ble_evt(p_ble_evt);
    ble_button_on_ble_evt(&m_button, p_ble_evt);      //<-------
    ble_led_on_ble_evt(&m_led, p_ble_evt);            //<-------
}






/*----------------------------------------------------------------------------*/
/*--                          STANDARD FUNCTIONS                            --*/
/*----------------------------------------------------------------------------*/



#if (BT_LED_ON==1)
    #define LED_ADV 21
    #define LED_CONN 22
#endif



void start_ble(){
    #if (BT_LED_ON==1)
        nrf_gpio_cfg_output(LED_ADV);
        nrf_gpio_cfg_output(LED_CONN);
        nrf_gpio_pin_set(LED_ADV);
        nrf_gpio_pin_set(LED_CONN);
    #endif
    // Initialize.
    timers_init();
    ble_stack_init();
    gap_params_init();
    services_init();
    advertising_init();
    conn_params_init();
    // Start execution.
    advertising_start();
}



//ASSERT CALLBACK
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name){
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}



//TIMERS INIT
static void timers_init(void){
    // Initialize timer module, making it use the scheduler
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
}



//GAP INIT
static void gap_params_init(void){
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);
    err_code = sd_ble_gap_device_name_set(&sec_mode,(const uint8_t *)DEVICE_NAME,strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);
    memset(&gap_conn_params, 0, sizeof(gap_conn_params));
    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;
    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}



//DISCONNECT IF CONNECTION FAILED
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt){
    uint32_t err_code;
    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED){
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}



//HANDLE CONNECTION ERRORS
static void conn_params_error_handler(uint32_t nrf_error){
    APP_ERROR_HANDLER(nrf_error);
}



//INIT CONNECTION
static void conn_params_init(void){
    uint32_t err_code;
    ble_conn_params_init_t cp_init;
    memset(&cp_init, 0, sizeof(cp_init));
    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;
    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}



//ADVERTISING START
static void advertising_start(void){
    uint32_t             err_code;
    ble_gap_adv_params_t adv_params;
    // Start advertising
    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;
    adv_params.p_peer_addr = NULL;
    adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    adv_params.interval    = APP_ADV_INTERVAL;
    adv_params.timeout     = APP_ADV_TIMEOUT_IN_SECONDS;
    err_code = sd_ble_gap_adv_start(&adv_params);
    APP_ERROR_CHECK(err_code);
    #if (BT_LED_ON==1)
        nrf_gpio_pin_clear(LED_ADV);
        nrf_gpio_pin_set(LED_CONN);
    #endif
}



//HANDLE BLE EVENTS
static void on_ble_evt(ble_evt_t * p_ble_evt){
    uint32_t err_code;
    switch (p_ble_evt->header.evt_id){
        case BLE_GAP_EVT_CONNECTED:
            #if (BT_LED_ON==1)
                nrf_gpio_pin_clear(LED_CONN);
                nrf_gpio_pin_set(LED_ADV);
            #endif
            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
            err_code = app_button_enable();
            APP_ERROR_CHECK(err_code);
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            err_code = app_button_disable();
            APP_ERROR_CHECK(err_code);
            advertising_start();
            break;
        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            // Pairing not supported
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle,
                                                   BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP,
                                                   NULL,
                                                   NULL);
            APP_ERROR_CHECK(err_code);
            break;
        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            // No system attributes have been stored.
            err_code = sd_ble_gatts_sys_attr_set(m_conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;
        default:
            break;
    }
}


//BLE STACK INIT
static void ble_stack_init(void){
    uint32_t err_code;
    nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;
    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);
    ble_enable_params_t ble_enable_params;
    err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                    PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);
    APP_ERROR_CHECK(err_code);
    //Check the ram settings against the used number of links
    CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT,PERIPHERAL_LINK_COUNT);
    // Enable BLE stack.
    err_code = softdevice_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);
    ble_gap_addr_t addr;
    err_code = sd_ble_gap_address_get(&addr);
    APP_ERROR_CHECK(err_code);
    err_code = sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_NONE, &addr);
    APP_ERROR_CHECK(err_code);
    // Subscribe for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}



//MANAGE POWER WHILE WAITING FOR A BLE EVENT
void power_manage(void){
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}
