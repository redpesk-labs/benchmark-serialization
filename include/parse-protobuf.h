#pragma once

#include "data.h"
#include "../sensordata.pb-c.h"

size_t protobuf_serialize_sensorData(SensorData *sensorData, uint8_t *buf);
void protobuf_deserialize_sensorData(uint8_t *buf, SensorData *sensorData, size_t length);