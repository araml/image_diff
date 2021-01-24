cmake_minimum_required(VERSION 3.0)

project(image_diff)

set(CMAKE_CXX_FLAGS "-ggdb -std=c++2a -Wall -Wextra -Wno-long-long -Wno-variadic-macros")

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

    add_custom_test(hello_test tests/test.cpp)
endif ()
