cmake_minimum_required (VERSION 2.6)
project(probStruct CXX)
add_compile_options(-std=c++11)

include_directories(include)
set(HEADER_FILES include/MurmurHash3.h include/CountMinSketch.h)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -std=c++11 -fno-exceptions -Wall -fno-omit-frame-pointer")

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

#########main build
add_executable(probStruct src/main.cc src/MurmurHash3.cc ${HEADER_FILES})


# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in
               googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )


# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
                 ${CMAKE_BINARY_DIR}/googletest-build)

# The gtest/gmock targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include"
                      "${gmock_SOURCE_DIR}/include")
endif()


#########test
include_directories(googletest-src/googletest/include/gtest)

add_executable(probTest src/MurmurHash3.cc test/CountMinSketchTest.cc)

target_link_libraries(probTest gtest gtest_main)
add_test(probTest probTest)

#make it so that "make tests" will build probTest and gtest
add_custom_target(tests)
add_dependencies(tests probTest)
add_dependencies(probTest gtest gtest_main gmock gmock_main)

#exclude pprTest and gtest stuff from "make"
set_target_properties(probTest gtest gtest_main gmock gmock_main PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)


