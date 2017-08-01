
#include <stdint.h>
#include <string.h>
#include "app_util_platform.h"
#include "nordic_common.h"
#include "nrf.h"
#include "bleStartup.h"
#include "nrf_drv_twi.h"
#include "BNO055.h"


ret_code_t twi_master_init(void){
    ret_code_t ret_code;
    //Setup SDA and SCL pins, I2C frequency and interrupt priority
    const nrf_drv_twi_config_t config = {
        .scl                = 7,
        .sda                = 30,
        .frequency          = NRF_TWI_FREQ_400K,
        .interrupt_priority = TWI1_CONFIG_IRQ_PRIORITY
    };
    do{
        ret_code = nrf_drv_twi_init(&m_twi, &config, NULL,NULL);
        if(ret_code!= NRF_SUCCESS){
            break;
        }
        nrf_drv_twi_enable(&m_twi);
    }while(0);
    return ret_code;
}


int main(void){
    start_ble();
    twi_master_init();
    BNO055_init();

    while(1){
        BNO055_readEul();
        uint8_t x=(uint8_t) dataBNO055.d0/10;
        uint8_t y=(uint8_t) dataBNO055.d1/10;
        uint8_t z=(uint8_t) dataBNO055.d2/10;
        ble_orientation_angle1_notify(&m_orientation, x);
        ble_orientation_angle2_notify(&m_orientation, y);
        ble_orientation_angle3_notify(&m_orientation, z);
        nrf_delay_ms(10);
    }

}
