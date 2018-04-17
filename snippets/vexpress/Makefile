CORES			=	4

CROSS_COMPILE	?=	arm-linux-gnueabihf-

RM				?=	rm
INSTALL			?=	install

LOOP_DEV_NAME	?=	loop0p1
LOOP_DEV_FILE	=	/dev/mapper/$(LOOP_DEV_NAME)

WORKING_DIR		=	$(shell pwd)
INSTALL_DIR		=	$(WORKING_DIR)/_install

UBOOT_SRC			=	git://git.denx.de/u-boot.git
UBOOT_VER			=	v2017.05
UBOOT_DIR			=	$(WORKING_DIR)/u-boot
UBOOT_FILE			=	$(UBOOT_DIR)/u-boot
UBOOT_FINAL_FILE	= $(INSTALL_DIR)/u-boot

BUSYBOX_SRC		=	git://git.busybox.net/busybox
BUSYBOX_VER		=	1_26_2
BUSYBOX_DIR		=	$(WORKING_DIR)/busybox

LINUX_SRC				=	git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git
LINUX_VER				=	v4.11.1
LINUX_DIR				=	$(WORKING_DIR)/linux-stable
LINUX_IMAGE_FILE		=	$(LINUX_DIR)/arch/arm/boot/zImage
LINUX_DTB_FILE			=	$(LINUX_DIR)/arch/arm/boot/dts/vexpress-v2p-ca9.dtb
LINUX_IMAGE_FINAL_FILE	=	$(INSTALL_DIR)/zImage
LINUX_DTB_FINAL_FILE	=	$(INSTALL_DIR)/vexpress-v2p-ca9.dtb

SD_IMAGE_FINAL_FILE 	=	$(INSTALL_DIR)/sdcard.img
NOR_FLASH_0_FINAL_FILE	=	$(INSTALL_DIR)/flash-nor-0.img
NOR_FLASH_1_FINAL_FILE	=	$(INSTALL_DIR)/flash-nor-1.img

ETC_DIR			=	$(WORKING_DIR)/etc

ROOTFS_DIR		=	$(INSTALL_DIR)/rootfs
SD_IMAGE_DIR	=	$(INSTALL_DIR)/sdcard-mount


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
	-kernel $(UBOOT_FINAL_FILE) \
	-pflash $(NOR_FLASH_0_FINAL_FILE) \
	-pflash $(NOR_FLASH_1_FINAL_FILE)


.PHONY: run-kernel
run-kernel:
	$(call qemuwarn)
	qemu-system-arm \
	-M vexpress-a9 -cpu cortex-a9 \
	-m 512M \
	-nographic \
	-kernel $(LINUX_IMAGE_FINAL_FILE) \
	-dtb $(LINUX_DTB_FINAL_FILE) \
	-append "console=ttyAMA0 root=/dev/mmcblk0p1" \
	-sd $(SD_IMAGE_FINAL_FILE) \
	-pflash $(NOR_FLASH_0_FINAL_FILE) \
	-pflash $(NOR_FLASH_1_FINAL_FILE)


.PHONY: run
run:
	$(call qemuwarn)
	qemu-system-arm \
	-M vexpress-a9 -cpu cortex-a9 \
	-m 512M \
	-nographic \
	-kernel $(UBOOT_FINAL_FILE) \
	-sd $(SD_IMAGE_FINAL_FILE) \
	-pflash $(NOR_FLASH_0_FINAL_FILE) \
	-pflash $(NOR_FLASH_1_FINAL_FILE)


################################################################################
#
# Building
#
.PHONY: clean
clean: $(addsuffix -clean, uboot busybox linux)


.PHONY: distclean
distclean: $(addsuffix -distclean, uboot busybox linux)


.PHONY: clean-installdir
clean-installdir:
	$(RM) -r $(INSTALL_DIR)


.PHONY: download
download: $(addsuffix -download, uboot busybox linux)


.PHONY: download-force
download-force: $(addsuffix -download-force, uboot busybox linux)


.PHONY: config
config: clean clean-output $(addsuffix -config, uboot busybox linux)


.PHONY: build
build: $(addsuffix -build, uboot busybox linux)


