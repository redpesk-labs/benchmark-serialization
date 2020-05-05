#ifndef MAIN_H
#define MAIN_H

#include "include/config-parsing.h"
#include "include/struct-parsing.h"
#include "include/udpServer.h"
#include "include/sp70c-data-handle.h"
#include "include/parse-json.h"
#include "include/parse-cbor.h"

/* Function Prototype */
void error(char *msg);
void debug_parse(struct json_object *data_json);
int verification(SensorData sensorData_1, SensorData sensorData_2);
void printResult(int err, double time);

#endif