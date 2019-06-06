
#include <unistd.h>
#include "bmi055.h"


static uint8_t readFromRegister(uint8_t registerToReadFrom)
{
    esp_err_t error_code = ESP_OK;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    uint8_t data = 0;
    error_code = i2c_master_start(cmd);
    I2C_ERROR_CHECK(error_code);

    error_code = i2c_master_write_byte(cmd, (I2C_ACCELEROMETER_SLAVEADDRESS << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    I2C_ERROR_CHECK(error_code);

    error_code = i2c_master_write_byte(cmd, registerToReadFrom, ACK_CHECK_EN);
    I2C_ERROR_CHECK(error_code);

    error_code = i2c_master_start(cmd);
    I2C_ERROR_CHECK(error_code);

    error_code = i2c_master_write_byte(cmd, (I2C_ACCELEROMETER_SLAVEADDRESS << 1) | I2C_MASTER_READ, ACK_CHECK_EN);
    I2C_ERROR_CHECK(error_code);

    error_code = i2c_master_read_byte(cmd, &data, I2C_MASTER_ACK);
    I2C_ERROR_CHECK(error_code);

    error_code = i2c_master_stop(cmd);
    I2C_ERROR_CHECK(error_code);

    error_code = i2c_master_cmd_begin(I2C_NUM_0, cmd, 0xFF);
    // I2C_ERROR_CHECK(error_code);

    // configPRINTF(("READ: %d\r\n", data));

    i2c_cmd_link_delete(cmd);

    // sleep(1);

    return data;
}

void initializeI2C() {
    esp_err_t error_code = ESP_OK;

    i2c_config_t i2c_config;
    i2c_config.mode = I2C_MODE_MASTER;
    i2c_config.sda_io_num = I2C_GPIO_SDA;
    i2c_config.sda_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_config.scl_io_num = I2C_GPIO_SCL;
    i2c_config.scl_pullup_en = GPIO_PULLUP_ENABLE;
    i2c_config.master.clk_speed = I2C_FREQUENCY;

    error_code = i2c_set_timeout(I2C_NUM_0, 1048575);
    I2C_ERROR_CHECK(error_code);

    error_code = i2c_param_config(I2C_NUM_0, &i2c_config);
    I2C_ERROR_CHECK(error_code);

    error_code = i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
    I2C_ERROR_CHECK(error_code);

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    error_code = i2c_master_start(cmd);
    I2C_ERROR_CHECK(error_code);

    error_code = i2c_master_write_byte(cmd, (I2C_ACCELEROMETER_SLAVEADDRESS << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    I2C_ERROR_CHECK(error_code);

    error_code = i2c_master_write_byte(cmd, PMU_RANGE, ACK_CHECK_EN);
    I2C_ERROR_CHECK(error_code);

    error_code = i2c_master_write_byte(cmd, 5, ACK_CHECK_EN);
    I2C_ERROR_CHECK(error_code);

    error_code = i2c_master_stop(cmd);
    I2C_ERROR_CHECK(error_code);

    error_code = i2c_master_cmd_begin(I2C_NUM_0, cmd, 0xFF);
    // I2C_ERROR_CHECK(error_code);

    i2c_cmd_link_delete(cmd);

}

void updateAccelerometer()
{
    esp_err_t error_code = ESP_OK;

    // readFromRegister(FIFO_READ);

    short x_lsb = readFromRegister(ACCD_X_LSB);
    short x_msb = readFromRegister(ACCD_X_MSB);
    short y_lsb = readFromRegister(ACCD_Y_LSB);
    short y_msb = readFromRegister(ACCD_Y_MSB);
    short z_lsb = readFromRegister(ACCD_Z_LSB);
    short z_msb = readFromRegister(ACCD_Z_MSB);

    short x = ((x_lsb >> 5) & 0x0003) | ((x_msb << 3) & 0x07F8);
    short y = ((y_lsb >> 5) & 0x0003) | ((y_msb << 3) & 0x07F8);
    short z = ((z_lsb >> 5) & 0x0003) | ((z_msb << 3) & 0x07F8);

    configPRINTF(("x: %d, y: %d, z: %d\r\n", x, y, z));
    I2C_ERROR_CHECK(error_code);

    sleep(1);
}

/*-----------------------------------------------------------*/
