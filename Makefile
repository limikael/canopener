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
	g++ -Iinclude -c $< -o $@

bin/test: $(OBJS) test/testmain.cpp
	g++ -Iinclude $^ -o bin/test 

bin/mockdevice: $(OBJS) test/mockdevice.cpp
	g++ -Iinclude $^ -o bin/mockdevice 
