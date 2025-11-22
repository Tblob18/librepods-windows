cmake_minimum_required(VERSION 3.16)

# Test suite for LibrePods Windows
project(librepods-tests)

# Enable testing
enable_testing()

# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Test Bluetooth Network)

# System Requirements Test
add_executable(test_system_requirements
    test_system_requirements.cpp
)

target_link_libraries(test_system_requirements
    PRIVATE
    Qt6::Test
    Qt6::Bluetooth
    Qt6::Network
)

# Add test to CTest
add_test(NAME SystemRequirements COMMAND test_system_requirements)

# Set test properties
set_tests_properties(SystemRequirements PROPERTIES
    TIMEOUT 60
    LABELS "requirements;system"
)

# On Windows, link additional libraries
if(WIN32)
    target_link_libraries(test_system_requirements
        PRIVATE
        ws2_32
        Bthprops
    )
endif()

# Installation (optional - tests are usually not installed)
# Uncomment if you want to install test executables
# install(TARGETS test_system_requirements
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/tests
# )

# Print test configuration info only in verbose mode
if(CMAKE_VERBOSE_MAKEFILE OR DEFINED ENV{VERBOSE})
    message(STATUS "Test suite configured successfully")
    message(STATUS "Run 'ctest' or 'ctest --verbose' to execute tests")
endif()
