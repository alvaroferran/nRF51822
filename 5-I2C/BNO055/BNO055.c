//--------------------------------------------------------------
//--    BNO055
//--    IMU 9DOF Sensor Fusion
//--------------------------------------------------------------
//--    Adapted for the NFR51822
//--------------------------------------------------------------
//--    Library created by
//--        Alvaro Ferran (alvaroferran)
//--    based on the work of
//--        Kris Winer (https://github.com/kriswiner/BNO-055)
//--------------------------------------------------------------
//--    Released on September 2015
//--    under the GPL v2
//--------------------------------------------------------------


#include "BNO055.h"



static void BNO055_writeByte(uint8_t subAddress, uint8_t value){
    uint8_t data[2] = {subAddress,value};
    nrf_drv_twi_tx(&m_twi, BNO055_ADDRESS, &data, 2, false);
}



static void BNO055_readBytes(uint8_t subAddress, uint8_t count, uint8_t * dest){
    nrf_drv_twi_tx(&m_twi, BNO055_ADDRESS, &subAddress, 1, true);  //keep connection alive
    nrf_drv_twi_rx(&m_twi, BNO055_ADDRESS, &dest[0], count);
}



void BNO055_init() {
    GPwrMode = NormalG;    // Gyro power mode
    Gscale = GFS_250DPS;  // Gyro full scale
    //Godr = GODR_250Hz;    // Gyro sample rate
    Gbw = GBW_23Hz;       // Gyro bandwidth
    Ascale = AFS_2G;      // Accel full scale
    //Aodr = AODR_250Hz;    // Accel sample rate
    APwrMode = NormalA;    // Accel power mode
    Abw = ABW_31_25Hz;    // Accel bandwidth, accel sample rate divided by ABW_divx
    //Mscale = MFS_4Gauss;  // Select magnetometer full-scale resolution
    MOpMode = Regular;    // Select magnetometer perfomance mode
    MPwrMode = Normal;    // Select magnetometer power mode
    Modr = MODR_10Hz;     // Select magnetometer ODR when in BNO055 bypass mode
    PWRMode = Normalpwr;    // Select BNO055 power mode
    OPRMode = NDOF;       //

    // Select BNO055 config mode
    BNO055_writeByte(BNO055_OPR_MODE, CONFIGMODE );
    nrf_delay_ms(25);
    // Select page 1 to configure sensors
    BNO055_writeByte(BNO055_PAGE_ID, 0x01);
    // Configure ACC
    BNO055_writeByte(BNO055_ACC_CONFIG, APwrMode << 5 | Abw << 3 | Ascale );
    // Configure GYR
    BNO055_writeByte(BNO055_GYRO_CONFIG_0, Gbw << 3 | Gscale );
    BNO055_writeByte(BNO055_GYRO_CONFIG_1, GPwrMode);
    // Configure MAG
    BNO055_writeByte(BNO055_MAG_CONFIG, MPwrMode << 5 | MOpMode << 3 | Modr );
    // Select page 0 to read sensors
    BNO055_writeByte(BNO055_PAGE_ID, 0x00);
    // Select BNO055 gyro temperature source
    BNO055_writeByte(BNO055_TEMP_SOURCE, 0x01 );
    // Select BNO055 sensor units (temperature in degrees C, rate in dps, accel in mg)
    BNO055_writeByte(BNO055_UNIT_SEL, 0x01 );
    // Select BNO055 system power mode
    BNO055_writeByte(BNO055_PWR_MODE, PWRMode );
    // Select BNO055 system operation mode
    BNO055_writeByte(BNO055_OPR_MODE, OPRMode );
    nrf_delay_ms(25);
 }



void BNO055_readEul(){
    uint8_t rawData[6];
    BNO055_readBytes(BNO055_EUL_HEADING_LSB, 6, &rawData[0]);
    dataBNO055.intD0 = ((int16_t)rawData[1] << 8) | rawData[0] ;  // Turn the MSB and
    dataBNO055.intD1 = ((int16_t)rawData[3] << 8) | rawData[2] ;  //LSB into a signed
    dataBNO055.intD2 = ((int16_t)rawData[5] << 8) | rawData[4] ;  //16-bit value
    dataBNO055.d0= (double)dataBNO055.intD0/16.;                  //X
    dataBNO055.d1= (double)dataBNO055.intD1/16.;                  //Y
    dataBNO055.d2= (double)dataBNO055.intD2/16.;                  //Z
}



