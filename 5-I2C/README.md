
### BLE I2C

This example has a single BLE service (Orientation) with three characteristics
to notify the data received by the sensor.

	ORIENTATION_UUID_SERVICE      0x1210
	ORIENTATION_UUID_ANGLE1_CHAR  0x1211
	ORIENTATION_UUID_ANGLE2_CHAR  0x1212
	ORIENTATION_UUID_ANGLE3_CHAR  0x1213

The sensor used in this case is Bosch's 9DOF IMU BNO055. The board reads the
IMU's Euler angles over I2C and sends them over BLE to the corresponding angle
characteristics.
