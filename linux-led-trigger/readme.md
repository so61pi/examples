**softwares**

- Linux v4.11.


**load led description from device tree**

```
drivers/leds/leds-gpio.c::gpio_led_probe                                                                        |
    drivers/leds/leds-gpio.c::gpio_leds_create                                                                  |
        drivers/base/property.c::device_get_child_node_count                                                    | count number of leds
        led.default_trigger = drivers/base/property.c::fwnode_property_read_string("linux,default-trigger")     | get led default-trigger
        drivers/base/property.c::fwnode_property_read_string("default-state")                                   |
        drivers/leds/leds-gpio.c::create_gpio_led                                                               |
            drivers/gpio/gpiolib.c::gpiod_direction_output                                                      | set the GPIO direction to output
            drivers/leds/led-class.c::devm_led_classdev_register                                                |
                drivers/leds/led-class.c::led_classdev_register                                                 |
                    drivers/leds/led-triggers.c::led_trigger_set_default                                        | add led to the led list of the trigger
```


**led trigger**

- heartbeat

```
drivers/leds/trigger/ledtrig-heartbeat.c::heartbeat_trig_init   | register led heartbeat trigger to system
    drivers/leds/led-triggers.c::led_trigger_register           |
```

- cpu

```
drivers/leds/trigger/ledtrig-cpu.c::ledtrig_cpu_init            | register led trigger for all cpus
    drivers/leds/led-triggers.c::led_trigger_register_simple    |
        drivers/leds/led-triggers.c::led_trigger_register       |
```
