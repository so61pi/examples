**softwares**

- Linux v4.11.


**device register**

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


**pinmux setup**

```c
static const struct pinctrl_ops pcs_pinctrl_ops = {
    .get_groups_count = pinctrl_generic_get_group_count,
    .get_group_name = pinctrl_generic_get_group_name,
    .get_group_pins = pinctrl_generic_get_group_pins,
    .pin_dbg_show = pcs_pin_dbg_show,
    .dt_node_to_map = pcs_dt_node_to_map,
    .dt_free_map = pcs_dt_free_map,
};

static const struct pinmux_ops pcs_pinmux_ops = {
    .get_functions_count = pinmux_generic_get_function_count,
    .get_function_name = pinmux_generic_get_function_name,
    .get_function_groups = pinmux_generic_get_function_groups,
    .set_mux = pcs_set_mux,
    .gpio_request_enable = pcs_request_gpio,
};
```

```
drivers/base/pinctrl.c::pinctrl_bind_pins // set pin multiplexing resgiter to loaded values
    drivers/pinctrl/core.c::devm_pinctrl_get
        drivers/pinctrl/core.c::pinctrl_get
            drivers/pinctrl/core.c::create_pinctrl
                drivers/pinctrl/devicetree.c::pinctrl_dt_to_map
                    drivers/pinctrl/devicetree.c::dt_to_map_one_config
                        struct pinctrl_ops::dt_node_to_map = drivers/pinctrl/pinctrl-single.c::pcs_dt_node_to_map
    drivers/pinctrl/core.c::pinctrl_select_state
        drivers/pinctrl/pinmux.c::pinmux_enable_setting
            pinmux_ops::set_mux = drivers/pinctrl/pinctrl-single.c::pcs_set_mux // set up pin multiplexing for hardware
                struct pcs_device::read = pcs_readl
                struct pcs_device::write = pcs_writel

drivers/pinctrl/pinctrl-single.c::pcs_dt_node_to_map // load register address and value to configure pin multiplexing later
    drivers/pinctrl/pinctrl-single.c::pcs_parse_one_pinctrl_entry
        drivers/pinctrl/devicetree.c::pinctrl_count_index_with_args("pinctrl-single,pins") // count number of pins.
        // calculate pinmux register locations for this set of pins and their values
        drivers/pinctrl/pinctrl-single.c::pcs_add_function // add calculated vals above to struct pinctrl_dev::pin_function_tree
        drivers/pinctrl/core.c::pinctrl_generic_add_group // add pins to struct pinctrl_dev::pin_group_tree

drivers/pinctrl/pinctrl-single.c::pcs_probe
    include/linux/of.h::of_property_read_u32("pinctrl-single,register-width")
    pcs->desc.pctlops = &pcs_pinctrl_ops;
    pcs->desc.pmxops = &pcs_pinmux_ops;
```
