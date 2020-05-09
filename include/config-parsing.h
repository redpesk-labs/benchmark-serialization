#pragma once

#define NUMBER_PORT				1
#define PARSING_BUFFER_SIZE		14

#define DATA_TESTED				10000

#ifdef BENCH_JSON
#include "parse-json.h"
#endif

#ifdef BENCH_CBOR
#include "parse-cbor.h"
#endif

#if BENCH_XDR
#include "parse-xdr.h"
#endif
