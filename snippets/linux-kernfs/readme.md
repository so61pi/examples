## Software

- Linux v4.10.13.

## Flow

```txt
fs/kernfs/file.c::__kernfs_create_file
    fs/kernfs/dir.c::kernfs_new_node(KERNFS_FILE)
        fs/kernfs/dir.c::__kernfs_new_node
    fs/kernfs/dir.c::kernfs_add_one
        fs/kernfs/dir.c::kernfs_link_sibling
        fs/kernfs/dir.c::kernfs_activate

include/linux/kernfs.h::kernfs_create_file_ns
    fs/kernfs/file.c::__kernfs_create_file

include/linux/kernfs.h::kernfs_create_file
    include/linux/kernfs.h::kernfs_create_file_ns

fs/kernfs/dir.c::kernfs_create_dir_ns
    fs/kernfs/dir.c::kernfs_new_node(KERNFS_DIR)
    fs/kernfs/dir.c::kernfs_add_one

include/linux/kernfs.h::kernfs_create_dir
    fs/kernfs/dir.c::kernfs_create_dir_ns

fs/kernfs/symlink.c::kernfs_create_link
    fs/kernfs/dir.c::kernfs_new_node(KERNFS_LINK)
    fs/kernfs/dir.c::kernfs_add_one

fs/kernfs/mount.c::kernfs_init
    mm/slab_common.c::kmem_cache_create("kernfs_node_cache")
```

## Attributes read/write flow

```txt
struct file_operations::read = fs/kernfs/file.c::kernfs_fop_read
    struct kernfs_ops::read = fs/sysfs/file.c::sysfs_kf_read
        struct sysfs_ops::show

struct file_operations::write = fs/kernfs/file.c::kernfs_fop_write
    struct kernfs_ops::write = fs/sysfs/file.c::sysfs_kf_write
        struct sysfs_ops::store
```

## Device attributes

- `drivers/base/core.c`

```c
static const struct sysfs_ops dev_sysfs_ops = {
    .show   = dev_attr_show,
    .store  = dev_attr_store,
};

static struct kobj_type device_ktype = {
    .release    = device_release,
    .sysfs_ops  = &dev_sysfs_ops,
    .namespace  = device_namespace,
};
```

## Bus attributes

- `drivers/base/bus.c`

```c
static const struct sysfs_ops bus_sysfs_ops = {
    .show   = bus_attr_show,
    .store  = bus_attr_store,
};

static struct kobj_type bus_ktype = {
    .sysfs_ops  = &bus_sysfs_ops,
    .release    = bus_release,
};
```

## Device driver attributes

- `drivers/base/bus.c`

```c
static const struct sysfs_ops driver_sysfs_ops = {
    .show   = drv_attr_show,
    .store  = drv_attr_store,
};

static const struct sysfs_ops driver_sysfs_ops = {
    .show   = drv_attr_show,
    .store  = drv_attr_store,
};
```

## `kernfs` file operations

- `fs/kernfs/file.c`

```c
const struct file_operations kernfs_file_fops = {
    .read       = kernfs_fop_read,
    .write      = kernfs_fop_write,
    .llseek     = generic_file_llseek,
    .mmap       = kernfs_fop_mmap,
    .open       = kernfs_fop_open,
    .release    = kernfs_fop_release,
    .poll       = kernfs_fop_poll,
    .fsync      = noop_fsync,
};
```

## Reference

- `Documentation/driver-model/device.txt`
- `Documentation/filesystems/sysfs.txt`
