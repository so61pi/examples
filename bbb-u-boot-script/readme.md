- The U-Boot version is `v2017.01`.
- `bootcmd` is the entry.
- The script is gotten by U-Boot command `printenv`.

```shell
arch="arm"
baudrate="115200"
board="am335x"
board_name="A335BNLT"
board_rev="00C0"
board_serial="3016BBBK0034"
boot_fdt="try"
boot_fit="0"
boot_prefixes="/ /boot/"
boot_script_dhcp="boot.scr.uimg"
boot_scripts="boot.scr.uimg boot.scr"
boot_targets="mmc0 legacy_mmc0 mmc1 legacy_mmc1 nand0 pxe dhcp "
bootcount="1"
bootdelay="2"
bootdir="/boot"
bootenvfile="uEnv.txt"
bootfile="zImage"
bootm_size="0x10000000"
bootpart="0:2"
console="ttyO0,115200n8"
cpu="armv7"
dfu_alt_info_emmc="rawemmc raw 0 3751936"
dfu_alt_info_mmc="boot part 0 1;rootfs part 0 2;MLO fat 0 1;MLO.raw raw 0x100 0x100;u-boot.img.raw raw 0x300 0x400;spl-os-args.raw raw 0x80 0x80;spl-os-image.raw raw 0x900 0x2000;spl-os-args fat 0 1;spl-os-image fat 0 1;u-boot.img fat 0 1;uEnv.txt fat 0 1"
dfu_alt_info_ram="kernel ram 0x80200000 0x4000000;fdt ram 0x80f80000 0x80000;ramdisk ram 0x81000000 0x4000000"
efi_dtb_prefixes="/ /dtb/ /dtb/current/"
eth1addr="04:a3:16:ad:2f:74"
ethact="cpsw"
ethaddr="04:a3:16:ad:2f:72"
fdt_addr_r="0x88000000"
fdtaddr="0x88000000"
fdtfile="undefined"
fit_bootfile="fitImage.itb"
fit_loadaddr="0x88000000"
kernel_addr_r="0x82000000"
loadaddr="0x82000000"
mmcdev="0"
mmcrootfstype="ext4 rootwait"
nfsopts="nolock"
partitions="uuid_disk=${uuid_gpt_disk};name=rootfs,start=2MiB,size=-,uuid=${uuid_gpt_rootfs}"
pxefile_addr_r="0x80100000"
ramdisk_addr_r="0x88080000"
ramroot="/dev/ram0 rw"
ramrootfstype="ext2"
rdaddr="0x88080000"
rootpath="/export/rootfs"
scriptaddr="0x80000000"
soc="am33xx"
spibusno="0"
spiimgsize="0x362000"
spiroot="/dev/mtdblock4 rw"
spirootfstype="jffs2"
spisrcaddr="0xe0000"
static_ip="${ipaddr}:${serverip}:${gatewayip}:${netmask}:${hostname}::off"
stderr="ns16550_serial"
stdin="ns16550_serial"
stdout="ns16550_serial"
usbnet_devaddr="04:a3:16:ad:2f:72"
vendor="ti"
ver="U-Boot 2017.01-dirty (Mar 10 2017 - 19:28:09 +0700)"


args_fit {
    setenv bootargs console=${console} 
}


args_mmc {
    run finduuid;
    setenv bootargs console=${console} ${optargs} root=PARTUUID=${uuid} rw rootfstype=${mmcrootfstype}
}


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


boot_net_usb_start {
    usb start
}


bootcmd {
    if test ${boot_fit} -eq 1; then
        run update_to_fit;
    fi;
    run findfdt;
    run init_console;
    run envboot;
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


bootcmd_legacy_mmc0 {
    setenv mmcdev 0;
    setenv bootpart 0:2 ;
    run mmcboot
}


bootcmd_legacy_mmc1 {
    setenv mmcdev 1;
    setenv bootpart 1:2 ;
    run mmcboot
}


bootcmd_mmc0 {
    setenv devnum 0;
    run mmc_boot
}


bootcmd_mmc1 {
    setenv devnum 1;
    run mmc_boot
}


bootcmd_nand {
    run nandboot
}


bootcmd_pxe {
    run boot_net_usb_start;
    dhcp;
    if pxe get; then
        pxe boot;
    fi
}


bootscript {
    echo Running bootscript from mmc${mmcdev} ...;
    source ${loadaddr}
}


distro_bootcmd {
    for target in ${boot_targets}; do
        run bootcmd_${target};
    done
}


envboot {
    mmc dev ${mmcdev};
    if mmc rescan; then
        echo SD/MMC found on device ${mmcdev};
        if run loadbootscript; then
            run bootscript;
        else
            if run loadbootenv; then
                echo Loaded env from ${bootenvfile};
                run importbootenv;
            fi;
            if test -n $uenvcmd; then
                echo Running uenvcmd ...;
                run uenvcmd;
            fi;
        fi;
    fi;
}


findfdt {
    if test $board_name = A335BONE; then
        setenv fdtfile am335x-bone.dtb;
    fi;
    if test $board_name = A335BNLT; then
        setenv fdtfile am335x-boneblack.dtb;
    fi;
    if test $board_name = BBG1; then
        setenv fdtfile am335x-bonegreen.dtb;
    fi;
    if test $board_name = A33515BB; then
        setenv fdtfile am335x-evm.dtb;
    fi;
    if test $board_name = A335X_SK; then
        setenv fdtfile am335x-evmsk.dtb;
    fi;
    if test $board_name = A335_ICE; then
        setenv fdtfile am335x-icev2.dtb;
    fi;
    if test $fdtfile = undefined; then
        echo WARNING: Could not determine device tree to use;
    fi;
}


finduuid {
    part uuid mmc ${bootpart} uuid
}


importbootenv {
    echo Importing environment from mmc${mmcdev} ...;
    env import -t ${loadaddr} ${filesize}
}


init_console {
    if test $board_name = A335_ICE; then
        setenv console ttyO3,115200n8;
    else
        setenv console ttyO0,115200n8;
    fi;
}


load_efi_dtb {
    load ${devtype} ${devnum}:${distro_bootpart} ${fdt_addr_r} ${prefix}${efi_fdtfile}
}


loadbootenv {
    fatload mmc ${mmcdev} ${loadaddr} ${bootenvfile}
}


loadbootscript {
    load mmc ${mmcdev} ${loadaddr} boot.scr
}


loadfdt {
    load ${devtype} ${bootpart} ${fdtaddr} ${bootdir}/${fdtfile}
}


loadfit {
    run args_fit;
    bootm ${loadaddr}:kernel@1 ${loadaddr}:ramdisk@1 ${loadaddr}:${fdtfile};
}


loadimage {
    load ${devtype} ${bootpart} ${loadaddr} ${bootdir}/${bootfile}
}


loadramdisk {
    load mmc ${mmcdev} ${rdaddr} ramdisk.gz
}


mmc_boot {
    if mmc dev ${devnum}; then
        setenv devtype mmc;
        run scan_dev_for_boot_part;
    fi
}


mmcboot {
    mmc dev ${mmcdev};
    setenv devnum ${mmcdev};
    setenv devtype mmc;
    if mmc rescan; then
        echo SD/MMC found on device ${mmcdev};
        if run loadimage; then
            if test ${boot_fit} -eq 1; then
                run loadfit;
            else
                run mmcloados;
            fi;
        fi;
    fi;
}


mmcloados {
    run args_mmc;
    if test ${boot_fdt} = yes || test ${boot_fdt} = try; then
        if run loadfdt; then
            bootz ${loadaddr} - ${fdtaddr};
        else
            if test ${boot_fdt} = try; then
                bootz;
            else
                echo WARN: Cannot load the DT;
            fi;
        fi;
    else
        bootz;
    fi;
}


netargs {
    setenv bootargs console=${console} ${optargs} root=/dev/nfs nfsroot=${serverip}:${rootpath},${nfsopts} rw ip=dhcp
}


netboot {
    echo Booting from network ...;
    setenv autoload no;
    dhcp;
    run netloadimage;
    run netloadfdt;
    run netargs;
    bootz ${loadaddr} - ${fdtaddr}
}


netloadfdt {
    tftp ${fdtaddr} ${fdtfile}
}


netloadimage {
    tftp ${loadaddr} ${bootfile}
}


ramargs {
    setenv bootargs console=${console} ${optargs} root=${ramroot} rootfstype=${ramrootfstype}
}


ramboot {
    echo Booting from ramdisk ...;
    run ramargs;
    bootz ${loadaddr} ${rdaddr} ${fdtaddr}
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


spiargs {
    setenv bootargs console=${console} ${optargs} root=${spiroot} rootfstype=${spirootfstype}
}


spiboot {
    echo Booting from spi ...;
    run spiargs;
    sf probe ${spibusno}:0;
    sf read ${loadaddr} ${spisrcaddr} ${spiimgsize};
    bootz ${loadaddr}
}


update_to_fit {
    setenv loadaddr ${fit_loadaddr};
    setenv bootfile ${fit_bootfile}
}


usb_boot {
    usb start;
    if usb dev ${devnum}; then
        setenv devtype usb;
        run scan_dev_for_boot_part;
    fi
}
```