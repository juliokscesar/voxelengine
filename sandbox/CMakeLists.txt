cmake_minimum_required(VERSION 3.28)
project(vesandbox)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(SRC_FILES
    ${SRC_DIR}/main.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC
    ${_CORE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    vecore
)
