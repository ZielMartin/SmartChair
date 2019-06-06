#ifndef BMI055_H
#define BMI055_H

#include "driver/i2c.h"
#include "driver/gpio.h"

#define I2C_GPIO_SDA GPIO_NUM_18
#define I2C_GPIO_SCL GPIO_NUM_19

#define I2C_FREQUENCY 400000

#define I2C_ACCELEROMETER_SLAVEADDRESS 0x18
#define I2C_GYRO_SLAVEADDRESS 0x68
#define ACK_CHECK_EN true



#define ACCD_X_LSB 0x02
#define ACCD_X_MSB 0x03
#define ACCD_Y_LSB 0x04
#define ACCD_Y_MSB 0x05
#define ACCD_Z_LSB 0x06
#define ACCD_Z_MSB 0x07
// ...
#define PMU_RANGE 0x0F
// ...
#define FIFO_CONFIG 0x3E
#define FIFO_READ 0x3F

#define I2C_ERROR_CHECK(X) ESP_ERROR_CHECK(X)

void initializeI2C(void);
void updateAccelerometer(void);

#endif // !BMI055_H
