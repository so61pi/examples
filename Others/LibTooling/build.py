#
# print all clang & LLVM lib files
#

import os
import subprocess

llvmhome = "E:/llvm/"
mainfile = "tool.cpp"

clanglibs = ""
llvmlibs  = ""
otherlibs = ""
for file in os.listdir(llvmhome + "build/Release/lib"):
    if file.endswith(".lib"):
        if file.startswith("clang"):
            clanglibs += file + " "
        elif file.startswith("LLVM"):
            llvmlibs += file + " "
        else:
            otherlibs += file + " "

command = "cl /EHsc /MD " + mainfile + " "
for path in ["/llvm/tools/clang/include/", "/llvm/include/", "/build/include/", "/build/tools/clang/include/"]:
    command += "/I" + llvmhome + path + " "
command += clanglibs + " " + llvmlibs + " "
command += "/link /LIBPATH:" + llvmhome + "build/Release/lib"

subprocess.call(command)
