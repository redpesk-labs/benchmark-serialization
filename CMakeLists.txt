cmake_minimum_required(VERSION 3.10)

# set the project name
project(benchmark)

set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)


# add the executable
add_executable(benchmark 
main.c
src/parse-json.c 
src/sp70c-data-handle.c
src/udpServer.c
src/parse-cbor.c)

# add the binary tree to the search path for include files
target_include_directories(benchmark PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/include"
                          )

target_link_libraries(benchmark json-c cbor)