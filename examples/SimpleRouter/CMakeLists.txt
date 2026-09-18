cmake_minimum_required(VERSION 3.15)

project(SimpleHelloWorld VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)

FetchContent_Declare(
        CppFlask
        GIT_REPOSITORY https://github.com/acherrum/cpp-flask.git
        GIT_TAG        origin/master
        GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(CppFlask)

add_subdirectory(routes)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE CppFlask Routes)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/www"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/www"
        COMMENT "Copying www directory to the build directory"
)