.PHONY: install
install: clean-installdir $(addsuffix -install, uboot busybox linux)
	mkdir -p $(ROOTFS_DIR)/sys
	mkdir -p $(ROOTFS_DIR)/dev
	mkdir -p $(ROOTFS_DIR)/proc
	mkdir -p $(ROOTFS_DIR)/tmp
	mkdir -p $(ROOTFS_DIR)/etc
	mkdir -p $(ROOTFS_DIR)/root
	mkdir -p $(ROOTFS_DIR)/var
	cp -a $(ETC_DIR)/. $(ROOTFS_DIR)/etc

	@sudo true
	$(call pannelnotice,Use n to create partitions,Use w to save changes and quit)
	fallocate -l 64M $(SD_IMAGE_FINAL_FILE)
	fdisk $(SD_IMAGE_FINAL_FILE)

	sudo kpartx -a $(SD_IMAGE_FINAL_FILE)
	@echo "Wait 3 seconds..."
	@sleep 3
	sudo mkfs.ext3 -F $(LOOP_DEV_FILE)
	mkdir -p $(SD_IMAGE_DIR)
	sudo mount -t ext3 $(LOOP_DEV_FILE) $(SD_IMAGE_DIR) -o loop
	sudo cp -a $(ROOTFS_DIR)/. $(SD_IMAGE_DIR)
	tree $(SD_IMAGE_DIR)
	sudo umount $(SD_IMAGE_DIR)
	sudo kpartx -d $(SD_IMAGE_FINAL_FILE)

	fallocate -l 64M $(NOR_FLASH_0_FINAL_FILE)
	fallocate -l 64M $(NOR_FLASH_1_FINAL_FILE)


################################################################################
#
# U-Boot
#
.PHONY: uboot-distclean
.PHONY: uboot-clean
.PHONY: uboot-download
.PHONY: uboot-download-force
.PHONY: uboot-config
.PHONY: uboot-build
.PHONY: uboot-install

uboot-distclean:
	$(MAKE) -C $(UBOOT_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) distclean


uboot-clean:
	$(MAKE) -C $(UBOOT_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) clean


uboot-download:
	[ -d $(notdir $(UBOOT_DIR)) ] || git clone --branch $(UBOOT_VER) $(UBOOT_SRC)


uboot-download-force:
	$(RM) -r $(notdir $(UBOOT_DIR))
	git clone --branch $(UBOOT_VER) $(UBOOT_SRC)


uboot-config: uboot-clean
	$(MAKE) -C $(UBOOT_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) vexpress_ca9x4_defconfig
	$(MAKE) -C $(UBOOT_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) menuconfig


uboot-build:
	$(MAKE) -C $(UBOOT_DIR) -j$(CORES) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE)


uboot-install:
	mkdir -p $(INSTALL_DIR)
	cp $(UBOOT_FILE) $(UBOOT_FINAL_FILE)


################################################################################
#
# BusyBox
#
.PHONY: busybox-distclean
.PHONY: busybox-clean
.PHONY: busybox-download
.PHONY: busybox-download-force
.PHONY: busybox-config
.PHONY: busybox-build
.PHONY: busybox-install

busybox-distclean:
	$(MAKE) -C $(BUSYBOX_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) distclean


busybox-clean:
	$(MAKE) -C $(BUSYBOX_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) clean


busybox-download:
	[ -d $(notdir $(BUSYBOX_DIR)) ] || git clone --branch $(BUSYBOX_VER) $(BUSYBOX_SRC)


busybox-download-force:
	$(RM) -r $(notdir $(BUSYBOX_DIR))
	git clone --branch $(BUSYBOX_VER) $(BUSYBOX_SRC)


busybox-config: busybox-clean
	$(MAKE) -C $(BUSYBOX_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) defconfig
	$(MAKE) -C $(BUSYBOX_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) menuconfig


busybox-build:
	$(MAKE) -C $(BUSYBOX_DIR) -j$(CORES) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE)


busybox-install:
	mkdir -p $(ROOTFS_DIR)
	$(MAKE) -C $(BUSYBOX_DIR) -j$(CORES) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) install CONFIG_PREFIX=$(ROOTFS_DIR)


################################################################################
#
# Linux
#
.PHONY: linux-distclean
.PHONY: linux-clean
.PHONY: linux-download
.PHONY: linux-download-force
.PHONY: linux-config
.PHONY: linux-build
.PHONY: linux-install

linux-distclean:
	$(MAKE) -C $(LINUX_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) distclean


linux-clean:
	$(MAKE) -C $(LINUX_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) clean


linux-download:
	[ -d $(notdir $(LINUX_DIR)) ] || git clone --branch $(LINUX_VER) $(LINUX_SRC)


linux-download-force:
	$(RM) -r $(notdir $(LINUX_DIR))
	git clone --branch $(LINUX_VER) $(LINUX_SRC)


linux-config: linux-clean
	$(MAKE) -C $(LINUX_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) vexpress_defconfig
	$(MAKE) -C $(LINUX_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) menuconfig


linux-build:
	$(MAKE) -C $(LINUX_DIR) ARCH=arm CROSS_COMPILE=$(CROSS_COMPILE) all


linux-install:
	mkdir -p $(INSTALL_DIR)
	cp $(LINUX_DTB_FILE) $(LINUX_DTB_FINAL_FILE)
	cp $(LINUX_IMAGE_FILE) $(LINUX_IMAGE_FINAL_FILE)
