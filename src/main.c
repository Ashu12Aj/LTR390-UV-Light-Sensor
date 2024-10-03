// /* For UV index Value*/

// #include <zephyr/kernel.h>
// #include <zephyr/device.h>
// #include <zephyr/drivers/i2c.h>
// #include <zephyr/sys/printk.h>

// #define SLEEP_TIME_MS 1000

// /* Define LTR390 I2C address and register addresses */
// #define LTR390_I2C_ADDR 0x53
// #define LTR390_MAIN_CTRL 0x00
// #define LTR390_MEAS_RATE 0x04
// #define LTR390_GAIN 0x05
// #define LTR390_UVS_DATA_0 0x10
// #define LTR390_UVS_DATA_1 0x11
// #define LTR390_UVS_DATA_2 0x12
// #define LTR390_MAIN_STATUS 0x07

// /* Define measurement settings */
// #define UVS_MODE 0x0A  // Set UVS mode (UVS active)
// #define RESOLUTION_18BIT_TIME100MS 0x20
// #define GAIN_3X 0x01

// /* Get the node identifier of the sensor */
// #define I2C0_NODE DT_NODELABEL(mysensor)

// int main(void)
// {
//     int ret;

//     /* Retrieve the API-specific device structure */
//     static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C0_NODE);
//     if (!device_is_ready(dev_i2c.bus))
//     {
//         printk("I2C bus %s is not ready!\n", dev_i2c.bus->name);
//         return -1;
//     }

//     /* Set UVS mode in MAIN_CTRL register */
//     uint8_t config_main[2] = {LTR390_MAIN_CTRL, UVS_MODE};
//     ret = i2c_write_dt(&dev_i2c, config_main, sizeof(config_main));
//     if (ret != 0)
//     {
//         printk("Failed to write to MAIN_CTRL register\n");
//         return -1;
//     }

//     /* Configure measurement rate and resolution */
//     uint8_t config_meas[2] = {LTR390_MEAS_RATE, RESOLUTION_18BIT_TIME100MS};
//     ret = i2c_write_dt(&dev_i2c, config_meas, sizeof(config_meas));
//     if (ret != 0)
//     {
//         printk("Failed to write to MEAS_RATE register\n");
//         return -1;
//     }

//     /* Configure gain */
//     uint8_t config_gain[2] = {LTR390_GAIN, GAIN_3X};
//     ret = i2c_write_dt(&dev_i2c, config_gain, sizeof(config_gain));
//     if (ret != 0)
//     {
//         printk("Failed to write to GAIN register\n");
//         return -1;
//     }

//     while (1)
//     {
//         /* Check status if data is ready */
//         uint8_t status = 0;
//         ret = i2c_reg_read_byte_dt(&dev_i2c, LTR390_MAIN_STATUS, &status);
//         if (ret != 0 || !(status & 0x08))
//         {
//             printk("Data not ready, waiting...\n");
//             k_msleep(SLEEP_TIME_MS);
//             continue;
//         }

//         /* Read UVS data (3 bytes) */
//         uint8_t uv_reading[3] = {0};
//         ret = i2c_burst_read_dt(&dev_i2c, LTR390_UVS_DATA_0, uv_reading, 3);
//         if (ret != 0)
//         {
//             printk("Failed to read UVS data\n");
//             return -1;
//         }

//         /* Combine the three bytes into a single UV index value */
//         int uv_index = (uv_reading[2] << 16) | (uv_reading[1] << 8) | uv_reading[0];

//         /* Print the UV index value */
//         printk("UV Index: %d\n", uv_index);

//         k_msleep(SLEEP_TIME_MS);
//     }
// }

///////////////////

/* FOR ALS Value*/

// #include <zephyr/kernel.h>
// #include <zephyr/device.h>
// #include <zephyr/drivers/i2c.h>
// #include <zephyr/sys/printk.h>
// #include <math.h>

// #define SLEEP_TIME_MS 1000

// /* Define LTR390 I2C address and register addresses */
// #define LTR390_I2C_ADDR 0x53
// #define LTR390_MAIN_CTRL 0x00
// #define LTR390_MEAS_RATE 0x04
// #define LTR390_GAIN 0x05
// #define LTR390_ALS_DATA_0 0x0D
// #define LTR390_ALS_DATA_1 0x0E
// #define LTR390_ALS_DATA_2 0x0F
// #define LTR390_MAIN_STATUS 0x07

