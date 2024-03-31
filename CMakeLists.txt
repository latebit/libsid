cmake_minimum_required(VERSION 3.18)
project(sid)

set(CMAKE_C_STANDARD 23)

file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.c")
add_executable(${PROJECT_NAME} main.c ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} SDL2)

add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)