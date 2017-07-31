
#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "bleStartup.h"


int led1=23, led2=24;
int btn1=17, btn2=18;


void toggleLed(int pin, int state){
    if (state) nrf_gpio_pin_clear(pin);
    else nrf_gpio_pin_set(pin);
}
// CALLBACK TO WRITE TO LED1
void led1_write_handler(ble_led_t * p_led, uint8_t led_value){
    toggleLed(led1, led_value);
}
// CALLBACK TO WRITE TO LED2
void led2_write_handler(ble_led_t * p_led, uint8_t led_value){
    toggleLed(led2, led_value);
}



int main(void){

    //Set GPIO
    nrf_gpio_cfg_output(led1);
    nrf_gpio_cfg_output(led2);
    nrf_gpio_pin_set(led1);
    nrf_gpio_pin_set(led2);
    nrf_gpio_cfg_input(btn1,NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(btn2,NRF_GPIO_PIN_PULLUP);

    //Start BLE
    start_ble();

    int stateOld1=0, stateOld2=0;
    while(1){
        int state1= nrf_gpio_pin_read(btn1)==0 ? 1 : 0;
        int state2= nrf_gpio_pin_read(btn2)==0 ? 1 : 0;
        if(state1!= stateOld1)
            ble_button_btn1_notify(&m_button, state1);
        if(state2!= stateOld2)
            ble_button_btn2_notify(&m_button, state2);
        stateOld1=state1;
        stateOld2=state2;
    }
}
