#ifndef PARSE_JSON_H
#define PARSE_JSON_H

#include "config-parsing.h"
#include "struct-parsing.h"
#include "sp70c-data-handle.h"

void parse_to_json(SensorData *sensorData, json_object *my_json, enum option my_option);
void json_to_sensorData(json_object *my_json, SensorData *sensorData, enum option my_option);

struct json_object *SensorVersionToJsonObject(SensorVersion * data, enum option my_option);
SensorVersion JsonObjectToSensorVersion(struct json_object * data, enum option my_option);

struct json_object *SensorStatusToJsonObject(SensorStatus * data, enum option my_option);
SensorStatus JsonObjectToSensorStatus(struct json_object * data, enum option my_option);

struct json_object *TargetStatusToJsonObject(TargetStatus * data, enum option my_option);
TargetStatus JsonObjectToTargetStatus(struct json_object * data, enum option my_option);

struct json_object *TargetInfoToJsonObject(TargetInfo * data, enum option my_option);
TargetInfo JsonObjectToTargetInfo(struct json_object * data, enum option my_option);

#endif