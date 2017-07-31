
### BLE ADC

This example has a single BLE service (Analog) with two characteristics reading
from the ADC on pins 1 and 2. This service has an array with the different
notification functions to make the code cleaner by being able to select
each *_notify()* programmatically.

    ANALOG_UUID_SERVICE      0x1210
    ANALOG_UUID_ADC1_CHAR    0x1211
    ANALOG_UUID_ADC2_CHAR    0x1212

The ADC has eight channels that can be read simultaneously. The channels are
selected with the corresponding NRF_ADC_CONFIG_INPUT_x, but to simplify the
selection the pin number can also be used. The ADC inputs are connected to the
GPIOs as follows:

    NRF_ADC_CONFIG_INPUT_0 = P0.26
    NRF_ADC_CONFIG_INPUT_1 = P0.27
    NRF_ADC_CONFIG_INPUT_2 = P0.01
    NRF_ADC_CONFIG_INPUT_3 = P0.02
    NRF_ADC_CONFIG_INPUT_4 = P0.03
    NRF_ADC_CONFIG_INPUT_5 = P0.04
    NRF_ADC_CONFIG_INPUT_6 = P0.05
    NRF_ADC_CONFIG_INPUT_7 = P0.06

The channels are selected with the default parameters, which can be changed
if needed. This initial configuration is:

    .config.config.resolution = NRF_ADC_CONFIG_RES_10BIT;
    .config.config.input =      NRF_ADC_CONFIG_SCALING_INPUT_FULL_SCALE;
    .config.config.reference =  NRF_ADC_CONFIG_REF_VBG;
