# Locate the ruby-static library and include directories

find_path(RUBY_STATIC_INCLUDE_DIR
    NAMES ruby.h
    HINTS ${CMAKE_SOURCE_DIR}/3rd/ruby-1.8.7
)

find_library(RUBY_STATIC_LIBRARY
    NAMES ruby-static libmsvcrt-ruby18-static
    HINTS ${CMAKE_SOURCE_DIR}/3rd/ruby-1.8.7
)

# Provide the user with information about the results
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(RubyStatic
    REQUIRED_VARS RUBY_STATIC_INCLUDE_DIR RUBY_STATIC_LIBRARY
)

if(RubyStatic_FOUND)
    set(RubyStatic_LIBRARIES ${RUBY_STATIC_LIBRARY})
    set(RubyStatic_INCLUDE_DIRS ${RUBY_STATIC_INCLUDE_DIR})
else()
    set(RubyStatic_LIBRARIES)
    set(RubyStatic_INCLUDE_DIRS)
endif()
