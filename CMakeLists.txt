cmake_minimum_required(VERSION 3.14.4)

project(AdvPT VERSION 1.0
              DESCRIPTION "Tensors"
              LANGUAGES CXX)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
include(external/googletests.cmake)

if (BUILD_TESTING)
    add_executable(test_matvec test_matvec.cpp)
    target_link_libraries(test_matvec PRIVATE GTest::gtest_main)

    include(GoogleTest)
    # Finds all the Google tests associated with the executable
    gtest_discover_tests(test_matvec)
endif()



#add_executable(test_tensor test_tensor.cpp)
#target_compile_features(test_tensor PRIVATE cxx_std_20)
#target_compile_options(test_tensor PRIVATE -Wall -Wextra -pedantic -Werror -pg -g)
#target_link_options(test_tensor PRIVATE -pg)
#
#add_executable(test_matvec test_matvec.cpp)
#target_compile_features(test_matvec PRIVATE cxx_std_20)
#target_compile_options(test_matvec PRIVATE -Wall -Wextra -pedantic -Werror -pg -g)
#target_link_options(test_matvec PRIVATE -pg)
#