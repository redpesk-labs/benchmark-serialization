cmake_minimum_required(VERSION 3.10)

include(FindPkgConfig)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# list option
option(BENCH_JSON				"Enable json-c benchmark" 					ON)
option(BENCH_JSON_ARRAY			"Enable json-c option array benchmark" 		OFF)
option(BENCH_FASTJSON			"Enable fastJson benchmark" 				ON)
option(BENCH_FASTJSON_ARRAY 	"Enable fastjson option array benchmark"	OFF)
option(BENCH_JSONCPP			"Enable jsoncpp lib of nlohmann" 			ON)
option(BENCH_JSONSTRING			"Enable json string representation"			ON)
option(BENCH_JSONSTRING_ARRAY	"Enable jsonstring option array benchmark"	OFF)
option(BENCH_JSMN				"Enable jsmn option"						ON)
option(BENCH_JSMN_ARRAY			"Enable option array for jsmn"				OFF)
option(BENCH_CBOR				"Enable cbor benchmark" 					ON)
option(BENCH_CBOR_ARRAY			"Enable cbor option array benchmark" 		OFF)
option(BENCH_XDR				"Enable xdr benchmark" 						ON)
option(BENCH_PROTOBUF			"Enable protobuf benchmark" 				ON)
option(BENCH_DEBUG				"Enable debug infos" 						OFF)

# set the project name
project(benchmark)

