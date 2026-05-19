#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT our_driver

/* The devicetree node identifier for the "app-led" alias. */
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

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

    if (!gpio_is_ready_dt(&led)) {
        return 1;
    }
    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) {
        return 2;
    }
    return 0;
}

DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_iomico_lecture);