#pragma once

#include "struct-parsing.h"
#include <cbor.h>

typedef enum
{
	type_uint8,
	type_uint16,
	type_int16,
	type_float,
	type_bool,
} type;

typedef union
{
	uint8_t data_uint8;
	int8_t data_int8;
	uint16_t data_uint16;
	int16_t data_int16;
	float data_float;
	bool data_bool;
} type_data;

void parse_to_cbor(SensorData* sensorData, cbor_item_t* my_cbor);
void cbor_to_sensorData(cbor_item_t* my_cbor, SensorData* sensorData);

cbor_item_t* SensorVersionToCbor(SensorVersion data);
SensorVersion cborToSensorVersion(cbor_item_t* cborData);

cbor_item_t* SensorStatusToCbor(SensorStatus data);
SensorStatus cborToSensorStatus(cbor_item_t* cborData);

cbor_item_t* TargetStatusToCbor(TargetStatus data);
TargetStatus cborToTargetStatus(cbor_item_t* cborData);

cbor_item_t* TargetInfoToCbor(TargetInfo data);
TargetInfo cborToTargetInfo(cbor_item_t* cborData);

void addMapCbor(cbor_item_t* cborMap, type_data value, type type_data, char* key);
