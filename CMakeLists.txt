cmake_minimum_required(VERSION 3.18)
project(sid VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 23)

file(GLOB PARSER_SRC "${CMAKE_SOURCE_DIR}/src/parser/*.c")
list(FILTER PARSER_SRC EXCLUDE REGEX ".test.c$")
file(GLOB LIB_SRC "${CMAKE_SOURCE_DIR}/src/lib/*.c")
list(FILTER LIB_SRC EXCLUDE REGEX ".test.c$")

add_executable(${PROJECT_NAME}-cli "${CMAKE_SOURCE_DIR}/src/cli/main.c" ${LIB_SRC} ${PARSER_SRC})
target_link_libraries(${PROJECT_NAME}-cli SDL2)

add_library(${PROJECT_NAME} SHARED ${LIB_SRC})
target_link_libraries(${PROJECT_NAME} SDL2)

enable_testing()

add_executable(parser_parser_test "${CMAKE_SOURCE_DIR}/src/parser/parser.test.c" ${PARSER_SRC})
target_link_libraries(parser_parser_test ${PROJECT_NAME}) 
add_test(NAME parser_parser_test COMMAND parser_parser_test)