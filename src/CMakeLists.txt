file(GLOB src CONFIGURE_DEPENDS "*.cpp")
add_library(src ${src})
set_target_properties(src PROPERTIES LINKER_LANGUAGE CXX)
