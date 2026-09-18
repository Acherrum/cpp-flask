
set(LIBRARY_NAME Routes)

add_library(${LIBRARY_NAME}
        src/Router.cpp
)

target_include_directories(${LIBRARY_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${LIBRARY_NAME} PRIVATE CppFlask)