cmake_minimum_required(VERSION 3.6)

set (PROJECT_NAME, M3Game)
set (TARGET_NAME Match3Game)

project("${TARGET_NAME}")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 COMPONENTS graphics window system)
include_directories(${SFML_INCLUDE_DIR})

if(${SFML_FOUND})
    add_compile_options(-std=c++14)
    file(GLOB_RECURSE srcs src/*.h src/*.cpp src/*.hpp)
    add_executable(${TARGET_NAME} ${srcs})
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})

    target_link_libraries(${TARGET_NAME} ${SFML_LIBRARIES})
    include_directories("${CMAKE_SOURCE_DIR}/Dependencies/nlohmann/include/")
    FILE(COPY resources DESTINATION "${CMAKE_BINARY_DIR}")
else() 
    message(SEND_ERROR "Could not find SFML library. Please refer to: https://www.sfml-dev.org/")
    if(WIN32)
	    message("You should consider adding SFML library path to 'PATH' environment variable")
	endif()
endif()
