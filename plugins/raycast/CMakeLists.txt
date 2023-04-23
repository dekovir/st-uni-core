set(UNICORE_RAYCAST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UNICORE_RAYCAST_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE UNICORE_RAYCAST_SOURCE 
	"${UNICORE_RAYCAST_DIR}/*.cpp"
	"${UNICORE_RAYCAST_DIR}/*.hpp"
	"${UNICORE_RAYCAST_INCLUDE}/*.hpp"
)

add_library(unicore-raycast STATIC ${UNICORE_RAYCAST_SOURCE})
unicore_init_target(unicore-raycast)
#unicore_group_sources_by_folder(unicore-raycast)
target_include_directories(unicore-raycast PUBLIC "${UNICORE_RAYCAST_INCLUDE}")
set_target_properties(unicore-raycast PROPERTIES FOLDER "Plugins")
target_link_libraries(unicore-raycast PUBLIC unicore)
target_compile_definitions(unicore-raycast PUBLIC UNICORE_USE_RAYCAST)

function(unciore_link_raycast target)
	target_link_libraries(${target} PUBLIC unicore-raycast)
endfunction()