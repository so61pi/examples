# kset

```
kset_init
    kobject_init_internal

kset_register
    kset_init
        kobject_init_internal
    kobject_add_internal
    kobject_uevent(KOBJ_ADD)


kset_create_and_add
    kset_create
    kset_register
```


# kobject

```
kobject_init_and_add
    kobject_init
    kobject_add_varg

kobject_create_and_add
    kobject_create
    kobject_add

kobject_create
    kobject_init(dynamic_kobj_ktype)
        kobject_init_internal

kobject_add
    kobject_add_varg
        kobject_add_internal
            kobj_kset_join
            create_dir
                fs/sysfs/dir.c::sysfs_create_dir_ns
                    fs/kernfs/dir.c::kernfs_create_dir_ns
            populate_dir
                include/linux/sysfs.h::sysfs_create_file
                    fs/sysfs/file.c::sysfs_create_file_ns
                        sysfs_add_file_mode_ns
                            fs/kernfs/file.c::__kernfs_create_file
```


# uevent

```
kobject_uevent
kobject_uevent_env
    uevent_ops->filter
    add_uevent_var("ACTION")
    add_uevent_var("DEVPATH")
    add_uevent_var("SUBSYSTEM")
    uevent_ops->uevent
    netlink_broadcast_filtered
```


# sysfs

```
fs/sysfs/dir.c::sysfs_create_dir_ns
    kernfs_create_dir_ns
    
sysfs_create_file_ns
    sysfs_add_file_mode_ns
        __kernfs_create_file

sysfs_create_bin_file
    sysfs_add_file
        sysfs_add_file_mode_ns

fs/sysfs/symlink.c::sysfs_create_link
    sysfs_do_create_link
        sysfs_do_create_link_sd
            kernfs_create_link

fs/sysfs/group.c::sysfs_create_group
    internal_create_group
        kernfs_create_dir
        create_files
            sysfs_add_file_mode_ns

sysfs_add_file_to_group
    sysfs_add_file

sysfs_merge_group
    sysfs_add_file
    
sysfs_add_link_to_group
    sysfs_create_link_sd

sysfs_create_file
    sysfs_create_file_ns
        sysfs_add_file_mode_ns

fs/sysfs/mount.c::sysfs_init
    kernfs_create_root -> sysfs_root
    register_filesystem(sysfs_fs_type)
```


# kernelfs

```
fs/kernfs/file.c::__kernfs_create_file
    kernfs_new_node(KERNFS_FILE)
        __kernfs_new_node
    kernfs_add_one
        kernfs_link_sibling
        kernfs_activate

include/linux/kernfs.h::kernfs_create_file_ns
    __kernfs_create_file

include/linux/kernfs.h::kernfs_create_file
    kernfs_create_file_ns

fs/kernfs/dir.c::kernfs_create_dir_ns
    kernfs_new_node(KERNFS_DIR)
    kernfs_add_one

include/linux/kernfs.h::kernfs_create_dir
    kernfs_create_dir_ns

fs/kernfs/symlink.c::kernfs_create_link
    kernfs_new_node(KERNFS_LINK)
    kernfs_add_one

fs/kernfs/mount.c::kernfs_init
    mm/slab_common.c::kmem_cache_create("kernfs_node_cache")
```


# bus

```
drivers/base/bus.c::bus_register
    kzalloc(struct subsys_private)
    bus_create_file
        sysfs_create_file
    kset_create_and_add("devices") // create devices set
    kset_create_and_add("drivers") // create drivers set
    add_probe_files
        bus_create_file(bus_attr_drivers_probe)
            sysfs_create_file
        bus_create_file(bus_attr_drivers_autoprobe)
            sysfs_create_file
    bus_add_groups
        sysfs_create_groups

drivers/base/bus.c::subsys_system_register
    subsys_register(system_kset)
        bus_register
        device_register

drivers/base/bus.c::bus_add_device
    device_add_attrs
    device_add_groups
    sysfs_create_link
    klist_add_tail
```


