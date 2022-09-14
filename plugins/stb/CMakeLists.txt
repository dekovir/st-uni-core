#stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/libs/stb")
set_target_properties(stb PROPERTIES FOLDER "Libs")

#unicore-stb
set(UNICORE_STB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UNICORE_STB_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE UNICORE_STB_SOURCE 
	"${UNICORE_STB_DIR}/*.cpp"
	"${UNICORE_STB_DIR}/*.hpp"
	"${UNICORE_STB_INCLUDE}/*.hpp"
)

add_library(unicore-stb STATIC ${UNICORE_STB_SOURCE})
unicore_init_target(unicore-stb)
unicore_group_sources_by_folder(unicore-stb)
target_include_directories(unicore-stb PUBLIC "${UNICORE_STB_INCLUDE}")
set_target_properties(unicore-stb PROPERTIES FOLDER "Plugins")
target_link_libraries(unicore-stb PUBLIC unicore stb)
target_compile_definitions(unicore-stb PUBLIC UNICORE_USE_STB)
target_compile_definitions(unicore-stb PRIVATE STB_IMAGE_IMPLEMENTATION STBI_NO_STDIO STBI_ONLY_JPEG STBI_ONLY_PNG STBI_ONLY_BMP)

function(unciore_link_stb target)
	target_link_libraries(${target} PUBLIC unicore-stb)
endfunction()