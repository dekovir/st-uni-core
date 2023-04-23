#wasm3
set(BUILD_WASI none CACHE STRING "" FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/wasm3/source")
set_target_properties(m3 PROPERTIES FOLDER "Libs")

#unicore-wasm
set(UNICORE_WASM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UNICORE_WASM_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE UNICORE_WASM_SOURCE 
	"${UNICORE_WASM_DIR}/*.cpp"
	"${UNICORE_WASM_DIR}/*.hpp"
	"${UNICORE_WASM_INCLUDE}/*.hpp"
)

add_library(unicore-wasm STATIC ${UNICORE_WASM_SOURCE})
unicore_init_target(unicore-wasm)
#unicore_group_sources_by_folder(unicore-wasm)
target_include_directories(unicore-wasm PUBLIC "${UNICORE_WASM_INCLUDE}")
set_target_properties(unicore-wasm PROPERTIES FOLDER "Plugins")
target_link_libraries(unicore-wasm PUBLIC unicore m3)
target_compile_definitions(unicore-wasm PUBLIC UNICORE_USE_WASM)

function(unciore_link_wasm target)
	target_link_libraries(${target} PUBLIC unicore-wasm)
endfunction()