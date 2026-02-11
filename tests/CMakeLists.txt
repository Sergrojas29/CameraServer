# ./tests/CMakeLists.txt

# Create the test executable
add_executable(unit_tests 
    test_main.cpp
    # Add other test files here later, e.g., CollageTests.cpp
)

# Link against GTest and your core library
target_link_libraries(unit_tests 
    PRIVATE 
    GTest::gtest_main 
    camera_core
)

# Register the tests with CTest
include(GoogleTest)
gtest_discover_tests(unit_tests)