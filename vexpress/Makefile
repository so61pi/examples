CORES			=	4

CROSS_COMPILE	?=	arm-linux-gnueabihf-

RM				?=	rm
INSTALL			?=	install

LOOPDEVNAME		?=	loop0p1

WORKINGDIR		=	$(shell pwd)
OUTPUTDIR		=	$(WORKINGDIR)/output

UBOOTSRC		=	git://git.denx.de/u-boot.git
UBOOTVER		=	v2017.05
UBOOTDIR		=	$(WORKINGDIR)/u-boot

BUSYBOXSRC		=	git://git.busybox.net/busybox
BUSYBOXVER		=	1_26_2
BUSYBOXDIR		=	$(WORKINGDIR)/busybox

LINUXSRC		=	git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git
LINUXVER		=	v4.11.1
LINUXDIR		=	$(WORKINGDIR)/linux-stable

SDIMG 			=	$(OUTPUTDIR)/sdcard.img
NORFLASH0		=	$(OUTPUTDIR)/flash-nor-0.img
NORFLASH1		=	$(OUTPUTDIR)/flash-nor-1.img

ETCDIR			=	$(WORKINGDIR)/etc

ROOTFSDIR		=	$(OUTPUTDIR)/rootfs
SDIMGDIR		=	$(OUTPUTDIR)/sdcard-mount


.PHONY: all
all:
	@echo download - config - build - install
	@echo
	@echo --------------------
	@echo mmc dev 0
	@echo mmc rescan
	@echo mmcinfo
	@echo ext4ls mmc 0:1
	@echo 
	@echo --------------------
	@echo protect off all
	@echo erase all
	@echo flinfo
	@echo 
	@echo md 0x61000000
	@echo mw.b 0x61000000 0xAA 0x10
	@echo md 0x61000000
	@echo 
	@echo md 0x40000000
	@echo cp.b 0x61000000 0x40000000 0x10
	@echo md 0x40000000
	@echo 
	@echo md 0x44000000
	@echo cp.b 0x61000000 0x44000000 0x10
	@echo md 0x44000000


define pannelnotice
	@echo "----------------------------------------"
	$(if $1, @echo $1)
	$(if $2, @echo $2)
	$(if $3, @echo $3)
	$(if $4, @echo $4)
	$(if $5, @echo $5)
	@echo "----------------------------------------"
endef


define qemuwarn
	$(call pannelnotice,Use CTRL+A X to quit QEMU)
	@sleep 2
endef


################################################################################
#
# Running
#
.PHONY: run-uboot
run-uboot:
	$(call qemuwarn)
	qemu-system-arm \
	-M vexpress-a9 -cpu cortex-a9 \
	-m 512M \
	-nographic \
	-kernel $(OUTPUTDIR)/u-boot \
	-pflash $(NORFLASH0) \
	-pflash $(NORFLASH1)


.PHONY: run-kernel
run-kernel:
	$(call qemuwarn)
	qemu-system-arm \
	-M vexpress-a9 -cpu cortex-a9 \
	-m 512M \
	-nographic \
	-kernel $(OUTPUTDIR)/zImage \
	-dtb $(OUTPUTDIR)/vexpress-v2p-ca9.dtb \
	-append "console=ttyAMA0 root=/dev/mmcblk0p1" \
	-sd $(SDIMG) \
	-pflash $(NORFLASH0) \
	-pflash $(NORFLASH1)


.PHONY: run
run:
	$(call qemuwarn)
	qemu-system-arm \
	-M vexpress-a9 -cpu cortex-a9 \
	-m 512M \
	-nographic \
	-kernel $(OUTPUTDIR)/u-boot \
	-sd $(SDIMG) \
	-pflash $(NORFLASH0) \
	-pflash $(NORFLASH1)


################################################################################
#
# Building
#
.PHONY: clean
clean: $(addsuffix -clean, uboot busybox linux)


.PHONY: clean-output
clean-output:
	$(RM) -r $(OUTPUTDIR)


.PHONY: download
download: $(addsuffix -download, uboot busybox linux)


.PHONY: download-force
download-force: $(addsuffix -download-force, uboot busybox linux)


.PHONY: config
config: clean clean-output $(addsuffix -config, uboot busybox linux)


.PHONY: build
build: $(addsuffix -build, uboot busybox linux)


