set(UNICORE_FNT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UNICORE_FNT_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE UNICORE_FNT_SOURCE 
	"${UNICORE_FNT_DIR}/*.cpp"
	"${UNICORE_FNT_DIR}/*.hpp"
	"${UNICORE_FNT_INCLUDE}/*.hpp"
)

add_library(unicore-fnt STATIC ${UNICORE_FNT_SOURCE})
unicore_init_target(unicore-fnt)
unicore_group_sources_by_folder(unicore-fnt)
target_include_directories(unicore-fnt PUBLIC "${UNICORE_FNT_INCLUDE}")
set_target_properties(unicore-fnt PROPERTIES FOLDER "UniCore")
target_link_libraries(unicore-fnt PUBLIC unicore unicore-xml)
target_compile_definitions(unicore-fnt PUBLIC UNICORE_USE_FNT)