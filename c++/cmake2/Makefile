.PHONY: all
all:
	[ -d build ] || mkdir build
	cd build && cmake -G Ninja -DBUILD_WITH_SANITIZER=ON .. && cmake --build .


.PHONY: clean
clean:
	rm -rf build
