# OPTIONS ######################################################################
option(UNICORE_EXAMPLES_ALL "Add all examples projects" OFF)
option(UNICORE_EXAMPLE_MINIMAL "Add Minimal project" ${UNICORE_EXAMPLES_ALL})
option(UNICORE_EXAMPLE_UI "Add ui example project" ${UNICORE_EXAMPLES_ALL})
option(UNICORE_EXAMPLE_WASM "Add wasm example project" ${UNICORE_EXAMPLES_ALL})
option(UNICORE_TESTBED "Add Testbed project" ${UNICORE_EXAMPLES_ALL})

# EXAMPLE_MINIMAL ##############################################################
if (UNICORE_EXAMPLE_MINIMAL)
	set(EXAMPLE_MINIMAL_DIR "${EXAMPLES_DIR}/minimal")
	file(GLOB EXAMPLE_MINIMAL_SRC "${EXAMPLE_MINIMAL_DIR}/*")

	add_executable(minimal "${EXAMPLE_MINIMAL_SRC}")
	unicore_init_executable(minimal)
	set_target_properties(minimal PROPERTIES FOLDER "Examples")
endif()

# EXAMPLE_UI ###################################################################
if (UNICORE_EXAMPLE_UI)
	set(EXAMPLE_UI_DIR "${EXAMPLES_DIR}/ui")
	file(GLOB EXAMPLE_UI_SRC "${EXAMPLE_UI_DIR}/*" "${CMAKE_CURRENT_SOURCE_DIR}/assets/*.xml")

	add_executable(ui "${EXAMPLE_UI_SRC}")
	unicore_init_executable(ui)
	unicore_init_assets(ui "${CMAKE_CURRENT_SOURCE_DIR}/assets")
	set_target_properties(ui PROPERTIES FOLDER "Examples")

	unciore_link_fnt(ui)
	unciore_link_imgui(ui)
	unciore_link_stb(ui)
	unciore_link_ui(ui)
	unciore_link_xml(ui)
endif()

# EXAMPLE_WASM #################################################################
if (UNICORE_EXAMPLE_WASM)
	set(EXAMPLE_WASM_DIR "${EXAMPLES_DIR}/wasm")
	file(GLOB EXAMPLE_WASM_SRC "${EXAMPLE_WASM_DIR}/*")

	add_executable(wasm "${EXAMPLE_WASM_SRC}")
	unicore_init_executable(wasm)
	unicore_init_assets(wasm "${CMAKE_CURRENT_SOURCE_DIR}/assets")
	set_target_properties(wasm PROPERTIES FOLDER "Examples")

	unciore_link_fnt(wasm)
	unciore_link_stb(wasm)
	unciore_link_wasm(wasm)
	unciore_link_xml(wasm)
endif()

# TESTBED ######################################################################
if (UNICORE_TESTBED)
	set(TESTBED_DIR "${EXAMPLES_DIR}/testbed")
	file(GLOB TESTBED_SRC "${TESTBED_DIR}/*")

	add_executable(testbed "${TESTBED_SRC}")
	unicore_init_executable(testbed)
	unicore_init_assets(testbed "${CMAKE_CURRENT_SOURCE_DIR}/assets")
	set_target_properties(testbed PROPERTIES FOLDER "Examples")

	unciore_link_fnt(testbed)
	unciore_link_grid(testbed)
	unciore_link_imgui(testbed)
	unciore_link_pattern(testbed)
	unciore_link_stb(testbed)
	unciore_link_szip(testbed)
	unciore_link_ui(testbed)
	unciore_link_xml(testbed)
endif()