rem rmdir build/Emscripten /s/q
mkdir "build"
mkdir "build/Emscripten"
cmake -S . -B build/Emscripten -G Ninja ^
 -DCMAKE_BUILD_TYPE=Release ^
 -DCMAKE_TOOLCHAIN_FILE="%EMSDK%/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake" ^
 -DUNICORE_MINIMAL=ON
cmake --build build/Emscripten
pause 0
