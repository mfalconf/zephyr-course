#ifndef OUR_DRIVER_H
#define OUR_DRIVER_H

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief API extension to change a dynamic parameter of the driver.
 *
 * @param dev Pointer to the device structure.
 * @param new_value The new value to set.
 */
void our_driver_set_custom_param(const struct device *dev, int new_value);

#ifdef __cplusplus
}
#endif

#endif /* OUR_DRIVER_H */
