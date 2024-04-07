cmake_minimum_required(VERSION 3.22.1)

if(DEFINED ENV{VERSION})
  set(VERSION $ENV{VERSION})
else()
  set(VERSION "0.0.0")
endif()

project(sid VERSION ${VERSION} LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS_DEBUG "-g -fno-omit-frame-pointer -Wall -Wpedantic -fsanitize=address -gdwarf-2")

set(BIN_NAME "${PROJECT_NAME}play")
option(SDL2_PATH "Path to SDL2" $ENV{SDL2_PATH})

find_package(SDL2 REQUIRED CONFIG)

file(GLOB LIB_SRC "${CMAKE_SOURCE_DIR}/src/lib/**/*.c")
list(FILTER LIB_SRC EXCLUDE REGEX ".test.c$")
file(GLOB LIB_HEADERS "${CMAKE_SOURCE_DIR}/src/lib/**/*.h")

add_executable(${BIN_NAME} "${CMAKE_SOURCE_DIR}/src/cli/main.c" ${LIB_SRC})
target_link_libraries(${BIN_NAME} SDL2::SDL2-static)

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

install(TARGETS ${BIN_NAME} DESTINATION bin)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)
install(FILES ${LIB_HEADERS} DESTINATION include/${PROJECT_NAME})
install(DIRECTORY ${SDL2_INCLUDE_DIRS} DESTINATION include)

# Package with CPack
set(CPACK_PACKAGE_VENDOR "Manuel Spagnolo")
set(CPACK_PACKAGE_CONTACT "manuelspagnolo@duck.com")
set(CPACK_PACKAGE_DESCRIPTION "A SID chip emulator.")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsdl2-2.0-0")
set(CPACK_PACKAGE_VERSION ${VERSION})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(CPACK_GENERATOR "DEB;TGZ")
elseif (APPLE)
  set(CPACK_GENERATOR "DragNDrop;TGZ")
endif()

include(CPack)
