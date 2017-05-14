**create layer**

- This layer is created by issuing `yocto-layer create mylayer` with example.


**prepare**

- Add `/opt/yocto/meta-mylayer` to `BBLAYERS` in `conf/bblayers.conf`.
- Add `CORE_IMAGE_EXTRA_INSTALL += "example"` to `conf/local.conf`.


**build**

```shell
bitbake example
bitbake core-image-minimal
```


**run**

```shell
runqemu qemux86 tmp/deploy/images/qemux86/bzImage-qemux86.bin tmp/deploy/images/qemux86/core-image-minimal-qemux86.ext4
```