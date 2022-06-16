cmake_minimum_required(VERSION 3.8.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (EMSCRIPTEN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sUSE_SDL=2")
endif ()

# OPTIONS ######################################################################
option(UNICORE_TESTBED "Add Testbed project" OFF)

# PROJECT ######################################################################
project(UniCore)

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/")

function(unicore_group_sources_by_folder target)
	set(last_dir "")
	set(files "")

	get_target_property(sources ${target} SOURCES)

	foreach(file ${sources})
		file(RELATIVE_PATH relative_file "${CMAKE_CURRENT_LIST_DIR}" ${file})
		get_filename_component(dir "${relative_file}" DIRECTORY)
		if(NOT "${dir}" STREQUAL "${last_dir}")
			if(files)
				source_group("${last_dir}" FILES ${files})
			endif()
			set(files "")
		endif()
		set(files ${files} ${file})
		set(last_dir "${dir}")
	endforeach()

	if(files)
		source_group("${last_dir}" FILES ${files})
	endif()
endfunction()

function(unicore_init_target target)
endfunction()

# UNICORE ######################################################################
set(UNICORE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UNICORE_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE UNICORE_SOURCE "${UNICORE_DIR}/*.cpp" "${UNICORE_DIR}/*.hpp" "${UNICORE_INCLUDE}/*.hpp")

add_library(unicore STATIC ${UNICORE_SOURCE})
unicore_init_target(unicore)
unicore_group_sources_by_folder(unicore)
target_include_directories(unicore PUBLIC "${UNICORE_INCLUDE}")
set_target_properties(unicore PROPERTIES FOLDER "UniCore")

# SDL2
if (NOT EMSCRIPTEN)
	set(SDL_STATIC ON CACHE BOOL "" FORCE)
	set(SDL_SHARED OFF CACHE BOOL "" FORCE)
	set(SDL_AUDIO OFF CACHE BOOL "" FORCE)
	set(SDL_LIBC ON CACHE BOOL "" FORCE)
	set(SDL_TEST_ENABLED_BY_DEFAULT OFF CACHE BOOL "" FORCE)

	add_subdirectory("${LIB_DIR}/SDL2")
	set_target_properties(SDL2-static SDL2main PROPERTIES FOLDER "Libs")
	target_link_libraries(unicore PRIVATE SDL2-static)
endif()

if (WIN32)
	target_link_libraries(unicore PRIVATE Shlwapi)
endif()

target_compile_definitions(unicore PUBLIC UNICORE_USE_SDL2)
#target_compile_options(unicore PUBLIC -fno-exceptions)

# MAIN #########################################################################
set(MAIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/main")
file(GLOB_RECURSE MAIN_SRC "${MAIN_DIR}/*.cpp" "${MAIN_DIR}/*.h")
add_library(unicore-main STATIC ${MAIN_SRC})
unicore_group_sources_by_folder(unicore-main)
target_include_directories(unicore-main PUBLIC "${MAIN_DIR}")
target_link_libraries(unicore-main PUBLIC unicore)
set_target_properties(unicore-main PROPERTIES FOLDER "UniCore")

if (NOT EMSCRIPTEN)
	target_link_libraries(unicore-main PRIVATE SDL2main)
	target_compile_definitions(unicore-main PUBLIC UNICORE_USE_SDL2_MAIN)
endif()

# FUNCTIONS ####################################################################
function(unicore_init_executable target_name)
	if (WIN32)
		set_target_properties(${target_name}
			PROPERTIES
				WIN32_EXECUTABLE ON
		)
	elseif(EMSCRIPTEN)
		target_link_libraries(${target_name} PRIVATE
			"--bind"
			"--emrun"
			"-s ALLOW_MEMORY_GROWTH=1"
		)
		set_target_properties(${target_name}
			PROPERTIES
				SUFFIX ".html"
		)
	else()
		message(FATAL_ERROR "Unsupported platform")
	endif()

	unicore_init_target(${target_name})
	target_link_libraries(${target_name} PRIVATE unicore unicore-main)
endfunction()

function(unicore_init_assets target_name directory)
	if(EMSCRIPTEN)
		target_link_libraries(${target_name} PRIVATE
			"--preload-file ../../${directory}@/${directory}"
		)
	endif()
endfunction()

# TESTBED ######################################################################
if (UNICORE_TESTBED)
	set(TESTBED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/testbed")
	file(GLOB TESTBED_SRC "${TESTBED_DIR}/*.cpp" "${TESTBED_DIR}/*.h")

	add_executable(testbed "${TESTBED_SRC}")
	unicore_init_executable(testbed)
	unicore_group_sources_by_folder(testbed)
	unicore_init_assets(testbed assets)

	if (MSVC)
		set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT testbed)
		set_target_properties(${target_name} PROPERTIES
			VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/assets
		)
	endif()
endif()