OBJS=$(patsubst %.cpp, obj/%.o, $(notdir $(wildcard src/*.cpp test/test-*.cpp))) \
    $(patsubst %.cpp, obj/%.o, $(notdir src/canopener-quickjs.cpp))

vpath %.cpp src test

all: bin/test #bin/mockdevice

.PHONY: test
test: bin/test
	./bin/test

include/canopener/cof-defines.h src/cof-defines.cpp: lib/cof-schema.js
	node scripts/generate-cof-defines.js

src/canopener-quickjs.cpp include/canopener/canopener-quickjs.h: lib/canopener.json
	peabind -o src/canopener-quickjs.cpp -I include/canopener -p canopener_quickjs_ lib/canopener.json

obj:
	mkdir -p obj

obj/%.o: %.cpp include/*.h include/canopener/*.h include/canopener/canopener-quickjs.h | obj 
	g++ -std=c++20 -Iinclude -Iext/quickjs -c $< -o $@

bin/test: $(OBJS) test/testmain.cpp 
	g++ -std=c++20 -Iinclude -o bin/test $^ ext/quickjs/libquickjs.a -Iext/quickjs

#bin/mockdevice: $(OBJS) test/mockdevice.cpp
#	g++ -std=c++20 -Iinclude $^ -o bin/mockdevice 
