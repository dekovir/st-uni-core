# OPTIONS ######################################################################
option(UNICORE_EXAMPLES "Add all examples projects" OFF)
option(UNICORE_MINIMAL "Add Minimal project" ${UNICORE_EXAMPLES})
option(UNICORE_TESTBED "Add Testbed project" ${UNICORE_EXAMPLES})
option(UNICORE_EXAMPLE_WASM "Add Testbed project" ${UNICORE_EXAMPLES})

# MINIMAL ######################################################################
if (UNICORE_MINIMAL)
	set(MINIMAL_DIR "${EXAMPLES_DIR}/minimal")
	file(GLOB MINIMAL_SRC "${MINIMAL_DIR}/*.cpp" "${MINIMAL_DIR}/*.hpp")

	add_executable(minimal "${MINIMAL_SRC}")
	unicore_init_executable(minimal)
	set_target_properties(minimal PROPERTIES FOLDER "Examples")
endif()

# TESTBED ######################################################################
if (UNICORE_TESTBED)
	set(TESTBED_DIR "${EXAMPLES_DIR}/testbed")
	file(GLOB TESTBED_SRC "${TESTBED_DIR}/*.cpp" "${TESTBED_DIR}/*.hpp")

	add_executable(testbed "${TESTBED_SRC}")
	unicore_init_executable(testbed)
	unicore_init_assets(testbed "${CMAKE_CURRENT_SOURCE_DIR}/assets")
	set_target_properties(testbed PROPERTIES FOLDER "Examples")
endif()

# WASM #########################################################################
if (UNICORE_EXAMPLE_WASM)
	set(WASM_DIR "${EXAMPLES_DIR}/wasm")
	file(GLOB WASM_SRC "${WASM_DIR}/*.cpp" "${WASM_DIR}/*.hpp")

	add_executable(wasm "${WASM_SRC}")
	unicore_init_executable(wasm)
	unicore_init_assets(wasm "${CMAKE_CURRENT_SOURCE_DIR}/assets")
	set_target_properties(wasm PROPERTIES FOLDER "Examples")
endif()