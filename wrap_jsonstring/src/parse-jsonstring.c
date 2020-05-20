#include "parse-jsonstring.h"
#include "mjson.h"

/// @brief Serialize a SensorVersion object using json representation.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
char* jsonstring_serialize_SensorVersion(SensorVersion input)
{
    char *output;
    int elements = 5;
    output = malloc(sizeof(input)+ elements*SIZE_KEY_MAX);
    sprintf(output, "{ \"dataType\" :%u, \"result\" :%d,\"master\" :%u, \"second\" :%u, \"step\" :%u}",
                     input.dataType, input.result, input.master, input.second, input.step);
    return output;
}

/// @brief Deserialize a SensorVersion object using json representation.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int jsonstring_deserialize_SensorVersion(const char* input, int length, SensorVersion* output)
{
    double temp = 0;

    if (!mjson_get_number(input, length, "$.dataType", &temp)) return EXIT_FAILURE;
    output->dataType = (uint8_t) temp;

    if (!mjson_get_number(input, length, "$.result", &temp)) return EXIT_FAILURE;
    output->result = (bool) temp;

    if (!mjson_get_number(input, length, "$.master", &temp)) return EXIT_FAILURE;
    output->master = (uint8_t) temp;

    if (!mjson_get_number(input, length, "$.second", &temp)) return EXIT_FAILURE;
    output->second = (uint8_t) temp;

    if (!mjson_get_number(input, length, "$.step", &temp)) return EXIT_FAILURE;
    output->step = (uint8_t) temp;

    return EXIT_SUCCESS;

}

/// @brief Serialize a SensorStatus object using json representation.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
char* jsonstring_serialize_SensorStatus(SensorStatus input)
{
    char *output;
    int elements = 3;
    output = malloc(sizeof(input)+ elements*SIZE_KEY_MAX);
    sprintf(output, "{ \"actlMode\" :%u, \"rollcount\" :%u, \"cfgStatus\" :%u}",
                    input.actl_mode, input.rollcount, input.cfgStatus);
    return output;
}

/// @brief Deserialize a SensorStatus object using json representation.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int jsonstring_deserialize_SensorStatus(const char* input, int length, SensorStatus* output)
{
    double temp = 0;

    if (!mjson_get_number(input, length, "$.actlMode", &temp)) return EXIT_FAILURE;
    output->actl_mode = (uint8_t) temp;

    if (!mjson_get_number(input, length, "$.rollcount", &temp)) return EXIT_FAILURE;
    output->rollcount = (uint8_t) temp;

    if (!mjson_get_number(input, length, "$.cfgStatus", &temp)) return EXIT_FAILURE;
    output->cfgStatus = (uint8_t) temp;

    return EXIT_SUCCESS;

}

/// @brief Serialize a TargetStatus object using json representation.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
char* jsonstring_serialize_TargetStatus(TargetStatus input)
{
    char *output;
    int elements = 2;
    output = malloc(sizeof(input)+ elements*SIZE_KEY_MAX);
    sprintf(output, "{ \"noOfTarget\" :%u, \"rollcount\" :%u}",
                     input.noOfTarget, input.rollcount);
    return output;
}

/// @brief Deserialize a TargetStatus object using json representation.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int jsonstring_deserialize_TargetStatus(const char* input, int length, TargetStatus* output)
{
    double temp = 0;

    if (!mjson_get_number(input, length, "$.noOfTarget", &temp)) return EXIT_FAILURE;
    output->noOfTarget = (uint8_t) temp;

    if (!mjson_get_number(input, length, "$.rollcount", &temp)) return EXIT_FAILURE;
    output->rollcount = (uint8_t) temp;

    return EXIT_SUCCESS;

}

/// @brief Serialize a TargetInfo object using json representation.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
char* jsonstring_serialize_TargetInfo(TargetInfo input)
{
    char *output;
    int elements = 7;
    output = malloc(sizeof(input)+ elements*SIZE_KEY_MAX);
    sprintf(output, "{ \"index\" :%u, \"rcs\" :%f,\"range\" :%f, \"azimuth\" :%i, \"vrel\" :%f, \"rollcount\" :%u, \"snr\" :%i}",
                     input.index, input.rcs, input.range, input.azimuth, input.vrel, input.rollCount, input.SNR);
    return output;
}

/// @brief Deserialize a TargetInfo object using json representation.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int jsonstring_deserialize_TargetInfo(const char* input, int length, TargetInfo* output)
{
    double temp = 0;

    if (!mjson_get_number(input, length, "$.index", &temp)) return EXIT_FAILURE;
    output->index = (uint8_t) temp;

    if (!mjson_get_number(input, length, "$.rcs", &temp)) return EXIT_FAILURE;
    output->rcs = (float) temp;

    if (!mjson_get_number(input, length, "$.range", &temp)) return EXIT_FAILURE;
    output->range = (float) temp;

    if (!mjson_get_number(input, length, "$.azimuth", &temp)) return EXIT_FAILURE;
    output->azimuth = (int16_t) temp;

    if (!mjson_get_number(input, length, "$.vrel", &temp)) return EXIT_FAILURE;
    output->vrel = (float) temp;

    if (!mjson_get_number(input, length, "$.rollcount", &temp)) return EXIT_FAILURE;
    output->rollCount = (uint8_t) temp;

    if (!mjson_get_number(input, length, "$.snr", &temp)) return EXIT_FAILURE;
    output->SNR = (uint8_t) temp;

    return EXIT_SUCCESS;

}

