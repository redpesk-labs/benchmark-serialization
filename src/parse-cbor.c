#include "parse-cbor.h"

inline void addMapCbor_uint8	(cbor_item_t* cborMap, uint8_t value, char* key)	{ type_data td; td.data_int8 = value; addMapCbor(cborMap, td, type_uint8, key); }
inline void addMapCbor_uint16	(cbor_item_t* cborMap, uint16_t value, char* key)	{ type_data td; td.data_uint16 = value; addMapCbor(cborMap, td, type_uint16, key); }
inline void addMapCbor_int16	(cbor_item_t* cborMap, int16_t value, char* key)	{ type_data td; td.data_int16 = value; addMapCbor(cborMap, td, type_int16, key); }
inline void addMapCbor_float	(cbor_item_t* cborMap, float value, char* key)		{ type_data td; td.data_float = value; addMapCbor(cborMap, td, type_float, key); }
inline void addMapCbor_bool		(cbor_item_t* cborMap, bool value, char* key)		{ type_data td; td.data_bool = value; addMapCbor(cborMap, td, type_bool, key); }

void parse_to_cbor(SensorData* sensorData, cbor_item_t* my_cbor)
{
	//cbor_serialize(my_cbor, &sensorData,sizeof(SensorData));
	cbor_map_add(my_cbor, (struct cbor_pair) {
		.key = cbor_move(cbor_build_string("SensorVersion")),
			.value = cbor_move(SensorVersionToCbor(sensorData->version))
	});
	cbor_map_add(my_cbor, (struct cbor_pair) {
		.key = cbor_move(cbor_build_string("SensorStatus")),
			.value = cbor_move(SensorStatusToCbor(sensorData->sStatus))
	});
	cbor_map_add(my_cbor, (struct cbor_pair) {
		.key = cbor_move(cbor_build_string("TargetStatus")),
			.value = cbor_move(TargetStatusToCbor(sensorData->tStatus))
	});
	cbor_map_add(my_cbor, (struct cbor_pair) {
		.key = cbor_move(cbor_build_string("SensorInfo")),
			.value = cbor_move(TargetInfoToCbor(sensorData->tInfo))
	});
}

void cbor_to_sensorData(cbor_item_t* my_cbor, SensorData* sensorData)
{
	struct cbor_pair* cborPairTmp = cbor_map_handle(my_cbor);
	/* cbor_item_t *mapCborSensorVersion = cbor_new_definite_map(5);
	cbor_item_t *mapCborSensorStatus = cbor_new_definite_map(3);
	cbor_item_t *mapCborTargetStatus = cbor_new_definite_map(2);
	cbor_item_t *mapCborTargetInfo = cbor_new_definite_map(7);
	cbor_item_t *mapCborSensorVersion;
	cbor_item_t *mapCborSensorStatus;
	cbor_item_t *mapCborTargetStatus;
	cbor_item_t *mapCborTargetInfo;


	mapCborSensorVersion = cborPairTmp[0].value;
	mapCborSensorStatus = cborPairTmp[1].value;
	mapCborTargetStatus = cborPairTmp[2].value;
	mapCborTargetInfo = cborPairTmp[3].value; */

	sensorData->version = cborToSensorVersion(cborPairTmp[0].value);
	sensorData->sStatus = cborToSensorStatus(cborPairTmp[1].value);
	sensorData->tStatus = cborToTargetStatus(cborPairTmp[2].value);
	sensorData->tInfo = cborToTargetInfo(cborPairTmp[3].value);
}

cbor_item_t* SensorVersionToCbor(SensorVersion data)
{
	cbor_item_t* cbor_temp = cbor_new_definite_map(5);
	addMapCbor_uint8(cbor_temp, data.dataType, "DataType");
	addMapCbor_uint8(cbor_temp, data.master, "Master");
	addMapCbor_uint8(cbor_temp, data.result, "Result");
	addMapCbor_uint8(cbor_temp, data.second, "Second");
	addMapCbor_uint8(cbor_temp, data.step, "Step");
	return cbor_temp;

}

SensorVersion cborToSensorVersion(cbor_item_t* cborData)
{
	struct cbor_pair* cborPairTmp = cbor_map_handle(cborData);
	SensorVersion sensorVersionTmp;
	sensorVersionTmp.dataType = cbor_get_uint8(cborPairTmp[0].value);
	sensorVersionTmp.master = cbor_get_uint8(cborPairTmp[1].value);
	sensorVersionTmp.result = cbor_get_uint8(cborPairTmp[2].value);
	sensorVersionTmp.second = cbor_get_uint8(cborPairTmp[3].value);
	sensorVersionTmp.step = cbor_get_uint8(cborPairTmp[4].value);
	return sensorVersionTmp;
}

cbor_item_t* SensorStatusToCbor(SensorStatus data)
{
	cbor_item_t* cbor_temp = cbor_new_definite_map(3);
	addMapCbor_uint8(cbor_temp, data.actl_mode, "ActlMode");
	addMapCbor_uint8(cbor_temp, data.cfgStatus, "CfgStatus");
	addMapCbor_uint8(cbor_temp, data.rollcount, "Rollcount");
	return cbor_temp;
}