void BNO055_readQuat(){
    uint8_t rawData[8];
    BNO055_readBytes(BNO055_QUA_DATA_W_LSB, 8, &rawData[0]);
    dataBNO055.intD0 = ((int16_t)rawData[1] << 8) | rawData[0] ;
    dataBNO055.intD1 = ((int16_t)rawData[3] << 8) | rawData[2] ;
    dataBNO055.intD2 = ((int16_t)rawData[5] << 8) | rawData[4] ;
    dataBNO055.intD3 = ((int16_t)rawData[7] << 8) | rawData[6] ;
    dataBNO055.d0= (double)dataBNO055.intD0/16384.;               //Q0
    dataBNO055.d1= (double)dataBNO055.intD1/16384.;               //Q1
    dataBNO055.d2= (double)dataBNO055.intD2/16384.;               //Q2
    dataBNO055.d3= (double)dataBNO055.intD3/16384.;               //Q3
}



void BNO055_readAccel(){
    uint8_t rawData[6];
    BNO055_readBytes(BNO055_ACC_DATA_X_LSB, 6, &rawData[0]);
    dataBNO055.intD0 = ((int16_t)rawData[1] << 8) | rawData[0] ;
    dataBNO055.intD1 = ((int16_t)rawData[3] << 8) | rawData[2] ;
    dataBNO055.intD2 = ((int16_t)rawData[5] << 8) | rawData[4] ;
    dataBNO055.d0 = (double)dataBNO055.intD0;                     //X
    dataBNO055.d1 = (double)dataBNO055.intD1;                     //Y
    dataBNO055.d2 = (double)dataBNO055.intD2;                     //Z
}



void BNO055_readGyro(){
    uint8_t rawData[6];
    BNO055_readBytes(BNO055_GYR_DATA_X_LSB, 6, &rawData[0]);
    dataBNO055.intD0 = ((int16_t)rawData[1] << 8) | rawData[0] ;
    dataBNO055.intD1 = ((int16_t)rawData[3] << 8) | rawData[2] ;
    dataBNO055.intD2 = ((int16_t)rawData[5] << 8) | rawData[4] ;
    dataBNO055.d0= (double)dataBNO055.intD0/16.;                  //X
    dataBNO055.d1= (double)dataBNO055.intD1/16.;                  //Y
    dataBNO055.d2= (double)dataBNO055.intD2/16.;                  //Z
}



void BNO055_readMag(){
    uint8_t rawData[6];
    BNO055_readBytes(BNO055_MAG_DATA_X_LSB, 6, &rawData[0]);
    dataBNO055.intD0 = ((int16_t)rawData[1] << 8) | rawData[0] ;
    dataBNO055.intD1 = ((int16_t)rawData[3] << 8) | rawData[2] ;
    dataBNO055.intD2 = ((int16_t)rawData[5] << 8) | rawData[4] ;
    dataBNO055.d0= (double)dataBNO055.intD0/1.6;                  //X
    dataBNO055.d1= (double)dataBNO055.intD1/1.6;                  //Y
    dataBNO055.d2= (double)dataBNO055.intD2/1.6;                  //Z
}



void BNO055_readLinAcc(){
    uint8_t rawData[6];
    BNO055_readBytes(BNO055_LIA_DATA_X_LSB, 6, &rawData[0]);
    dataBNO055.intD0 = ((int16_t)rawData[1] << 8) | rawData[0] ;
    dataBNO055.intD1 = ((int16_t)rawData[3] << 8) | rawData[2] ;
    dataBNO055.intD2 = ((int16_t)rawData[5] << 8) | rawData[4] ;
    dataBNO055.d0 = (double)dataBNO055.intD0;                     //X
    dataBNO055.d1 = (double)dataBNO055.intD1;                     //Y
    dataBNO055.d2 = (double)dataBNO055.intD2;                     //Z
}



void BNO055_readGrav(){
    uint8_t rawData[6];
    BNO055_readBytes(BNO055_GRV_DATA_X_LSB, 6, &rawData[0]);
    dataBNO055.intD0 = ((int16_t)rawData[1] << 8) | rawData[0] ;
    dataBNO055.intD1 = ((int16_t)rawData[3] << 8) | rawData[2] ;
    dataBNO055.intD2 = ((int16_t)rawData[5] << 8) | rawData[4] ;
    dataBNO055.d0 = (double)dataBNO055.intD0;                     //X
    dataBNO055.d1 = (double)dataBNO055.intD1;                     //Y
    dataBNO055.d2 = (double)dataBNO055.intD2;                     //Z
}



void BNO055_readTemp(){
    uint8_t rawData;
    BNO055_readBytes(BNO055_TEMP, 1, &rawData);
    dataBNO055.intD0=(int16_t)rawData;
    dataBNO055.d0=(double)dataBNO055.intD0;                                   //ºC
    dataBNO055.d1=(double)dataBNO055.d0*1.8+32;                               //ºF
}
