# arm start

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


# setup machine based on device tree

```
init/main.c::start_kernel
    arch/arm/kernel/setup.c::setup_arch
        arch/arm/kernel/devtree.c::setup_machine_fdt // setup machine_desc
            drivers/of/fdt.c::of_flat_dt_match_machine
        arch/arm/include/asm/mach/arch.h::machine_desc::init_early // arch/arm/mach-omap2/io.c::am33xx_init_early
            arch/arm/mach-omap2/omap_hwmod_33xx_data.c::am33xx_hwmod_init
```


# add devices based on dts

```
init/main.c::start_kernel
    init/main.c::rest_init
        init/main.c::kernel_init
            init/main.c::kernel_init_freeable
                init/main.c::do_basic_setup
                    init/main.c::do_initcalls
                        init/main.c::do_initcall_level
                            init/main.c::do_one_initcall
                                arch/arm/kernel/setup.c::customize_machine // use machine_desc
                                    arch/arm/include/asm/mach/arch.h::machine_desc::init_machine // arch/arm/mach-omap2/board-generic.c::omap_generic_init
                                        arch/arm/mach-omap2/pdata-quirks.c::pdata_quirks_init
                                            drivers/of/platform.c::of_platform_populate
                                                drivers/of/platform.c::of_platform_bus_create
                                                    drivers/of/platform.c::of_platform_device_create_pdata
                                                        drivers/of/device.c::of_device_add
                                                            drivers/base/core.c::device_add
```


# arch/arm/mach-omap2/board-generic.c

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
