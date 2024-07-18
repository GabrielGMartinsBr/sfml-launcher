cmake_minimum_required(VERSION 3.5.0)
project(ORM-SLauncher VERSION 0.1.0)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 11)


# Debug
include(CTest)
enable_testing()

# Source Files
file(GLOB_RECURSE source_files src/*.cpp src/*.hpp)

include_directories(src/base)
include_directories(src)

# CPack 
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# SFML
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
link_libraries(sfml-system sfml-window sfml-graphics sfml-network sfml-audio)


# Executable
add_executable(${PROJECT_NAME} ${source_files})


# Ruby
find_package(RubyStatic REQUIRED)
include_directories(${RubyStatic_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${RubyStatic_LIBRARIES} -lcrypt)


# Boost Filesystem
find_package(Boost COMPONENTS system filesystem REQUIRED)
target_link_libraries(${PROJECT_NAME} 
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)

# Zlib
find_package(ZLIB REQUIRED)
target_include_directories(
  ${PROJECT_NAME} PRIVATE
  ${ZLIB_INCLUDE_DIRS}
)
target_link_libraries(
  ${PROJECT_NAME}
  ZLIB::ZLIB
)