cmake_minimum_required(VERSION 3.12)
project(ORM-SLauncher VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)

# Source Files
file(GLOB_RECURSE source_files src/*.cpp src/*.hpp)

# Executable
add_executable(${PROJECT_NAME} ${source_files})

# Debug
include(CTest)
enable_testing()

# CPack 
include(CPack)

# FetchContent
include(FetchContent)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PRIVATE src/base)
target_include_directories(${PROJECT_NAME} PRIVATE GLOB_RECURSE src/modules)



# Zlib
# Fetch Zlib
FetchContent_Declare(
    Zlib
    GIT_REPOSITORY https://github.com/madler/zlib
    GIT_TAG v1.3.1
)
set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Download and add Zlib to your project
FetchContent_MakeAvailable(Zlib)

target_link_libraries(${PROJECT_NAME} zlib)




# Boost Filesystem
# Configure FetchContent to download Boost
FetchContent_Declare(
  Boost
  GIT_REPOSITORY https://github.com/boostorg/boost.git
  GIT_TAG boost-1.86.0
)

# Initialize and configure Boost
FetchContent_MakeAvailable(Boost)

# Link Boost components to the target
target_link_libraries(${PROJECT_NAME} 
  Boost::system
  Boost::filesystem
  Boost::thread
)




# SFML
FetchContent_Declare(
  Sfml
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.2
)
FetchContent_MakeAvailable(Sfml)
# find_package(SFML 2.6 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-network sfml-audio)




# Ruby
find_package(RubyStatic REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${RubyStatic_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${RubyStatic_LIBRARIES} -lcrypt)
target_compile_options(${PROJECT_NAME} PRIVATE -Wno-register)




# Fluidsynth
FetchContent_Declare(
    fluidsynth
    GIT_REPOSITORY https://github.com/FluidSynth/fluidsynth.git
    GIT_TAG v2.3.7
)

# set(ENABLE_ALSA OFF CACHE BOOL "Disable ALSA support" FORCE)
# set(ENABLE_JACK OFF CACHE BOOL "Disable JACK support" FORCE)
# set(ENABLE_PULSEAUDIO OFF CACHE BOOL "Disable PulseAudio support" FORCE)
set(ENABLE_READLINE OFF CACHE BOOL "Disable readline support" FORCE)
set(ENABLE_DBUS OFF CACHE BOOL "Disable D-Bus support" FORCE)
set(ENABLE_SOUNDFONTS_INSTALL OFF CACHE BOOL "Disable SoundFont installation" FORCE)
set(ENABLE_SHARED OFF CACHE BOOL "Build as static library" FORCE)
set(ENABLE_NETWORK OFF CACHE BOOL "Disable network support" FORCE)
set(FLUIDSYNTH_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(fluidsynth)

target_link_libraries(${PROJECT_NAME} libfluidsynth)