cmake_minimum_required(VERSION 3.10)

option(BENCH_JSON	"Enable json-c benchmark" ON)
option(BENCH_CBOR	"Enable cbor benchmark" ON)
option(BENCH_XDR	"Enable xdr benchmark" ON)
option(BENCH_DEBUG	"Enable debug infos" OFF)

# set the project name
project(benchmark)

	# ---------- Add the main executable target -------------------------------
	add_executable(benchmark
		main.c
		src/sp70c-data-handle.c
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
		target_sources(benchmark PRIVATE src/parse-json.c)
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
	endif()

	# ---------- xdr enabled --------------------------------------------------
	if (BENCH_XDR)
		target_compile_definitions(benchmark PRIVATE BENCH_XDR)
		target_sources(benchmark PRIVATE src/parse-xdr.c)
	endif()

	# ---------- debug enabled ------------------------------------------------
	if (BENCH_DEBUG)
		target_compile_definitions(benchmark PRIVATE BENCH_DEBUG)
	endif()
