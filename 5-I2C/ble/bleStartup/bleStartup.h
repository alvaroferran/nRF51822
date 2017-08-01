
#ifndef BLE_STARTUP_H
#define BLE_STARTUP_H

#include "app_timer.h"
#include "bsp.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "softdevice_handler.h"
#include "ble_gap.h"
#include "ble_parameters.h"

//BLE SERVICES
#include "ble_orientation.h"
ble_orientation_t  m_orientation;    //Orientation Service instance.


static uint16_t      m_conn_handle = BLE_CONN_HANDLE_INVALID; //Handle of the current connection.

void start_ble();
static void advertising_init(void);
static void services_init(void);
static void ble_evt_dispatch(ble_evt_t * p_ble_evt);
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name);
static void timers_init(void);
static void gap_params_init(void);
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt);
static void conn_params_error_handler(uint32_t nrf_error);
static void conn_params_init(void);
static void advertising_start(void);
static void on_ble_evt(ble_evt_t * p_ble_evt);
static void ble_stack_init(void);
void power_manage(void);

#endif
