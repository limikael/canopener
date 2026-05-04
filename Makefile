.PHONY: cof-defines test test-device test-wip

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

test-wip: cof-defines
	wrapcc g++ \
		-std=c++20 \
		-o bin/testmain \
		-Iinclude \
		src/Entry.cpp \
		src/EntryContainer.cpp \
		src/Pdo.cpp \
		test/test-EntryContainer.cpp \
		test/testmain.cpp
	./bin/testmain