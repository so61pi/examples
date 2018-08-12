## Documents

- https://developer.mozilla.org/en-US/docs/Mozilla/Developer_guide/Build_Instructions/Simple_Firefox_build/Linux_and_MacOS_build_preparation
- https://developer.mozilla.org/en-US/docs/Mozilla/Debugging
- https://developer.mozilla.org/en-US/docs/Mozilla/Developer_guide
- https://developer.mozilla.org/en-US/docs/Mozilla/QA/Running_automated_tests
- https://www.joshmatthews.net/bugsahoy/
- https://coventryuniversity.github.io/mozbugs/
- https://moz-conduit.readthedocs.io/en/latest/phabricator-user.html#submitting-patches


## jsshell

- https://wiki.mozilla.org/JavaScript:New_to_SpiderMonkey
- https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey

```shell
cd /mozilla-central/js/src


# Compile jsshell
autoconf2.13
mkdir build_DBG.OBJ
cd build_DBG.OBJ
../configure --enable-debug --disable-optimize
make -j4


# run tests
./tests/jstests.py ./build_DBG.OBJ/dist/bin/js


# Debug locally
gdb --args ./build_DBG.OBJ/dist/bin/js -e 'function f() { return 1; } f();'


# Start gdbserver
gdbserver :2159 ./build_DBG.OBJ/dist/bin/js -e "'function f() { return 1; } f();'"

# On gdb client side
target remote 172.17.0.2:2159
directory ..
monitor exit
```


## mach

```shell
mach lint <path>

mach doc
mach build
mach run

mach bootstrap
mach mercurial-setup

mach help
mach mach-commands

mach jstests
mach jsapi-tests

mach static-analysis
```
