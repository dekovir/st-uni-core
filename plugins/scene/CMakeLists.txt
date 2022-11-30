#unicore-scene
set(UNICORE_SCENE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UNICORE_SCENE_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE UNICORE_SCENE_SOURCE 
	"${UNICORE_SCENE_DIR}/*.cpp"
	"${UNICORE_SCENE_DIR}/*.hpp"
	"${UNICORE_SCENE_INCLUDE}/*.hpp"
)

add_library(unicore-scene STATIC ${UNICORE_SCENE_SOURCE})
unicore_init_target(unicore-scene)
unicore_group_sources_by_folder(unicore-scene)
target_include_directories(unicore-scene PUBLIC "${UNICORE_SCENE_INCLUDE}")
set_target_properties(unicore-scene PROPERTIES FOLDER "Plugins")
target_link_libraries(unicore-scene PUBLIC unicore)
target_compile_definitions(unicore-scene PUBLIC UNICORE_USE_SCENE)

function(unciore_link_scene target)
	target_link_libraries(${target} PUBLIC unicore-scene)
endfunction()