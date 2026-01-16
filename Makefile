OBJS=$(patsubst %.cpp, obj/%.o, $(notdir $(wildcard src/*.cpp test/test-*.cpp))) \
    $(patsubst %.cpp, obj/%.o, $(notdir src/canopener-quickjs.cpp)) \
    $(patsubst %.cpp, obj/%.o, $(notdir src/cof-defines.cpp))

vpath %.cpp src test

all: bin/test #bin/mockdevice

.PHONY: clean
clean:
	rm -f src/cof-defines.cpp
	rm -f include/canopener/cof-defines.h
	rm -f src/canopener-quickjs.cpp
	rm -f obj/*

.PHONY: test
test: bin/test
	./bin/test

include/canopener/cof-defines.h src/cof-defines.cpp: js/cof-schema.js
	node js/generate-cof-defines.js

src/canopener-quickjs.cpp include/canopener/canopener-quickjs.h: js/canopener-api.json
	peabind -o src/canopener-quickjs.cpp -I include/canopener -p canopener_quickjs_ js/canopener-api.json

obj:
	mkdir -p obj

obj/%.o: %.cpp include/*.h include/canopener/*.h include/canopener/canopener-quickjs.h include/canopener/cof-defines.h | obj 
	g++ -std=c++20 -Iinclude -Iext/quickjs -c $< -o $@

bin/test: $(OBJS) test/testmain.cpp 
	g++ -std=c++20 -Iinclude -o bin/test $^ ext/quickjs/libquickjs.a -Iext/quickjs

#bin/mockdevice: $(OBJS) test/mockdevice.cpp
#	g++ -std=c++20 -Iinclude $^ -o bin/mockdevice 
