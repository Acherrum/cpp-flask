
set(LIBRARY_NAME HtmlBuilder)

file(GLOB_RECURSE SOURCES src/*.cpp)

add_library(${LIBRARY_NAME} STATIC ${SOURCES})

target_include_directories(${LIBRARY_NAME} 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${LIBRARY_NAME} PUBLIC JsonObject)

install(TARGETS ${LIBRARY_NAME}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)

file(GLOB_RECURSE TEST_SOURCES gtest/*.cpp)

add_executable(${LIBRARY_NAME}_gtest ${TEST_SOURCES})
target_link_libraries(${LIBRARY_NAME}_gtest PRIVATE ${LIBRARY_NAME} gtest_main gtest)

file(GLOB_RECURSE RESOURCE_FILES gtest/*.file)
foreach( RESOURCE_FILE IN LISTS RESOURCE_FILES)
add_custom_command(
    TARGET ${LIBRARY_NAME}_gtest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            "${RESOURCE_FILE}"
            "${CMAKE_CURRENT_BINARY_DIR}"
    COMMENT "Copying ${RESOURCE_FILE} to the build directory"
)
endforeach()

gtest_discover_tests(${LIBRARY_NAME}_gtest DISCOVERY_MODE PRE_TEST)
