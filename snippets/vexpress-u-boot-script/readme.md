***Below is the U-Boot script for vexpress-a9.***

- The U-Boot version is `v2017.05`.
- `bootcmd` is the entry.
- The script is gotten by U-Boot command `printenv`.

```
arch=arm
baudrate=38400
board=vexpress
board_name=vexpress
boot_prefixes=/ /boot/
boot_script_dhcp=boot.scr.uimg
boot_scripts=boot.scr.uimg boot.scr
boot_targets=mmc1 mmc0 pxe dhcp 
bootargs=root=/dev/sda1 rw console=ttyAMA0,38400n8 mem=1024M mtdparts=armflash:1M@0x800000(uboot),7M@0x1000000(kernel),24M@0x2000000(initrd) mmci.fmax=190000 devtmpfs.mount=0 vmalloc=256M
bootdelay=2
bootfile=boot.scr.uimg
bootfstype=ext4
console=ttyAMA0,38400n8
cpu=armv7
devnum=0
devplist=1
devtype=mmc
dram=1024M
efi_dtb_prefixes=/ /dtb/ /dtb/current/
ethact=smc911x-0
ethaddr=52:54:00:12:34:56
kernel_addr=0x44100000
kernel_addr_r=0x80008000
loadaddr=0x80008000
maxramdisk=0x1800000
mtd=armflash:1M@0x800000(uboot),7M@0x1000000(kernel),24M@0x2000000(initrd)
pxefile_addr_r=0x88000000
ramdisk_addr=0x44800000
ramdisk_addr_r=0x61000000
root=/dev/sda1 rw
scriptaddr=0x88000000
stderr=serial
stdin=serial
stdout=serial
vendor=armltd


boot_a_script {
    load ${devtype} ${devnum}:${distro_bootpart} ${scriptaddr} ${prefix}${script};
    source ${scriptaddr}
}


boot_efi_binary {
    load ${devtype} ${devnum}:${distro_bootpart} ${kernel_addr_r} efi/boot/bootarm.efi;
    if fdt addr ${fdt_addr_r}; then
        bootefi ${kernel_addr_r} ${fdt_addr_r};
    else
        bootefi ${kernel_addr_r} ${fdtcontroladdr};
    fi
}


boot_extlinux {
    sysboot ${devtype} ${devnum}:${distro_bootpart} any ${scriptaddr} ${prefix}extlinux/extlinux.conf
}


bootcmd {
    run distro_bootcmd;
    run bootflash;
}


bootcmd_dhcp {
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


bootcmd_mmc0 {
    setenv devnum 0;
    run mmc_boot
}


bootcmd_mmc1 {
    setenv devnum 1;
    run mmc_boot
}


bootcmd_pxe {
    dhcp;
    if pxe get; then
        pxe boot;
    fi
}


bootflash {
    run flashargs;
    cp ${ramdisk_addr} ${ramdisk_addr_r} ${maxramdisk};
    bootm ${kernel_addr} ${ramdisk_addr_r}
}


distro_bootcmd {
    for target in ${boot_targets}; do
        run bootcmd_${target};
    done
}


flashargs {
    setenv bootargs root=${root} console=${console} mem=${dram} mtdparts=${mtd} mmci.fmax=190000 devtmpfs.mount=0  vmalloc=256M
}


load_efi_dtb {
    load ${devtype} ${devnum}:${distro_bootpart} ${fdt_addr_r} ${prefix}${efi_fdtfile}
}


mmc_boot {
    if mmc dev ${devnum}; then
        setenv devtype mmc;
        run scan_dev_for_boot_part;
    fi
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


scan_dev_for_extlinux {
    if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}extlinux/extlinux.conf; then
        echo Found ${prefix}extlinux/extlinux.conf;
        run boot_extlinux;
        echo SCRIPT FAILED: continuing...;
    fi
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
```