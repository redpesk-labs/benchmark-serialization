#include "parse-jsonstring.h"
#include "mjson.h"

struct sensorversion_t {
    unsigned int dataType;
    bool result;
    unsigned int master;
    unsigned int second;
    unsigned int step;
};
struct sensorstatus_t {
    unsigned int actl_mode;
    unsigned int rollcount;
    unsigned int cfgStatus;
};
struct targetstatus_t {
    unsigned int noOfTarget;
    unsigned int rollcount;
};
struct targetinfo_t {
    unsigned int index;
    double rcs;
    double range;
    int azimuth;
    double vrel;
    unsigned int rollcount;
    int snr;
};

struct sensordata_t {
    struct sensorversion_t sv[1];
    struct sensorstatus_t ss[1];
    struct targetstatus_t ts[1];
    struct targetinfo_t ti[1];
    unsigned int tis;
};

static struct sensordata_t sd;

/// @brief Serialize a SensorVersion object using json representation.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
char* jsonstring_serialize_SensorVersion(SensorVersion input)
{
    char *output;
    int elements = 5;
    output = malloc(sizeof(input) + elements*SIZE_KEY_MAX);
    sprintf(output, "{\"dataType\":%u,\"result\":%d,\"master\":%u,\"second\":%u,\"step\":%u}",
                     input.dataType, input.result, input.master, input.second, input.step);
    return output;
}

/// @brief Deserialize a SensorVersion object using json representation.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
const struct json_attr_t jsonstring_deserialize_SensorVersion(SensorVersion* output)
{
    unsigned int dt_temp = 0;
    int temp;
    bool r_temp;
    unsigned int m_temp;
    unsigned int se_temp;
    unsigned int st_temp;
    const struct json_attr_t sensorversion_attrs[] = {
        {"dataType",    t_uinteger,     STRUCTOBJECT(SensorVersion, dataType)},
        {"result",      t_boolean,      STRUCTOBJECT(SensorVersion, result)},
        {"master",      t_uinteger,     STRUCTOBJECT(SensorVersion, master)},
        {"second",      t_uinteger,     STRUCTOBJECT(SensorVersion, second)},
        {"step",        t_uinteger,     STRUCTOBJECT(SensorVersion, step)},
        {NULL},
    };

    return(struct json_attr_t) {"sensorversion",   t_array,     STRUCTARRAY(output,
					                                            sensorversion_attrs,
					                                            &temp)};
} 

/// @brief Serialize a SensorStatus object using json representation.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
char* jsonstring_serialize_SensorStatus(SensorStatus input)
{
    char *output;
    int elements = 3;
    output = malloc(sizeof(input)+ elements*SIZE_KEY_MAX);
    sprintf(output, "{\"actlMode\":%u,\"rollcount\":%u,\"cfgStatus\":%u}",
                    input.actl_mode, input.rollcount, input.cfgStatus);
    return output;
}

/// @brief Serialize a TargetStatus object using json representation.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
char* jsonstring_serialize_TargetStatus(TargetStatus input)
{
    char *output;
    int elements = 2;
    output = malloc(sizeof(input)+ elements*SIZE_KEY_MAX);
    sprintf(output, "{\"noOfTarget\":%u,\"rollcount\":%u}",
                     input.noOfTarget, input.rollcount);
    return output;
}

