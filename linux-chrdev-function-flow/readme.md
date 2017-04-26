1. **register_chrdev**

        include/linux/fs.h::register_chrdev
            __register_chrdev
                __register_chrdev_region
                cdev_alloc
                    kobject_init(ktype_cdev_dynamic)
                cdev_add
                    kobj_map(cdev_map, exact_match, exact_lock)


2. **chrdev_open**

        chrdev_open
            p = inode->i_cdev
            if (!p) p = kobj_lookup(cdev_map, inode->i_rdev) // decltype(i_rdev) == dev_t
            replace_fops(filp, fops_get(p->ops));


3. **cdev_init**

        cdev_init
            kobject_init(ktype_cdev_default)
            cdev->ops = fops


4. **chrdev_init**

        chrdev_init
            kobj_map_init


5. **mknod**

        fs/namei.c::SYSCALL_DEFINE3(mknod)
            sys_mknodat [SYSCALL_DEFINE4(mknodat)]
                vfs_mknod
                    inode_operations::mknod [e.g. ext4_dir_inode_operations -> ext4_mknod]
                        fs/inode.c::init_special_inode 
                            inode->i_fop = &def_chr_fops;
