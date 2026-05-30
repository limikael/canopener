.PHONY: cof-defines test test-device test-wip test-standalone test-main

test: test-main test-standalone

cof-defines:
	node js/generate-cof-defines.js

check-dep-diff:
	diff include/async_primitives.hpp node_modules/peabind/include/async_primitives.hpp

test-main: cof-defines check-dep-diff
	peabind js/bindings.json -tquickjs -otest/bindings.out.cpp -pcanopener_
	rm -f vgcore.*
	wrapcc --linker=g++ gcc \
		-DPEABIND \
		-g -O0 \
		-std=c++20 \
		-o bin/testmain \
		-Iinclude \
		$(shell peabind --lib-conf cargs -tquickjs) \
		src/*.cpp \
		test/test-*.cpp \
		test/testmain.cpp \
		test/bindings.out.cpp \
		$(shell peabind --lib-conf vendor-cargs -tquickjs)
	valgrind --quiet \
		--leak-check=full \
		--show-leak-kinds=all \
		--error-exitcode=1 \
		--errors-for-leak-kinds=all \
		./bin/testmain

test-standalone: cof-defines check-dep-diff
	wrapcc --linker=g++ gcc \
		-o bin/teststandalone \
		-g -O0 \
		-std=c++20 \
		-Iinclude \
		test/teststandalone.cpp
	valgrind --quiet \
		--leak-check=full \
		--show-leak-kinds=all \
		--error-exitcode=1 \
		--errors-for-leak-kinds=all \
		./bin/teststandalone