// /* Measurement settings */
// #define ALS_MODE 0x02  // Set ALS mode (ALS active)
// #define RESOLUTION_18BIT_TIME100MS 0x20
// #define GAIN_3X 0x01

// /* Conversion factors */
// #define GAIN_FACTOR 3.0  // Based on GAIN_3X
// #define INTEGRATION_TIME_FACTOR 1.0  // For 18-bit resolution
// #define WFAC 1.0  // Window factor (use 1 if no glass or window)

// #define LUX_MULTIPLIER 0.6  // From the datasheet

// /* Get the node identifier of the sensor */
// #define I2C0_NODE DT_NODELABEL(mysensor)

// int write_register(struct i2c_dt_spec dev_i2c, uint8_t reg, uint8_t value) {
//     uint8_t data[2] = {reg, value};
//     return i2c_write_dt(&dev_i2c, data, sizeof(data));
// }

// double calculate_lux(int als_data) {
//     double lux = (LUX_MULTIPLIER * als_data) / (GAIN_FACTOR * INTEGRATION_TIME_FACTOR) * WFAC;
//     return lux;
// }

// int main(void)
// {
//     int ret;

//     /* Retrieve the API-specific device structure */
//     static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C0_NODE);
//     if (!device_is_ready(dev_i2c.bus))
//     {
//         printk("I2C bus %s is not ready!\n", dev_i2c.bus->name);
//         return -1;
//     }

//     /* Step 1: Switch to ALS mode */
//     ret = write_register(dev_i2c, LTR390_MAIN_CTRL, ALS_MODE);
//     if (ret != 0) {
//         printk("Failed to set ALS mode in MAIN_CTRL register\n");
//         return -1;
//     }

//     /* Step 2: Configure measurement rate and resolution */
//     ret = write_register(dev_i2c, LTR390_MEAS_RATE, RESOLUTION_18BIT_TIME100MS);
//     if (ret != 0) {
//         printk("Failed to configure MEAS_RATE register\n");
//         return -1;
//     }

//     /* Step 3: Configure gain */
//     ret = write_register(dev_i2c, LTR390_GAIN, GAIN_3X);
//     if (ret != 0) {
//         printk("Failed to configure GAIN register\n");
//         return -1;
//     }

//     while (1)
//     {
//         /* Step 4: Check if ALS data is ready */
//         uint8_t status = 0;
//         ret = i2c_reg_read_byte_dt(&dev_i2c, LTR390_MAIN_STATUS, &status);
//         if (ret != 0 || !(status & 0x08))  // Check if ALS data is ready
//         {
//             printk("ALS data not ready, waiting...\n");
//             k_msleep(SLEEP_TIME_MS);
//             continue;
//         }

//         /* Step 5: Read ALS data (3 bytes) */
//         uint8_t als_reading[3] = {0};
//         ret = i2c_burst_read_dt(&dev_i2c, LTR390_ALS_DATA_0, als_reading, 3);
//         if (ret != 0)
//         {
//             printk("Failed to read ALS data\n");
//             return -1;
//         }

//         /* Step 6: Combine ALS readings */
//         int als_value = (als_reading[2] << 16) | (als_reading[1] << 8) | als_reading[0];

//         /* Step 7: Calculate and print lux value */
//         int lux = calculate_lux(als_value);
//         printk("ALS Value: %d\n", lux);

//         k_msleep(SLEEP_TIME_MS);
//     }
// }

////////////

/* For Both parameter */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/sys/printk.h>
#include <math.h>

#define SLEEP_TIME_MS 1000

/* Define LTR390 I2C address and register addresses */
#define LTR390_I2C_ADDR 0x53
#define LTR390_MAIN_CTRL 0x00
#define LTR390_MEAS_RATE 0x04
#define LTR390_GAIN 0x05
#define LTR390_UVS_DATA_0 0x10
#define LTR390_UVS_DATA_1 0x11
#define LTR390_UVS_DATA_2 0x12
#define LTR390_ALS_DATA_0 0x0D
#define LTR390_ALS_DATA_1 0x0E
#define LTR390_ALS_DATA_2 0x0F
#define LTR390_MAIN_STATUS 0x07

