enable_testing()

set(BINARY ${CMAKE_PROJECT_NAME}_test)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES true *.h *.cpp)
set(SOURCES ${TEST_SOURCES})

set(SOURCES ${TEST_SOURCES})

include_directories("../src/thirdparty/spdlog/include")


include(GoogleTest)
add_executable(${BINARY} ${TEST_SOURCES})
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib GTest::gtest)

gtest_discover_tests(${BINARY})


