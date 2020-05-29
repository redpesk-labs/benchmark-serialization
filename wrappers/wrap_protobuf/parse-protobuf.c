#include "parse-protobuf.h"

/// @brief Serialize a SensorVersion object using protobuf.
/// @param[in] input The object to serialize.
/// @param[out] output The serialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
 int protobuf_serialize_sensorVersion(SensorVersion* input, SensorVersionMessage* output)
{
    output->datatype = input->dataType;
    output->result = input->result;
    output->master = input->master;
    output->second = input->second;
    output->step = input->step;
    return EXIT_SUCCESS;
}

/// @brief Deserialize a SensorVersion object using protobuf.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int protobuf_deserialize_sensorVersion(SensorVersionMessage* input, SensorVersion *output)
{
    output->dataType = input->datatype;
    output->result = input->result;
    output->master = input->master;
    output->second = input->second;
    output->step = input->step;
    return EXIT_SUCCESS;
}

/// @brief Serialize a SensorStatus object using protobuf.
/// @param[in] input The object to serialize.
/// @param[out] output The serialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int protobuf_serialize_sensorStatus(SensorStatus* input, SensorStatusMessage* ouput)
{
    ouput->actl_mode = input->actl_mode;
    ouput->rollcount = input->rollcount;
    ouput->cfgstatus = input->cfgStatus;
    return EXIT_SUCCESS;
}

/// @brief Deserialize a SensorStatus object using protobuf.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int protobuf_deserialize_sensorStatus(SensorStatusMessage* input, SensorStatus* output)
{
    output->actl_mode = input->actl_mode;
    output->rollcount = input->rollcount;
    output->cfgStatus = input->cfgstatus;
    return EXIT_SUCCESS;
}

/// @brief Serialize a TargetStatus object using protobuf.
/// @param[in] input The object to serialize.
/// @param[out] output The serialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int protobuf_serialize_targetStatus(TargetStatus* input, TargetStatusMessage* output)
{
    output->nooftarget = input->noOfTarget;
    output->rollcount = input->rollcount;
    return EXIT_SUCCESS;
}

/// @brief Deserialize a TargetStatus object using protobuf.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int protobuf_deserialize_targetStatus(TargetStatusMessage* input, TargetStatus *output)
{
    output->noOfTarget = input->nooftarget;
    output->rollcount = input->rollcount;
    return EXIT_SUCCESS;
}

/// @brief Serialize a TargetInfo object using protobuf.
/// @param[in] input The object to serialize.
/// @param[out] output The serialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int protobuf_serialize_targetInfo(TargetInfo* input, TargetInfoMessage* output)
{
    output->index = input->index;
    output->rcs = input->rcs;
    output->range = input->range;
    output->azimuth = input->azimuth;
    output->vrel = input->vrel;
    output->rollcount = input->rollCount;
    output->snr = input->SNR;
    return EXIT_SUCCESS;
}

/// @brief Deserialize a TargetInfo object using protobuf.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int protobuf_deserialize_targetInfo(TargetInfoMessage* input, TargetInfo *output)
{
    output->index = input->index;
    output->rcs = input->rcs;
    output->range = input->range;
    output->azimuth = input->azimuth;
    output->vrel = input->vrel;
    output->rollCount = input->rollcount;
    output->SNR = input->snr;
}

