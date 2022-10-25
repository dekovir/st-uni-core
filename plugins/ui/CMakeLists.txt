#unicore-ui
option(UNICORE_PLUGIN_UI_LINK_IMGUI "Add all unciore plugins" ${UNICORE_PLUGIN_UI})

set(UNICORE_UI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UNICORE_UI_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE UNICORE_UI_SOURCE 
	"${UNICORE_UI_DIR}/*.cpp"
	"${UNICORE_UI_DIR}/*.hpp"
	"${UNICORE_UI_INCLUDE}/*.hpp"
)

add_library(unicore-ui STATIC ${UNICORE_UI_SOURCE})
unicore_init_target(unicore-ui)
unicore_group_sources_by_folder(unicore-ui)
target_include_directories(unicore-ui PUBLIC "${UNICORE_UI_INCLUDE}")
set_target_properties(unicore-ui PROPERTIES FOLDER "Plugins")
target_link_libraries(unicore-ui PUBLIC unicore)
target_compile_definitions(unicore-ui PUBLIC UNICORE_USE_UI)

if (UNICORE_PLUGIN_UI_LINK_IMGUI)
	if (COMMAND unciore_link_imgui)
		unciore_link_imgui(unicore-ui)
	else()
		message(WARNING "Failed to link to unicore-imgui")
	endif()
endif()

function(unciore_link_ui target)
	target_link_libraries(${target} PUBLIC unicore-ui)
endfunction()