**softwares**

- Linux v4.10.8.


**sequence**

- `early_initcall` functions are called in `do_pre_smp_initcalls`, from `__initcall_start` to before `__initcall0_start`.
- `do_initcalls` calls functions from `__initcall0_start` to before `__initcall_end`.
- functions with the same `id` go into the same section.

```
fn                          id          sections created by         sections created        initcall_t entry                note
                                        __define_initcall           by linker script        for function fn
----------------------------------------------------------------------------------------------------------------------------------------------------------------
early_initcall              early       .initcallearly.init         __initcall_start        __initcall_##fn##early

pure_initcall               0           .initcall0.init             __initcall0_start       __initcall_##fn##0

core_initcall               1           .initcall1.init             __initcall1_start       __initcall_##fn##1
core_initcall_sync          1s          .initcall1s.init                                    __initcall_##fn##1s

postcore_initcall           2           .initcall2.init             __initcall2_start       __initcall_##fn##2
postcore_initcall_sync      2s          .initcall2s.init                                    __initcall_##fn##2s

arch_initcall               3           .initcall3.init             __initcall3_start       __initcall_##fn##3
arch_initcall_sync          3s          .initcall3s.init                                    __initcall_##fn##3s

subsys_initcall             4           .initcall4.init             __initcall4_start       __initcall_##fn##4
subsys_initcall_sync        4s          .initcall4s.init                                    __initcall_##fn##4s

fs_initcall                 5           .initcall5.init             __initcall5_start       __initcall_##fn##5
fs_initcall_sync            5s          .initcall5s.init                                    __initcall_##fn##5s
rootfs_initcall             rootfs      .initcallrootfs.init                                __initcall_##fn##rootfs         populate rootfs in populate_rootfs
                                                                                                                            or default_rootfs

device_initcall             6           .initcall6.init             __initcall6_start       __initcall_##fn##6              load built-in modules
device_initcall_sync        6s          .initcall6s.init                                    __initcall_##fn##6s

late_initcall               7           .initcall7.init             __initcall7_start       __initcall_##fn##7
late_initcall_sync          7s          .initcall7s.init                                    __initcall_##fn##7s
                                                                    __initcall_end
```


**initcall functions order**

- `console_initcall` -> `security_initcall` -> `early_initcall` -> `_initcall` and `_initcall_sync`

```
init/main.c::start_kernel                               |
    drivers/tty/tty_io.c::console_init                  | call all `console_initcall` functions
    security/security.c::security_init                  |
        security/security.c::do_security_initcalls      | call all `security_initcall` functions
    init/main.c::rest_init                              |
        kernel/fork.c::kernel_thread(kernel_init)       | start `kernel_init` before `kthreadd`, the reason is in the kernel source code
        kernel/fork.c::kernel_thread(kthreadd)          |

init/main.c::kernel_init                                                    |
    init/main.c::kernel_init_freeable                                       |
        kernel/sched/completion.c::wait_for_completion(&kthreadd_done);     | wait for `kthreadd_done` from `kthreadd`
        init/main.c::do_pre_smp_initcalls                                   | call all `early_initcall` functions
        init/main.c::do_basic_setup                                         |
            init/main.c::do_initcalls                                       | call other `_initcall` and `_initcall_sync` functions which have id from 0 to 7s
```


**references**

- `include/asm-generic/vmlinux.lds.h`, line 698.
- `include/linux/init.h`, line 146.
- `init/main.c`.
