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

size_t protobuf_serialize_sensorData(SensorData *sensorData, uint8_t *buf)
{
    //void *buf;             // Buffer to store serialized data
    size_t length;           // Length of serialized data

    SensorDataMessage msg = SENSOR_DATA_MESSAGE__INIT;
    SensorVersionMessage msg_sversion = SENSOR_VERSION_MESSAGE__INIT;
    SensorStatusMessage msg_sstatus = SENSOR_STATUS_MESSAGE__INIT;
    TargetStatusMessage msg_tstatus = TARGET_STATUS_MESSAGE__INIT;
    TargetInfoMessage msg_tinfo = TARGET_INFO_MESSAGE__INIT;

    protobuf_serialize_sensorVersion(sensorData->version, &msg_sversion);
    protobuf_serialize_sensorStatus(sensorData->sStatus, &msg_sstatus);
    protobuf_serialize_targetStatus(sensorData->tStatus, &msg_tstatus);
    protobuf_serialize_targetInfo(sensorData->tInfo, &msg_tinfo);

    msg.sensorversion = &msg_sversion;
    msg.sensorstatus = &msg_sstatus;
    msg.targetstatus = &msg_tstatus;
    msg.targetinfo = &msg_tinfo;
    msg.tinfosize = (uint32_t)sensorData->tInfoSize;


    length = sensor_data_message__get_packed_size(&msg);
    //buf = malloc(length);
    sensor_data_message__pack(&msg, buf);
    return length;

}

void protobuf_deserialize_sensorData(uint8_t *buf, SensorData *sensorData, size_t length)
{
    SensorDataMessage *msg;
    SensorVersionMessage *msg_sversion;
    SensorStatusMessage *msg_sstatus;
    TargetStatusMessage *msg_tstatus;
    TargetInfoMessage *msg_tinfo; 

    msg = sensor_data_message__unpack(NULL,length,buf);
    msg_sversion = msg->sensorversion;
    msg_sstatus = msg->sensorstatus;
    msg_tstatus = msg->targetstatus;
    msg_tinfo = msg->targetinfo;

    SensorVersion sensorVersionTemp;
    memset(&sensorVersionTemp, 0, sizeof(SensorVersion));
    SensorStatus sensorStatusTemp;
    memset(&sensorStatusTemp, 0, sizeof(SensorStatus));
    TargetStatus targetStatusTemp;
    memset(&targetStatusTemp, 0, sizeof(TargetStatus));
    TargetInfo targetInfoTemp;
    memset(&targetInfoTemp, 0, sizeof(TargetInfo));

    protobuf_deserialize_sensorVersion(msg_sversion, &sensorVersionTemp);
    protobuf_deserialize_sensorStatus(msg_sstatus, &sensorStatusTemp);
    protobuf_deserialize_targetStatus(msg_tstatus, &targetStatusTemp);
    protobuf_deserialize_targetInfo(msg_tinfo, &targetInfoTemp);

    sensorData->version = sensorVersionTemp;
    sensorData->sStatus = sensorStatusTemp;
    sensorData->tStatus = targetStatusTemp;
    sensorData->tInfo = targetInfoTemp;
    sensorData->tInfoSize = (uint8_t)msg->tinfosize;

    sensor_data_message__free_unpacked(msg,NULL);
}