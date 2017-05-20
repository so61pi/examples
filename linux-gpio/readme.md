**softwares**

- Linux v4.11.


**export/unexport pin to userspace**

- export pin 8 to userspace

```shell
echo 8 > /sys/class/gpio/export
```

- unexport pin 8

```shell
echo 8 > /sys/class/gpio/unexport
```

```
drivers/gpio/gpiolib-sysfs.c::export_store                  |
    drivers/gpio/gpiolib.c::gpio_to_desc                    | get pin description
    drivers/gpio/gpiolib.c::gpiod_request                   |
    drivers/gpio/gpiolib-sysfs.c::gpiod_export              |
        drivers/base/core.c::device_create_with_groups      | create /sys/class/gpio/gpio<n>/ directory, the function that change the value & diection are value_store & direction_store

drivers/gpio/gpiolib-sysfs.c::unexport_store
    drivers/gpio/gpiolib.c::gpio_to_desc
    drivers/gpio/gpiolib.c::gpiod_free
```


**configure/read pin**

- configure pin 8 as output, high

```shell
echo "high" > /sys/class/gpio/gpio8/direction
```

- read pin 8 value

```shell
cat /sys/class/gpio/gpio8/value
```

```
drivers/gpio/gpiolib-sysfs.c::value_store               |
    drivers/gpio/gpiolib.c::gpiod_set_value_cansleep    | set value of pin

static DEVICE_ATTR_RW(value);   | create dev_attr_value with store & show functions are value_store & value_show
```

```
drivers/gpio/gpiolib-sysfs.c::direction_store               |
    drivers/gpio/gpiolib.c::gpiod_direction_output_raw(1)   | set pin to output high in case the value is "high"
    drivers/gpio/gpiolib.c::gpiod_direction_output_raw(0)   | set pin to output low in case the value is "out" or "low"
    drivers/gpio/gpiolib.c::gpiod_direction_input           | set pin to input in case the value is "in"

static DEVICE_ATTR_RW(direction);   | create dev_attr_direction with store & show functions are direction_store & direction_show
```


**references**

- Documentation/gpio/gpio.txt
- Documentation/gpio/sysfs.txt