.PHONY: install
install: clean-output $(addsuffix -install, uboot busybox linux)
	$(INSTALL) -d $(ROOTFSDIR)/sys
	$(INSTALL) -d $(ROOTFSDIR)/dev
	$(INSTALL) -d $(ROOTFSDIR)/proc
	$(INSTALL) -d $(ROOTFSDIR)/tmp
	$(INSTALL) -d $(ROOTFSDIR)/etc
	$(INSTALL) -d $(ROOTFSDIR)/root
	$(INSTALL) -d $(ROOTFSDIR)/var
	cp -a $(ETCDIR)/. $(ROOTFSDIR)/etc

	@sudo true
	$(call pannelnotice,Use n to create partitions,Use w to save changes and quit)
	fallocate -l 64M $(SDIMG)
	fdisk $(SDIMG)

	sudo kpartx -a $(SDIMG)
	@echo "Wait 3 seconds..."
	@sleep 3
	sudo mkfs.ext3 -F /dev/mapper/$(LOOPDEVNAME)
	mkdir -p $(SDIMGDIR)
	sudo mount -t ext3 /dev/mapper/$(LOOPDEVNAME) $(SDIMGDIR) -o loop
	sudo cp -a $(ROOTFSDIR)/. $(SDIMGDIR)
	tree $(SDIMGDIR)
	sudo umount $(SDIMGDIR)
	sudo kpartx -d $(SDIMG)

	fallocate -l 64M $(NORFLASH0)
	fallocate -l 64M $(NORFLASH1)


################################################################################
#
# U-Boot
#
.PHONY: uboot-clean
.PHONY: uboot-download
.PHONY: uboot-download-force
.PHONY: uboot-config
.PHONY: uboot-build
.PHONY: uboot-install

uboot-clean:
	$(MAKE) -C $(UBOOTDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) distclean


uboot-download:
	[ -d $(notdir $(UBOOTDIR)) ] || git clone --branch $(UBOOTVER) $(UBOOTSRC)


uboot-download-force:
	$(RM) -r $(notdir $(UBOOTDIR))
	git clone --branch $(UBOOTVER) $(UBOOTSRC)


uboot-config: uboot-clean
	$(MAKE) -C $(UBOOTDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) vexpress_ca9x4_defconfig
	$(MAKE) -C $(UBOOTDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) menuconfig


uboot-build:
	$(MAKE) -C $(UBOOTDIR) -j$(CORES) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE)


uboot-install:
	$(INSTALL) -d $(OUTPUTDIR)
	$(INSTALL) $(UBOOTDIR)/u-boot $(OUTPUTDIR)


################################################################################
#
# BusyBox
#
.PHONY: busybox-clean
.PHONY: busybox-download
.PHONY: busybox-download-force
.PHONY: busybox-config
.PHONY: busybox-build
.PHONY: busybox-install

busybox-clean:
	$(MAKE) -C $(BUSYBOXDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) distclean


busybox-download:
	[ -d $(notdir $(BUSYBOXDIR)) ] || git clone --branch $(BUSYBOXVER) $(BUSYBOXSRC)


busybox-download-force:
	$(RM) -r $(notdir $(BUSYBOXDIR))
	git clone --branch $(BUSYBOXVER) $(BUSYBOXSRC)


busybox-config: busybox-clean
	$(MAKE) -C $(BUSYBOXDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) defconfig
	$(MAKE) -C $(BUSYBOXDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) menuconfig


busybox-build:
	$(MAKE) -C $(BUSYBOXDIR) -j$(CORES) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE)


busybox-install:
	$(INSTALL) -d $(ROOTFSDIR)
	$(MAKE) -C $(BUSYBOXDIR) -j$(CORES) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) install CONFIG_PREFIX=$(ROOTFSDIR)


################################################################################
#
# Linux
#
.PHONY: linux-clean
.PHONY: linux-download
.PHONY: linux-download-force
.PHONY: linux-config
.PHONY: linux-build
.PHONY: linux-install

linux-clean:
	$(MAKE) -C $(LINUXDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) distclean


linux-download:
	[ -d $(notdir $(LINUXDIR)) ] || git clone --branch $(LINUXVER) $(LINUXSRC)


linux-download-force:
	$(RM) -r $(notdir $(LINUXDIR))
	git clone --branch $(LINUXVER) $(LINUXSRC)


linux-config: linux-clean
	$(MAKE) -C $(LINUXDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) vexpress_defconfig
	$(MAKE) -C $(LINUXDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) menuconfig


linux-build:
	$(MAKE) -C $(LINUXDIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) all


linux-install:
	$(INSTALL) -d $(OUTPUTDIR)
	$(INSTALL) $(LINUXDIR)/arch/arm/boot/dts/vexpress-v2p-ca9.dtb $(OUTPUTDIR)
	$(INSTALL) $(LINUXDIR)/arch/arm/boot/zImage $(OUTPUTDIR)
