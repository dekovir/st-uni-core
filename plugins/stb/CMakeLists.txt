#stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/libs/stb")
set_target_properties(stb PROPERTIES FOLDER "Libs")

#unicore-stb
option(UNICORE_STB_C_LEXER "Add stb_c_lexer implementation" OFF)
option(UNICORE_STB_DIVIDE "Add stb_divide implementation" OFF)
option(UNICORE_STB_DXT "Add stb_dxt implementation" OFF)
option(UNICORE_STB_EASY_FONT "Add stb_easy_font implementation" ON)
option(UNICORE_STB_IMAGE "Add stb_image implementation" ON)
option(UNICORE_STB_IMAGE_RESIZE "Add stb_image_resize implementation" OFF)
option(UNICORE_STB_IMAGE_WRITE "Add stb_image_write implementation" OFF)
option(UNICORE_STB_PERLIN "Add stb_perlin implementation" OFF)
option(UNICORE_STB_RECT_PACK "Add stb_rect_pack implementation" OFF)
option(UNICORE_STB_TRUETYPE "Add stb_truetype implementation" OFF)

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

if (UNICORE_STB_C_LEXER)
	target_compile_definitions(unicore-stb PUBLIC UNICORE_USE_STB_C_LEXER)
	target_compile_definitions(unicore-stb PRIVATE STB_C_LEXER_IMPLEMENTATION)
endif()

if (UNICORE_STB_DIVIDE)
	target_compile_definitions(unicore-stb PUBLIC UNICORE_USE_STB_DIVIDE)
	target_compile_definitions(unicore-stb PRIVATE STB_DIVIDE_IMPLEMENTATION)
endif()

if (UNICORE_STB_DXT)
	target_compile_definitions(unicore-stb PUBLIC UNICORE_USE_STB_DXT)
	target_compile_definitions(unicore-stb PRIVATE STB_DXT_IMPLEMENTATION)
endif()

if (UNICORE_STB_EASY_FONT)
	target_compile_definitions(unicore-stb PUBLIC UNICORE_USE_STB_EASY_FONT)
endif()

if(UNICORE_STB_IMAGE)
	target_compile_definitions(unicore-stb PUBLIC UNICORE_USE_STB_IMAGE STBI_NO_STDIO)
	target_compile_definitions(unicore-stb PRIVATE STB_IMAGE_IMPLEMENTATION STBI_ONLY_JPEG STBI_ONLY_PNG STBI_ONLY_BMP)
endif()

if(UNICORE_STB_IMAGE_RESIZE)
	target_compile_definitions(unicore-stb PUBLIC UNICORE_USE_STB_IMAGE_RESIZE)
	target_compile_definitions(unicore-stb PRIVATE STB_IMAGE_RESIZE_IMPLEMENTATION)
endif()

if(UNICORE_STB_IMAGE_WRITE)
	target_compile_definitions(unicore-stb PUBLIC UNICORE_USE_STB_IMAGE_WRITE STBI_WRITE_NO_STDIO)
	target_compile_definitions(unicore-stb PRIVATE STB_IMAGE_WRITE_IMPLEMENTATION)
endif()

if (UNICORE_STB_PERLIN)
	target_compile_definitions(unicore-stb PUBLIC UNICORE_USE_STB_PERLIN)
	target_compile_definitions(unicore-stb PRIVATE STB_PERLIN_IMPLEMENTATION)
endif()

if (UNICORE_STB_RECT_PACK)
	target_compile_definitions(unicore-stb PUBLIC UNICORE_USE_STB_RECT_PACK)
	target_compile_definitions(unicore-stb PRIVATE STB_RECT_PACK_IMPLEMENTATION)
endif()

if (UNICORE_STB_TRUETYPE)
	target_compile_definitions(unicore-stb PUBLIC UNICORE_USE_STB_TRUETYPE)
	target_compile_definitions(unicore-stb PRIVATE STB_TRUETYPE_IMPLEMENTATION)
endif()

function(unciore_link_stb target)
	target_link_libraries(${target} PUBLIC unicore-stb)
endfunction()