#include "parse-protobuf.h"


 int protobuf_serialize_sensorVersion(SensorVersion input, SensorVersionMessage* output)
{
    output->datatype = input.dataType;
    output->result = input.result;
    output->master = input.master;
    output->second = input.second;
    output->step = input.step;
    return 0;
}

int protobuf_deserialize_sensorVersion(SensorVersionMessage *input, SensorVersion *output)
{
    output->dataType = input->datatype;
    output->result = input->result;
    output->master = input->master;
    output->second = input->second;
    output->step = input->step;
    return 0;
}

int protobuf_serialize_sensorStatus(SensorStatus input, SensorStatusMessage* ouput)
{
    ouput->actl_mode = input.actl_mode;
    ouput->rollcount = input.rollcount;
    ouput->cfgstatus = input.cfgStatus;
    return 0;
}

int protobuf_deserialize_sensorStatus(SensorStatusMessage *input, SensorStatus* output)
{
    output->actl_mode = input->actl_mode;
    output->rollcount = input->rollcount;
    output->cfgStatus = input->cfgstatus;
    return 0;
}

int protobuf_serialize_targetStatus(TargetStatus input, TargetStatusMessage* output)
{
    output->nooftarget = input.noOfTarget;
    output->rollcount = input.rollcount;
    return 0;
}

int protobuf_deserialize_targetStatus(TargetStatusMessage *input, TargetStatus *output)
{
    output->noOfTarget = input->nooftarget;
    output->rollcount = input->rollcount;
    return 0;
}

int protobuf_serialize_targetInfo(TargetInfo input, TargetInfoMessage* output)
{
    output->index = input.index;
    output->rcs = input.rcs;
    output->range = input.range;
    output->azimuth = input.azimuth;
    output->vrel = input.vrel;
    output->rollcount = input.rollCount;
    output->snr = input.SNR;
    return 0;
}

int protobuf_deserialize_targetInfo(TargetInfoMessage *msg, TargetInfo *targetInfo)
{
    targetInfo->index = msg->index;
    targetInfo->rcs = msg->rcs;
    targetInfo->range = msg->range;
    targetInfo->azimuth = msg->azimuth;
    targetInfo->vrel = msg->vrel;
    targetInfo->rollCount = msg->rollcount;
    targetInfo->SNR = msg->snr;
}

size_t protobuf_serialize_sensorData(SensorData sensorData, uint8_t *buf)
{
    size_t length;           // Length of serialized data

    SensorDataMessage msg = SENSOR_DATA_MESSAGE__INIT;
    SensorVersionMessage msg_sversion = SENSOR_VERSION_MESSAGE__INIT;
    SensorStatusMessage msg_sstatus = SENSOR_STATUS_MESSAGE__INIT;
    TargetStatusMessage msg_tstatus = TARGET_STATUS_MESSAGE__INIT;
    TargetInfoMessage msg_tinfo = TARGET_INFO_MESSAGE__INIT;

    protobuf_serialize_sensorVersion(sensorData.version, &msg_sversion);
    protobuf_serialize_sensorStatus(sensorData.sStatus, &msg_sstatus);
    protobuf_serialize_targetStatus(sensorData.tStatus, &msg_tstatus);
    protobuf_serialize_targetInfo(sensorData.tInfo, &msg_tinfo);

    msg.sensorversion = &msg_sversion;
    msg.sensorstatus = &msg_sstatus;
    msg.targetstatus = &msg_tstatus;
    msg.targetinfo = &msg_tinfo;
    msg.tinfosize = (uint32_t)sensorData.tInfoSize;


    length = sensor_data_message__get_packed_size(&msg);
    sensor_data_message__pack(&msg, buf);
    return length;

}

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

    return 0;
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
int protobuf_serialize(void* ctx, SensorData input, void** output)
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