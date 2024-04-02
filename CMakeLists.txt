cmake_minimum_required(VERSION 3.18)
project(sid)

set(CMAKE_C_STANDARD 23)

file(GLOB PARSER_SRC "${CMAKE_SOURCE_DIR}/src/parser/*.c")
file(GLOB LIB_SRC "${CMAKE_SOURCE_DIR}/src/lib/*.c")

add_executable(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/src/cli/main.c" ${LIB_SRC} ${PARSER_SRC})

target_link_libraries(${PROJECT_NAME} SDL2)

add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)