#pragma once

#include "data.h"
#include "serializer.h"
#include <nlohmann/json.hpp> 

class ParseJsonCpp
{
public:
    void jsoncpp_get_serializer(Serializer*);
};