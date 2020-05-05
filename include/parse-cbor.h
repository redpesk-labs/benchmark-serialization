#include "config-parsing.h"
#include "struct-parsing.h"

enum type
{
    type_uint8,
    type_uint16,
    type_float,
    type_bool,
};
union type_data
{
    uint8_t data_uint8;
    int8_t data_int8;
    uint16_t data_uint16;
    int16_t data_int16;
    float data_float;
    bool data_bool;
};

void parse_to_cbor(SensorData *sensorData, cbor_item_t *my_cbor);
void cbor_to_sensorData(cbor_item_t *my_cbor, SensorData *sensorData);

cbor_item_t *SensorVersionToCbor(SensorVersion data);
SensorVersion cborToSensorVersion(cbor_item_t *cborData);

cbor_item_t *SensorStatusToCbor(SensorStatus data);
SensorStatus cborToSensorStatus(cbor_item_t *cborData);

cbor_item_t *TargetStatusToCbor(TargetStatus data);
TargetStatus cborToTargetStatus(cbor_item_t *cborData);

cbor_item_t *TargetInfoToCbor(TargetInfo * data);
TargetInfo *cborToTargetInfo(cbor_item_t *cborData);

void addMapCbor(cbor_item_t *cborMap, union type_data value, enum type type_data, char *key);