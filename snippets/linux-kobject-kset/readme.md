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
