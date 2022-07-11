# OPTIONS ######################################################################
option(UNICORE_EXAMPLES "Add all examples projects" OFF)
option(UNICORE_MINIMAL "Add Minimal project" ${UNICORE_EXAMPLES})
option(UNICORE_TESTBED "Add Testbed project" ${UNICORE_EXAMPLES})

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
	unicore_init_assets(testbed assets)
	set_target_properties(testbed PROPERTIES FOLDER "Examples")
endif()