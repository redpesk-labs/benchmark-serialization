cmake_minimum_required(VERSION 3.10)

# set the project name
project(benchmark)

	# ---------- Find json-c --------------------------------------------------
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

	# ---------- Find cbor ----------------------------------------------------
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

	# ---------- Add the main executable target -------------------------------
	add_executable(benchmark
		main.c
		src/parse-json.c
		src/sp70c-data-handle.c
		src/parse-cbor.c
		src/parse-xdr.c
	)

	# add the binary tree to the search path for include files
	target_include_directories(benchmark PRIVATE
		"${PROJECT_SOURCE_DIR}/include"
		"${JSONC_INCLUDE_DIRS}"
		"${CBOR_INCLUDE_DIRS}"
	)

	target_link_libraries(benchmark PRIVATE
		"${JSONC_LIBRARIES}"
		"${CBOR_LIBRARIES}"
	)

	target_compile_features(benchmark PRIVATE
		c_std_11
	)
