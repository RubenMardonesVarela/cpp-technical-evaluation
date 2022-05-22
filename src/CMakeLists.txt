set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.hpp *.cpp)

set(SOURCES ${SOURCES})

include_directories(${GST_INCLUDE_DIRS})
include_directories("../thirdparty/spdlog/include")

# Creates library for unit testing 
add_library(${CMAKE_PROJECT_NAME}_lib STATIC ${SOURCES})
add_executable(${BINARY} ${SOURCES})
target_link_libraries(${BINARY} ${GST_LIBRARIES} gstapp-1.0 gstvideo-1.0 stdc++fs)


