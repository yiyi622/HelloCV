cmake_minimum_required(VERSION 3.10)
project(traffic_light)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(traffic_light traffic_light.cpp)

target_link_libraries(traffic_light ${OpenCV_LIRS})

set_target_properties(traffic_light_detection PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
