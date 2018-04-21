## Hardware

- BeagleBone Black Rev C.

## Software

- Linux v4.10.8.

## Flow

- `ttyS0` -> index = 0

```txt
init/main.c::start_kernel                                                                               | 1st, set up the boot console when kernel starts.
    drivers/tty/tty_io.c::console_init                                                                  |
        drivers/tty/serial/8250/8250_core.c::univ8250_console_init                                      | `console_initcall(univ8250_console_init)`
            kernel/printk/printk.c::register_console                                                    | add console driver `struct console` to `console_drivers`
                struct console::match = drivers/tty/serial/8250/8250_core.c::univ8250_console_match     |
                    drivers/tty/serial/serial_core.c::uart_parse_earlycon                               |
                    drivers/tty/serial/8250/8250_port.c::serial8250_console_setup                       |
                        drivers/tty/serial/serial_core.c::uart_parse_options                            |
                        drivers/tty/serial/serial_core.c::uart_set_options                              |


drivers/char/mem.c::chr_dev_init                                                | 2nd, register driver for `/dev/tty` and `/dev/console`.
    drivers/tty/tty_io.c::tty_init                                              |
        register_chrdev_region(MKDEV(TTYAUX_MAJOR, 0), 1, "/dev/tty")           |
        register_chrdev_region(MKDEV(TTYAUX_MAJOR, 1), 1, "/dev/console")       |


drivers/tty/tty_io.c::tty_open                                                                          | 3rd, when user opens `/dev/console`.
    drivers/tty/tty_io.c::tty_open_current_tty                                                          |
        drivers/tty/tty_io.c::get_current_tty                                                           |
    drivers/tty/tty_io.c::tty_open_by_driver                                                            |
        drivers/tty/tty_io.c::tty_lookup_driver                                                         |
            // MKDEV(TTYAUX_MAJOR, 1) -> struct tty_driver *console_driver = console_device(index);     | get `struct tty_driver` from `struct console` in `console_drivers`
```

## Reference

- `Documentation/serial/driver`.
