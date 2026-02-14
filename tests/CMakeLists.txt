# ./tests/CMakeLists.txt

# Create the test executable
add_executable(unit_tests 
    CameraClientTests.cpp
    CollageCreateTests.cpp
    SessionInfoTests.cpp
    UploadTests.cpp
    # ServerTests.cpp
)

# Link against GTest and your core library
target_link_libraries(unit_tests 
    PRIVATE 
    GTest::gtest_main 
    camera_core
)

include(GoogleTest)
gtest_discover_tests(unit_tests)