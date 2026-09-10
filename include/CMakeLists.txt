file(GLOB include CONFIGURE_DEPENDS "*.h")
add_library(include ${include})
set_target_properties(include PROPERTIES LINKER_LANGUAGE CXX)

