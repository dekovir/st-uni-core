#unicore-pattern
set(UNICORE_PATTERN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UNICORE_PATTERN_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE UNICORE_PATTERN_SOURCE 
	"${UNICORE_PATTERN_DIR}/*.cpp"
	"${UNICORE_PATTERN_DIR}/*.hpp"
	"${UNICORE_PATTERN_INCLUDE}/*.hpp"
)

add_library(unicore-pattern STATIC ${UNICORE_PATTERN_SOURCE})
unicore_init_target(unicore-pattern)
unicore_group_sources_by_folder(unicore-pattern)
target_include_directories(unicore-pattern PUBLIC "${UNICORE_PATTERN_INCLUDE}")
set_target_properties(unicore-pattern PROPERTIES FOLDER "Plugins")
target_link_libraries(unicore-pattern PUBLIC unicore)
target_compile_definitions(unicore-pattern PUBLIC UNICORE_USE_PATTERN)

function(unciore_link_pattern target)
	target_link_libraries(${target} PUBLIC unicore-pattern)
endfunction()