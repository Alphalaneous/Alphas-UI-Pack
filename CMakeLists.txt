cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(AlphasUIPack VERSION 1.0.0)

file(GLOB_RECURSE CPPSOURCES CONFIGURE_DEPENDS src/*.cpp)

foreach(FILE ${CPPSOURCES})
    if(FILE MATCHES "/examples/")
        list(REMOVE_ITEM CPPSOURCES ${FILE})
    endif()
endforeach()

include_directories(${CMAKE_SOURCE_DIR}/include)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    file(GLOB_RECURSE OBJCXXSOURCES CONFIGURE_DEPENDS src/*.mm)
    enable_language(OBJCXX)
    set(SOURCES ${CPPSOURCES} ${OBJCXXSOURCES})
else()
    set(SOURCES ${CPPSOURCES})
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    target_link_libraries(${PROJECT_NAME} ${COCOA_LIBRARY})
endif()

if (PROJECT_IS_TOP_LEVEL)
    target_compile_definitions(${PROJECT_NAME} PRIVATE ALPHALANEOUS_UI_PACK_EXPORTING)
endif()

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})
