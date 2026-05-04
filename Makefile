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
		src/Device.cpp \
		src/EntryContainer.cpp \
		src/MasterDevice.cpp \
		src/RemoteDevice.cpp \
		src/RemoteCmd.cpp \
		src/Pdo.cpp \
		src/cof.cpp \
		src/cof-defines.cpp \
		src/protocol.cpp \
		test/test-EntryContainer.cpp \
		test/test-Device.cpp \
		test/test-Bus.cpp \
		test/test-cof.cpp \
		test/test-castx.cpp \
		test/test-remote.cpp \
		test/test-DataView.cpp \
		test/testmain.cpp
	./bin/testmain