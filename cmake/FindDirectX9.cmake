message("Looking for DirectX9...")

file(GLOB DX9_SEARCH_PATHS
	"C:/Program Files (x86)/Microsoft DirectX SDK*/Include"
	"$ENV{DXSDK_DIR}/Include"
	"$ENV{PROGRAMFILES}/Microsoft DirectX SDK*/Include"
)

find_path(DX9_INCLUDE_DIRS
          NAMES d3dx9.h 
          PATHS ${DX9_SEARCH_PATHS})


set(DX9_LIBRARY_PATHS
    "${DX9_INCLUDE_DIRS}/../Lib/x86/"
)

find_library(DX9_LIB d3dx9.lib ${DX9_LIBRARY_PATHS} NO_DEFAULT_PATH)
set(DX9_LIBRARIES ${DX9_LIB})

add_library(DirectX9 INTERFACE)

target_include_directories(DirectX9 INTERFACE ${DX9_INCLUDE_DIRS})
target_link_libraries(DirectX9 INTERFACE ${DX9_LIBRARIES})