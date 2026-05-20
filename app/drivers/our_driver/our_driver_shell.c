#include "zephyr/toolchain.h"
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
#include <sys/errno.h>
#include <zephyr/shell/shell.h>

static int sensor_fetch_cmd_handler(const struct shell * sh, int argc, char **argv) {
    //shell_info(sh, "Sensor->Fetch()");

    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    const struct device *dev = shell_device_get_binding("our_driver");
    if(!dev) {
        shell_error(sh, "Could not find device %s", "our_driver");
        return -EFAULT;
    }

    int ret = sensor_sample_fetch_chan(dev, SENSOR_CHAN_AMBIENT_TEMP);
    if(ret != 0) {
        shell_error(sh, "Could fetch device %s channel", "our_driver");
        return -EFAULT;
    }

    shell_info(sh, "Device fetched ok");
    return 0;
}

static int sensor_channel_get_cmd_handler(const struct shell * sh, int argc, char **argv) {
    //shell_info(sh, "Sensor->Channel_read()");
    
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    const struct device *dev = shell_device_get_binding("our_driver");
    if(!dev) {
        shell_error(sh, "Could not find device %s", "our_driver");
        return -EFAULT;
    }

    struct sensor_value val;
    int ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &val);
    if(ret != 0) {
        shell_error(sh, "Could read device %s channel", "our_driver");
        return -EFAULT;
    }

    shell_info(sh, "Device channel read ok - value = %d", val.val1);
    return 0;
}

static int sensor_info_cmd_handler(const struct shell * sh, int argc, char **argv) {
    //shell_info(sh, "Sensor->Info()");
    
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    const struct device *dev = shell_device_get_binding("our_driver");
    if(!dev) {
        shell_error(sh, "Could not find device %s", "our_driver");
        return -EFAULT;
    }
    
    shell_info(sh, "State = %s", device_is_ready(dev)?"ready":"not ready");
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sensor_sub,
    SHELL_CMD_ARG(fetch, NULL, "sensor sample fetch", sensor_fetch_cmd_handler, 1, 0),
    SHELL_CMD_ARG(read, NULL, "sensor channel get", sensor_channel_get_cmd_handler, 1, 0),
    SHELL_CMD_ARG(info, NULL, "sensor info", sensor_info_cmd_handler, 1, 0),   
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sensor_sub, "Sensor commands", NULL);