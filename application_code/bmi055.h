#ifndef BMI055_H
#define BMI055_H

#include "driver/i2c.h"


#define I2C_ACCELEROMETER_SLAVEADDRESS 22
#define I2C_GYRO_SLAVEADDRESS 104
#define ACK_CHECK_EN true



#define ACCD_X_LSB 0x02
#define ACCD_X_MSB 0x03
#define ACCD_Y_LSB 0x04
#define ACCD_Y_MSB 0x05
#define ACCD_Z_LSB 0x06
#define ACCD_Z_MSB 0x07

#define I2C_ERROR_CHECK(X) ESP_ERROR_CHECK(X)

uint8_t readFromRegister(uint8_t registerToReadFrom, i2c_cmd_handle_t *cmd_ptr);
void updateAccelerometer(void *params);

#endif // !BMI055_H
