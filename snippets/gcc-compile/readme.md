## Read https://gcc.gnu.org/wiki/InstallingGCC

## Install required things.

```shell
sudo apt-get install flex libgmp-dev libmpfr-dev libmpc-dev
```

## Configure - compile - install

```shell
mkdir build-gcc
cd build-gcc
../gcc/configure --prefix=$HOME/work/tools/gcc-trunk --program-suffix=-trunk --enable-languages=c,c++ --disable-multilib --enable-threads=posix
make
make install-strip
```

- Read more about configuration [here](https://gcc.gnu.org/install/configure.html).

## After install, there should be an instruction show you how to use the new library, it looks like below:

```txt
Libraries have been installed in:
   $HOME/work/tools/gcc/lib/../lib64

If you ever happen to want to link against installed libraries
in a given directory, LIBDIR, you must either use libtool, and
specify the full pathname of the library, or use the `-LLIBDIR'
flag during linking and do at least one of the following:
   - add LIBDIR to the `LD_LIBRARY_PATH' environment variable
     during execution
   - add LIBDIR to the `LD_RUN_PATH' environment variable
     during linking
   - use the `-Wl,-rpath -Wl,LIBDIR' linker flag
   - have your system administrator add LIBDIR to `/etc/ld.so.conf'

See any operating system documentation about shared libraries for
more information, such as the ld(1) and ld.so(8) manual pages.
```

- Use it when you need to compile and run your program.

```shell
export LD_RUN_PATH=LIBDIR
export LD_LIBRARY_PATH=LIBDIR
```

- **In case you have too many problems with multiple libraries in your system and just want a quick test, add `-static-libstdc++` to `g++` compile command (or link command).**
