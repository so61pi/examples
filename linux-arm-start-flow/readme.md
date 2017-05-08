**Hardware**

- BeagleBone Black Rev C.


**Software**

- Linux [4.4](https://github.com/beagleboard/linux/tree/4.4).


**ARM start**

```
arch/arm/boot/bootp/init.S::_start
    arch/arm/boot/bootp/kernel.S::kernel_start
        arch/arm/boot/compressed/head.S
            arch/arm/boot/compressed/misc.c::decompress_kernel
            arch/arm/boot/compressed/head.S::__enter_kernel
                arch/arm/kernel/head.S
                    arch/arm/kernel/head.S::__enable_mmu
                        arch/arm/kernel/head.S::__turn_mmu_on
                            arch/arm/kernel/head-common.S::__mmap_switched
                                init/main.c::start_kernel
```


**Setup machine based on device tree**

- The `machine_desc` array is placed in `.init.arch.info` section, between `__arch_info_begin` and `__arch_info_end`.

```
init/main.c::start_kernel
    arch/arm/kernel/setup.c::setup_arch
        arch/arm/kernel/devtree.c::setup_machine_fdt
            drivers/of/fdt.c::of_flat_dt_match_machine // get `/compatible` list (in device tree) then find the matching `machine_desc` entry.
        drivers/of/fdt.c::unflatten_device_tree // create tree of `device_nodes` from flat blob passed by bootloader, the root node is `of_root`.
        arch/arm/include/asm/mach/arch.h::machine_desc::init_early = arch/arm/mach-omap2/io.c::am33xx_init_early
    init/main.c::rest_init
        init/main.c::kernel_init
            init/main.c::kernel_init_freeable
                init/main.c::do_basic_setup
                    init/main.c::do_initcalls
                        init/main.c::do_initcall_level
                            init/main.c::do_one_initcall
                                arch/arm/kernel/setup.c::customize_machine // `arch_initcall(customize_machine)`
                                    arch/arm/include/asm/mach/arch.h::machine_desc::init_machine = arch/arm/mach-omap2/board-generic.c::omap_generic_init
                                        arch/arm/mach-omap2/pdata-quirks.c::pdata_quirks_init
                                            drivers/of/platform.c::of_platform_populate

drivers/of/platform.c::of_platform_populate // load driver of all devices described in device tree.
    include/linux/of.h::of_find_node_by_path
        drivers/of/base.c::of_find_node_opts_by_path
            drivers/of/dynamic.c::of_node_get
        drivers/of/platform.c::of_platform_bus_create
            drivers/of/platform.c::of_platform_device_create_pdata
                drivers/of/base.c::of_device_is_available // check `status` property.
                    drivers/of/base.c::__of_device_is_available
                        drivers/of/base.c::__of_get_property("status")
                drivers/of/platform.c::of_device_alloc
                    drivers/base/platform.c::platform_device_alloc
                    drivers/of/address.c::of_address_to_resource
                drivers/of/device.c::of_device_add
                    drivers/base/core.c::device_add // eventually calls `struct platform_driver::probe` (e.g. `drivers/net/ethernet/ti/cpsw.c::cpsw_probe`).
            drivers/of/platform.c::of_platform_bus_create // recursively call to child node.
```


**arch/arm/mach-omap2/board-generic.c**

- `DT_MACHINE_START` puts the `machine_desc` struct in `.arch.info.init` section which eventually is placed inside `.init.arch.info` section.

```c
#ifdef CONFIG_SOC_AM33XX
static const char *const am33xx_boards_compat[] __initconst = {
	"ti,am33xx",
	NULL,
};

DT_MACHINE_START(AM33XX_DT, "Generic AM33XX (Flattened Device Tree)")
	.reserve	= omap_reserve,
	.map_io		= am33xx_map_io,
	.init_early	= am33xx_init_early,
	.init_machine	= omap_generic_init,
	.init_late	= am33xx_init_late,
	.init_time	= omap3_gptimer_timer_init,
	.dt_compat	= am33xx_boards_compat,
	.restart	= am33xx_restart,
MACHINE_END
#endif
```
