message("Looking for RakHook...")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(RAKHOOK_DIR "${CMAKE_SOURCE_DIR}/3rdparty/RakHook/debug")
else ()
    set(RAKHOOK_DIR "${CMAKE_SOURCE_DIR}/3rdparty/RakHook/release")
endif ()

file(GLOB_RECURSE RAKHOOK_LIBRARIES ${RAKHOOK_DIR}/lib/*.lib)

add_library(RakHook INTERFACE)

message(STATUS "${RAKHOOK_DIR}/include")
message(STATUS "${RAKHOOK_LIBRARIES}")

target_include_directories(RakHook INTERFACE "${RAKHOOK_DIR}/include")
target_link_libraries(RakHook INTERFACE ${RAKHOOK_LIBRARIES})