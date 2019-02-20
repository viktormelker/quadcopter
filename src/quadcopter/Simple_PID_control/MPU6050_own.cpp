#include "MPU6050_own.h"
#include "Arduino.h"
#include "Wire.h"

// --------------------------------------------------------
// MPU6050_read
//
// This is a common function to read multiple bytes 
// from an I2C device.
//
// It uses the boolean parameter for Wire.endTransMission()
// to be able to hold or release the I2C-bus. 
// This is implemented in Arduino 1.0.1.
//
// Only this function is used to read. 
// There is no function for a single byte.
//
int MPU6050_read(int start, uint8_t *buffer, int size)
{
    int i, n, error;

    Wire.beginTransmission(MPU6050_I2C_ADDRESS);
    n = Wire.write(start);
    if (n != 1)
        return (-10);

    n = Wire.endTransmission(false);    // hold the I2C-bus
    /* This section is commented out since the error code returned
        does not comply with the arduino specification in the 
        arduino due implementation*/
    /*if (n != 0)
    {
        Serial.print("crazy error code after endTransmission\n");
        Serial.print(n, DEC);
        return (n);
    }*/
    
    // Third parameter is true: release I2C-bus after data is read.
    Wire.requestFrom(MPU6050_I2C_ADDRESS, size, true);
    i = 0;
    while(Wire.available() && i<size)
    {
        buffer[i++]=Wire.read();
    }
    if ( i != size)
        return (-11);

    return (0);  // return : no error
}


// --------------------------------------------------------
// MPU6050_write
//
// This is a common function to write multiple bytes to an I2C device.
//
// If only a single register is written,
// use the function MPU_6050_write_reg().
//
// Parameters:
//   start : Start address, use a define for the register
//   pData : A pointer to the data to write.
//   size  : The number of bytes to write.
//
// If only a single register is written, a pointer
// to the data has to be used, and the size is
// a single byte:
//   int data = 0;        // the data to write
//   MPU6050_write (MPU6050_PWR_MGMT_1, &c, 1);
//
int MPU6050_write(int start, const uint8_t *pData, int size)
{
    int n, error;

    Wire.beginTransmission(MPU6050_I2C_ADDRESS);
    n = Wire.write(start);        // write the start address
    if (n != 1)
        return (-20);

    n = Wire.write(pData, size);  // write data bytes
    if (n != size)
        return (-21);

    error = Wire.endTransmission(true); // release the I2C-bus
    if (error != 0)
        return (error);

    return (0);         // return : no error
}

// --------------------------------------------------------
// MPU6050_write_reg
//
// An extra function to write a single register.
// It is just a wrapper around the MPU_6050_write()
// function, and it is only a convenient function
// to make it easier to write a single register.
//
int MPU6050_write_reg(int reg, uint8_t data)
{
    int error;

    error = MPU6050_write(reg, &data, 1);

    return (error);
}

/* 
    This function initializes gyro settings
*/
int init_gyro()
{
    int error;

    // Set gyro sensitivity to handle the range 0 - 1000 degrees/s
    error = MPU6050_write_reg(MPU6050_GYRO_CONFIG, MPU6050_FS_SEL_1000 | MPU6050_XG_ST);
    return error;
}

/* 
    This function initializes acceleromter settings
*/
int init_accelerometer()
{
    int error;
    
    // Set accelerometer sensitivity to be in range 0-16 g.
    error = MPU6050_write_reg(MPU6050_ACCEL_CONFIG, MPU6050_AFS_SEL_16G);
    if (error)
    {
        return error;
    }

    return error;
}

/* 
    This function initializes the MPU6050 with settings that
    are not directly associated with a specific sensor.
*/
int init_MPU6050()
{
    int error;
    uint8_t c;
    
    // Select clock source as gyro X-axis (recommended in RM-MPU_6000A)
    error = MPU6050_write_reg(MPU6050_PWR_MGMT_1, MPU6050_CLKSEL_X);
    if (error)
    {
        return error;
    }

    // According to the datasheet, the 'sleep' bit
    // should read a '1'. But I read a '0'.
    // That bit has to be cleared, since the sensor
    // is in sleep mode at power-up. Even if the
    // bit reads '0'.
    error = MPU6050_read (MPU6050_PWR_MGMT_2, &c, 1);

    // Clear the 'sleep' bit to start the sensor.
    MPU6050_write_reg (MPU6050_PWR_MGMT_1, 0);

    // enable passing through SDA and SCL to aux_DA and AUX_CL
    //error = MPU6050_write_reg();
}

void read_sensor_values(uint8_t * outdata)
{
    int error;
    double dT;
    accel_t_gyro_union accel_t_gyro;

    // Read the raw sensor values.
    // Read 14 bytes at once, 
    // containing acceleration, temperature and gyro.
    // With the default settings of the MPU-6050,
    // there is no filter enabled, and the values
    // are not very stable.
    error = MPU6050_read (MPU6050_ACCEL_XOUT_H, (uint8_t *) &accel_t_gyro, sizeof(accel_t_gyro));
    if(error !=0)
    {
        Serial.print("Got error while reading from MPU6050");
        Serial.print((", error = "));
        Serial.println(error,DEC);
    }
    else  
    {
        // Swap all high and low bytes.
        // After this, the registers values are swapped, 
        // so the structure name like x_accel_l does no 
        // longer contain the lower byte.
        uint8_t swap;
        #define SWAP(x,y) swap = x; x = y; y = swap
    
        SWAP (accel_t_gyro.reg.x_accel_h, accel_t_gyro.reg.x_accel_l);
        SWAP (accel_t_gyro.reg.y_accel_h, accel_t_gyro.reg.y_accel_l);
        SWAP (accel_t_gyro.reg.z_accel_h, accel_t_gyro.reg.z_accel_l);
        SWAP (accel_t_gyro.reg.t_h, accel_t_gyro.reg.t_l);
        SWAP (accel_t_gyro.reg.x_gyro_h, accel_t_gyro.reg.x_gyro_l);
        SWAP (accel_t_gyro.reg.y_gyro_h, accel_t_gyro.reg.y_gyro_l);
        SWAP (accel_t_gyro.reg.z_gyro_h, accel_t_gyro.reg.z_gyro_l);

        memcpy((void *) outdata, &accel_t_gyro, sizeof(accel_t_gyro));
    }
}

double get_temperature(int16_t intemp)
{
    // The temperature sensor is -40 to +85 degrees Celsius.
    // It is a signed integer.
    // According to the datasheet: 
    //   340 per degrees Celsius, -512 at 35 degrees.
    // At 0 degrees: -512 - (340 * 35) = -12412
    return ( (double) intemp + 12412.0) / 340.0;
}