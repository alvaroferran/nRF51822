
#include <stdint.h>
#include <string.h>
#include "app_util_platform.h"
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_drv_adc.h"
#include "nrf_log.h"


#define ADC_NUM_CHANNELS 2
static nrf_adc_value_t adc_buffer[ADC_NUM_CHANNELS];

enum analogPins {
	P0_26 = NRF_ADC_CONFIG_INPUT_0,
	P0_27 = NRF_ADC_CONFIG_INPUT_1,
	P0_01 = NRF_ADC_CONFIG_INPUT_2,
	P0_02 = NRF_ADC_CONFIG_INPUT_3,
	P0_03 = NRF_ADC_CONFIG_INPUT_4,
	P0_04 = NRF_ADC_CONFIG_INPUT_5,
	P0_05 = NRF_ADC_CONFIG_INPUT_6,
	P0_06 = NRF_ADC_CONFIG_INPUT_7
};


static void adc_event_handler(nrf_drv_adc_evt_t const * p_event){
	nrf_drv_adc_buffer_convert(adc_buffer, ADC_NUM_CHANNELS);
    if (p_event->type == NRF_DRV_ADC_EVT_DONE){
        for (int i = 0; i < p_event->data.done.size; i++){
            int value= p_event->data.done.p_buffer[i];
            NRF_LOG_PRINTF("Channel %d : %d  \t",i, value);
	    }
        NRF_LOG_PRINTF("\r\n \0");
    }
}


static void adc_config(void){
    nrf_drv_adc_config_t config = NRF_DRV_ADC_DEFAULT_CONFIG;
	nrf_drv_adc_init(&config, adc_event_handler);

    // Declare channels used
    static nrf_drv_adc_channel_t channel1 = NRF_DRV_ADC_DEFAULT_CHANNEL(P0_01);
    static nrf_drv_adc_channel_t channel2 = NRF_DRV_ADC_DEFAULT_CHANNEL(P0_02);

    // Change default configuration if needed
    channel1.config.config.input     = NRF_ADC_CONFIG_SCALING_INPUT_ONE_THIRD;
    channel1.config.config.reference = NRF_ADC_CONFIG_REF_SUPPLY_ONE_THIRD;
    channel2.config.config.input     = NRF_ADC_CONFIG_SCALING_INPUT_ONE_THIRD;
    channel2.config.config.reference = NRF_ADC_CONFIG_REF_SUPPLY_ONE_THIRD;

    // Enable channels
    nrf_drv_adc_channel_enable(&channel1);
    nrf_drv_adc_channel_enable(&channel2);

	nrf_drv_adc_buffer_convert(adc_buffer, ADC_NUM_CHANNELS);
}


int main(void){
	adc_config();

	NRF_LOG_INIT();
	NRF_LOG_PRINTF("ADC example\r\n");

    while (true){
		nrf_drv_adc_sample();   // Trigger ADC conversion
		nrf_delay_ms(10);
    }
}
