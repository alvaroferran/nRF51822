
### BLE PWM

This example has a single BLE service (Led) with two characteristics to control
the LEDs on pins 23 and 24.

	LED_UUID_SERVICE       0x1310
	LED_UUID_LED1          0x1311
	LED_UUID_LED2          0x1312

The nRF51 DK board has three timers available: TIMER0, TIMER1 & TIMER2.
This means that there can be up to three PWM instances, each with two channels,
so up to six PWM pins.

However, TIMER0 is used by the BLE stack, so only four pins are available while
using it.

The timers used to create the PWM instances must be defined in the file
**config/nrf_drv_config.h** such as **#define TIMERx_ENABLED 1**

In this example LEDs 1 and 2 are increasing and decreasing their intensities in
an infinite loop and LEDs 3 and 4 are controlled over BLE, with their duty cycle
set as soon as the percentage is received.
