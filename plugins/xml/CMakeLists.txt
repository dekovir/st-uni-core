set(UNICORE_XML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(UNICORE_XML_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE UNICORE_XML_SOURCE 
	"${UNICORE_XML_DIR}/*.cpp"
	"${UNICORE_XML_DIR}/*.hpp"
	"${UNICORE_XML_INCLUDE}/*.hpp"
)

add_library(unicore-xml STATIC ${UNICORE_XML_SOURCE})
unicore_init_target(unicore-xml)
unicore_group_sources_by_folder(unicore-xml)
target_include_directories(unicore-xml PUBLIC "${UNICORE_XML_INCLUDE}")
set_target_properties(unicore-xml PROPERTIES FOLDER "UniCore")
target_link_libraries(unicore-xml PUBLIC unicore tinyxml2)
target_compile_definitions(unicore-xml PUBLIC UNICORE_USE_XML)