/// @brief Serialize a SensorData object using json representation.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
char* jsonstring_serialize_SensorData(SensorData input)
{
    int elements = 5;
    char* sVersion_element = jsonstring_serialize_SensorVersion(input.version);
    char* sStatus_element = jsonstring_serialize_SensorStatus(input.sStatus);
    char* tStatus_element = jsonstring_serialize_TargetStatus(input.tStatus);
    char* tInfo_element = jsonstring_serialize_TargetInfo(input.tInfo);
    char* output;
    output = malloc(sizeof(&sVersion_element) + sizeof(&sStatus_element) + sizeof(&tStatus_element) + sizeof(&tInfo_element) + elements*SIZE_KEY_MAX);
    sprintf(output, "{ \"sensorversion\" :%s, \"sensorstatus\" :%s, \"targetstatus\" :%s, \"targetinfo\" :%s, \"targetinfosize\" :%u}",
                    sVersion_element,
                    sStatus_element,
                    tStatus_element,
                    tInfo_element,
                    input.tInfoSize);
    free(sVersion_element);
    free(sStatus_element);
    free(tStatus_element);
    free(tInfo_element);
    return output;
}

/// @brief Deserialize a SensorData object using json representation.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int jsonstring_deserialize_SensorData(char* input, SensorData* output)
{   
    int length = 0;
    while(input[length] != '\0') length++;
    const char* p;
    int n;
    double temp;

    mjson_find(input, length,"$.sensorversion", &p, &n);
    if(jsonstring_deserialize_SensorVersion(p, n, &output->version)) return EXIT_FAILURE;

    mjson_find(input, length,"$.sensorstatus", &p, &n);
    if(jsonstring_deserialize_SensorStatus(p, n, &output->sStatus)) return EXIT_FAILURE;

    mjson_find(input, length,"$.targetstatus", &p, &n);
    if(jsonstring_deserialize_TargetStatus(p, n, &output->tStatus)) return EXIT_FAILURE;

    mjson_find(input, length,"$.targetinfo", &p, &n);
    if(jsonstring_deserialize_TargetInfo(p, n, &output->tInfo)) return EXIT_FAILURE;

    if (!mjson_get_number(input, length, "$.targetinfosize", &temp)) return EXIT_FAILURE;
    output->tInfoSize = (uint8_t) temp;
    return EXIT_SUCCESS;
}

// ---------- Serializer interface --------------------------------------------

/// @brief Initialize a context for cbor serialisation
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsonstring_init(void* ctx)
{

	return EXIT_SUCCESS;
}

/// @brief Cleanup a context of cbor serialisation.
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsonstring_cleanup(void* ctx)
{
	// Nothing to do
	return EXIT_SUCCESS;
}

/// @brief Print a string representation of the serialized data to the console.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to print.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsonstring_print(void* ctx, void* data)
{
	if (!data) return EXIT_FAILURE;
    printf("%s\n", (char* )data);
	return EXIT_SUCCESS;
}

/// @brief Free the allocated serialized object.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to free.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsonstring_freeobject(void* ctx, void* data)
{		
	if (data) {
        free(data);
    }
	return EXIT_SUCCESS;
}

/// @brief Serialize an object.
/// @param[in] ctx Context object.
/// @param[in] input Object to serialize.
/// @param[out] output Serialization result if any.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsonstring_serialize(void* ctx, SensorData input, void** output)
{
	if (!output) return EXIT_FAILURE;
	*output = jsonstring_serialize_SensorData(input);	
	return EXIT_SUCCESS;
}


/// @brief Deserialize an object.
/// @param[in] ctx Context object.
/// @param[in] input Data to deserialize.
/// @param[out] output Deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsonstring_deserialize(void* ctx, void* input, SensorData* output)
{
	if (!input || !output) return EXIT_FAILURE;
	return jsonstring_deserialize_SensorData((char *)input, output);
}

/// @brief Get the jsonstring serializer.
/// @param[out] s The serializer.
void jsonstring_get_serializer(Serializer* s)
{
	if (s)
	{
		s->context		= NULL;
		s->init			= jsonstring_init;
		s->cleanup		= jsonstring_cleanup;
		s->print		= jsonstring_print;
		s->freeobject	= jsonstring_freeobject;
		s->serialize	= jsonstring_serialize;
		s->deserialize	= jsonstring_deserialize;
	}
}