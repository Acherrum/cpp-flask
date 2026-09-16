
set(LIBRARY_NAME JsonObject)

add_library(${LIBRARY_NAME} STATIC src/JsonObject.cpp
)

target_include_directories(${LIBRARY_NAME} 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../external/rapidjson/include>
)

install(TARGETS ${LIBRARY_NAME}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)


add_executable(${LIBRARY_NAME}_gtest gtest/JsonObjectTest.cpp)
target_link_libraries(${LIBRARY_NAME}_gtest PRIVATE ${LIBRARY_NAME} gtest_main gtest)
gtest_discover_tests(${LIBRARY_NAME}_gtest)
