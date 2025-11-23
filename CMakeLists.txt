cmake_minimum_required(VERSION 3.26)
project(yorenov-cheat)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE PROJECT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE PROJECT_SOURCE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)


add_library(${PROJECT_NAME} SHARED
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCE_HEADERS}
        ${PROJECT_SOURCES}
)

set(USE_IMGUI ON)
set(USE_D3D9 ON)
set(USE_RAKHOOK ON)
set(USE_PSDK ON)
set(USE_SAMPAPI ON)

if (USE_IMGUI)
    if (NOT USE_D3D9)
        message(FATAL_ERROR "Can't use imgui without d3d9!")
    endif ()

    add_subdirectory("3rdparty/imgui")
    target_link_libraries(${PROJECT_NAME} PRIVATE imgui)
    add_compile_definitions(IMGUI_DEFINE_MATH_OPERATORS)
endif ()

if (USE_D3D9)
    include(FindDirectX9)
    target_link_libraries(${PROJECT_NAME} PRIVATE DirectX9)
endif ()

if (USE_RAKHOOK)
    include(FindRakHook)
    target_link_libraries(${PROJECT_NAME} PRIVATE RakHook)
endif ()

if (USE_PSDK)
    find_package(PluginSDK REQUIRED gtasa)
    target_link_libraries(${PROJECT_NAME} PRIVATE PluginSDK::gtasa)
endif ()

if (USE_SAMPAPI)
    add_subdirectory("3rdparty/SAMP-API-multiver")
    target_link_libraries(${PROJECT_NAME} PRIVATE sampapi)
endif ()

add_subdirectory("3rdparty/kthook")
target_link_libraries(${PROJECT_NAME} PRIVATE kthook)

add_subdirectory("3rdparty/detours")
target_link_libraries(${PROJECT_NAME} PRIVATE detours)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_USE_MULTITHREADED ON)
target_link_libraries(${PROJECT_NAME} PRIVATE
        ${Boost_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC
        ${Boost_INCLUDE_DIRS})

if (MSVC)
    add_compile_definitions(_WIN32_WINNT=0x0601)
endif ()

find_package(Boost REQUIRED COMPONENTS system thread)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found!")
endif ()



# Threads
find_package(Threads REQUIRED)

# OpenSSL
find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE
        OpenSSL::SSL OpenSSL::Crypto)

# websocketpp
find_package(websocketpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE
        websocketpp::websocketpp)

# themida
include(${CMAKE_SOURCE_DIR}/cmake/FindThemidaSDK.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE
        ThemidaSDK)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/")

add_compile_definitions(_USE_MATH_DEFINES)

set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        SUFFIX ".asi"
)