# find libstatgrab pakage
pkg_check_modules(LIBSTATGRAB REQUIRED libstatgrab)

	# ---------- json-c enabled -----------------------------------------------
	if (BENCH_JSON)

		if(BENCH_JSON_ARRAY)
			set(TARGET_NAME "benchmark_jsonc_array")
		else()
			set(TARGET_NAME "benchmark_jsonc")
		endif()

		add_executable(${TARGET_NAME}
			main.c
			wrappers/wrap_c/parse-c.c
			bench_conf/bench-conf.c
			bench_option/option-timer.c
			bench_option/option-cpu.c
			bench_option/utilities-fct.c
		)

		target_include_directories(${TARGET_NAME} PRIVATE 
			"${PROJECT_SOURCE_DIR}/include" 
			"${PROJECT_SOURCE_DIR}/wrappers/wrap_c"
			"${PROJECT_SOURCE_DIR}/wrappers/wrap_jsonc"
			"${PROJECT_SOURCE_DIR}/bench_conf"
			"${PROJECT_SOURCE_DIR}/bench_option"
			"${LIBSTATGRAB_INCLUDE_DIRS}"
		)
		target_compile_features(${TARGET_NAME} PRIVATE c_std_11)

		pkg_check_modules(JSONC REQUIRED json-c)

		target_compile_definitions(${TARGET_NAME} PRIVATE BENCH_JSON)
		target_include_directories(${TARGET_NAME} PRIVATE "${JSONC_INCLUDE_DIRS}")
		target_link_libraries(${TARGET_NAME} PRIVATE "${JSONC_LIBRARIES}" "${LIBSTATGRAB_LIBRARIES}" pthread)
		target_sources(${TARGET_NAME} PRIVATE "wrappers/wrap_jsonc/parse-jsonc.c")

		if (BENCH_JSON_ARRAY)
			target_compile_definitions(${TARGET_NAME} PRIVATE BENCH_JSON_ARRAY)
		endif()

		install(TARGETS ${TARGET_NAME} DESTINATION /usr/bin)

	endif()

	

	# ---------- FastJson enabled -----------------------------------------------
	if (BENCH_FASTJSON)

		if(BENCH_FASTJSON_ARRAY)
			set(TARGET_NAME "benchmark_fastjson_array")
		else()
			set(TARGET_NAME "benchmark_fastjson")
		endif()

		add_executable(${TARGET_NAME}
			main.c
			wrappers/wrap_c/parse-c.c
			bench_conf/bench-conf.c
			bench_option/option-timer.c
			bench_option/option-cpu.c
			bench_option/utilities-fct.c
		)

		target_include_directories(${TARGET_NAME} PRIVATE 
									"${PROJECT_SOURCE_DIR}/include" 
									"${PROJECT_SOURCE_DIR}/wrappers/wrap_c"
									"${PROJECT_SOURCE_DIR}/wrappers/wrap_fastjson" 
									"${PROJECT_SOURCE_DIR}/bench_conf"
									"${PROJECT_SOURCE_DIR}/bench_option"
									"${LIBSTATGRAB_INCLUDE_DIRS}")
		target_compile_features(${TARGET_NAME} PRIVATE c_std_11)

		pkg_check_modules(FASTJSON REQUIRED libfastjson)

		target_compile_definitions(${TARGET_NAME} PRIVATE BENCH_FASTJSON)
		target_include_directories(${TARGET_NAME} PRIVATE "${FASTJSON_INCLUDE_DIRS}")
		target_link_libraries(${TARGET_NAME} PRIVATE "${FASTJSON_LIBRARIES}" "${LIBSTATGRAB_LIBRARIES}" pthread)
		target_sources(${TARGET_NAME} PRIVATE "wrappers/wrap_fastjson/parse-fastjson.c")

		if (BENCH_FASTJSON_ARRAY)
			target_compile_definitions(${TARGET_NAME} PRIVATE BENCH_FASTJSON_ARRAY)
		endif()

		install(TARGETS ${TARGET_NAME} DESTINATION /usr/bin)
	endif()


	# ---------- json-cpp nlohmann enabled -----------------------------------------------
	if (BENCH_JSONCPP)

		add_executable(benchmark_jsoncpp
			main.cpp
		)

		target_include_directories(benchmark_jsoncpp PRIVATE 
									"${PROJECT_SOURCE_DIR}/include" 
									"${PROJECT_SOURCE_DIR}/wrappers/wrap_c"
									"${PROJECT_SOURCE_DIR}/wrappers/wrap_jsoncpp" 
									"${PROJECT_SOURCE_DIR}/bench_conf"
									"${PROJECT_SOURCE_DIR}/bench_option"
									"${LIBSTATGRAB_INCLUDE_DIRS}")
		target_compile_features(benchmark_jsoncpp PRIVATE c_std_11)

		find_path(JSONCPP_INCLUDE_DIRS json.hpp
			HINTS $ENV{CROSS_ROOT} ${JSONCPP_INSTALL_DIR} $ENV{JSONCPP_INSTALL_DIR}
			PATH_SUFFIXES "include" "include/nlohmann"
		)
		if (NOT DEFINED JSONCPP_INCLUDE_DIRS OR "${JSONCPP_INCLUDE_DIRS}" STREQUAL "JSONCPP_INCLUDE_DIRS-NOTFOUND")
			message(FATAL_ERROR "nlohmann/json's header file (json.hpp) was not found!")
		else()
			message(STATUS "nlohmann/json's header file found: ${JSONCPP_INCLUDE_DIRS}")
		endif()

		set(CMAKE_PREFIX_PATH "/usr/lib/cmake" ${CMAKE_PREFIX_PATH})		
		find_package(nlohmann_json REQUIRED)

		target_compile_definitions(benchmark_jsoncpp PRIVATE BENCH_JSONCPP)
		target_link_libraries(benchmark_jsoncpp PRIVATE nlohmann_json "${LIBSTATGRAB_LIBRARIES}" pthread)
		target_sources(benchmark_jsoncpp PRIVATE "wrappers/wrap_jsoncpp/parse-jsoncpp.cpp")

		install(TARGETS benchmark_jsoncpp DESTINATION /usr/bin)
	endif()

	# ---------- json string enabled --------------------------------------------------
	if (BENCH_JSONSTRING)

		if(BENCH_FASTJSON_ARRAY)
			set(TARGET_NAME "benchmark_jsonstring_array")
		else()
			set(TARGET_NAME "benchmark_jsonstring")
		endif()

		add_executable(${TARGET_NAME}
			main.c
			wrappers/wrap_c/parse-c.c
			bench_conf/bench-conf.c
			bench_option/option-timer.c
			bench_option/option-cpu.c
			bench_option/utilities-fct.c
		)

		target_include_directories(${TARGET_NAME} PRIVATE 
									"${PROJECT_SOURCE_DIR}/include" 
									"${PROJECT_SOURCE_DIR}/wrappers/wrap_c"
									"${PROJECT_SOURCE_DIR}/wrappers/wrap_jsonstring" 
									"${PROJECT_SOURCE_DIR}/bench_conf"
									"${PROJECT_SOURCE_DIR}/bench_option"
									"${LIBSTATGRAB_INCLUDE_DIRS}")
		target_compile_features(${TARGET_NAME} PRIVATE c_std_11)
		target_link_libraries(${TARGET_NAME} PRIVATE "${LIBSTATGRAB_LIBRARIES}" pthread)
		target_compile_definitions(${TARGET_NAME} PRIVATE BENCH_JSONSTRING)
		target_sources(${TARGET_NAME} PRIVATE "wrappers/wrap_jsonstring/parse-jsonstring.c" "wrappers/wrap_jsonstring/mjson.c")

		if (BENCH_JSONSTRING_ARRAY)
			target_compile_definitions(${TARGET_NAME} PRIVATE BENCH_JSONSTRING_ARRAY)
		endif()

		install(TARGETS ${TARGET_NAME} DESTINATION /usr/bin)
	endif()

	# ---------- jsmn enabled --------------------------------------------------
	if (BENCH_JSMN)

		if(BENCH_JSMN_ARRAY)
			set(TARGET_NAME "benchmark_jsmn_array")
		else()
			set(TARGET_NAME "benchmark_jsmn")
		endif()	

		add_executable(${TARGET_NAME}
			main.c
			wrappers/wrap_c/parse-c.c
			bench_conf/bench-conf.c
			bench_option/option-timer.c
			bench_option/option-cpu.c
			bench_option/utilities-fct.c
		)

		target_include_directories(${TARGET_NAME} PRIVATE 
									"${PROJECT_SOURCE_DIR}/include" 
									"${PROJECT_SOURCE_DIR}/wrappers/wrap_c"
									"${PROJECT_SOURCE_DIR}/wrappers/wrap_jsmn"
									"${PROJECT_SOURCE_DIR}/bench_conf"
									"${PROJECT_SOURCE_DIR}/bench_option"
									"${LIBSTATGRAB_INCLUDE_DIRS}")
		
		target_compile_features(${TARGET_NAME} PRIVATE c_std_11)
		target_link_libraries(${TARGET_NAME} PRIVATE "${LIBSTATGRAB_LIBRARIES}" pthread)
		target_compile_definitions(${TARGET_NAME} PRIVATE BENCH_JSMN)
		target_sources(${TARGET_NAME} PRIVATE "wrappers/wrap_jsmn/wrap-jsmn.c")

		if (BENCH_JSMN_ARRAY)
			target_compile_definitions(${TARGET_NAME} PRIVATE BENCH_JSMN_ARRAY)
		endif()

		install(TARGETS ${TARGET_NAME} DESTINATION /usr/bin)
	endif()

	# ---------- cbor enabled -------------------------------------------------
	if (BENCH_CBOR)

		if(BENCH_CBOR_ARRAY)
			set(TARGET_NAME "benchmark_cbor_array")
		else()
			set(TARGET_NAME "benchmark_cbor")
		endif()	

		add_executable(${TARGET_NAME}
			main.c
			wrappers/wrap_c/parse-c.c
			bench_conf/bench-conf.c
			bench_option/option-timer.c
			bench_option/option-cpu.c
			bench_option/utilities-fct.c
		)

		target_include_directories(${TARGET_NAME} PRIVATE 
									"${PROJECT_SOURCE_DIR}/include"
									"${PROJECT_SOURCE_DIR}/wrappers/wrap_c" 
									"${PROJECT_SOURCE_DIR}/wrappers/wrap_cbor" 
									"${PROJECT_SOURCE_DIR}/bench_conf"
									"${PROJECT_SOURCE_DIR}/bench_option"
									"${LIBSTATGRAB_INCLUDE_DIRS}")
		target_compile_features(${TARGET_NAME} PRIVATE c_std_11)
		
		pkg_check_modules(CBOR REQUIRED libcbor)

		target_compile_definitions(${TARGET_NAME} PRIVATE BENCH_CBOR)
		target_include_directories(${TARGET_NAME} PRIVATE "${CBOR_INCLUDE_DIRS}")
		target_link_libraries(${TARGET_NAME} PRIVATE "${CBOR_LIBRARIES}" "${LIBSTATGRAB_LIBRARIES}" pthread)
		target_sources(${TARGET_NAME} PRIVATE "wrappers/wrap_cbor/parse-cbor.c")

		if (BENCH_CBOR_ARRAY)
			target_compile_definitions(${TARGET_NAME} PRIVATE BENCH_CBOR_ARRAY)
		endif()

		install(TARGETS ${TARGET_NAME} DESTINATION /usr/bin)
	endif()

	# ---------- xdr enabled --------------------------------------------------
	if (BENCH_XDR)

		add_executable(benchmark_xdr
			main.c
			wrappers/wrap_c/parse-c.c
			bench_conf/bench-conf.c
			bench_option/option-timer.c
			bench_option/option-cpu.c
			bench_option/utilities-fct.c
		)

		target_include_directories(benchmark_xdr PRIVATE 
			"${PROJECT_SOURCE_DIR}/include" 
			"${PROJECT_SOURCE_DIR}/wrappers/wrap_c"
			"${PROJECT_SOURCE_DIR}/wrappers/wrap_xdr" 
			"${PROJECT_SOURCE_DIR}/bench_conf"
			"${PROJECT_SOURCE_DIR}/bench_option"
			"${LIBSTATGRAB_INCLUDE_DIRS}")

		target_compile_features(benchmark_xdr PRIVATE c_std_11)
		
		pkg_check_modules(RPC REQUIRED libtirpc)

		if ("$ENV{TARGET_NM}" STREQUAL "aarch64-linux-gnu-nm")
			message(STATUS "xdr in cross build mode : $ENV{TARGET_NM}")
			#target_include_directories(benchmark_xdr PRIVATE "/usr/include/tirpc")
		else()
			message(STATUS "xdr in native mode")
		endif()

		target_compile_definitions(benchmark_xdr PRIVATE BENCH_XDR)
		target_include_directories(benchmark_xdr PRIVATE "${RPC_INCLUDE_DIRS}")
		target_link_libraries(benchmark_xdr PRIVATE "${RPC_LIBRARIES}" "${LIBSTATGRAB_LIBRARIES}" pthread)
		target_sources(benchmark_xdr PRIVATE "wrappers/wrap_xdr/parse-xdr.c")

		install(TARGETS benchmark_xdr DESTINATION /usr/bin)
	endif()

	# ---------- protobuf enabled --------------------------------------------------
	if (BENCH_PROTOBUF)

		add_executable(benchmark_protobuf
			main.c
			wrappers/wrap_c/parse-c.c
			bench_conf/bench-conf.c
			bench_option/option-timer.c
			bench_option/option-cpu.c
			bench_option/utilities-fct.c
		)

		target_include_directories(benchmark_protobuf PRIVATE 
									"${PROJECT_SOURCE_DIR}/include" 
									"${PROJECT_SOURCE_DIR}/wrappers/wrap_c"
									"${PROJECT_SOURCE_DIR}/wrappers/wrap_protobuf" 
									"${PROJECT_SOURCE_DIR}/bench_conf"
									"${PROJECT_SOURCE_DIR}/bench_option"
									"${LIBSTATGRAB_INCLUDE_DIRS}")
		target_compile_features(benchmark_protobuf PRIVATE c_std_11)

		pkg_check_modules(PROTOBUFC REQUIRED libprotobuf-c)

		target_compile_definitions(benchmark_protobuf PRIVATE BENCH_PROTOBUF)
		target_include_directories(benchmark_protobuf PRIVATE "${PROTOBUFC_INCLUDE_DIRS}")
		target_link_libraries(benchmark_protobuf PRIVATE "${PROTOBUFC_LIBRARIES}" "${LIBSTATGRAB_LIBRARIES}" pthread)
		target_sources(benchmark_protobuf PRIVATE "sensordata.pb-c.c")
		target_sources(benchmark_protobuf PRIVATE "wrappers/wrap_protobuf/parse-protobuf.c")

		install(TARGETS benchmark_protobuf DESTINATION /usr/bin)
	endif()


	# ---------- debug enabled ------------------------------------------------
	if (BENCH_DEBUG)

		add_executable(benchmark_debug
			main.c
			wrappers/wrap_c/parse-c.c
			bench_conf/bench-conf.c
			bench_option/option-timer.c
			bench_option/option-cpu.c
			bench_option/utilities-fct.c
		)

		target_include_directories(benchmark_debug PRIVATE 
									"${PROJECT_SOURCE_DIR}/include" 
									"${PROJECT_SOURCE_DIR}/wrappers/wrap_c"
									"${PROJECT_SOURCE_DIR}/bench_conf"
									"${PROJECT_SOURCE_DIR}/bench_option"
									"${LIBSTATGRAB_INCLUDE_DIRS}")
		target_compile_features(benchmark_debug PRIVATE c_std_11)
		target_link_libraries(benchmark_debug PRIVATE "${LIBSTATGRAB_LIBRARIES}" pthread)
		target_compile_definitions(benchmark_debug PRIVATE BENCH_DEBUG)

	endif()