# device

```
drivers/base/core.c::device_register
    device_initialize // add device to devices_kset
        dev->kobj.kset = devices_kset
        kobject_init(device_ktype)
    device_add
        kobject_add
        device_create_file
            sysfs_create_file
        device_add_class_symlinks
            sysfs_create_link("of_node")
            sysfs_create_link("subsystem")
            sysfs_create_link("device")
        device_add_attrs
            device_add_groups
                sysfs_create_groups
        bus_add_device
        device_create_file(dev_attr_dev)
        device_create_sys_dev_entry
            device_to_dev_kobj
            sysfs_create_link
        devtmpfs_create_node
        blocking_notifier_call_chain(BUS_NOTIFY_ADD_DEVICE)
            __blocking_notifier_call_chain
                notifier_call_chain
                    notifier_block::notifier_call
        kobject_uevent(KOBJ_ADD)
        bus_probe_device
            device_initial_probe
                __device_attach
                    device_bind_driver
                        driver_sysfs_add
                            blocking_notifier_call_chain(BUS_NOTIFY_BIND_DRIVER)
                            sysfs_create_link
                            sysfs_create_link("driver")
                        driver_bound
                            blocking_notifier_call_chain(BUS_NOTIFY_BOUND_DRIVER)
                        blocking_notifier_call_chain(BUS_NOTIFY_DRIVER_NOT_BOUND)
                    bus_for_each_drv(__device_attach_driver)
                        __device_attach_driver
                            driver_match_device
                                struct bus_type::match
                            driver_probe_device
                                really_probe
                                    dev->driver = drv
                                    driver_sysfs_add
                                    devices_kset_move_last
                                    struct bus_type::probe
                                    struct device_driver::probe
                    async_schedule(__device_attach_async_helper)
            struct subsys_interface::add_dev
```


# driver

```
drivers/base/init.c::driver_init
    drivers/base/core.c::devices_init
        kset_create_and_add("devices", device_uevent_ops)   -> devices_kset
        kobject_create_and_add("dev")                       -> dev_kobj
        kobject_create_and_add("block", dev_kobj)           -> sysfs_dev_block_kobj
        kobject_create_and_add("char", dev_kobj)            -> sysfs_dev_char_kobj
    drivers/base/bus.c::buses_init
        kset_create_and_add("bus", bus_uevent_ops)          -> bus_kset
        kset_create_and_add("system", devices_kset->kobj)   -> system_kset
    drivers/base/class.c::classes_init
        kset_create_and_add("class")                        -> class_kset
    drivers/base/firmware.c::firmware_init
        kobject_create_and_add("firmware")                  -> firmware_kobj
    drivers/base/hypervisor.c::hypervisor_init
        kobject_create_and_add("hypervisor")                -> hypervisor_kobj

    drivers/base/platform.c::platform_bus_init
        drivers/base/core.c::device_register(&platform_bus)
        drivers/base/bus.c::bus_register(&platform_bus_type)
    drivers/base/cpu.c::cpu_dev_init
        drivers/base/bus.c::subsys_system_register(&cpu_subsys, cpu_root_attr_groups)
        cpu_dev_register_generic
            register_cpu
                drivers/base/core.c::device_register
                drivers/base/node.c::register_cpu_under_node
                    fs/sysfs/symlink.c::sysfs_create_link
    drivers/base/memory.c::memory_dev_init
        drivers/base/bus.c::subsys_system_register(&memory_subsys, memory_root_attr_groups)
    drivers/base/container.c::container_dev_init
        drivers/base/bus.c::subsys_system_register(&container_subsys)
    drivers/of/base.c::of_core_init
        kset_create_and_add("devicetree", firmware_kobj)    -> of_kset
        drivers/of/base.c::__of_attach_node_sysfs
            kobject_add
            __of_add_property_sysfs
                sysfs_create_bin_file
        fs/proc/generic.c::proc_symlink("device-tree", "/sys/firmware/devicetree/base")
            __proc_create
            proc_register
```
