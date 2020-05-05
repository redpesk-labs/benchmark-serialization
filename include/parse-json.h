#ifndef PARSE_JSON_H
#define PARSE_JSON_H

#include "config-parsing.h"
#include "struct-parsing.h"
#include "sp70c-data-handle.h"

void parse_to_json(SensorData *sensorData, json_object *my_json);
void json_to_sensorData(json_object *my_json, SensorData *sensorData);

struct json_object *SensorVersionToJsonObject(SensorVersion * data);
SensorVersion JsonObjectToSensorVersion(struct json_object * data);

struct json_object *SensorStatusToJsonObject(SensorStatus * data);
SensorStatus JsonObjectToSensorStatus(struct json_object * data);

struct json_object *TargetStatusToJsonObject(TargetStatus * data);
TargetStatus JsonObjectToTargetStatus(struct json_object * data);

struct json_object *TargetInfoToJsonObject(TargetInfo * data);
TargetInfo *JsonObjectToTargetInfo(struct json_object * data);

#endif