## Source

- This procedure is tested on busybox `1.26.2` and maybe applicable for other busybox versions.
- The busybox source code can be found at [https://busybox.net/downloads/](https://busybox.net/downloads/).

## Compiling

- We compile busybox with static library.

```shell
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- distclean
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- defconfig

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- menuconfig
    # Busybox Settings -> Build Options -> Build BusyBox as a static library (no shared libs)

make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- install CONFIG_PREFIX=/tmp/busybox
```
