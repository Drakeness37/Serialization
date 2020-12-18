cmake_minimum_required(VERSION 3.17)

project(json_laba)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)

add_executable(json_laba src/main.cpp data.h)

find_package(rapidjson REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++20 ${CMAKE_CXX_FLAGS} ${RAPIDJSON_CXX_FLAGS}")


include_directories(src ${RAPIDJSON_INCLUDE_DIRS})

find_package(Catch2 REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)

target_link_libraries(json_laba Catch2::Catch2 nlohmann_json::nlohmann_json ${RAPIDJSON_LIBRARIES} Qt5::Core)
