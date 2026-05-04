.PHONY: cof-defines

cof-defines:
	node js/generate-cof-defines.js

test: cof-defines
	wrapcc g++ \
		-std=c++20 \
		-o bin/testmain \
		-Iinclude \
		src/*.cpp \
		test/test-*.cpp \
		test/testmain.cpp
	./bin/testmain