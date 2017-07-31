
### UART

This example builds on the previous one (ADC) but uses the UART to print the ADC's
values instead.


The	NRF_LOG_INIT() macro starts a connection with the following parameters:

    Port:           /dev/ttyACMx
    Baud Rate:      115200
    Parity:         none
    Bits:           8
    Stop bits:      1
    Flow control:   none

The *linker_nrf51.ld* has been kept the same as in the other examples, so the
application code is uploaded to the memory zone at the end of the SoftDevice
stack'sinstead of replacing it, so there is no need to run *make flash_softdevice*
before uploading other examples.
