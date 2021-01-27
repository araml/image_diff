cmake_minimum_required(VERSION 3.0)

project(image_diff)

SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "-ggdb -O2 -std=c++2a -Wunused -Wall -Wextra -Wno-long-long -Wno-variadic-macros")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   add_compile_options (-fdiagnostics-color=always)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
   add_compile_options (-fcolor-diagnostics)
endif ()

macro(add_files name)
    file(GLOB ${name} src/${name}/*.h src/${name}/*.cpp src/${name}/*.c)
    include_directories(src/${name})
endmacro()

include_directories(src)

add_executable(image_diff
               src/main.cpp)
target_link_libraries(image_diff
                      "-lpthread")



option(BUILD_TESTS "Build unit tests")

if (BUILD_TESTS)
    enable_testing()
    macro(add_custom_test name file)
        string(REGEX REPLACE "(^.*/|\\.[^.]*$)" "" bare_name ${file})
        add_executable(${name} ${file})
        add_test(NAME ${name} COMMAND ${name}
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
        target_link_libraries(${name} ${LIBRARIES})
    endmacro()

    add_custom_test(test_strict_diff tests/test_strict_diff.cpp)
    add_custom_test(test_image tests/test_image.cpp)
    add_custom_test(test_image_iterator tests/test_image_iterator.cpp)
    add_custom_test(test_perceived_diff tests/test_perceived_diff.cpp)
endif ()
