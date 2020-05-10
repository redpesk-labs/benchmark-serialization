#include "parse-protobuf.h"

size_t parse_to_protobuf(SensorData *sensorData, uint8_t *buf)
{
    //void *buf;             // Buffer to store serialized data
    size_t length;           // Length of serialized data

    SensorDataMessage msg = SENSOR_DATA_MESSAGE__INIT;
    SensorVersionMessage msg_sversion = SENSOR_VERSION_MESSAGE__INIT;
    SensorStatusMessage msg_sstatus = SENSOR_STATUS_MESSAGE__INIT;
    TargetStatusMessage msg_tstatus = TARGET_STATUS_MESSAGE__INIT;
    TargetInfoMessage msg_tinfo = TARGET_INFO_MESSAGE__INIT;

    sensorVersionToProtobuf(&sensorData->version,&msg_sversion);
    sensorStatusToProtobuf(&sensorData->sStatus, &msg_sstatus);
    targetStatusToProtobuf(&sensorData->tStatus, &msg_tstatus);
    targetInfoToProtobuf(&sensorData->tInfo, &msg_tinfo);

    msg.sensorversion = &msg_sversion;
    msg.sensorstatus = &msg_sstatus;
    msg.targetstatus = &msg_tstatus;
    msg.targetinfo = &msg_tinfo;


    length = sensor_data_message__get_packed_size(&msg);
    //buf = malloc(length);
    sensor_data_message__pack(&msg, buf);
    return length;

}

void protobuf_to_sensorData(uint8_t *buf, SensorData *sensorData, size_t length)
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
    SensorStatus sensorStatusTemp;
    TargetStatus targetStatusTemp;
    TargetInfo targetInfoTemp;

    protobufToSensorVersion(msg_sversion, &sensorVersionTemp);
    protobufToSensorStatus(msg_sstatus, &sensorStatusTemp);
    protobufToTargetStatus(msg_tstatus, &targetStatusTemp);
    protobufToTargetInfo(msg_tinfo, &targetInfoTemp);

    sensorData->version = sensorVersionTemp;
    sensorData->sStatus = sensorStatusTemp;
    sensorData->tStatus = targetStatusTemp;
    sensorData->tInfo = targetInfoTemp;

    sensor_data_message__free_unpacked(msg,NULL);
}

void sensorVersionToProtobuf(SensorVersion *sensorVersion, SensorVersionMessage *msg)
{
    msg->datatype = sensorVersion->dataType;
    msg->result = sensorVersion->result;
    msg->master = sensorVersion->master;
    msg->second = sensorVersion->second;
    msg->step = sensorVersion->step;
}

void protobufToSensorVersion(SensorVersionMessage *msg, SensorVersion *sensorVersion)
{
    sensorVersion->dataType = msg->datatype;
    sensorVersion->result = msg->result;
    sensorVersion->master = msg->master;
    sensorVersion->second = msg->second;
    sensorVersion->step = msg->step;
}

void sensorStatusToProtobuf(SensorStatus *sensorStatus, SensorStatusMessage *msg)
{
    msg->actl_mode = sensorStatus->actl_mode;
    msg->rollcount = sensorStatus->rollcount;
    msg->cfgstatus = sensorStatus->cfgStatus;
}

void protobufToSensorStatus(SensorStatusMessage *msg, SensorStatus *sensorStatus)
{
    sensorStatus->actl_mode = msg->actl_mode;
    sensorStatus->rollcount = msg->rollcount;
    sensorStatus->cfgStatus = msg->cfgstatus;
}

void targetStatusToProtobuf(TargetStatus *targetStatus, TargetStatusMessage *msg)
{
    msg->nooftarget = targetStatus->noOfTarget;
    msg->rollcount = targetStatus->rollcount;
}

void protobufToTargetStatus(TargetStatusMessage *msg, TargetStatus *targetStatus)
{
    targetStatus->noOfTarget = msg->nooftarget;
    targetStatus->rollcount = msg->rollcount;
}

void targetInfoToProtobuf(TargetInfo *targetInfo, TargetInfoMessage *msg)
{
    msg->index = targetInfo->index;
    msg->rcs = targetInfo->rcs;
    msg->range = targetInfo->range;
    msg->azimuth = targetInfo->azimuth;
    msg->vrel = targetInfo->vrel;
    msg->rollcount = targetInfo->rollCount;
    msg->snr = targetInfo->SNR;
}

void protobufToTargetInfo(TargetInfoMessage *msg, TargetInfo *targetInfo)
{
    targetInfo->index = msg->index;
    targetInfo->rcs = msg->rcs;
    targetInfo->range = msg->range;
    targetInfo->azimuth = msg->azimuth;
    targetInfo->vrel = msg->vrel;
    targetInfo->rollCount = msg->rollcount;
    targetInfo->SNR = msg->snr;
}

