#pragma once

#include "data.h"
#include "../sensordata.pb-c.h"

size_t parse_to_protobuf(SensorData *sensorData, uint8_t *buf);
void protobuf_to_sensorData(uint8_t *buf, SensorData *sensorData, size_t length);

void sensorVersionToProtobuf(SensorVersion *sensorVersion, SensorVersionMessage *msg);
void protobufToSensorVersion(SensorVersionMessage *msg, SensorVersion *sensorVersion);

void sensorStatusToProtobuf(SensorStatus *sensorStatus, SensorStatusMessage *msg);
void protobufToSensorStatus(SensorStatusMessage *msg, SensorStatus *sensorStatus);

void targetStatusToProtobuf(TargetStatus *targetStatus, TargetStatusMessage *msg);
void protobufToTargetStatus(TargetStatusMessage *msg, TargetStatus *targetStatus);

void targetInfoToProtobuf(TargetInfo *targetInfo, TargetInfoMessage *msg);
void protobufToTargetInfo(TargetInfoMessage *msg, TargetInfo *targetInfo);