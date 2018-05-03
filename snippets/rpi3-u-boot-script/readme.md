- The U-Boot version is `v2017.11` with config `rpi_3_32b_config`.

```shell
bootdelay=2
baudrate=115200
preboot=usb start
loadaddr=0x00200000
arch=arm
cpu=armv7
board=rpi
board_name=rpi
vendor=raspberrypi
soc=bcm283x
stdin=serial,usbkbd
stdout=serial,vidconsole
stderr=serial,vidconsole
fdt_high=ffffffff
initrd_high=ffffffff
fdt_addr_r=0x00000100
pxefile_addr_r=0x00100000
kernel_addr_r=0x01000000
scriptaddr=0x02000000
ramdisk_addr_r=0x02100000
efi_dtb_prefixes=/ /dtb/ /dtb/current/
boot_prefixes=/ /boot/
boot_scripts=boot.scr.uimg boot.scr
boot_script_dhcp=boot.scr.uimg
boot_targets=mmc0 usb0 pxe dhcp

bootcmd {
    run distro_bootcmd
}

dhcpuboot {
    usb start;
    dhcp u-boot.uimg;
    bootm
}

mmc_boot {
    if mmc dev ${devnum}; then
        setenv devtype mmc;
        run scan_dev_for_boot_part;
    fi
}

boot_net_usb_start {
    usb start
}

usb_boot {
    usb start;
    if usb dev ${devnum}; then
        setenv devtype usb;
        run scan_dev_for_boot_part;
    fi
}

boot_efi_binary {
    if fdt addr ${fdt_addr_r}; then
        bootefi bootmgr ${fdt_addr_r};
    else
        bootefi bootmgr ${fdtcontroladdr};
    fi;
    load ${devtype} ${devnum}:${distro_bootpart} ${kernel_addr_r} efi/boot/bootarm.efi;
    if fdt addr ${fdt_addr_r}; then
        bootefi ${kernel_addr_r} ${fdt_addr_r};
    else
        bootefi ${kernel_addr_r} ${fdtcontroladdr};
    fi
}

load_efi_dtb {
    load ${devtype} ${devnum}:${distro_bootpart} ${fdt_addr_r} ${prefix}${efi_fdtfile}
}

scan_dev_for_efi {
    setenv efi_fdtfile ${fdtfile};
    if test -z "${fdtfile}" -a -n "${soc}"; then
        setenv efi_fdtfile ${soc}-${board}${boardver}.dtb;
    fi;
    for prefix in ${efi_dtb_prefixes}; do
        if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}${efi_fdtfile}; then
            run load_efi_dtb;
        fi;
    done;
    if test -e ${devtype} ${devnum}:${distro_bootpart} efi/boot/bootarm.efi; then
        echo Found EFI removable media binary efi/boot/bootarm.efi;
        run boot_efi_binary;
        echo EFI LOAD FAILED: continuing...;
    fi;
    setenv efi_fdtfile
}

boot_extlinux {
    sysboot ${devtype} ${devnum}:${distro_bootpart} any ${scriptaddr} ${prefix}extlinux/extlinux.conf
}

scan_dev_for_extlinux {
    if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}extlinux/extlinux.conf; then
        echo Found ${prefix}extlinux/extlinux.conf;
        run boot_extlinux;
        echo SCRIPT FAILED: continuing...;
    fi
}

boot_a_script {
    load ${devtype} ${devnum}:${distro_bootpart} ${scriptaddr} ${prefix}${script};
    source ${scriptaddr}
}

scan_dev_for_scripts {
    for script in ${boot_scripts}; do
        if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}${script}; then
            echo Found U-Boot script ${prefix}${script};
            run boot_a_script;
            echo SCRIPT FAILED: continuing...;
        fi;
    done
}

scan_dev_for_boot {
    echo Scanning ${devtype} ${devnum}:${distro_bootpart}...;
    for prefix in ${boot_prefixes}; do
        run scan_dev_for_extlinux;
        run scan_dev_for_scripts;
    done;
    run scan_dev_for_efi;
}

scan_dev_for_boot_part {
    part list ${devtype} ${devnum} -bootable devplist;
    env exists devplist || setenv devplist 1;
    for distro_bootpart in ${devplist}; do
        if fstype ${devtype} ${devnum}:${distro_bootpart} bootfstype; then
            run scan_dev_for_boot;
        fi;
    done
}

bootcmd_mmc0 {
    setenv devnum 0;
    run mmc_boot
}

bootcmd_usb0 {
    setenv devnum 0;
    run usb_boot
}

bootcmd_pxe {
    run boot_net_usb_start;
    dhcp;
    if pxe get; then
        pxe boot;
    fi
}

bootcmd_dhcp {
    run boot_net_usb_start;
    if dhcp ${scriptaddr} ${boot_script_dhcp}; then
        source ${scriptaddr};
    fi;
    setenv efi_fdtfile ${fdtfile};
    if test -z "${fdtfile}" -a -n "${soc}"; then
        setenv efi_fdtfile ${soc}-${board}${boardver}.dtb;
    fi;
    setenv efi_old_vci ${bootp_vci};
    setenv efi_old_arch ${bootp_arch};
    setenv bootp_vci PXEClient:Arch:00010:UNDI:003000;
    setenv bootp_arch 0xa;
    if dhcp ${kernel_addr_r}; then
        tftpboot ${fdt_addr_r} dtb/${efi_fdtfile};
        if fdt addr ${fdt_addr_r}; then
            bootefi ${kernel_addr_r} ${fdt_addr_r};
        else
            bootefi ${kernel_addr_r} ${fdtcontroladdr};
        fi;
    fi;
    setenv bootp_vci ${efi_old_vci};
    setenv bootp_arch ${efi_old_arch};
    setenv efi_fdtfile;
    setenv efi_old_arch;
    setenv efi_old_vci;
}

distro_bootcmd {
    for target in ${boot_targets}; do
        run bootcmd_${target};
    done
}
```
