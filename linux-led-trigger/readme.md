## Software

- Linux v4.11.

## Load led description from device tree

```txt
drivers/leds/leds-gpio.c::gpio_led_probe                                                                        |
    drivers/leds/leds-gpio.c::gpio_leds_create                                                                  |
        drivers/base/property.c::device_get_child_node_count                                                    | count number of leds
        include/linux/gpio/consumer.h::devm_fwnode_get_gpiod_from_child                                         | get `gpio_desc` for pin which is specified in device tree by property "gpios" or "gpio"
            drivers/gpio/devres.c::devm_fwnode_get_index_gpiod_from_child                                       |
                drivers/gpio/gpiolib.c::fwnode_get_named_gpiod                                                  |
        led.default_trigger = drivers/base/property.c::fwnode_property_read_string("linux,default-trigger")     | get led default-trigger
        drivers/base/property.c::fwnode_property_read_string("default-state")                                   |
        drivers/leds/leds-gpio.c::create_gpio_led                                                               |
            drivers/gpio/gpiolib.c::gpiod_direction_output                                                      | set the GPIO direction to output
            drivers/leds/led-class.c::devm_led_classdev_register                                                |
                drivers/leds/led-class.c::led_classdev_register                                                 |
                    drivers/leds/led-triggers.c::led_trigger_set_default                                        | add led to the led list of the trigger
```

## Led trigger

- `heartbeat`

```txt
drivers/leds/trigger/ledtrig-heartbeat.c::heartbeat_trig_init   | register led heartbeat trigger to system
    drivers/leds/led-triggers.c::led_trigger_register           |
```

- `cpu`

```txt
drivers/leds/trigger/ledtrig-cpu.c::ledtrig_cpu_init            | register led trigger for all cpus
    drivers/leds/led-triggers.c::led_trigger_register_simple    |
        drivers/leds/led-triggers.c::led_trigger_register       |
```

- `mmc`

```txt
drivers/mmc/core/host.c::mmc_add_host                                                       |
    drivers/leds/led-triggers.c::led_trigger_register_simple(dev_name(&host->class_dev))    | `host->class_dev` name is set in `drivers/mmc/core/host.c::::mmc_alloc_host` (`dev_set_name(&host->class_dev, "mmc%d", host->index);`)
        drivers/leds/led-triggers.c::led_trigger_register                                   |
```
