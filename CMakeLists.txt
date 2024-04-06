cmake_minimum_required(VERSION 3.22.1)
project(sid VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS_DEBUG "-g -fno-omit-frame-pointer -Wall -Wpedantic -fsanitize=address -gdwarf-2")
option(SDL2_PATH "Path to SDL2" $ENV{SDL2_PATH})

find_package(SDL2 REQUIRED CONFIG)

file(GLOB PARSER_SRC "${CMAKE_SOURCE_DIR}/src/parser/*.c")
list(FILTER PARSER_SRC EXCLUDE REGEX ".test.c$")
file(GLOB LIB_SRC "${CMAKE_SOURCE_DIR}/src/lib/*.c")
list(FILTER LIB_SRC EXCLUDE REGEX ".test.c$")

add_executable(${PROJECT_NAME}-cli "${CMAKE_SOURCE_DIR}/src/cli/main.c" ${LIB_SRC} ${PARSER_SRC})
target_link_libraries(${PROJECT_NAME}-cli SDL2::SDL2)

add_library(${PROJECT_NAME} SHARED ${LIB_SRC})
target_link_libraries(${PROJECT_NAME} SDL2::SDL2)

enable_testing()

file(GLOB TEST_SRC "${CMAKE_SOURCE_DIR}/src/*/*.test.c")
foreach(test_file ${TEST_SRC})
  get_filename_component(test_name ${test_file} NAME_WE)
  add_executable(${test_name} ${test_file} ${PARSER_SRC} "${CMAKE_SOURCE_DIR}/test/test.c")
  target_link_libraries(${test_name} ${PROJECT_NAME})
  add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()