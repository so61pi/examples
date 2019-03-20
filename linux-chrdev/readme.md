## register_chrdev

```txt
include/linux/fs.h::register_chrdev
    __register_chrdev
        __register_chrdev_region
        cdev_alloc
            kobject_init(ktype_cdev_dynamic)
        cdev_add
            kobj_map(cdev_map, exact_match, exact_lock)
```

## chrdev_open

```txt
chrdev_open
    p = inode->i_cdev
    if (!p) p = kobj_lookup(cdev_map, inode->i_rdev) // decltype(i_rdev) == dev_t
    replace_fops(filp, fops_get(p->ops));
```

## cdev_init

```txt
cdev_init
    kobject_init(ktype_cdev_default)
    cdev->ops = fops
```

## chrdev_init

```txt
chrdev_init
    kobj_map_init
```

## mknod

```txt
fs/namei.c::SYSCALL_DEFINE3(mknod)
    sys_mknodat [SYSCALL_DEFINE4(mknodat)]
        vfs_mknod
            inode_operations::mknod [e.g. ext4_dir_inode_operations -> ext4_mknod]
                fs/inode.c::init_special_inode 
                    inode->i_fop = &def_chr_fops;
```