/// @brief Serialize a SensorData object using protobuf.
/// @param[in] input The object to serialize.
/// @param[out] output The serialized object.
/// @return length The length of the serialized data
size_t protobuf_serialize_sensorData(SensorData* input, uint8_t *output)
{
    size_t length;

    SensorDataMessage msg = SENSOR_DATA_MESSAGE__INIT;
    SensorVersionMessage msg_sversion = SENSOR_VERSION_MESSAGE__INIT;
    SensorStatusMessage msg_sstatus = SENSOR_STATUS_MESSAGE__INIT;
    TargetStatusMessage msg_tstatus = TARGET_STATUS_MESSAGE__INIT;
    TargetInfoMessage msg_tinfo = TARGET_INFO_MESSAGE__INIT;

    protobuf_serialize_sensorVersion(&input->version, &msg_sversion);
    protobuf_serialize_sensorStatus(&input->sStatus, &msg_sstatus);
    protobuf_serialize_targetStatus(&input->tStatus, &msg_tstatus);
    protobuf_serialize_targetInfo(&input->tInfo, &msg_tinfo);

    msg.sensorversion = &msg_sversion;
    msg.sensorstatus = &msg_sstatus;
    msg.targetstatus = &msg_tstatus;
    msg.targetinfo = &msg_tinfo;
    msg.tinfosize = (uint32_t)input->tInfoSize;


    length = sensor_data_message__get_packed_size(&msg);
    sensor_data_message__pack(&msg, output);
    return length;

}

/// @brief Deserialize a SensorData object using protobuf.
/// @param[in] buf The object to deserialize.
/// @param[in] length The length of the object to deserialize
/// @param[out] senorData The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int protobuf_deserialize_sensorData(uint8_t *buf, SensorData *sensorData, size_t length)
{
    SensorDataMessage *msg;
    msg = sensor_data_message__unpack(NULL,length,buf);

    protobuf_deserialize_sensorVersion(msg->sensorversion, &sensorData->version);
    protobuf_deserialize_sensorStatus(msg->sensorstatus, &sensorData->sStatus);
    protobuf_deserialize_targetStatus(msg->targetstatus, &sensorData->tStatus);
    protobuf_deserialize_targetInfo(msg->targetinfo, &sensorData->tInfo);
    sensorData->tInfoSize = (uint8_t)msg->tinfosize;

    sensor_data_message__free_unpacked(msg,NULL);

    return EXIT_SUCCESS;
}

// ---------- Serializer interface --------------------------------------------

/// @brief Initialize a context for protobuf serialisation
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int protobuf_init(void* ctx)
{
	return EXIT_SUCCESS;
}

/// @brief Cleanup a context of protobuf serialisation.
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int protobuf_cleanup(void* ctx)
{
	// Nothing to do
	return EXIT_SUCCESS;
}

/// @brief Print a string representation of the serialized data to the console.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to print.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int protobuf_print(void* ctx, void* data)
{
	if (!data) return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

/// @brief Free the allocated serialized object.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to free.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int protobuf_freeobject(void* ctx, void* data)
{
	return EXIT_SUCCESS;
}

/// @brief Serialize an object.
/// @param[in] ctx Context object.
/// @param[in] input Object to serialize.
/// @param[out] output Serialization result if any.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int protobuf_serialize(void* ctx, SensorData* input, void** output)
{
	if (!output) return EXIT_FAILURE;
    int length;
    memset(&length, 0, sizeof(length));
	length = protobuf_serialize_sensorData(input, (uint8_t *)output);	
    *(size_t *)ctx = length;
	return EXIT_SUCCESS;
}


/// @brief Deserialize an object.
/// @param[in] ctx Context object.
/// @param[in] input Data to deserialize.
/// @param[out] output Deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int protobuf_deserialize(void* ctx, void* input, SensorData* output)
{
	if (!input || !output) return EXIT_FAILURE;
	return protobuf_deserialize_sensorData((uint8_t *)input, output, *(size_t *)ctx);
}

/// @brief Get the protobuf serializer.
/// @param[out] s The serializer.
void protobuf_get_serializer(Serializer* s)
{
	if (s)
	{
		s->context		= NULL;
		s->init			= protobuf_init;
		s->cleanup		= protobuf_cleanup;
		s->print		= protobuf_print;
		s->freeobject	= protobuf_freeobject;
		s->serialize	= protobuf_serialize;
		s->deserialize	= protobuf_deserialize;
	}
}