cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 11)

project(file_signature_generator)

include_directories(${CMAKE_SOURCE_DIR})

find_package(OpenSSL REQUIRED)

include_directories(include
                    include/interfaces
                    utils/include)
file(GLOB SOURCES "src/*.cpp" "utils/src/*.cpp" "main.cpp")
file(GLOB HEADERS "utils/include/*.h" "include/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} OpenSSL::SSL pthread)
