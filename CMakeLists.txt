cmake_minimum_required(VERSION 3.10)

option(BENCH_JSON			"Enable json-c benchmark" OFF)
option(BENCH_JSON_ARRAY		"Enable json-c option array benchmark" OFF)
option(BENCH_FASTJSON		"Enable fastJson benchmark" ON)
option(BENCH_FASTJSON_ARRAY "Enable fastjson option array benchmark" OFF)
option(BENCH_JSONCPP		"Enable josncpp lib of nlohmann" OFF)
option(BENCH_CBOR			"Enable cbor benchmark" OFF)
option(BENCH_CBOR_ARRAY		"Enable cbor option array benchmark" OFF)
option(BENCH_XDR			"Enable xdr benchmark" OFF)
option(BENCH_PROTOBUF		"Enable protobuf benchmark" OFF)
option(BENCH_DEBUG			"Enable debug infos" OFF)

# set the project name
project(benchmark)

	# ---------- Add the main executable target -------------------------------
	add_executable(benchmark
		main.c
		#src/sp70c-data-handle.c
	)

	target_include_directories(benchmark PRIVATE "${PROJECT_SOURCE_DIR}/include" )
	target_compile_features(benchmark PRIVATE c_std_11)

	# ---------- json-c enabled -----------------------------------------------
	if (BENCH_JSON)
		find_path(JSONC_INCLUDE_DIRS json.h
			HINTS ${JSONC_INSTALL_DIR} $ENV{JSONC_INSTALL_DIR}
			PATH_SUFFIXES "include" "json-c" "include/json-c"
		)
		if (NOT DEFINED JSONC_INCLUDE_DIRS OR "${JSONC_INCLUDE_DIRS}" STREQUAL "JSONC_INCLUDE_DIRS-NOTFOUND")
			message(FATAL_ERROR "json-c's header file (json.h) was not found!")
		else()
			message(STATUS "json-c's header file found: ${JSONC_INCLUDE_DIRS}")
		endif()

		find_library(JSONC_LIBRARIES
			NAMES json-c libjson-c
			HINTS ${JSONC_INSTALL_DIR} $ENV{JSONC_INSTALL_DIR}
			PATH_SUFFIXES "lib"
		)
		if (NOT DEFINED JSONC_LIBRARIES OR "${JSONC_LIBRARIES}" STREQUAL "JSONC_LIBRARIES-NOTFOUND")
			message(FATAL_ERROR "json-c's library was not found!")
		else()
			message(STATUS "json-c's library found: ${JSONC_LIBRARIES}")
		endif()

		target_compile_definitions(benchmark PRIVATE BENCH_JSON)
		target_include_directories(benchmark PRIVATE "${JSONC_INCLUDE_DIRS}")
		target_link_libraries(benchmark PRIVATE "${JSONC_LIBRARIES}")
		target_sources(benchmark PRIVATE "src/parse-jsonc.c")

		if (BENCH_JSON_ARRAY)
			target_compile_definitions(benchmark PRIVATE BENCH_JSON_ARRAY)
		endif()

	endif()

		# ---------- FastJson enabled -----------------------------------------------
		if (BENCH_FASTJSON)
		find_path(FASTJSON_INCLUDE_DIRS json.h
			HINTS ${FASTJSON_INSTALL_DIR} $ENV{FASTJSON_INSTALL_DIR}
			PATH_SUFFIXES "include" "libfastjson" "include/libfastjson"
		)
		if (NOT DEFINED FASTJSON_INCLUDE_DIRS OR "${FASTJSON_INCLUDE_DIRS}" STREQUAL "FASTJSON_INCLUDE_DIRS-NOTFOUND")
			message(FATAL_ERROR "fastjson's header file (json.h) was not found!")
		else()
			message(STATUS "fastjson-c's header file found: ${FASTJSON_INCLUDE_DIRS}")
		endif()

		find_library(FASTJSON_LIBRARIES
			NAMES fastjson libfastjson
			HINTS ${FASTJSON_INSTALL_DIR} $ENV{FASTJSON_INSTALL_DIR}
			PATH_SUFFIXES "lib"
		)
		if (NOT DEFINED FASTJSON_LIBRARIES OR "${FASTJSON_LIBRARIES}" STREQUAL "FASTJSON_LIBRARIES-NOTFOUND")
			message(FATAL_ERROR "fastjson's library was not found!")
		else()
			message(STATUS "fastjson's library found: ${FASTJSON_LIBRARIES}")
		endif()

		target_compile_definitions(benchmark PRIVATE BENCH_FASTJSON)
		target_include_directories(benchmark PRIVATE "${FASTJSON_INCLUDE_DIRS}")
		target_link_libraries(benchmark PRIVATE "${FASTJSON_LIBRARIES}")
		target_sources(benchmark PRIVATE "src/parse-fastjson.c")

		if (BENCH_FASTJSON_ARRAY)
			target_compile_definitions(benchmark PRIVATE BENCH_FASTJSON_ARRAY)
		endif()

	endif()
	# ---------- json-cpp nlohmann enabled -----------------------------------------------
	if (BENCH_JSONCPP)
		find_path(JSONCPP_INCLUDE_DIRS json.hpp
			HINTS ${JSONCPP_INSTALL_DIR} $ENV{JSONCPP_INSTALL_DIR}
			PATH_SUFFIXES "include" "include/nlohmann"
		)
		if (NOT DEFINED JSONCPP_INCLUDE_DIRS OR "${JSONCPP_INCLUDE_DIRS}" STREQUAL "JSONCPP_INCLUDE_DIRS-NOTFOUND")
			message(FATAL_ERROR "nlohmann/json's header file (json.hpp) was not found!")
		else()
			message(STATUS "nlohmann/json's header file found: ${JSONCPP_INCLUDE_DIRS}")
		endif()

		set(CMAKE_PREFIX_PATH "/usr/lib/cmake";${CMAKE_PREFIX_PATH})		
		find_package(nlohmann_json REQUIRED)

		# find_library(JSONCPP_LIBRARIES
		# 	NAMES nlohmann libnlohmann
		# 	HINTS ${JSONC_INSTALL_DIR} $ENV{JSONC_INSTALL_DIR} "/usr/lib/cmake"
		# 	PATH_SUFFIXES "lib"
		# )
		# if (NOT DEFINED JSONCPP_LIBRARIES OR "${JSONCPP_LIBRARIES}" STREQUAL "JSONCPP_LIBRARIES-NOTFOUND")
		# 	message(FATAL_ERROR "nlohmann/json's library was not found!")
		# else()
		# 	message(STATUS "nlohmann/json's library found: ${JSONCPP_LIBRARIES}")
		# endif()

		target_compile_definitions(benchmark PRIVATE BENCH_JSONCPP)
		target_include_directories(benchmark PRIVATE "${JSONCPP_INCLUDE_DIRS}")
		#target_link_libraries(benchmark PRIVATE "${JSONCPP_LIBRARIES}")
		target_link_libraries(benchmark PRIVATE nlohmann_json)
		target_sources(benchmark PRIVATE "src/parse-jsoncpp.cpp")

	endif()


	# ---------- cbor enabled -------------------------------------------------
	if (BENCH_CBOR)
		find_path(CBOR_INCLUDE_DIRS cbor.h
			HINTS ${CBOR_INSTALL_DIR} $ENV{CBOR_INSTALL_DIR}
			PATH_SUFFIXES "include" "cbor" "include/cbor"
		)
		if (NOT DEFINED CBOR_INCLUDE_DIRS OR "${CBOR_INCLUDE_DIRS}" STREQUAL "CBOR_INCLUDE_DIRS-NOTFOUND")
			message(FATAL_ERROR "cbor's header file (cbor.h) was not found!")
		else()
			message(STATUS "cbor's header file found: ${CBOR_INCLUDE_DIRS}")
		endif()

		find_library(CBOR_LIBRARIES
			NAMES cbor libcbor
			HINTS ${CBOR_INSTALL_DIR} $ENV{CBOR_INSTALL_DIR}
			PATH_SUFFIXES "lib"
		)
		if (NOT DEFINED CBOR_LIBRARIES OR "${CBOR_LIBRARIES}" STREQUAL "CBOR_LIBRARIES-NOTFOUND")
			message(FATAL_ERROR "cbor's library was not found!")
		else()
			message(STATUS "cbor's library found: ${CBOR_LIBRARIES}")
		endif()

		target_compile_definitions(benchmark PRIVATE BENCH_CBOR)
		target_include_directories(benchmark PRIVATE "${CBOR_INCLUDE_DIRS}")
		target_link_libraries(benchmark PRIVATE "${CBOR_LIBRARIES}")
		target_sources(benchmark PRIVATE src/parse-cbor.c)

		if (BENCH_CBOR_ARRAY)
			target_compile_definitions(benchmark PRIVATE BENCH_CBOR_ARRAY)
		endif()
	endif()

	# ---------- xdr enabled --------------------------------------------------
	if (BENCH_XDR)
		target_compile_definitions(benchmark PRIVATE BENCH_XDR)
		target_sources(benchmark PRIVATE src/parse-xdr.c)
	endif()

	# ---------- protobuf enabled --------------------------------------------------
	if (BENCH_PROTOBUF)
		find_path(PROTOBUFC_INCLUDE_DIRS protobuf-c.h
			HINTS ${PROTOBUFC_INSTALL_DIR} $ENV{PROTOBUFCC_INSTALL_DIR}
			PATH_SUFFIXES "include" "protobuf-c" "include/protobuf-c"
		)
		if (NOT DEFINED PROTOBUFC_INCLUDE_DIRS OR "${PROTOBUFC_INCLUDE_DIRS}" STREQUAL "PROTOBUFC_INCLUDE_DIRS-NOTFOUND")
			message(FATAL_ERROR "protobuf-c's header file (protobuf-c.h) was not found!")
		else()
			message(STATUS "protobuf-c's header file found: ${PROTOBUFC_INCLUDE_DIRS}")
		endif()

		find_library(PROTOBUFC_LIBRARIES
			NAMES protobuf-c libprotobuf-c
			HINTS ${PROTOBUFC_INSTALL_DIR} $ENV{PROTOBUFC_INSTALL_DIR}
			PATH_SUFFIXES "lib"
		)
		if (NOT DEFINED PROTOBUFC_LIBRARIES OR "${PROTOBUFC_LIBRARIES}" STREQUAL "PROTOBUFC_LIBRARIES-NOTFOUND")
			message(FATAL_ERROR "protobuf-c's library was not found!")
		else()
			message(STATUS "protobuf-c's library found: ${PROTOBUFC_LIBRARIES}")
		endif()

		target_compile_definitions(benchmark PRIVATE BENCH_PROTOBUF)
		target_include_directories(benchmark PRIVATE "${PROTOBUFC_INCLUDE_DIRS}")
		target_link_libraries(benchmark PRIVATE "${PROTOBUFC_LIBRARIES}")
		target_sources(benchmark PRIVATE "sensordata.pb-c.c")
		target_sources(benchmark PRIVATE "src/parse-protobuf.c")
	endif()

	# ---------- debug enabled ------------------------------------------------
	if (BENCH_DEBUG)
		target_compile_definitions(benchmark PRIVATE BENCH_DEBUG)
	endif()
