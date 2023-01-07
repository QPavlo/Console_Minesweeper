cmake_minimum_required(VERSION 3.16)
project(minesweeper)

set(CMAKE_CXX_FLAGS -O3)
set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "src/sources/*.cpp")
file(GLOB HEADERS "src/headers/*.hpp")

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

find_package(Boost 1.71 COMPONENTS program_options REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
