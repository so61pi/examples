**Software**

- GCC 6.3.0
- Binutils 2.28
- Glibc 2.25
- Linux 4.10.6


**Note**

- If the `--with-sysroot` directory is a subdirectory of `--exec-prefix`, then **it will be found relative to the GCC binaries if the installation tree is moved**.
    + Documented at https://gcc.gnu.org/install/configure.html, option `--with-sysroot`.
    + Implemented in `gcc/gcc.c:4514`.
    + Example [here](https://sourceware.org/ml/crossgcc/2013-06/msg00027.html).
- The `--sysroot` will be passed to linker and its path is concatenated with the paths in `libc.so`. This can leads to something like `/path/to/sysroot/path/to/sysroot/...`.


**Reference**

- https://gcc.gnu.org/install/
- https://gcc.gnu.org/onlinedocs/libstdc++/manual/configure.html
- https://www.gnu.org/software/libc/manual/html_node/Configuring-and-compiling.html
- https://wiki.linaro.org/WorkingGroups/ToolChain/BuildingGNUToolchains
- http://preshing.com/20141119/how-to-build-a-gcc-cross-compiler/
- https://sourceware.org/ml/crossgcc/2013-06/msg00027.html