SensorStatus cborToSensorStatus(cbor_item_t* cborData)
{
	struct cbor_pair* cborPairTmp = cbor_map_handle(cborData);
	SensorStatus sensorStatusTmp;
	sensorStatusTmp.actl_mode = cbor_get_uint8(cborPairTmp[0].value);
	sensorStatusTmp.cfgStatus = cbor_get_uint8(cborPairTmp[1].value);
	sensorStatusTmp.rollcount = cbor_get_uint8(cborPairTmp[2].value);
	return sensorStatusTmp;
}

cbor_item_t* TargetStatusToCbor(TargetStatus data)
{
	cbor_item_t* cbor_temp = cbor_new_definite_map(2);
	addMapCbor_uint8(cbor_temp, data.noOfTarget, "NoOfTarget");
	addMapCbor_uint8(cbor_temp, data.rollcount, "Rollcount");
	return cbor_temp;
}

TargetStatus cborToTargetStatus(cbor_item_t* cborData)
{
	struct cbor_pair* cborPairTmp = cbor_map_handle(cborData);
	TargetStatus targetStatusTmp;
	targetStatusTmp.noOfTarget = cbor_get_uint8(cborPairTmp[0].value);
	targetStatusTmp.rollcount = cbor_get_uint8(cborPairTmp[1].value);
	return targetStatusTmp;
}

cbor_item_t* TargetInfoToCbor(TargetInfo data)
{
	cbor_item_t* cbor_temp = cbor_new_definite_map(7);
	addMapCbor_int16(cbor_temp, data.azimuth, "Azimuth");
	addMapCbor_uint8(cbor_temp, data.index, "Index");
	addMapCbor_float(cbor_temp, data.range, "Range");
	addMapCbor_float(cbor_temp, data.rcs, "RCS");
	addMapCbor_uint8(cbor_temp, data.rollCount, "Rollcount");
	addMapCbor_uint8(cbor_temp, data.SNR, "SNR");
	addMapCbor_float(cbor_temp, data.vrel, "Vrel");
	return cbor_temp;
}

TargetInfo cborToTargetInfo(cbor_item_t* cborData)
{
	struct cbor_pair* cborPairTmp = cbor_map_handle(cborData);

	TargetInfo targetInfoTmp;
	targetInfoTmp.azimuth = cbor_get_uint16(cborPairTmp[0].value);
	targetInfoTmp.index = cbor_get_uint8(cborPairTmp[1].value);
	targetInfoTmp.range = cbor_float_get_float2(cborPairTmp[2].value);
	targetInfoTmp.rcs = cbor_float_get_float2(cborPairTmp[3].value);
	targetInfoTmp.rollCount = cbor_get_uint8(cborPairTmp[4].value);
	targetInfoTmp.SNR = cbor_get_uint8(cborPairTmp[5].value);
	targetInfoTmp.vrel = cbor_float_get_float2(cborPairTmp[6].value);

	return targetInfoTmp;
}


void addMapCbor(cbor_item_t* cborMap, type_data value, type type_data, char* key)
{
	switch (type_data) {
	case type_uint8:
		cbor_map_add(cborMap, (struct cbor_pair) {
			.key = cbor_move(cbor_build_string(key)),
				.value = cbor_move(cbor_build_uint8(value.data_uint8))
		});
		break;
	case type_uint16:
		cbor_map_add(cborMap, (struct cbor_pair) {
			.key = cbor_move(cbor_build_string(key)),
				.value = cbor_move(cbor_build_uint16(value.data_uint16))
		});
		break;
	case type_int16:
		cbor_map_add(cborMap, (struct cbor_pair) {
			.key = cbor_move(cbor_build_string(key)),
				.value = (cbor_move(cbor_build_uint16(value.data_int16)))
		});
		/*
			if(value.data_int16 > 0) {
				cbor_map_add(cborMap, (struct cbor_pair) {
					.key = cbor_move(cbor_build_string(key)),
					.value = (cbor_move(cbor_build_uint16(value.data_int16)))
				});
			}else {
				cbor_item_t *item_temp = cbor_build_uint16(value.data_int16);
				cbor_mark_negint(item_temp);
				cbor_map_add(cborMap, (struct cbor_pair) {
					.key = cbor_move(cbor_build_string(key)),
					.value = (cbor_move(item_temp))
				});
			}
			*/
		break;
	case type_float:
		cbor_map_add(cborMap, (struct cbor_pair) {
			.key = cbor_move(cbor_build_string(key)),
				.value = cbor_move(cbor_build_float2(value.data_float))
		});
		break;
	case type_bool:
		cbor_map_add(cborMap, (struct cbor_pair) {
			.key = cbor_move(cbor_build_string(key)),
				.value = cbor_move(cbor_build_bool(value.data_bool))
		});
		break;
	default:
		break;
	}
}
