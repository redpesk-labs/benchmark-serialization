#include "parse-cbor.h"

// typredef
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

// Prototype inline function
void addMapCbor_uint8	(cbor_item_t* cborMap, uint8_t value, char* key);
void addMapCbor_uint16	(cbor_item_t* cborMap, uint16_t value, char* key);
void addMapCbor_int16	(cbor_item_t* cborMap, int16_t value, char* key);
void addMapCbor_float	(cbor_item_t* cborMap, float value, char* key);
void addMapCbor_bool	(cbor_item_t* cborMap, bool value, char* key);

void addArrayCbor_uint8	(cbor_item_t* cborMap, uint8_t value, int index);
void addArrayCbor_uint16	(cbor_item_t* cborMap, uint16_t value, int index);
void addArrayCbor_int16	(cbor_item_t* cborMap, int16_t value, int index);
void addArrayCbor_float	(cbor_item_t* cborMap, float value, int index);
void addArrayCbor_bool	(cbor_item_t* cborMap, bool value, int index);

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

void addArrayCbor(cbor_item_t* cborArray, type_data value, type type_data, int index)
{
    switch (type_data) {
        case type_uint8:
            cbor_array_set(cborArray, index, cbor_move(cbor_build_uint8(value.data_uint8)));
            break;
        case type_uint16:
            cbor_array_set(cborArray, index, cbor_move(cbor_build_uint16(value.data_uint16)));
            break;
        case type_int16:
            cbor_array_set(cborArray, index, cbor_move(cbor_build_uint16(value.data_int16)));
            break;
        case type_float:
            cbor_array_set(cborArray, index, cbor_move(cbor_build_float2(value.data_float)));
            break;
        case type_bool:
            cbor_array_set(cborArray, index, cbor_move(cbor_build_bool(value.data_bool)));  
            break;
        default:
            break;
    }    
}


// inline function
inline void addMapCbor_uint8	(cbor_item_t* cborMap, uint8_t value, char* key)	{ type_data td; td.data_int8 = value; addMapCbor(cborMap, td, type_uint8, key); }
inline void addMapCbor_uint16	(cbor_item_t* cborMap, uint16_t value, char* key)	{ type_data td; td.data_uint16 = value; addMapCbor(cborMap, td, type_uint16, key); }
inline void addMapCbor_int16	(cbor_item_t* cborMap, int16_t value, char* key)	{ type_data td; td.data_int16 = value; addMapCbor(cborMap, td, type_int16, key); }
inline void addMapCbor_float	(cbor_item_t* cborMap, float value, char* key)		{ type_data td; td.data_float = value; addMapCbor(cborMap, td, type_float, key); }
inline void addMapCbor_bool		(cbor_item_t* cborMap, bool value, char* key)		{ type_data td; td.data_bool = value; addMapCbor(cborMap, td, type_bool, key); }

inline void addArrayCbor_uint8	(cbor_item_t* cborMap, uint8_t value, int index)	{ type_data td; td.data_int8 = value; addArrayCbor(cborMap, td, type_uint8, index); }
inline void addArrayCbor_uint16	(cbor_item_t* cborMap, uint16_t value, int index)	{ type_data td; td.data_uint16 = value; addArrayCbor(cborMap, td, type_uint16, index); }
inline void addArrayCbor_int16	(cbor_item_t* cborMap, int16_t value, int index)	{ type_data td; td.data_int16 = value; addArrayCbor(cborMap, td, type_int16, index); }
inline void addArrayCbor_float	(cbor_item_t* cborMap, float value, int index)		{ type_data td; td.data_float = value; addArrayCbor(cborMap, td, type_float, index); }
inline void addArrayCbor_bool	(cbor_item_t* cborMap, bool value, int index)		{ type_data td; td.data_bool = value; addArrayCbor(cborMap, td, type_bool, index); }


/// @brief Serialize a SensorVersion object using cbor.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
struct cbor_item_t* cbor_serialize_SensorVersion(SensorVersion input, enum option opt)
{
	struct cbor_item_t* out;
    switch (opt){
		case ARRAY:
			out = cbor_new_definite_array(5);
			addArrayCbor_uint8(out, input.dataType, 0);
			addArrayCbor_uint8(out, input.master, 1);
			addArrayCbor_uint8(out, input.result, 2);
			addArrayCbor_uint8(out, input.second, 3);
			addArrayCbor_uint8(out, input.step, 4);
			out =cbor_move(out);
			break;
		
		case MAP:
			out = cbor_new_definite_map(5); 
			addMapCbor_uint8(out, input.dataType, "DataType");
			addMapCbor_uint8(out, input.master, "Master");
			addMapCbor_uint8(out, input.result, "Result");
			addMapCbor_uint8(out, input.second, "Second");
			addMapCbor_uint8(out, input.step, "Step");
			break;
		
		default :
			break;
    }
	
