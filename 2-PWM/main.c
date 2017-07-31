
#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "bleStartup.h"
#include "app_pwm.h"


double map(double x, double in_min, double in_max, double out_min, double out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



// PWM callback functions
static volatile bool pwmReady1; // A flag indicating PWM status.
void pwmCallback1(uint32_t pwm_id1){pwmReady1=true;}
static volatile bool pwmReady2;
void pwmCallback2(uint32_t pwm_id2){pwmReady2=true;}
/* Create the instance "PWMx" using TIMER1 or 2. (TIMER0 used by BT, TIMER3 and TIMER4 not present in nRF51)*/
/* Maximum of 6 PWM pins available, 4 if using BT */
/* Set TIMERx to 1 in nrf_drv_config.h  */
APP_PWM_INSTANCE(PWM1,1);
APP_PWM_INSTANCE(PWM2,2);




// CALLBACK TO WRITE TO LED1
void led1_write_handler(ble_led_t * p_led, uint8_t led_value){
    while (app_pwm_channel_duty_set(&PWM2, 0, led_value) == NRF_ERROR_BUSY);
}
// CALLBACK TO WRITE TO LED2
void led2_write_handler(ble_led_t * p_led, uint8_t led_value){
    while (app_pwm_channel_duty_set(&PWM2, 1, led_value) == NRF_ERROR_BUSY);
}



int main(void){

    //Start BLE
    start_ble();

    int pin1=21, pin2=22, pin3=23, pin4=24;
    int period_us=10;

    // app_pwm_config_t pwm1 = APP_PWM_DEFAULT_CONFIG_1CH(period_us, pin1); // One or two channels per PWM
    app_pwm_config_t pwm1 = APP_PWM_DEFAULT_CONFIG_2CH(period_us, pin1, pin2);
    ret_code_t err1 = app_pwm_init(&PWM1,&pwm1,pwmCallback1);
    APP_ERROR_CHECK(err1);
    app_pwm_enable(&PWM1);

    app_pwm_config_t pwm2 = APP_PWM_DEFAULT_CONFIG_2CH(period_us, pin3, pin4);
    ret_code_t err2 = app_pwm_init(&PWM2,&pwm2,pwmCallback2);
    APP_ERROR_CHECK(err2);
    app_pwm_enable(&PWM2);

    int dms=20;

    while(1){
        for (int i=0; i<100; i++){
            while (app_pwm_channel_duty_set(&PWM1, 0, i) == NRF_ERROR_BUSY); // Set PWM until succesful
            while (app_pwm_channel_duty_set(&PWM1, 1, map(i,0,100,100,0)) == NRF_ERROR_BUSY);
            nrf_delay_ms(dms);
        }
        for (int i=100; i>0; i--){
            while (app_pwm_channel_duty_set(&PWM1, 0, i) == NRF_ERROR_BUSY);
            while (app_pwm_channel_duty_set(&PWM1, 1, map(i,0,100,100,0)) == NRF_ERROR_BUSY);
            nrf_delay_ms(dms);
        }
    }
}
