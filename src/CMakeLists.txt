#set(_srcs Kobuki_impl.cpp KobukiBase.cpp SerialPort.cpp Thread.cpp Translator.cpp Transport.cpp DockingController.cpp libkobuki.cpp kobukicwrapper.cpp)
set(_srcs stargazer.cpp StarGazer_impl.cpp)
set(_demo_srcs demo.c)

message("hoge")
message(${CMAKE_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/libaqua/include)

find_package(Threads)
if (UNIX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif (UNIX)

add_library(${PROJECT_NAME} SHARED ${_srcs})
add_library(${PROJECT_NAME}_static STATIC ${_srcs})

add_executable(${PROJECT_NAME}_demo ${_demo_srcs})
target_link_libraries(${PROJECT_NAME}_demo ${PROJECT_NAME}_static ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_demo ${PROJECT_NAME}_static
    EXPORT ${PROJECT_NAME}
    RUNTIME DESTINATION ${BIN_INSTALL_DIR} 
    LIBRARY DESTINATION ${LIB_INSTALL_DIR} 
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR} )
install(EXPORT ${PROJECT_NAME}
    DESTINATION ${LIB_INSTALL_DIR}/${PROJECT_NAME}
    FILE ${PROJECT_NAME}Depends.cmake)

