#7z
set(7Z_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/7zip")
file(GLOB_RECURSE 7Z_SOURCES ${7Z_DIR}/*.h ${7Z_DIR}/*.c)

add_library(7z ${7Z_SOURCES})
unicore_init_target(7z)
target_include_directories(7z PUBLIC ${7Z_DIR})
set_target_properties(7z PROPERTIES FOLDER "Libs")

#unicore-szip
set(UNICORE_SZIP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UNICORE_SZIP_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE UNICORE_SZIP_SOURCE 
	"${UNICORE_SZIP_DIR}/*.cpp"
	"${UNICORE_SZIP_DIR}/*.hpp"
	"${UNICORE_SZIP_INCLUDE}/*.hpp"
)

add_library(unicore-szip STATIC ${UNICORE_SZIP_SOURCE})
unicore_init_target(unicore-szip)
#unicore_group_sources_by_folder(unicore-szip)
target_include_directories(unicore-szip PUBLIC "${UNICORE_SZIP_INCLUDE}")
set_target_properties(unicore-szip PROPERTIES FOLDER "Plugins")
target_link_libraries(unicore-szip PUBLIC unicore 7z)
target_compile_definitions(unicore-szip PUBLIC UNICORE_USE_7ZIP)

function(unciore_link_szip target)
	target_link_libraries(${target} PUBLIC unicore-szip)
endfunction()