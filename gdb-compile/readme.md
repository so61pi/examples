## Read

- https://sourceware.org/gdb/wiki/BuildingCrossGDBandGDBserver

## Get the source code

- You can get it from
  - https://git.linaro.org/toolchain/binutils-gdb.git
  - https://www.gnu.org/software/gdb/current/

## Compile

- For running on PC, debugging PC program

```shell
mkdir build && cd build
../configure
make -j4
```

- For running on PC, debugging ARM program on board (remote debugging)

```shell
mkdir build && cd build
../configure --target=arm-linux-gnueabi
make -j4
```

- For running on ARM board (debug ARM program of course)

```shell
mkdir build && cd build
../configure --build=x86_64-pc-linux-gnu --host=arm-linux-gnueabi
make -j4
```
