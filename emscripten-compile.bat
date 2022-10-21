rem rmdir build/Emscripten /s/q
mkdir "build"
mkdir "build/Emscripten"
cmake -S . -B build/Emscripten -G Ninja ^
 -DCMAKE_BUILD_TYPE=Release ^
 -DCMAKE_TOOLCHAIN_FILE="%EMSDK%/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" ^
 -DUNICORE_EXAMPLES_ALL=ON
cmake --build build/Emscripten

ls -sh1 build/emscripten/examples/minimal.*
ls -sh1 build/emscripten/examples/testbed.*
ls -sh1 build/emscripten/examples/ui.*
ls -sh1 build/emscripten/examples/wasm.*

pause 0