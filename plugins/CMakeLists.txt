# OPTIONS ######################################################################
option(UNICORE_PLUGINS_ALL "Add all unciore plugins" ON)
option(UNICORE_PLUGIN_FNT "Add unicore-fnt plugin" ${UNICORE_PLUGINS_ALL})
option(UNICORE_PLUGIN_GRID "Add unicore-grid plugin" ${UNICORE_PLUGINS_ALL})
option(UNICORE_PLUGIN_IMGUI "Add unicore-imgui plugin" ${UNICORE_PLUGINS_ALL})
option(UNICORE_PLUGIN_PATTERN "Add unicore-pattern plugin" ${UNICORE_PLUGINS_ALL})
option(UNICORE_PLUGIN_SCENE "Add unicore-scene plugin" ${UNICORE_PLUGINS_ALL})
option(UNICORE_PLUGIN_STB "Add unicore-stb plugin (stb)" ${UNICORE_PLUGINS_ALL})
option(UNICORE_PLUGIN_SZIP "Add unicore-szip plugin (7zip)" ${UNICORE_PLUGINS_ALL})
option(UNICORE_PLUGIN_UI "Add unicore-ui plugin" ${UNICORE_PLUGINS_ALL})
option(UNICORE_PLUGIN_WASM "Add unicore-wasm plugin" ${UNICORE_PLUGINS_ALL})
option(UNICORE_PLUGIN_XML "Add unicore-xml plugin (tinyxml2)" ${UNICORE_PLUGINS_ALL})

# PLUGINS
if (UNICORE_PLUGIN_FNT)
	add_subdirectory("${PLUGINS_DIR}/fnt")
endif()

if (UNICORE_PLUGIN_XML)
	add_subdirectory("${PLUGINS_DIR}/xml")
endif()

if (UNICORE_PLUGIN_GRID)
	add_subdirectory("${PLUGINS_DIR}/grid")
endif()

if (UNICORE_PLUGIN_IMGUI)
	add_subdirectory("${PLUGINS_DIR}/imgui")
endif()

if (UNICORE_PLUGIN_PATTERN)
	add_subdirectory("${PLUGINS_DIR}/pattern")
endif()

if (UNICORE_PLUGIN_STB)
	add_subdirectory("${PLUGINS_DIR}/stb")
endif()

if (UNICORE_PLUGIN_SZIP)
	add_subdirectory("${PLUGINS_DIR}/szip")
endif()

if (UNICORE_PLUGIN_WASM)
	add_subdirectory("${PLUGINS_DIR}/wasm")
endif()

# Need to be after imgui and xml
if (UNICORE_PLUGIN_UI)
	add_subdirectory("${PLUGINS_DIR}/ui")
endif()

if (UNICORE_PLUGIN_SCENE)
	add_subdirectory("${PLUGINS_DIR}/scene")
endif()