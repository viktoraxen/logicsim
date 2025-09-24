find_package(GTest REQUIRED)

# TODO: Add source files needed for tests
add_executable(logicsimTest 
    main.cpp 
    tests.cpp
    ../src/node.cpp
    ../src/gate.cpp
    ../src/scheduler.cpp
)

target_link_libraries(logicsimTest GTest::gtest GTest::gtest_main pthread)

target_include_directories(logicsimTest PRIVATE ${CMAKE_SOURCE_DIR}/include)

gtest_discover_tests(logicsimTest PROPERTIES ENVIRONMENT "GTEST_COLOR=1")
