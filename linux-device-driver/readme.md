**softwares**

- Linux v4.11.


**device resgiter**

```
drivers/base/core.c::device_register
    drivers/base/core.c::device_initialize // add device to devices_kset
        lib/kobject.c::kobject_init(device_ktype)
    drivers/base/core.c::device_add // add device to device hierarchy
        drivers/base/core.c::device_create_file
            include/linux/sysfs.h::sysfs_create_file
        drivers/base/core.c::device_add_class_symlinks
            fs/sysfs/symlink.c::sysfs_create_link("of_node")
            fs/sysfs/symlink.c::sysfs_create_link("subsystem")
            fs/sysfs/symlink.c::sysfs_create_link("device")
            fs/sysfs/symlink.c::sysfs_create_link(dev_name(dev))
        drivers/base/core.c::device_add_attrs
            drivers/base/core.c::device_add_groups
                fs/sysfs/group.c::sysfs_create_groups
            drivers/base/core.c::device_create_file
                include/linux/sysfs.h::sysfs_create_file
        drivers/base/bus.c::bus_add_device
            drivers/base/core.c::device_add_attrs
            drivers/base/core.c::device_add_groups
            fs/sysfs/symlink.c::sysfs_create_link(dev_name(dev))
            fs/sysfs/symlink.c::sysfs_create_link("subsystem")
        drivers/base/core.c::device_create_file(dev_attr_dev)
        drivers/base/core.c::device_create_sys_dev_entry
        drivers/base/devtmpfs.c::devtmpfs_create_node
        kernel/notifier.c::blocking_notifier_call_chain(BUS_NOTIFY_ADD_DEVICE)
            kernel/notifier.c::__blocking_notifier_call_chain
                kernel/notifier.c::notifier_call_chain
                    struct notifier_block::notifier_call
        lib/kobject_uevent.c::kobject_uevent(KOBJ_ADD)
        drivers/base/bus.c::bus_probe_device
            drivers/base/dd.c::device_initial_probe
                drivers/base/dd.c::__device_attach
                    drivers/base/dd.c::device_bind_driver // bind a driver to one device
                        drivers/base/dd.c::driver_sysfs_add
                            kernel/notifier.c::blocking_notifier_call_chain(BUS_NOTIFY_BIND_DRIVER)
                            fs/sysfs/symlink.c::sysfs_create_link
                            fs/sysfs/symlink.c::sysfs_create_link("driver")
                        drivers/base/dd.c::driver_bound
                            kernel/notifier.c::blocking_notifier_call_chain(BUS_NOTIFY_BOUND_DRIVER)
                        kernel/notifier.c::blocking_notifier_call_chain(BUS_NOTIFY_DRIVER_NOT_BOUND)
                    drivers/base/bus.c::bus_for_each_drv(drivers/base/dd.c::__device_attach_driver)
                        drivers/base/dd.c::__device_attach_driver
                            drivers/base/base.h::driver_match_device
                                struct bus_type::match
                            drivers/base/dd.c::driver_probe_device
                                drivers/base/dd.c::really_probe
                                    drivers/base/pinctrl.c::pinctrl_bind_pins
                                    dev->driver = drv
                                    drivers/base/dd.c::driver_sysfs_add
                                    struct bus_type::probe
                                    struct device_driver::probe // call driver's probe function, for platform driver, it's `platform_drv_probe`.
                    kernel/async.c::async_schedule(drivers/base/dd.c::__device_attach_async_helper)
                        drivers/base/bus.c::bus_for_each_drv(drivers/base/dd.c::__device_attach_driver)
            struct subsys_interface::add_dev
```
