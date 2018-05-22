- Read https://llvm.org/docs/GettingStarted.html and https://llvm.org/docs/GettingStarted.html#git-mirror
- The version info is in file `llvm/CMakeLists.txt`, in the following variables
	- `LLVM_VERSION_MAJOR`
   	- `LLVM_VERSION_MINOR`
    - `LLVM_VERSION_PATCH`
- Clone code and build

```shell
git clone --depth 1 https://git.llvm.org/git/llvm.git/

cd llvm/tools
git clone --depth 1 https://git.llvm.org/git/clang.git/

cd ../projects
git clone --depth 1 https://git.llvm.org/git/compiler-rt.git/
git clone --depth 1 https://git.llvm.org/git/libcxx.git/
git clone --depth 1 https://git.llvm.org/git/libcxxabi.git/

cd ..
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$HOME/work/tools/clang-trunk ..

make -j4
make install
```
