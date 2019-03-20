- The U-Boot version is `v2017.11`.
- `bootcmd` is the entry.
- The script is gotten by U-Boot command `printenv`.

```shell
arch=arm
baudrate=115200
board=sunxi
board_name=sunxi
boot_prefixes=/ /boot/
boot_script_dhcp=boot.scr.uimg
boot_scripts=boot.scr.uimg boot.scr
boot_targets=fel mmc0 scsi0 usb0 pxe dhcp 
bootdelay=2
bootm_size=0xa000000
console=ttyS0,115200
cpu=armv7
efi_dtb_prefixes=/ /dtb/ /dtb/current/
ethaddr=02:d7:08:82:b7:37
fdt_addr_r=0x43000000
fdtcontroladdr=7af41c80
fdtfile=sun7i-a20-cubieboard2.dtb
kernel_addr_r=0x42000000
mmc_bootdev=0
pxefile_addr_r=0x43200000
ramdisk_addr_r=0x43300000
scriptaddr=0x43100000
serial#=165166d70882b737
soc=sunxi
stderr=serial,vga
stdin=serial,usbkbd
stdout=serial,vga


boot_a_script {
    load ${devtype} ${devnum}:${distro_bootpart} ${scriptaddr} ${prefix}${script};
    source ${scriptaddr}
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


boot_extlinux {
    sysboot ${devtype} ${devnum}:${distro_bootpart} any ${scriptaddr} ${prefix}extlinux/extlinux.conf
}


boot_net_usb_start {
    usb start
}


bootcmd {
    run distro_bootcmd
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


bootcmd_fel {
    if test -n ${fel_booted} && test -n ${fel_scriptaddr}; then
        echo '(FEL boot)';
        source ${fel_scriptaddr};
    fi
}


bootcmd_mmc0 {
    setenv devnum 0;
    run mmc_boot
}


bootcmd_pxe {
    run boot_net_usb_start;
    dhcp;
    if pxe get; then
        pxe boot;
    fi
}


bootcmd_scsi0 {
    setenv devnum 0;
    run scsi_boot
}


bootcmd_usb0 {
    setenv devnum 0;
    run usb_boot
}



dfu_alt_info_ram {
    kernel ram 0x42000000 0x1000000;
    fdt ram 0x43000000 0x100000;
    ramdisk ram 0x43300000 0x4000000
}


distro_bootcmd {
    setenv scsi_need_init;
    for target in ${boot_targets}; do
        run bootcmd_${target};
    done
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


preboot {
    usb start
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


scsi_boot {
    run scsi_init;
    if scsi dev ${devnum}; then
        setenv devtype scsi;
        run scan_dev_for_boot_part;
    fi
}


scsi_init {
    if ${scsi_need_init}; then
        setenv scsi_need_init false;
        scsi scan;
    fi
}


usb_boot {
    usb start;
    if usb dev ${devnum}; then
        setenv devtype usb;
        run scan_dev_for_boot_part;
    fi
}
```