	return out;
}

/// @brief Deserialize a SensorVersion object using cbor.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int cbor_deserialize_SensorVersion(struct cbor_item_t* input, SensorVersion* output, enum option opt)
{
    struct cbor_pair *cborPairTmp ;
    switch (opt) {
        case ARRAY :
            output->dataType = cbor_get_uint8(cbor_move(cbor_array_get(input, 0)));
            output->master = cbor_get_uint8(cbor_move(cbor_array_get(input, 1)));
            output->result = cbor_get_uint8(cbor_move(cbor_array_get(input, 2)));
            output->second = cbor_get_uint8(cbor_move(cbor_array_get(input, 3)));
            output->step = cbor_get_uint8(cbor_move(cbor_array_get(input, 4)));
            break;
        case MAP :
            cborPairTmp = cbor_map_handle(input);
            output->dataType = cbor_get_uint8(cborPairTmp[0].value);
            output->master = cbor_get_uint8(cborPairTmp[1].value);
            output->result = cbor_get_uint8(cborPairTmp[2].value);
            output->second = cbor_get_uint8(cborPairTmp[3].value);
            output->step = cbor_get_uint8(cborPairTmp[4].value);
            break;
		default :
			break;
	}
	return EXIT_SUCCESS;
}

/// @brief Serialize a SensorStatus object using cbor.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
struct cbor_item_t* cbor_serialize_SensorStatus(SensorStatus input, enum option opt)
{
	cbor_item_t* out;

	switch (opt) {
    case ARRAY:
        out = cbor_new_definite_array(3);
		addArrayCbor_uint8(out, input.actl_mode, 0);
		addArrayCbor_uint8(out, input.cfgStatus, 1);
		addArrayCbor_uint8(out, input.rollcount, 2);
		out = cbor_move(out);
        break;
    case MAP:
        out = cbor_new_definite_map(3);
		addMapCbor_uint8(out, input.actl_mode, "ActlMode");
		addMapCbor_uint8(out, input.cfgStatus, "CfgStatus");
		addMapCbor_uint8(out, input.rollcount, "Rollcount");
        break;
	default :
		break;
    }
	return out;
}

/// @brief Deserialize a SensorStatus object using cbor.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int cbor_deserialize_SensorStatus(struct cbor_item_t* input, SensorStatus* output, enum option opt)
{
    struct cbor_pair *cborPairTmp ;

    switch (opt) {
        case ARRAY :
			output->actl_mode = cbor_get_uint8(cbor_move(cbor_array_get(input, 0)));
            output->cfgStatus = cbor_get_uint8(cbor_move(cbor_array_get(input, 1)));
			output->rollcount = cbor_get_uint8(cbor_move(cbor_array_get(input, 2)));
            break;
        case MAP :
			cborPairTmp = cbor_map_handle(input);
			output->actl_mode = cbor_get_uint8(cborPairTmp[0].value);
			output->cfgStatus = cbor_get_uint8(cborPairTmp[1].value);
			output->rollcount = cbor_get_uint8(cborPairTmp[2].value);
            break;
		default :
			break;
    }	

	return EXIT_SUCCESS;
}

/// @brief Serialize a TargetInfo object using cbor.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
struct cbor_item_t* cbor_serialize_TargetStatus(TargetStatus input, enum option opt)
{
	cbor_item_t *out;

    switch (opt) {
        case ARRAY :
            out = cbor_new_definite_array(2);
			addArrayCbor_uint8(out, input.noOfTarget, 0);
			addArrayCbor_uint8(out, input.rollcount, 1);
			out = cbor_move(out);
            break;
        case MAP :
            out = cbor_new_definite_map(2);
			addMapCbor_uint8(out, input.noOfTarget, "NoOfTarget");
			addMapCbor_uint8(out, input.rollcount, "Rollcount");
            break;
		default :
			break;      
    }
	
	return out;
}

