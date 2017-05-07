**softwares**

- Linux v4.10.13.


**intro**

- `sysfs` is just a wrapper of `kernfs` which actually takes care of node creation.
- files/links can be created by
    + `sysfs_create_file`
    + `sysfs_create_file_ns`
    + `sysfs_create_bin_file`
    + `sysfs_create_link`
- dirs can be created by
    + `sysfs_create_dir_ns`
- a group of files can be created by
    + `sysfs_create_group`


**flow**

```
fs/sysfs/mount.c::sysfs_init
    fs/kernfs/dir.c::kernfs_create_root -> sysfs_root
    fs/filesystems.c::register_filesystem(sysfs_fs_type)

fs/sysfs/dir.c::sysfs_create_dir_ns
    fs/kernfs/dir.c::kernfs_create_dir_ns

fs/sysfs/file.c::sysfs_create_file_ns
    fs/sysfs/file.c::sysfs_add_file_mode_ns
        fs/kernfs/file.c::__kernfs_create_file

fs/sysfs/file.c::sysfs_create_bin_file
    fs/sysfs/file.c::sysfs_add_file
        fs/sysfs/file.c::sysfs_add_file_mode_ns

fs/sysfs/symlink.c::sysfs_create_link
    fs/sysfs/symlink.c::sysfs_do_create_link
        fs/sysfs/symlink.c::sysfs_do_create_link_sd
            fs/kernfs/symlink.c::kernfs_create_link

fs/sysfs/group.c::sysfs_create_group
    fs/sysfs/group.c::internal_create_group
        fs/kernfs/dir.c::kernfs_create_dir
        fs/sysfs/group.c::create_files
            fs/sysfs/file.c::sysfs_add_file_mode_ns

fs/sysfs/file.c::sysfs_add_file_to_group
    fs/sysfs/file.c::sysfs_add_file

fs/sysfs/group.c::sysfs_merge_group
    fs/sysfs/file.c::sysfs_add_file

fs/sysfs/group.c::sysfs_add_link_to_group
    fs/sysfs/symlink.c::sysfs_create_link_sd

include/linux/sysfs.h::sysfs_create_file
    fs/sysfs/file.c::sysfs_create_file_ns
```


**attributes read/write functions**

- `fs/sysfs/file.c`

```c
static const struct kernfs_ops sysfs_file_kfops_empty = {
};

static const struct kernfs_ops sysfs_file_kfops_ro = {
    .seq_show   = sysfs_kf_seq_show,
};

static const struct kernfs_ops sysfs_file_kfops_wo = {
    .write      = sysfs_kf_write,
};

static const struct kernfs_ops sysfs_file_kfops_rw = {
    .seq_show   = sysfs_kf_seq_show,
    .write      = sysfs_kf_write,
};

static const struct kernfs_ops sysfs_prealloc_kfops_ro = {
    .read       = sysfs_kf_read,
    .prealloc   = true,
};

static const struct kernfs_ops sysfs_prealloc_kfops_wo = {
    .write      = sysfs_kf_write,
    .prealloc   = true,
};

static const struct kernfs_ops sysfs_prealloc_kfops_rw = {
    .read       = sysfs_kf_read,
    .write      = sysfs_kf_write,
    .prealloc   = true,
};

static const struct kernfs_ops sysfs_bin_kfops_ro = {
    .read       = sysfs_kf_bin_read,
};

static const struct kernfs_ops sysfs_bin_kfops_wo = {
    .write      = sysfs_kf_bin_write,
};

static const struct kernfs_ops sysfs_bin_kfops_rw = {
    .read       = sysfs_kf_bin_read,
    .write      = sysfs_kf_bin_write,
};

static const struct kernfs_ops sysfs_bin_kfops_mmap = {
    .read       = sysfs_kf_bin_read,
    .write      = sysfs_kf_bin_write,
    .mmap       = sysfs_kf_bin_mmap,
};
```


**references**

- `Documentation/filesystems/sysfs.txt`