/// @brief Serialize a TargetInfo object using json representation.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
char* jsonstring_serialize_TargetInfo(TargetInfo input)
{
    char *output;
    int elements = 7;
    output = malloc(sizeof(input)+ elements*SIZE_KEY_MAX);
    sprintf(output, "{\"index\":%u,\"rcs\":%f,\"range\":%f,\"azimuth\":%i,\"vrel\":%f,\"rollcount\":%u,\"snr\":%i}",
                     input.index, input.rcs, input.range, input.azimuth, input.vrel, input.rollCount, input.SNR);
    return output;
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
    sprintf(output, "{\"sensorversion\":[%s],\"sensorstatus\":[%s],\"targetstatus\":[%s],\"targetinfo\":[%s],\"targetinfosize\":%u}",
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

/// @brief Cast a sensorversion_t to a SensorVersion.
/// @param[in] input The object to cast.
/// @param[out] output Thecasted object.
void jsonstring_cast_SensorVersion(SensorVersion* output, struct sensorversion_t input)
{
    output->dataType = (uint8_t)input.dataType;
    output->result = (bool)input.result;
    output->master = (uint8_t)input.master;
    output->second = (uint8_t)input.second;
    output->step = (uint8_t)input.step;
}

/// @brief Cast a sensorstatus_t to a SensorStatus.
/// @param[in] input The object to cast.
/// @param[out] output Thecasted object.
void jsonstring_cast_SensorStatus(SensorStatus* output, struct sensorstatus_t input)
{
    output->actl_mode = (uint8_t)input.actl_mode;
    output->cfgStatus = (uint8_t)input.cfgStatus;
    output->rollcount = (uint8_t)input.rollcount;
}

/// @brief Cast a targetstatus_t to a TargetStatus.
/// @param[in] input The object to cast.
/// @param[out] output Thecasted object.
void jsonstring_cast_TargetStatus(TargetStatus* output, struct targetstatus_t input)
{
    output->noOfTarget = (uint8_t)input.noOfTarget;
    output->rollcount = (uint8_t)input.rollcount;
}

/// @brief Cast a targetinfo_t to a TargetInfo.
/// @param[in] input The object to cast.
/// @param[out] output Thecasted object.
void jsonstring_cast_TargetInfo(TargetInfo* output, struct targetinfo_t input)
{
    output->index = (uint8_t)input.index;
    output->rcs = (float)input.rcs;
    output->range = (float)input.range;
    output->azimuth = (int16_t)input.azimuth;
    output->vrel = (float)input.vrel;
    output->rollCount = (uint8_t)input.rollcount;
    output->SNR = (int8_t)input.snr;
}

/// @brief Cast a sensordata_t to a SensorData.
/// @param[in] input The object to cast.
/// @param[out] output Thecasted object.
void jsonstring_cast_SensorData(SensorData* output, struct sensordata_t input)
{
    jsonstring_cast_SensorVersion(&output->version, input.sv[0]);
    jsonstring_cast_SensorStatus(&output->sStatus, input.ss[0]);
    jsonstring_cast_TargetStatus(&output->tStatus, input.ts[0]);
    jsonstring_cast_TargetInfo(&output->tInfo, input.ti[0]);
    output->tInfoSize = (uint8_t)input.tis;
}
/// @brief Deserialize a SensorData object using json representation.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int jsonstring_deserialize_SensorData(char* input, SensorData* output)
{   
    int status;
    unsigned int temp;
    int count;

    const struct json_attr_t sensorversion_attrs[] = {
        {"dataType",    t_uinteger,     STRUCTOBJECT(struct sensorversion_t, dataType)},
        {"result",      t_uinteger,      STRUCTOBJECT(struct sensorversion_t, result)},
        {"master",      t_uinteger,     STRUCTOBJECT(struct sensorversion_t, master)},
        {"second",      t_uinteger,     STRUCTOBJECT(struct sensorversion_t, second)},
        {"step",        t_uinteger,     STRUCTOBJECT(struct sensorversion_t, step)},
        {NULL},
    };
    const struct json_attr_t sensorstatus_attrs[] = {
        {"actlMode",    t_uinteger,     STRUCTOBJECT(struct sensorstatus_t, actl_mode)},
        {"rollcount",   t_uinteger,     STRUCTOBJECT(struct sensorstatus_t, rollcount)},
        {"cfgStatus",   t_uinteger,     STRUCTOBJECT(struct sensorstatus_t, cfgStatus)},
        {NULL},
    };
    const struct json_attr_t targetstatus_attrs[] = {
        {"noOfTarget",      t_uinteger,  STRUCTOBJECT(struct targetstatus_t, noOfTarget)},
        {"rollcount",    t_uinteger,     STRUCTOBJECT(struct targetstatus_t, rollcount)},
        {NULL},
    };
    const struct json_attr_t targetinfo_attrs[] = {
        {"index",       t_uinteger,     STRUCTOBJECT(struct targetinfo_t, index)},
        {"rcs",         t_real,         STRUCTOBJECT(struct targetinfo_t, rcs)},
        {"range",       t_real,         STRUCTOBJECT(struct targetinfo_t, range)},
        {"azimuth",     t_integer,      STRUCTOBJECT(struct targetinfo_t, azimuth)},
        {"vrel",        t_real,         STRUCTOBJECT(struct targetinfo_t, vrel)},
        {"rollcount",   t_uinteger,     STRUCTOBJECT(struct targetinfo_t, rollcount)},
        {"snr",         t_integer,      STRUCTOBJECT(struct targetinfo_t, snr)}, 
        {NULL},
    };

    const struct json_attr_t sensordata_attrs[] = {
        {"sensorversion",   t_array,     STRUCTARRAY(sd.sv,
					                                sensorversion_attrs,
					                                &count)},
        {"sensorstatus",   t_array,     STRUCTARRAY(sd.ss,
					                                sensorstatus_attrs,
					                                &count)},
        {"targetstatus",   t_array,     STRUCTARRAY(sd.ts,
					                                targetstatus_attrs,
					                                &count)},
        {"targetinfo",   t_array,     STRUCTARRAY(sd.ti,
					                                targetinfo_attrs,
					                                &count)},
        {"targetinfosize",  t_uinteger,     .addr.integer = &sd.tis}, 
        {NULL},
    };

    memset(&sd, '\0', sizeof(sd));
    status = json_read_object(input, sensordata_attrs, NULL);
    jsonstring_cast_SensorData(output, sd);

    if (status != 0)
        puts(json_error_string(status));

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