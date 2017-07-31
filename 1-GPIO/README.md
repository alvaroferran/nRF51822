
### BLE Input Output

This example has two BLE services with two characteristics
each.
The first service is Button with the following UUIDs:

	BUTTON_UUID_SERVICE      0x1210
	BUTTON_UUID_BTN1_CHAR    0x1211
	BUTTON_UUID_BTN2_CHAR    0x1212

The BTN1 and BTN2 characteristics are set in notifying mode, so their values are
sent automatically as soon as the buttons on pins 17 and 18 change states.

The second service is Led with the UUIDs:

	LED_UUID_SERVICE       0x1310
	LED_UUID_LED1          0x1311
	LED_UUID_LED2          0x1312

Here the two characteristics LED1 and LED2 are writable and they turn on or off
the LEDs on pins 23 and 24 respectively.
