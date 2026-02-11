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
find_package(TBB REQUIRED)

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

# 3. Fetch GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

# 4. Fetch CPR (New HTTP Client)
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG        1.10.4  # Using a stable tag is safer than master
)
# Optimizations to make CPR build faster
set(CPR_USE_SYSTEM_CURL ON CACHE BOOL "" FORCE) # Use the curl we installed via apt-get
set(CPR_BUILD_TESTS OFF CACHE BOOL "" FORCE)     # Don't build CPR's own internal tests
FetchContent_MakeAvailable(cpr)

# --- BUILD CONFIGURATION ---

# 5. Define Core Source Files (Logic Only)
set(CORE_SOURCES 
    src/CameraClient.cpp
    src/CollageCreate.cpp
    src/UploadClient.cpp
    src/routes/DebugRoutes.cpp
    src/routes/SessionRoutes.cpp
    src/routes/CollageRoutes.cpp
)

# 6. Create a Library for the Core Logic
add_library(camera_core STATIC ${CORE_SOURCES})

target_include_directories(camera_core PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
    ${GPHOTO2_INCLUDE_DIRS}
    ${CUPS_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
)

# Link dependencies to the library
target_link_libraries(camera_core PUBLIC
    ${GPHOTO2_LIBRARIES}
    ${CUPS_LIBRARIES}
    ${OpenCV_LIBS}
    OpenMP::OpenMP_CXX
    TBB::tbb
    Crow::Crow
    nlohmann_json::nlohmann_json
    cpr::cpr
)

# --- EXECUTABLE: CAMERA SERVER ---

add_executable(camera_server src/main.cpp)
target_link_libraries(camera_server PRIVATE camera_core)

# --- EXECUTABLE: UNIT TESTS ---

enable_testing()

if(EXISTS "${CMAKE_SOURCE_DIR}/tests")
    add_subdirectory(tests)
endif()