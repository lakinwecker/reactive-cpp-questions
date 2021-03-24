cmake_minimum_required(VERSION 3.15.0)

project(questions)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules/" ${CMAKE_MODULE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
include(build/deps/conan/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

# Use modern C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#-------------------------------------------------------------------------------
# Dependencies from CONAN
include_directories(SYSTEM ${CONAN_INCLUDE_DIRS_BOOST})
link_directories(${CONAN_LIB_DIRS_BOOST})

#-------------------------------------------------------------------------------
# dependencies from git submodules
#-------------------------------------------------------------------------------
include_directories(SYSTEM thirdparty/zug)
include_directories(SYSTEM thirdparty/lager)
include_directories(SYSTEM thirdparty/immer)
include_directories(SYSTEM thirdparty/vivid/include)
include_directories(SYSTEM thirdparty/json/include)

# vivid library
set(VIVID_DEFINITIONS ${VIVID_DEFINITIONS} ANSI_DECLARATORS=1)
file(GLOB VIVID_SOURCES thirdparty/vivid/src/*)
add_library(vivid ${VIVID_SOURCES})
target_include_directories(vivid PRIVATE ${INCLUDES})
target_compile_definitions(vivid PRIVATE ${VIVID_DEFINITIONS})
target_compile_options(vivid PRIVATE ${VIVID_COMPILE_OPTIONS})
set(LIBRARIES ${LIBRARIES} vivid)

file(GLOB SOURCES src/*)

add_executable(questions ${SOURCES})
target_include_directories(questions PRIVATE src)
target_link_libraries(questions vivid)
target_compile_options(questions PRIVATE "-Wall" "-pedantic")
set_target_properties(questions PROPERTIES INSTALL_RPATH "./" BUILD_RPATH "./")