/* Measurement settings */
#define UVS_MODE 0x0A // Set UVS mode (UVS active)
#define ALS_MODE 0x02 // Set ALS mode (ALS active)
#define RESOLUTION_18BIT_TIME100MS 0x20
#define GAIN_3X 0x01

/* Conversion factors */
#define GAIN_FACTOR 3.0				// Based on GAIN_3X
#define INTEGRATION_TIME_FACTOR 1.0 // For 18-bit resolution
#define WFAC 1.0					// Window factor (use 1 if no glass or window)

#define LUX_MULTIPLIER 0.6	// From the datasheet

/* Get the node identifier of the sensor */
#define I2C0_NODE DT_NODELABEL(mysensor)

int write_register(struct i2c_dt_spec dev_i2c, uint8_t reg, uint8_t value)
{
	uint8_t data[2] = {reg, value};
	return i2c_write_dt(&dev_i2c, data, sizeof(data));
}

double calculate_lux(int als_data)
{
	return (LUX_MULTIPLIER * als_data) / (GAIN_FACTOR * INTEGRATION_TIME_FACTOR) * WFAC;
}

int main(void)
{
	int ret;

	/* Retrieve the API-specific device structure */
	static const struct i2c_dt_spec dev_i2c = I2C_DT_SPEC_GET(I2C0_NODE);
	if (!device_is_ready(dev_i2c.bus))
	{
		printk("I2C bus %s is not ready!\n", dev_i2c.bus->name);
		return -1;
	}

	/* Configure the sensor */
	ret = write_register(dev_i2c, LTR390_MEAS_RATE, RESOLUTION_18BIT_TIME100MS);
	if (ret != 0)
	{
		printk("Failed to configure MEAS_RATE register\n");
		return -1;
	}

	ret = write_register(dev_i2c, LTR390_GAIN, GAIN_3X);
	if (ret != 0)
	{
		printk("Failed to configure GAIN register\n");
		return -1;
	}

	while (1)
	{
		// Read ALS Data
		ret = write_register(dev_i2c, LTR390_MAIN_CTRL, ALS_MODE);
		if (ret != 0)
		{
			printk("Failed to set ALS mode in MAIN_CTRL register\n");
			return -1;
		}

		// Wait for ALS data to be ready
		k_msleep(100); // Allow time for measurement

		uint8_t status = 0;
		ret = i2c_reg_read_byte_dt(&dev_i2c, LTR390_MAIN_STATUS, &status);
		if (ret != 0 || !(status & 0x08))
		{
			printk("ALS data not ready, waiting...\n");
			k_msleep(SLEEP_TIME_MS);
			continue;
		}

		// Read ALS data (3 bytes)
		uint8_t als_reading[3] = {0};
		ret = i2c_burst_read_dt(&dev_i2c, LTR390_ALS_DATA_0, als_reading, 3);
		if (ret != 0)
		{
			printk("Failed to read ALS data\n");
			return -1;
		}
		int als_value = (als_reading[2] << 16) | (als_reading[1] << 8) | als_reading[0];
		int lux = calculate_lux(als_value);
		printk("ALS Lux Value: %d\n", lux);

		// Read UVS Data
		ret = write_register(dev_i2c, LTR390_MAIN_CTRL, UVS_MODE);
		if (ret != 0)
		{
			printk("Failed to set UVS mode in MAIN_CTRL register\n");
			return -1;
		}

		// Wait for UVS data to be ready
		k_msleep(100); // Allow time for measurement

		ret = i2c_reg_read_byte_dt(&dev_i2c, LTR390_MAIN_STATUS, &status);
		if (ret != 0 || !(status & 0x08))
		{
			printk("UVS data not ready, waiting...\n");
			k_msleep(SLEEP_TIME_MS);
			continue;
		}

		// Read UVS data (3 bytes)
		uint8_t uv_reading[3] = {0};
		ret = i2c_burst_read_dt(&dev_i2c, LTR390_UVS_DATA_0, uv_reading, 3);
		if (ret != 0)
		{
			printk("Failed to read UVS data\n");
			return -1;
		}

		int uv_index = (uv_reading[2] << 16) | (uv_reading[1] << 8) | uv_reading[0];
		/* Print the UV index value */
		printk("UV Index: %d\n", uv_index);

		k_msleep(SLEEP_TIME_MS);
	}
}
