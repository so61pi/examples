**Hardware**

- BeagleBone Black Rev C.


**Software**

- Linux [4.10.8](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git/tree/?id=refs/tags/v4.10.8).


**Call tree**
```
# 1st, set up the boot console when kernel starts.
init/main.c::start_kernel
    drivers/tty/tty_io.c::console_init
        drivers/tty/serial/8250/8250_core.c::univ8250_console_init
            kernel/printk/printk.c::register_console
                drivers/tty/serial/8250/8250_core.c::univ8250_console_match
                    drivers/tty/serial/serial_core.c::uart_parse_earlycon
                    drivers/tty/serial/8250/8250_port.c::serial8250_console_setup
                        drivers/tty/serial/serial_core.c::uart_parse_options
                        drivers/tty/serial/serial_core.c::uart_set_options


# 2nd, register driver for /dev/tty and /dev/console.
drivers/char/mem.c::chr_dev_init
    drivers/tty/tty_io.c::tty_init
        register_chrdev_region(MKDEV(TTYAUX_MAJOR, 0), 1, "/dev/tty")
        register_chrdev_region(MKDEV(TTYAUX_MAJOR, 1), 1, "/dev/console")


# 3rd, when user opens /dev/console.
drivers/tty/tty_io.c::tty_open
    drivers/tty/tty_io.c::tty_open_current_tty
        drivers/tty/tty_io.c::get_current_tty
    drivers/tty/tty_io.c::tty_open_by_driver
        drivers/tty/tty_io.c::tty_lookup_driver
            // MKDEV(TTYAUX_MAJOR, 1) -> struct tty_driver *console_driver = console_device(index);
```


**Reference**

- [Documentation/serial/driver](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git/tree/Documentation/serial/driver?id=refs/tags/v4.10.8).
