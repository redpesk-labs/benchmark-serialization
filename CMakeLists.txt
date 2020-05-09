cmake_minimum_required(VERSION 3.10)

# set the project name
project(benchmark)



# add the executable
add_executable(benchmark
	main.c
	src/parse-json.c
	src/sp70c-data-handle.c
	src/parse-cbor.c
	src/parse-protobuf.c
	src/parse-xdr.c
)

# add the binary tree to the search path for include files
target_include_directories(benchmark PRIVATE
	"${PROJECT_BINARY_DIR}"
	"${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(benchmark PRIVATE
	json-c
	cbor
	protobuf-c
)
