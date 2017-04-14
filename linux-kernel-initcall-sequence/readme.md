**Software**

- Linux [4.10.8](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git/tree/?id=refs/tags/v4.10.8).


**Sequence**

- `early_initcall` functions are called in [`do_pre_smp_initcalls`](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git/tree/init/main.c?id=refs/tags/v4.10.8#n885), from `__initcall_start` to before `__initcall0_start`.
- [`do_initcalls`](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git/tree/init/main.c?id=refs/tags/v4.10.8#n858) calls functions from `__initcall0_start` to before `__initcall_end`.
- Functions with the same `id` go into the same section.

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


**Reference**

- [`include/asm-generic/vmlinux.lds.h`](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git/tree/include/asm-generic/vmlinux.lds.h?id=refs/tags/v4.10.8#n698).
- [`include/linux/init.h`](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git/tree/include/linux/init.h?id=refs/tags/v4.10.8#n146).
- [`init/main.c`](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git/tree/init/main.c?id=refs/tags/v4.10.8).
