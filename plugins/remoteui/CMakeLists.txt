#unicore-remoteui
option(UNICORE_PLUGIN_REMOTEUI_LINK_XML "Link unicore-xml to unicore-remoteui" ${UNICORE_PLUGIN_REMOTEUI})
option(UNICORE_PLUGIN_REMOTEUI_LINK_IMGUI "Link unicore-imgui to unicore-remoteui" ${UNICORE_PLUGIN_REMOTEUI})

set(UNICORE_UI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UNICORE_UI_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE UNICORE_UI_SOURCE 
	"${UNICORE_UI_DIR}/*.cpp"
	"${UNICORE_UI_DIR}/*.hpp"
	"${UNICORE_UI_INCLUDE}/*.hpp"
)

add_library(unicore-remoteui STATIC ${UNICORE_UI_SOURCE})
unicore_init_target(unicore-remoteui)
unicore_group_sources_by_folder(unicore-remoteui)
target_include_directories(unicore-remoteui PUBLIC "${UNICORE_UI_INCLUDE}")
set_target_properties(unicore-remoteui PROPERTIES FOLDER "Plugins")
target_link_libraries(unicore-remoteui PUBLIC unicore)
target_compile_definitions(unicore-remoteui PUBLIC UNICORE_USE_REMOTEUI)

if (UNICORE_PLUGIN_REMOTEUI_LINK_XML)
	if (COMMAND unciore_link_xml)
		unciore_link_xml(unicore-remoteui)
	else()
		message(WARNING "Failed to link to unicore-xml")
	endif()
endif()

if (UNICORE_PLUGIN_REMOTEUI_LINK_IMGUI)
	if (COMMAND unciore_link_imgui)
		unciore_link_imgui(unicore-remoteui)
	else()
		message(WARNING "Failed to link to unicore-imgui")
	endif()
endif()

function(unciore_link_remoteui target)
	target_link_libraries(${target} PUBLIC unicore-remoteui)
endfunction()