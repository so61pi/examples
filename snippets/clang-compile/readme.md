- Read https://llvm.org/docs/GettingStarted.html and https://llvm.org/docs/GettingStarted.html#git-mirror
- The version info is in file `llvm/CMakeLists.txt`, in the following variables
	- `LLVM_VERSION_MAJOR`
   	- `LLVM_VERSION_MINOR`
    - `LLVM_VERSION_PATCH`
- Clone code and build

```shell
export LLVMBRANCH=release_60
export VERSIONSTR=6.0.1
export BUILDCORES=8

git clone -b ${LLVMBRANCH} https://git.llvm.org/git/llvm.git/              llvm                                     && \
git clone -b ${LLVMBRANCH} https://git.llvm.org/git/clang.git/             llvm/tools/clang                         && \
git clone -b ${LLVMBRANCH} https://git.llvm.org/git/clang-tools-extra.git/ llvm/tools/clang/tools/clang-tools-extra && \
git clone -b ${LLVMBRANCH} https://git.llvm.org/git/compiler-rt.git/       llvm/projects/compiler-rt                && \
git clone -b ${LLVMBRANCH} https://git.llvm.org/git/libcxx.git/            llvm/projects/libcxx                     && \
git clone -b ${LLVMBRANCH} https://git.llvm.org/git/libcxxabi.git/         llvm/projects/libcxxabi

cd llvm
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$HOME/work/tools/clang-${VERSIONSTR} ..
cmake --build . --target install -- -j${BUILDCORES}

unset BUILDCORES
unset VERSIONSTR
unset LLVMBRANCH
```
