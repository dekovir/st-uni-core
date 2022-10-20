# imgui ########################################################################
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui")
file(GLOB IMGUI_SRC 
	"${IMGUI_DIR}/*.h" "${IMGUI_DIR}/*.cpp"
	"${IMGUI_DIR}/misc/cpp/*.h" "${IMGUI_DIR}/misc/cpp/*.cpp"
)

add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC "${IMGUI_DIR}" "${IMGUI_DIR}/misc/cpp")
set_target_properties(imgui PROPERTIES FOLDER "Libs")

# unicore-imgui ################################################################
set(UNICORE_IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UNICORE_IMGUI_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE UNICORE_IMGUI_SOURCE 
	"${UNICORE_IMGUI_DIR}/*.cpp"
	"${UNICORE_IMGUI_DIR}/*.hpp"
	"${UNICORE_IMGUI_INCLUDE}/*.hpp"
)

add_library(unicore-imgui STATIC ${UNICORE_IMGUI_SOURCE})
unicore_init_target(unicore-imgui)
unicore_group_sources_by_folder(unicore-imgui)
target_include_directories(unicore-imgui PUBLIC "${UNICORE_IMGUI_INCLUDE}")
set_target_properties(unicore-imgui PROPERTIES FOLDER "Plugins")
target_link_libraries(unicore-imgui PUBLIC unicore imgui)
target_compile_definitions(unicore-imgui PUBLIC UNICORE_USE_IMGUI)

function(unciore_link_imgui target)
	target_link_libraries(${target} PUBLIC unicore-imgui)
endfunction()