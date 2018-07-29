## Documents

- https://developer.mozilla.org/en-US/docs/Mozilla/Developer_guide/Build_Instructions/Simple_Firefox_build/Linux_and_MacOS_build_preparation
- https://developer.mozilla.org/en-US/docs/Mozilla/Debugging
- https://developer.mozilla.org/en-US/docs/Mozilla/Developer_guide
- https://developer.mozilla.org/en-US/docs/Mozilla/QA/Running_automated_tests
- https://www.joshmatthews.net/bugsahoy/
- https://coventryuniversity.github.io/mozbugs/


## jsshell

- https://wiki.mozilla.org/JavaScript:New_to_SpiderMonkey
- https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey

```shell
cd mozilla-central/js/src


# Compile jsshell
cp -f configure.in configure; and chmod +x configure; and rm -rf build_DBG.OBJ; and mkdir build_DBG.OBJ; and cd build_DBG.OBJ; and ../configure --enable-debug --disable-optimize; and cd ..

make -j4 -C build_DBG.OBJ


# Debug locally
gdb --args ./build_DBG.OBJ/dist/bin/js -e 'function f() { return 1; } f();'


# Start gdbserver
gdbserver :2159 ./build_DBG.OBJ/dist/bin/js -e "'function f() { return 1; } f();'"

# On gdb client side
target remote 172.17.0.2:2159
directory ..
monitor exit
```
