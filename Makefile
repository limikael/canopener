WATCHSOURCES=test/*.cpp src/*.c* include/*.h include/canopener/*.h emcc_flags.rsp 

all: bin/test bin/mockdevice dist/cof-defines.js dist/cof-wasm.wasm dist/cof-wasm.js
js: dist/cof-wasm.wasm dist/cof-wasm.js dist/cof-defines.js

bin/mockdevice: $(WATCHSOURCES)
	g++ -obin/mockdevice -Iinclude src/*.c* test/mockdevice.cpp

bin/test: $(WATCHSOURCES)
	g++ -obin/test -Iinclude src/*.c* test/test-*.cpp

#bin/test-cof: ${WATCHSOURCES}
#	node scripts/generate-cof-defines.js
#	g++ -obin/test-cof -Iinclude src/cof.c src/cof-defines.c test/test-main-cof.cpp
#	./bin/test-cof

include/canopener/cof-defines.h src/cof-defines.c: lib/cof-schema.json
	node scripts/generate-cof-defines.js

dist/cof-wasm.wasm dist/cof-wasm.js: $(WATCHSOURCES)
	emcc @emcc_flags.rsp src/cof.c -Iinclude -odist/cof-wasm.js
