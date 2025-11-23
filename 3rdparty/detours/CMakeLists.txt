project(detours)

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME} INTERFACE detours.h)
target_include_directories(${PROJECT_NAME} INTERFACE ./)
target_link_libraries(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/detours.lib)