set(UNICORE_GRIDMAP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UNICORE_GRIDMAP_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE UNICORE_GRIDMAP_SOURCE 
	"${UNICORE_GRIDMAP_DIR}/*.cpp"
	"${UNICORE_GRIDMAP_DIR}/*.hpp"
	"${UNICORE_GRIDMAP_INCLUDE}/*.hpp"
)

add_library(unicore-gridmap STATIC ${UNICORE_GRIDMAP_SOURCE})
unicore_init_target(unicore-gridmap)
unicore_group_sources_by_folder(unicore-gridmap)
target_include_directories(unicore-gridmap PUBLIC "${UNICORE_GRIDMAP_INCLUDE}")
set_target_properties(unicore-gridmap PROPERTIES FOLDER "Plugins")
target_link_libraries(unicore-gridmap PUBLIC unicore)
target_compile_definitions(unicore-gridmap PUBLIC UNICORE_USE_GRIDMAP)