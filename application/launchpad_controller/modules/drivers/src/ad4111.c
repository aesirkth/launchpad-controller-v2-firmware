/*
 * AD4111 Analog to Digital Converter (ADC) Driver
 *
 * Author: Love Mitteregger
 * Created: 10 jun 2024
 *
 */

// Find Zephyr Device Tree Bindings
#define DT_DRV_COMPAT analog_ad4111 

// General libraries
#include <math.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// Zephyr libraries
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/devicetree.h>
#include <zephyr/linker/devicetree_regions.h>
// #include <zephyr/ztest.h> // Zephyrs Testing Framework, must also be enabled in prj.conf if used

// AESIR libraries
#include "../inc/ad4111.h"
// Placeholder

// Set logging system to capture any log messages generated by the code
LOG_MODULE_REGISTER(ad4111, CONFIG_SENSOR_LOG_LEVEL);

/* REMEMBER: DRIVER FUNCTIONS SHOULD BE DESIGNED TO OPERATE INDEPENDENTLY OF THE NUMBER OF DEVICES USING THEM. */

/* Data struct used to hold instance-specific data that may change at runtime. 
 * Each instance of the AD4111 driver will have its own ad4111_data structure, 
 * allowing independent operation of multiple instances. */
struct ad4111_config {
    struct spi_dt_spec spi; // This structure holds the SPI configuration for the AD4111 instance. It includes details like the SPI bus, chip select, and maximum frequency. The SPI_DT_SPEC_INST_GET(inst) macro in the AD4111_DEVICE_DEFINE macro fills this structure based on device tree settings.
    struct gpio_dt_spec cs_gpio;
    uint32_t spi_max_frequency;
    uint8_t channels;
};

struct ad4111_data {
    // Add instance-specific data here...
    //struct k_mutex ad4111_lock; // kernel mutex used to ensure that access to shared resources (like the SPI bus or data structures) is thread-safe. Prevents race conditions.
    //int sample_data; // variable that can hold the most recent ADC conversion result or any other temporary data specific to an instance of the AD4111.
};

/* Function for resetting the AD4111 ADC 
 * After a power-up cycle and when the power supplies are stable, a device reset is required. 
 * Furthermore, in situations where interface synchronization is lost, a device reset is also required. 
 * Returning CS high sets the digital interface to the default state and halts any serial interface operation. */
static int ad4111_reset(const struct device *dev) {
    const struct ad4111_config *config = dev->config;

    
    /* Toggle CS-complement to reset the device */
    /*
    gpio_pin_set_dt(&config->cs_gpio, 0); // Toggle CS-complement high for reset
    k_sleep(K_MSEC(1)); // wait a bit
    gpio_pin_set_dt(&config->cs_gpio, 1); // Toggle CS-complement low to allow data flow
    k_sleep(K_MSEC(1)); // wait a bit
    */
    return 0;
};

static int ad4111_init(const struct device *dev) {
    const struct ad4111_config *config = dev->config;

//    if (!device_is_ready(config->spi.bus)) {
//        LOG_ERR("SPI bus is not ready");
//        return -ENODEV;
//    }

//    if (!device_is_ready(config->cs_gpio.port)) {
//        LOG_ERR("CS GPIO is not ready");
//       return -ENODEV;
//    }

    /* Do other initialization stuff */
     ad4111_reset(dev);
    
    return 0;
};

// Struct utilizing the adc subsystem api 
static struct adc_api ad4111_api = {
    .reset = ad4111_reset,                   // Reset ADC
    .init = ad4111_init,                     // Initialize ADC
    .config_channel = NULL, //ad4111_config_channel, // Enable/Disable ADC channel
    .write_register = NULL, //ad4111_write_register, // Write to ADC register
    .read_register = NULL, //ad4111_read_register,   // Read from ADC register
    .read_data = NULL, //ad4111_read_data,           // Read from ADC data register NOTE: CONSIDER CHANGING NAME FOR THIS
};

// A macro defining the SPI configuration for AD4111 -- CURRENTLY NOT USED, SEE adc_ads7052.c GitHub FOR REF.
#define AD4111_SPI_CONFIG \
	SPI_OP_MODE_MASTER | SPI_MODE_CPOL | SPI_MODE_CPHA | SPI_WORD_SET(8) | SPI_TRANSFER_MSB

// A macro to easily define and initialize an instance of the ADC driver.
// Could possible use this: .spi = DEVICE_DT_GET(DT_INST_PHANDLE(inst, spi)),   
// .spi = SPI_DT_SPEC_INST_GET(inst, AD4111_SPI_CONFIG, 1U),   
// .cs_gpio = GPIO_DT_SPEC_INST_GET(inst, cs_gpios),           
 //       .spi_max_frequency = DT_INST_PROP(inst, spi_max_frequency),
   //     .channels = DT_INST_PROP(inst, channels),                   
    //    .spi = SPI_DT_SPEC_INST_GET(inst, AD4111_SPI_CONFIG, 1U),   
    //    .cs_gpio = GPIO_DT_SPEC_INST_GET(inst, cs_gpios),           
    //    .spi_max_frequency = DT_INST_PROP(inst, spi_max_frequency), 
    //    .channels = DT_INST_PROP(inst, channels),                   
#define AD4111_DEVICE_DEFINE(inst)                                  \
    static struct ad4111_data ad4111_data_##inst = {};                   \
                                                                    \
    /* Pull instance configuration from DeviceTree */               \
    static const struct ad4111_config ad4111_config_##inst = {      \
    };                                                              \
                                                                    \
    DEVICE_DT_INST_DEFINE(                                          \
        inst,                                                       \
        ad4111_init,                                                \
        NULL,                                                       \
        &ad4111_data_##inst,                                        \
        &ad4111_config_##inst,                                      \
        POST_KERNEL,                                                \
        90,                                   \
        &ad4111_api                                                \
    );


// Would like to add CONFIG_ADC_INIT_PRIORITY instead of '90' in the
// Macro definition above...

// Instantiate all defined instances
DT_INST_FOREACH_STATUS_OKAY(AD4111_DEVICE_DEFINE);