.PHONY: cof-defines test test-device test-wip

cof-defines:
	node js/generate-cof-defines.js

test: cof-defines
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
