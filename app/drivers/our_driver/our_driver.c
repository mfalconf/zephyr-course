#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

// Include the device-specific header
#include "our_driver.h"

#define DT_DRV_COMPAT our_driver
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

// Define the dynamic data structure
struct our_driver_data {
    int custom_param;
};

// Implement the API extension function
void our_driver_set_custom_param(const struct device *dev, int new_value) {
    struct our_driver_data *data = dev->data;
    data->custom_param = new_value;
    LOG_INF("MSG: API Extension called. Custom param set to: %d", data->custom_param);
}

int sensor_sample_fetch_impl(const struct device *dev, enum sensor_channel chan) {
    LOG_INF ("MSG: SAMPLE_FETCH - CHAN: %d", chan);
    return gpio_pin_set(led.port, led.pin, 1);
}
         
int sensor_channel_get_impl( const struct device *dev, enum sensor_channel chan, struct sensor_value *val) {
    (void) val;
    LOG_INF ("MSG: SAMPLE_GET - CHAN: %d", chan);
    return gpio_pin_set(led.port, led.pin, 0);                        
}

static const __attribute__((__aligned__(__alignof(
    struct sensor_driver_api)))) struct sensor_driver_api api_iomico_lecture
    __attribute__((section("."
                           "_sensor_driver_api"
                           "."
                           "static"
                           "."
                           "api_iomico_lecture_"))) __attribute__((__used__)) = {
                            .sample_fetch = sensor_sample_fetch_impl,
                            .channel_get = sensor_channel_get_impl,
};

static int init(const struct device *dev) {
    LOG_INF ("MSG: INIT");

    // Initialize default dynamic data
    struct our_driver_data *data = dev->data;
    data->custom_param = 0;

    if (!gpio_is_ready_dt(&led)) {
        return 1;
    }
    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) {
        return 2;
    }
    return 0;
}

// Instantiate the data structure
static struct our_driver_data driver_data_0;

// Register the device passing the dynamic data structure
DEVICE_DT_INST_DEFINE(0, init, NULL, &driver_data_0, NULL, POST_KERNEL, 80, &api_iomico_lecture);