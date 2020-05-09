#pragma once

#include "struct-parsing.h"
#include <json.h>

void parse_to_json(SensorData* sensorData, struct json_object* my_json);
void json_to_sensorData(struct json_object* my_json, SensorData* sensorData);

struct json_object *SensorVersionToJsonObject(SensorVersion* data);
SensorVersion JsonObjectToSensorVersion(struct json_object* data);

struct json_object *SensorStatusToJsonObject(SensorStatus* data);
SensorStatus JsonObjectToSensorStatus(struct json_object* data);

struct json_object *TargetStatusToJsonObject(TargetStatus* data);
TargetStatus JsonObjectToTargetStatus(struct json_object* data);

struct json_object *TargetInfoToJsonObject(TargetInfo* data);
TargetInfo JsonObjectToTargetInfo(struct json_object* data);
