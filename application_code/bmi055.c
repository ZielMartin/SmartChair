
#include "bmi055.h"



uint8_t readFromRegister(uint8_t registerToReadFrom, i2c_cmd_handle_t *cmd_ptr) {
    esp_err_t error_code = ESP_OK;
    uint8_t data = 0x0;
    i2c_cmd_handle_t cmd = &cmd_ptr;
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

    return data;
}




void updateAccelerometer(void *params)
{
    esp_err_t error_code = ESP_OK;

    static gpio_num_t i2c_gpio_sda = GPIO_NUM_18;
    static gpio_num_t i2c_gpio_scl = GPIO_NUM_19;
    static uint32_t i2c_frequency = 400000;

    i2c_config_t i2c_config;
    i2c_config.mode = I2C_MODE_MASTER;
    i2c_config.sda_io_num = i2c_gpio_sda;
    i2c_config.scl_io_num = i2c_gpio_scl;
    i2c_config.master.clk_speed = i2c_frequency;

    error_code = i2c_param_config(I2C_NUM_0, &i2c_config);
    I2C_ERROR_CHECK(error_code);

    error_code = i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
    I2C_ERROR_CHECK(error_code);
    
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    short x_lsb = readFromRegister(ACCD_X_LSB, cmd);
    short x_msb = readFromRegister(ACCD_X_MSB, cmd);
    short y_lsb = readFromRegister(ACCD_Y_LSB, cmd);
    short y_msb = readFromRegister(ACCD_Y_MSB, cmd);
    short z_lsb = readFromRegister(ACCD_Z_LSB, cmd);
    short z_msb = readFromRegister(ACCD_Z_MSB, cmd);

    short x = ((x_lsb >> 5) & 0x0003) | ((x_msb << 3) & 0x07F8);
    short y = ((y_lsb >> 5) & 0x0003) | ((y_msb << 3) & 0x07F8);
    short z = ((z_lsb >> 5) & 0x0003) | ((z_msb << 3) & 0x07F8);

    configPRINTF(("x: %f, y: %f, z: %f\r\n", x, y, z));
    I2C_ERROR_CHECK(error_code);

    
}

/*-----------------------------------------------------------*/
