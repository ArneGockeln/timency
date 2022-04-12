cmake_minimum_required(VERSION 3.21)
set(PROJECT_NAME timency)
project(${PROJECT_NAME} C)

set(CMAKE_C_STANDARD 23)

include(GNUInstallDirs)
find_library(CJSON_LIB cjson)
include_directories("${CMAKE_INSTALL_FULL_INCLUDEDIR}")

message(STATUS ${CJSON_LIB})
message(STATUS ${CMAKE_INSTALL_FULL_INCLUDEDIR}/cjson)

add_executable(${PROJECT_NAME} src/main.c src/data.h src/data.c src/tui.h src/tui.c)
target_link_libraries(${PROJECT_NAME} ncurses "${CJSON_LIB}")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/data/*.json $<TARGET_FILE_DIR:${PROJECT_NAME}>/)