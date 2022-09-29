#stb
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/libs/stb")
set_target_properties(stb PROPERTIES FOLDER "Libs")

#unicore-stb
option(UNICORE_STB_ALL "Add all stb" ON)
option(UNICORE_STB_EASY_FONT "Add stb_easy_font implementation" ${UNICORE_STB_ALL})
option(UNICORE_STB_IMAGE "Add stb_image implementation" ${UNICORE_STB_ALL})
option(UNICORE_STB_RECT_PACK "Add stb_rect_pack implementation" ${UNICORE_STB_ALL})
option(UNICORE_STB_TRUETYPE "Add stb_truetype implementation" ${UNICORE_STB_ALL})

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

if (UNICORE_STB_EASY_FONT)
	target_compile_definitions(unicore-stb PUBLIC UNICORE_USE_STB_EASY_FONT)
endif()

if(UNICORE_STB_IMAGE)
	target_compile_definitions(unicore-stb PUBLIC UNICORE_USE_STB_IMAGE STBI_NO_STDIO)
endif()

if (UNICORE_STB_RECT_PACK)
	target_compile_definitions(unicore-stb PUBLIC UNICORE_USE_STB_RECT_PACK)
endif()

if (UNICORE_STB_TRUETYPE)
	target_compile_definitions(unicore-stb PUBLIC UNICORE_USE_STB_TRUETYPE)
endif()

function(unciore_link_stb target)
	target_link_libraries(${target} PUBLIC unicore-stb)
endfunction()