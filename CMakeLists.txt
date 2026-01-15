cmake_minimum_required(VERSION 3.24)
project(CameraServer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- FIND DEPENDENCIES ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(GPHOTO2 REQUIRED libgphoto2)
pkg_check_modules(CUPS REQUIRED cups)
find_package(OpenCV REQUIRED)
find_package(OpenMP REQUIRED)

include(FetchContent)

# 1. Fetch Crow
FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(Crow)

# 2. Fetch Nlohmann JSON
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.2
)
FetchContent_MakeAvailable(json)

# --- BUILD CONFIGURATION ---

# 3. Define the Source Files
# We explicitly list the .cpp files here
set(SOURCE_FILES 
    src/main.cpp 
    src/CameraClient.cpp
)

add_executable(camera_server ${SOURCE_FILES})

# 4. Include Directories
# This tells the compiler: "When I say #include "header.h", look in the 'include' folder"
target_include_directories(camera_server PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${GPHOTO2_INCLUDE_DIRS}
    ${CUPS_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)

# 5. Link Libraries
target_link_libraries(camera_server PRIVATE
    ${GPHOTO2_LIBRARIES}
    ${CUPS_LIBRARIES}
    ${OpenCV_LIBS}
    OpenMP::OpenMP_CXX
    Crow::Crow
    nlohmann_json::nlohmann_json
)