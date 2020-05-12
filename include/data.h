#pragma once

#include <stdint.h>
#include <stdbool.h>

enum option
{
	MAP, 
	ARRAY,
};

typedef struct 
{
	uint8_t index;
	float rcs;
	float range;
	int16_t azimuth;
	float vrel;
	uint8_t rollCount;
	int8_t SNR;
} TargetInfo;

typedef struct
{
	uint8_t dataType;
	bool result;
	uint8_t master;
	uint8_t second;
	uint8_t step;
} SensorVersion;

typedef struct
{
	uint8_t actl_mode;
	uint8_t rollcount;
	uint8_t cfgStatus;
} SensorStatus;

typedef struct
{
	uint8_t noOfTarget;
	uint8_t rollcount;
} TargetStatus;

typedef struct {
	SensorVersion version;
	SensorStatus sStatus;
	TargetStatus tStatus;
	TargetInfo tInfo;
	uint8_t tInfoSize;
} SensorData;
