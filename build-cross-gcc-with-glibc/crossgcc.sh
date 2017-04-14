#!/bin/bash


CORES=4

TARGETARCH=arm
TARGET=arm-none-linux-gnueabihf

WORKINGDIR="$HOME/work/tmp/build-cross-gcc"
CROSSGCC="$WORKINGDIR/crossgcc/`openssl rand -hex 16`"
SYSROOT="$CROSSGCC/$TARGET/sysroot"
SOURCEDIR="$WORKINGDIR/source"

PATH="$CROSSGCC/bin:$PATH"


# Download packages.
mkdir -p "$SOURCEDIR"
cd "$SOURCEDIR"

wget http://ftpmirror.gnu.org/binutils/binutils-2.28.tar.bz2
wget http://ftpmirror.gnu.org/glibc/glibc-2.25.tar.xz
wget http://ftpmirror.gnu.org/gcc/gcc-6.3.0/gcc-6.3.0.tar.bz2
wget https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.10.6.tar.xz

for f in *.tar*; do tar xf $f; done


# Compile binutils.
mkdir "$SOURCEDIR/build-binutils"
cd "$SOURCEDIR/build-binutils"

../binutils-2.28/configure --prefix="$CROSSGCC" --with-sysroot="$SYSROOT" --target=$TARGET --disable-multilib
make -j $CORES
make -j $CORES install-strip


# Install Linux headers.
cd "$SOURCEDIR/linux-4.10.6"
make ARCH=$TARGETARCH INSTALL_HDR_PATH="$SYSROOT/usr" headers_install


# Compile compilers only.
mkdir "$SOURCEDIR/build-gcc"
cd "$SOURCEDIR/build-gcc"

../gcc-6.3.0/configure --prefix="$CROSSGCC" --with-sysroot="$SYSROOT" --target=$TARGET --enable-languages=c,c++ --with-float=hard --with-mode=thumb --with-arch=armv7-a --enable-threads=posix --disable-multilib --disable-nls
make -j $CORES all-gcc
make -j $CORES install-gcc


# Install glibc headers and some files for libgcc.
mkdir "$SOURCEDIR/build-glibc"
cd "$SOURCEDIR/build-glibc"

../glibc-2.25/configure --prefix="/usr" --build=$MACHTYPE --host=$TARGET --target=$TARGET --with-headers="$SYSROOT/usr/include" --disable-multilib --enable-kernel=4.10.6
make -j $CORES install-bootstrap-headers=yes install-headers install_root="$SYSROOT"
make -j $CORES csu/subdir_lib
mkdir -p "$SYSROOT/usr/lib"
install csu/crt1.o csu/crti.o csu/crtn.o "$SYSROOT/usr/lib"
$TARGET-gcc -nostdlib -nostartfiles -shared -x c /dev/null -o "$SYSROOT/usr/lib/libc.so"
touch "$SYSROOT/usr/include/gnu/stubs.h"



# Compile libgcc.
cd "$SOURCEDIR/build-gcc"
make -j $CORES all-target-libgcc
make -j $CORES install-target-libgcc


# Compile glibc.
cd "$SOURCEDIR/build-glibc"
make -j $CORES
make -j $CORES install install_root="$SYSROOT"


# Compile everything in gcc.
cd "$SOURCEDIR/build-gcc"
make -j $CORES all
make -j $CORES install-strip
