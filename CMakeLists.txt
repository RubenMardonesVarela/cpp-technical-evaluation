cmake_minimum_required(VERSION 3.8)

project(allreadevaluation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")

# Check that Gstreamer with version >= 1.4 is installed
find_package(PkgConfig REQUIRED)
pkg_search_module(GST REQUIRED gstreamer-1.0>=1.4
        gstreamer-sdp-1.0>=1.4
        gstreamer-app-1.0>=1.4
        gstreamer-video-1.0>=1.4
        )

add_subdirectory(src)
add_subdirectory("thirdparty/googletest")
add_subdirectory(test)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)


