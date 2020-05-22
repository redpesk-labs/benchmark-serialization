#pragma once

#include <stddef.h>
#include "data.h"
#include "serializer.h"

#define     SIZE_KEY_MAX    128
//#define MJSON_IMPLEMENT_STRTOD 1

void jsonstring_get_serializer(Serializer* s);