/// @brief Deserialize a TargetStatus object using cbor.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int cbor_deserialize_TargetStatus(struct cbor_item_t* input, TargetStatus* output, enum option opt)
{
	struct cbor_pair *cborPairTmp;

    switch (opt) {
        case ARRAY :
            output->noOfTarget = cbor_get_uint8(cbor_move(cbor_array_get(input, 0)));
            output->rollcount = cbor_get_uint8(cbor_move(cbor_array_get(input, 1)));            
            break;
        case MAP :
            cborPairTmp = cbor_map_handle(input);
            output->noOfTarget = cbor_get_uint8(cborPairTmp[0].value);
            output->rollcount = cbor_get_uint8(cborPairTmp[1].value);
            break;
		default:
			break;
    }

	return EXIT_SUCCESS;
}

/// @brief Serialize a TargetInfo object using cbor.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
struct cbor_item_t* cbor_serialize_TargetInfo(TargetInfo input, enum option opt)
{
    cbor_item_t *out;

    switch (opt) {
        case ARRAY :
            out = cbor_new_definite_array(7);
            addArrayCbor_int16((out), input.azimuth, 0);
            addArrayCbor_uint8((out), input.index, 1);
            addArrayCbor_float((out), input.range, 2);
            addArrayCbor_float((out), input.rcs, 3);
            addArrayCbor_uint8((out), input.rollCount, 4);
            addArrayCbor_uint8((out), input.SNR, 5);
            addArrayCbor_float((out), input.vrel, 6);
			out = cbor_move(out);
            break;
        case MAP :
			out = cbor_new_definite_map(7);
			addMapCbor_int16(out, input.azimuth, "Azimuth");
			addMapCbor_uint8(out, input.index, "Index");
			addMapCbor_float(out, input.range, "Range");
			addMapCbor_float(out, input.rcs, "RCS");
			addMapCbor_uint8(out, input.rollCount, "Rollcount");
			addMapCbor_uint8(out, input.SNR, "SNR");
			addMapCbor_float(out, input.vrel, "Vrel");
            break;
		default :
			break;
    }
	
    return out;
}

/// @brief Deserialize a TargetInfo object using cbor.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int cbor_deserialize_TargetInfo(struct cbor_item_t* input, TargetInfo* output, enum option opt)
{
    struct cbor_pair *cborPairTmp;

    switch (opt) {
    case ARRAY:
        output->azimuth = cbor_get_uint16(cbor_move(cbor_array_get(input, 0)));
        output->index = cbor_get_uint8(cbor_move(cbor_array_get(input, 1)));
        output->range = cbor_float_get_float2(cbor_move(cbor_array_get(input, 2)));
        output->rcs = cbor_float_get_float2(cbor_move(cbor_array_get(input, 3)));
        output->rollCount = cbor_get_uint8(cbor_move(cbor_array_get(input, 4)));
        output->SNR = cbor_get_uint8(cbor_move(cbor_array_get(input, 5)));
        output->vrel = cbor_float_get_float2(cbor_move(cbor_array_get(input, 6)));
        break;
    
    case MAP:
        cborPairTmp = cbor_map_handle(input);
        output->azimuth = cbor_get_uint16(cborPairTmp[0].value);
        output->index = cbor_get_uint8(cborPairTmp[1].value);
        output->range = cbor_float_get_float2(cborPairTmp[2].value);
        output->rcs = cbor_float_get_float2(cborPairTmp[3].value);
        output->rollCount = cbor_get_uint8(cborPairTmp[4].value);
        output->SNR = cbor_get_uint8(cborPairTmp[5].value);
        output->vrel = cbor_float_get_float2(cborPairTmp[6].value);
        break;

	default :
		break;
    }
	return EXIT_SUCCESS;
}

/// @brief Serialize a SensorData object using cbor.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
struct cbor_item_t* cbor_serialize_SensorData(SensorData input, enum option opt)
{

	cbor_item_t* out;

	switch (opt) {
	case ARRAY :
		out = cbor_new_definite_array(5);
		cbor_array_set(out, 0, cbor_serialize_SensorVersion(input.version, opt));
		cbor_array_set(out, 1, cbor_serialize_SensorStatus(input.sStatus, opt));
		cbor_array_set(out, 2, cbor_serialize_TargetStatus(input.tStatus, opt));
		cbor_array_set(out, 3, cbor_serialize_TargetInfo(input.tInfo, opt));
		addArrayCbor_uint8(out, input.tInfoSize, 4);
		break;

	case MAP :
		out = cbor_new_definite_map(5);
		cbor_map_add(out, (struct cbor_pair) {
			.key = cbor_move(cbor_build_string("SensorVersion")),
			.value = cbor_move(cbor_serialize_SensorVersion(input.version, opt))
		});
		cbor_map_add(out, (struct cbor_pair) {
			.key = cbor_move(cbor_build_string("SensorStatus")),
			.value = cbor_move(cbor_serialize_SensorStatus(input.sStatus, opt))
		});
		cbor_map_add(out, (struct cbor_pair) {
			.key = cbor_move(cbor_build_string("TargetStatus")),
			.value = cbor_move(cbor_serialize_TargetStatus(input.tStatus, opt))
		});
		cbor_map_add(out, (struct cbor_pair) {
			.key = cbor_move(cbor_build_string("TargetInfo")),
			.value = cbor_move(cbor_serialize_TargetInfo(input.tInfo, opt))
		});
		addMapCbor_uint8(out, input.tInfoSize, "tInfoSize");
		break;

	default :
		break;
	}

