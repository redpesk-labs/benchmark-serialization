cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BENCH_JSON			"Enable json-c benchmark" OFF)
option(BENCH_JSON_ARRAY		"Enable json-c option array benchmark" ON)
option(BENCH_FASTJSON		"Enable fastJson benchmark" ON)
option(BENCH_FASTJSON_ARRAY "Enable fastjson option array benchmark" ON)
option(BENCH_JSONCPP		"Enable jsoncpp lib of nlohmann" ON)
option(BENCH_CBOR			"Enable cbor benchmark" ON)
option(BENCH_CBOR_ARRAY		"Enable cbor option array benchmark" ON)
option(BENCH_XDR			"Enable xdr benchmark" ON)
option(BENCH_PROTOBUF		"Enable protobuf benchmark" ON)
option(BENCH_DEBUG			"Enable debug infos" OFF)

# set the project name
project(benchmark)


	# ---------- json-c enabled -----------------------------------------------
	if (BENCH_JSON)

		add_executable(benchmark_jsonc
			main.c
			#src/sp70c-data-handle.c
		)

		target_include_directories(benchmark_jsonc PRIVATE "${PROJECT_SOURCE_DIR}/include" )
		target_compile_features(benchmark_jsonc PRIVATE c_std_11)

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

		target_compile_definitions(benchmark_jsonc PRIVATE BENCH_JSON)
		target_include_directories(benchmark_jsonc PRIVATE "${JSONC_INCLUDE_DIRS}")
		target_link_libraries(benchmark_jsonc PRIVATE "${JSONC_LIBRARIES}")
		target_sources(benchmark_jsonc PRIVATE "src/parse-jsonc.c")

		if (BENCH_JSON_ARRAY)
			target_compile_definitions(benchmark_jsonc PRIVATE BENCH_JSON_ARRAY)
		endif()

	endif()

	# ---------- FastJson enabled -----------------------------------------------
	if (BENCH_FASTJSON)

		add_executable(benchmark_fastjson
			main.c
			#src/sp70c-data-handle.c
		)

		target_include_directories(benchmark_fastjson PRIVATE "${PROJECT_SOURCE_DIR}/include" )
		target_compile_features(benchmark_fastjson PRIVATE c_std_11)

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

		target_compile_definitions(benchmark_fastjson PRIVATE BENCH_FASTJSON)
		target_include_directories(benchmark_fastjson PRIVATE "${FASTJSON_INCLUDE_DIRS}")
		target_link_libraries(benchmark_fastjson PRIVATE "${FASTJSON_LIBRARIES}")
		target_sources(benchmark_fastjson PRIVATE "src/parse-fastjson.c")

		if (BENCH_FASTJSON_ARRAY)
			target_compile_definitions(benchmark_fastjson PRIVATE BENCH_FASTJSON_ARRAY)
		endif()

	endif()

	# ---------- json-cpp nlohmann enabled -----------------------------------------------
	if (BENCH_JSONCPP)

		add_executable(benchmark_jsoncpp
			main.cpp
			#src/sp70c-data-handle.c
		)

		target_include_directories(benchmark_jsoncpp PRIVATE "${PROJECT_SOURCE_DIR}/include" )
		target_compile_features(benchmark_jsoncpp PRIVATE c_std_11)

		find_path(JSONCPP_INCLUDE_DIRS json.hpp
			HINTS ${JSONCPP_INSTALL_DIR} $ENV{JSONCPP_INSTALL_DIR}
			PATH_SUFFIXES "include" "include/nlohmann"
		)
		if (NOT DEFINED JSONCPP_INCLUDE_DIRS OR "${JSONCPP_INCLUDE_DIRS}" STREQUAL "JSONCPP_INCLUDE_DIRS-NOTFOUND")
			message(FATAL_ERROR "nlohmann/json's header file (json.hpp) was not found!")
		else()
			message(STATUS "nlohmann/json's header file found: ${JSONCPP_INCLUDE_DIRS}")
		endif()

		set(CMAKE_PREFIX_PATH "/usr/lib/cmake" ${CMAKE_PREFIX_PATH})		
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

		target_compile_definitions(benchmark_jsoncpp PRIVATE BENCH_JSONCPP)
		#target_include_directories(benchmark PRIVATE "${JSONCPP_INCLUDE_DIRS}")
		#target_link_libraries(benchmark PRIVATE "${JSONCPP_LIBRARIES}")
		target_link_libraries(benchmark_jsoncpp PRIVATE nlohmann_json)
		target_sources(benchmark_jsoncpp PRIVATE "src/parse-jsoncpp.cpp")

	endif()

	# ---------- cbor enabled -------------------------------------------------
	if (BENCH_CBOR)

		add_executable(benchmark_cbor
			main.c
			#src/sp70c-data-handle.c
		)

		target_include_directories(benchmark_cbor PRIVATE "${PROJECT_SOURCE_DIR}/include" )
		target_compile_features(benchmark_cbor PRIVATE c_std_11)

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

		target_compile_definitions(benchmark_cbor PRIVATE BENCH_CBOR)
		target_include_directories(benchmark_cbor PRIVATE "${CBOR_INCLUDE_DIRS}")
		target_link_libraries(benchmark_cbor PRIVATE "${CBOR_LIBRARIES}")
		target_sources(benchmark_cbor PRIVATE src/parse-cbor.c)

		if (BENCH_CBOR_ARRAY)
			target_compile_definitions(benchmark_cbor PRIVATE BENCH_CBOR_ARRAY)
		endif()
	endif()

	# ---------- xdr enabled --------------------------------------------------
	if (BENCH_XDR)

		add_executable(benchmark_xdr
			main.c
			#src/sp70c-data-handle.c
		)

		target_include_directories(benchmark_xdr PRIVATE "${PROJECT_SOURCE_DIR}/include" )
		target_compile_features(benchmark_xdr PRIVATE c_std_11)

		target_compile_definitions(benchmark_xdr PRIVATE BENCH_XDR)
		target_sources(benchmark_xdr PRIVATE src/parse-xdr.c)
	endif()

	# ---------- protobuf enabled --------------------------------------------------
	if (BENCH_PROTOBUF)

		add_executable(benchmark_protobuf
			main.c
			#src/sp70c-data-handle.c
		)

		target_include_directories(benchmark_protobuf PRIVATE "${PROJECT_SOURCE_DIR}/include" )
		target_compile_features(benchmark_protobuf PRIVATE c_std_11)

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

		target_compile_definitions(benchmark_protobuf PRIVATE BENCH_PROTOBUF)
		target_include_directories(benchmark_protobuf PRIVATE "${PROTOBUFC_INCLUDE_DIRS}")
		target_link_libraries(benchmark_protobuf PRIVATE "${PROTOBUFC_LIBRARIES}")
		target_sources(benchmark_protobuf PRIVATE "sensordata.pb-c.c")
		target_sources(benchmark_protobuf PRIVATE "src/parse-protobuf.c")
	endif()

	# ---------- debug enabled ------------------------------------------------
	if (BENCH_DEBUG)

		add_executable(benchmark_debug
			main.c
			#src/sp70c-data-handle.c
		)

		target_include_directories(benchmark_debug PRIVATE "${PROJECT_SOURCE_DIR}/include" )
		target_compile_features(benchmark_debug PRIVATE c_std_11)

		target_compile_definitions(benchmark_debug PRIVATE BENCH_DEBUG)
	endif()
