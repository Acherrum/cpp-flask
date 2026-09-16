
set(LIBRARY_NAME HttpServer)

add_library(${LIBRARY_NAME} STATIC src/HttpServer.cpp src/Route.cpp
)

target_include_directories(${LIBRARY_NAME} 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../external>
)

target_link_libraries(${LIBRARY_NAME} PUBLIC JsonObject)

install(TARGETS ${LIBRARY_NAME}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
)
install(DIRECTORY include/ DESTINATION include)

if(WIN32)
    target_compile_definitions(${LIBRARY_NAME} PRIVATE _WIN32_WINNT=0x0A00 WINVER=0x0A00)
    target_link_libraries(${LIBRARY_NAME} PRIVATE ws2_32)
    add_compile_definitions(WIN32_LEAN_AND_MEAN)
endif()
