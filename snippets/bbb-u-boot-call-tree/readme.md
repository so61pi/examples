**Version and compiling command**

- This call tree is produced using U-Boot version `v2017.01`.
- The compiling commands

```shell
make CROSS_COMPILE=arm-linux-gnueabihf- distclean
make CROSS_COMPILE=arm-linux-gnueabihf- am335x_boneblack_defconfig
make CROSS_COMPILE=arm-linux-gnueabihf-
```


**SPL**

```
arch/arm/lib/vectors.S::_start @402F0400
    arch/arm/cpu/armv7/start.S::reset
        arch/arm/mach-omap2/lowlevel_init.S::save_boot_params
        arch/arm/cpu/armv7/start.S::cpu_init_cp15
        arch/arm/cpu/armv7/start.S::cpu_init_crit
            arch/arm/cpu/armv7/lowlevel_init.S::lowlevel_init
                arch/arm/mach-omap2/am33xx/board.c::s_init
        arch/arm/lib/crt0.S::_main
            common/init/board_init.c::board_init_f_alloc_reserve
            common/init/board_init.c::board_init_f_init_reserve
            arch/arm/mach-omap2/am33xx/board.c::board_init_f
                arch/arm/mach-omap2/am33xx/board.c::early_system_init
                arch/arm/mach-omap2/am33xx/board.c::board_early_init_f
                board/ti/am335x/board.c::sdram_init
            common/spl/spl.c::board_init_r
                common/dlmalloc.c::mem_malloc_init
                common/spl/spl.c::spl_init
                arch/arm/mach-omap2/timer.c::timer_init
                arch/arm/mach-omap2/boot-common.c::spl_board_init
                    arch/arm/mach-omap2/boot-common.c::save_omap_boot_params
                        arch/arm/mach-omap2/boot-common.c::omap_sys_boot_device
                    common/spl/spl.c::preloader_console_init
                        drivers/serial/serial.c::serial_init
                            drivers/serial/serial.c::get_current
                                board/ti/am335x/board.c::default_serial_console
                                    board/ti/am335x/board.h::board_is_icev2
                                        board/ti/common/board_detect.c::board_ti_is
                                        board/ti/common/board_detect.c::board_ti_get_rev
                            drivers/serial/serial_ns16550.c::eserial1_init [ get_current()->start() ]
                                drivers/serial/ns16550.c::ns16550_calc_divisor
                                drivers/serial/ns16550.c::NS16550_init
                    drivers/i2c/i2c_core.c::i2c_init
                        drivers/i2c/i2c_core.c::i2c_init_bus
                    arch/arm/mach-omap2/am33xx/board.c::arch_misc_init
                        drivers/usb/musb-new/musb_uboot.c::musb_register
                            drivers/usb/musb-new/musb_core.c::musb_init_controller
                    drivers/watchdog/omap_wdt.c::hw_watchdog_init
                    board/ti/am335x/board.c::am33xx_spl_board_init
                        arch/arm/mach-omap2/am33xx/sys_info.c::am335x_get_efuse_mpu_max_freq
                        drivers/power/pmic/pmic_tps65217.c::tps65217_reg_write
                        drivers/power/pmic/pmic_tps65217.c::tps65217_voltage_update
                        arch/arm/mach-omap2/am33xx/clock.c::do_setup_dpll
                        drivers/power/pmic/pmic_tps65217.c::tps65217_voltage_update
                        drivers/power/pmic/pmic_tps65217.c::tps65217_reg_write
                        arch/arm/mach-omap2/am33xx/clock.c::do_setup_dpll
                common/spl/spl.c::board_boot_order
                common/spl/spl.c::boot_from_devices
                    common/spl/spl.c::spl_ll_find_loader
                    common/spl/spl.c::spl_load_image
                        common/spl/spl_mmc.c::spl_mmc_load_image [ loader->load_image(spl_image, &bootdev) ]
                common/spl/spl.c::spl_board_prepare_for_linux
                arch/arm/lib/spl.c::jump_to_image_linux
                    arch/arm/cpu/armv7/cpu.c::cleanup_before_linux
                        arch/arm/cpu/armv7/cpu.c::cleanup_before_linux_select
                common/spl/spl.c::spl_board_prepare_for_boot
                arch/arm/mach-omap2/boot-common.c::jump_to_image_no_args
```


**U-Boot**

```
arch/arm/lib/vectors.S::_start @80800000
    arch/arm/cpu/armv7/start.S::reset
        arch/arm/lib/crt0.S::_main
            common/init/board_init.c::board_init_f_alloc_reserve
            common/init/board_init.c::board_init_f_init_reserve
            common/board_f.c::board_init_f
                lib/initcall.c::initcall_run_list
            arch/arm/lib/relocate.S::relocate_code
            arch/arm/lib/relocate.S::relocate_vectors
            arch/arm/cpu/armv7/start.S::c_runtime_cpu_setup
            common/board_f.c::coloured_LED_init
            common/board_f.c::red_led_on
            common/board_r.c::board_init_r
                lib/initcall.c::initcall_run_list(common/board_r.c::init_sequence_r)
                    common/board_r.c::initr_trace
                    common/board_r.c::initr_reloc
                    ...
                    common/board_r.c::run_main_loop
                        common/main.c::main_loop
                            include/bootstage.h::bootstage_mark_name
                                common/main.c::show_boot_progress
                            common/cli.c::cli_init
                            common/main.c::run_preboot_environment_command
                            common/autoboot.c::bootdelay_process
                            include/cli.h::cli_process_fdt
                            include/cli.h::cli_secure_boot_cmd
                            common/autoboot.c::autoboot_command
                                common/autoboot.c::abortboot
                                    common/autoboot.c::__abortboot
                            common/cli.c::cli_loop
                                common/cli_hush.c::parse_file_outer
                                    common/cli_hush.c::setup_file_in_str
                                    common/cli_hush.c::parse_stream_outer
                                        common/cli_hush.c::run_list
                                            common/cli_hush.c::run_list_real
                                                common/cli_hush.c::run_pipe_real
                                                    common/command.c::cmd_process
                                                        common/command.c::cmd_call
```
