cmake_minimum_required(VERSION 3.18)
project(sid VERSION 0.0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 17)

file(GLOB PARSER_SRC "${CMAKE_SOURCE_DIR}/src/parser/*.c")
file(GLOB LIB_SRC "${CMAKE_SOURCE_DIR}/src/lib/*.c")

add_executable(${PROJECT_NAME}-cli "${CMAKE_SOURCE_DIR}/src/cli/main.c" ${LIB_SRC} ${PARSER_SRC})
target_link_libraries(${PROJECT_NAME}-cli SDL2)

add_library(${PROJECT_NAME} SHARED ${LIB_SRC})
target_link_libraries(${PROJECT_NAME} SDL2)

add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)