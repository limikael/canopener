OBJS=$(patsubst %.cpp, obj/%.o, $(notdir $(wildcard src/*.cpp test/test-*.cpp)))

vpath %.cpp src test

all: bin/test bin/mockdevice

.PHONY: test
test: bin/test
	./bin/test

include/canopener/cof-defines.h src/cof-defines.cpp: lib/cof-schema.js
	node scripts/generate-cof-defines.js

obj:
	mkdir -p obj

obj/%.o: %.cpp include/*.h include/canopener/*.h | obj 
	g++ -std=c++20 -Iinclude -c $< -o $@

bin/test: $(OBJS) test/testmain.cpp
	g++ -std=c++20 -Iinclude $^ -o bin/test 

bin/mockdevice: $(OBJS) test/mockdevice.cpp
	g++ -std=c++20 -Iinclude $^ -o bin/mockdevice 
