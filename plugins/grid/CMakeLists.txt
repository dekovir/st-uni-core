#unicore-grid
set(UNICORE_GRIDDIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UNICORE_GRIDINCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE UNICORE_GRIDSOURCE 
	"${UNICORE_GRIDDIR}/*.cpp"
	"${UNICORE_GRIDDIR}/*.hpp"
	"${UNICORE_GRIDINCLUDE}/*.hpp"
)

add_library(unicore-grid STATIC ${UNICORE_GRIDSOURCE})
unicore_init_target(unicore-grid)
unicore_group_sources_by_folder(unicore-grid)
target_include_directories(unicore-grid PUBLIC "${UNICORE_GRIDINCLUDE}")
set_target_properties(unicore-grid PROPERTIES FOLDER "Plugins")
target_link_libraries(unicore-grid PUBLIC unicore)
target_compile_definitions(unicore-grid PUBLIC UNICORE_USE_GRID)

function(unciore_link_grid target)
	target_link_libraries(${target} PUBLIC unicore-grid)
endfunction()