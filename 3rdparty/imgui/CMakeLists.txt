cmake_minimum_required (VERSION 3.8)

project(imgui)

add_library (${PROJECT_NAME} "imgui.cpp" "imgui_demo.cpp" "imgui_draw.cpp" "imgui_impl_dx9.cpp" "imgui_impl_win32.cpp" "imgui_tables.cpp" "imgui_widgets.cpp")
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

#include(../FindDirectX9.cmake)
target_link_libraries(${PROJECT_NAME} PRIVATE ${DIRECTX_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${DIRECTX_INCLUDE_DIRS} )

target_link_libraries(${PROJECT_NAME} INTERFACE imgui)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")