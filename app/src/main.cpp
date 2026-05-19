#include <errno.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

int app_toggle_sensor() {
    static int current_state = 0;

    current_state = !current_state;
    LOG_INF("LED state: %s", current_state ? "ON" : "OFF");
    if(current_state) {
        // LED ON
        return sensor_sample_fetch_chan(driver, SENSOR_CHAN_AMBIENT_TEMP);
    } else {
        // LED OFF
        struct sensor_value val;
        return sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
    }
}

namespace debug {
    int test() {
        const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

        if(!device_is_ready(driver)) {
            return -ENODEV;
        }
        
        struct sensor_value val;
        auto ret = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
        LOG_INF("MSG: Channel get %d", ret);

        ret = sensor_sample_fetch_chan(driver, SENSOR_CHAN_AMBIENT_TEMP);
        LOG_INF("MSG: Channel fetch %d", ret);
        
        return 0;
    }
}

int main()
{
    int ret;
    
/*
    if(ret = debug::test()) {
        LOG_INF("MSG: INIT_ERROR");
        return ret;
    }
*/
    if(!device_is_ready(driver)) {
        return -ENODEV;
    }

    while (1) {
        if ((ret = app_toggle_sensor()) != 0) {
            return ret;
        }
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}
