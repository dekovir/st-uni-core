set(LOGIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(LOGIC_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE LOGIC_SOURCE 
	"${LOGIC_DIR}/*.cpp"
	"${LOGIC_DIR}/*.hpp"
	"${LOGIC_INCLUDE}/*.hpp"
)

add_library(unicore-app STATIC ${LOGIC_SOURCE})
unicore_init_target(unicore-app)
unicore_group_sources_by_folder(unicore-app)
target_include_directories(unicore-app PUBLIC "${LOGIC_INCLUDE}")
set_target_properties(unicore-app PROPERTIES FOLDER "UniCore")
target_link_libraries(unicore-app PUBLIC unicore)

if (UNICORE_XML)
	target_link_libraries(unicore-app PUBLIC unicore-xml)
endif()

if (UNICORE_FNT)
	target_link_libraries(unicore-app PUBLIC unicore-fnt)
endif()