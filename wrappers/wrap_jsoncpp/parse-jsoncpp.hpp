#pragma once

#include "data.h"
#include "serializer.h"
#include <nlohmann/json.hpp> 
#include <iostream>

class ParseJsonCpp
{
public:
    void jsoncpp_get_serializer(Serializer*);
};