	return out;
}

/// @brief Deserialize a SensorData object using cbor.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int cbor_deserialize_SensorData(struct cbor_item_t *input, SensorData* output, enum option opt)
{
    struct cbor_pair *temp;

    switch (opt) {
        case ARRAY : 
			if (cbor_deserialize_SensorVersion(cbor_move(cbor_array_get(input, 0)), &output->version, opt)) return EXIT_FAILURE;
			if (cbor_deserialize_SensorStatus(cbor_move(cbor_array_get(input, 1)), &output->sStatus, opt)) return EXIT_FAILURE;
			if (cbor_deserialize_TargetStatus(cbor_move(cbor_array_get(input, 2)), &output->tStatus, opt)) return EXIT_FAILURE;
			if (cbor_deserialize_TargetInfo(cbor_move(cbor_array_get(input, 3)), &output->tInfo, opt)) return EXIT_FAILURE;
			output->tInfoSize = cbor_get_uint8(cbor_move(cbor_array_get(input, 4)));
            break;

        case MAP :
            temp = cbor_map_handle(input);
			if (cbor_deserialize_SensorVersion(temp[0].value, &output->version, opt)) return EXIT_FAILURE;
			if (cbor_deserialize_SensorStatus(temp[1].value, &output->sStatus, opt)) return EXIT_FAILURE;
			if (cbor_deserialize_TargetStatus(temp[2].value, &output->tStatus, opt)) return EXIT_FAILURE;
			if (cbor_deserialize_TargetInfo(temp[3].value, &output->tInfo, opt)) return EXIT_FAILURE;
			output->tInfoSize = cbor_get_uint8(temp[4].value);
        	break;

		default :
			break;
    }
	return EXIT_SUCCESS;
}

// ---------- Serializer interface --------------------------------------------

/// @brief Initialize a context for JSON serialisation
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int cborc_init(void* ctx)
{
	return EXIT_SUCCESS;
}

/// @brief Cleanup a context of JSON serialisation.
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int cborc_cleanup(void* ctx)
{
	// Nothing to do
	return EXIT_SUCCESS;
}

/// @brief Print a string representation of the serialized data to the console.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to print.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int cborc_print(void* ctx, void* data)
{
	if (!data) return EXIT_FAILURE;
	//printf("%s", json_object_to_json_string_ext((struct json_object*)data, JSON_C_TO_STRING_PRETTY));
	return EXIT_SUCCESS;
}

/// @brief Free the allocated serialized object.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to free.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int cborc_freeobject(void* ctx, void* data)
{		
	if (data) cbor_intermediate_decref((struct cbor_item_t *)data);
	return EXIT_SUCCESS;
}

/// @brief Serialize an object.
/// @param[in] ctx Context object.
/// @param[in] input Object to serialize.
/// @param[out] output Serialization result if any.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int cborc_serialize(void* ctx, SensorData input, void** output)
{
	if (!output) return EXIT_FAILURE;
	enum option opt = *((int *)ctx);
	*output = (cbor_serialize_SensorData(input, opt));	
	return EXIT_SUCCESS;
}


/// @brief Deserialize an object.
/// @param[in] ctx Context object.
/// @param[in] input Data to deserialize.
/// @param[out] output Deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int cborc_deserialize(void* ctx, void* input, SensorData* output)
{
	if (!input || !output) return EXIT_FAILURE;
	enum option opt = *((int *)ctx);
	return cbor_deserialize_SensorData((struct cbor_item_t *)input, output, opt);
}

/// @brief Get the cbor serializer.
/// @param[out] s The serializer.
void cborc_get_serializer(Serializer* s)
{
	if (s)
	{
		s->context		= NULL;
		s->init			= cborc_init;
		s->cleanup		= cborc_cleanup;
		s->print		= cborc_print;
		s->freeobject	= cborc_freeobject;
		s->serialize	= cborc_serialize;
		s->deserialize	= cborc_deserialize;
	}
}
