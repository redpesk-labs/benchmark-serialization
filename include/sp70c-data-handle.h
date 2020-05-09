#pragma once

#include "struct-parsing.h"

int initTargetInfo(TargetInfo* tInfo);
int initSensorVersion(SensorVersion* sVersion);
int initSensorStatus(SensorStatus* sStatus);
int initTargetStatus(TargetStatus* tStatus);
int initData(SensorData* sensorData);

int parseRawBuffer(uint8_t* buffer, size_t size, SensorData* sensorData);
int parseSensorVersion(uint8_t* buffer, SensorData* sensorData);
int parseSensorStatus(uint8_t* buffer, SensorData* sensorData);
int parseTargetStatus(uint8_t* buffer, SensorData* sensorData);
int parseTargetInfo(uint8_t* buffer, SensorData* sensorData);
int parseAllTargetInfo(uint8_t* buffer, SensorData* sensorData);

int targetInfoCopy(TargetInfo* copy, TargetInfo* copied);
