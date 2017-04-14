1. Read http://llvm.org/docs/GettingStarted.html

2. Clone code and build

        git clone --depth 1 http://llvm.org/git/llvm.git
        cd llvm/tools
        git clone --depth 1 http://llvm.org/git/clang.git
        cd ..
        mkdir build
        cd build
        cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$HOME/work/tools/clang-trunk ..

        make -j4
        make install
