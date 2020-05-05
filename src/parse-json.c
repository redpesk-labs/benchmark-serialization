#include "parse-json.h"


void parse_to_json(SensorData *sensorData, json_object *my_json)
{  
    struct json_object *tempobject;
    struct json_object *SVobject;
    struct json_object *SSobject;
    struct json_object *TSobject;
    struct json_object *TIobject;
    //struct json_object *TIobjects=json_object_new_array();

    SVobject = SensorVersionToJsonObject(&sensorData->version);
    SSobject = SensorStatusToJsonObject(&sensorData->sStatus);
    TSobject = TargetStatusToJsonObject(&sensorData->tStatus);
    TIobject = TargetInfoToJsonObject(sensorData->tInfo);
    

    json_object_object_add(my_json, "SensorVersion", SVobject);
    json_object_object_add(my_json, "SensorStatus", SSobject);
    json_object_object_add(my_json, "TargetStatus", TSobject);
    json_object_object_add(my_json, "TargetInfos", TIobject);

}

void json_to_sensorData(json_object *my_json, SensorData *sensorData)
{
    SensorVersion sensor_version;
    SensorStatus sensor_status;
    TargetStatus target_status;
    TargetInfo *target_info;

    sensor_version = JsonObjectToSensorVersion(my_json);
    sensor_status = JsonObjectToSensorStatus(my_json);
    target_status = JsonObjectToTargetStatus(my_json);
    target_info = JsonObjectToTargetInfo(my_json);

    sensorData->version = sensor_version;
    sensorData->sStatus = sensor_status;
    sensorData->tStatus = target_status;
    sensorData->tInfo = target_info;
}

struct json_object *SensorVersionToJsonObject(SensorVersion * data)
{
    struct json_object *tempobject;
    struct json_object *dataType_json;
    struct json_object *result_json;
    struct json_object *master_json;
    struct json_object *second_json;
    struct json_object *step_json;

    tempobject = json_object_new_object();
    dataType_json = json_object_new_int(data->dataType);
    result_json = json_object_new_boolean(data->result);
    master_json = json_object_new_int(data->master);
    second_json = json_object_new_int(data->second);
    step_json = json_object_new_int(data->step);

    json_object_object_add(tempobject, "DataType", dataType_json);
    json_object_object_add(tempobject, "Result", result_json);
    json_object_object_add(tempobject, "Master", master_json);
    json_object_object_add(tempobject, "Second", second_json);
    json_object_object_add(tempobject, "Step", step_json);

    return tempobject;
}

SensorVersion JsonObjectToSensorVersion(struct json_object * data)
{
    SensorVersion tempobject;
    struct json_object *SVobject;
    struct json_object *dataType_json;
    struct json_object *result_json;
    struct json_object *master_json;
    struct json_object *second_json;
    struct json_object *step_json;
    SVobject = json_object_new_object();
    json_object_object_get_ex(data, "SensorVersion", &SVobject);
    json_object_object_get_ex(SVobject, "DataType", &dataType_json);
    json_object_object_get_ex(SVobject, "Result", &result_json);
    json_object_object_get_ex(SVobject, "Master", &master_json);
    json_object_object_get_ex(SVobject, "Second", &second_json);
    json_object_object_get_ex(SVobject, "Step", &step_json);
    tempobject.dataType = json_object_get_int(dataType_json);
    tempobject.result = json_object_get_boolean(result_json);
    tempobject.master = json_object_get_int(master_json);
    tempobject.second = json_object_get_int(second_json);
    tempobject.step = json_object_get_int(step_json);
    return tempobject;
}

struct json_object *SensorStatusToJsonObject(SensorStatus * data)
{
    struct json_object *tempobject;
    struct json_object *actlMode_json;
    struct json_object *rollCount_json;
    struct json_object *cfgStatus_json;

    tempobject = json_object_new_object();
    actlMode_json = json_object_new_int(data->actl_mode);
    rollCount_json = json_object_new_int(data->rollcount);
    cfgStatus_json = json_object_new_int(data->cfgStatus);

    json_object_object_add(tempobject, "Actl_mode", actlMode_json);
    json_object_object_add(tempobject, "Rollcount", rollCount_json);
    json_object_object_add(tempobject, "CfgStatus", cfgStatus_json);

    return tempobject;
}

SensorStatus JsonObjectToSensorStatus(struct json_object * data)
{
    SensorStatus tempobject;
    struct json_object *SSobject;
    struct json_object *actl_json;
    struct json_object *rollcount_json;
    struct json_object *cfg_json;
    SSobject = json_object_new_object();
    json_object_object_get_ex(data, "SensorStatus", &SSobject);
    json_object_object_get_ex(SSobject, "Actl_mode", &actl_json);
    json_object_object_get_ex(SSobject, "Rollcount", &rollcount_json);
    json_object_object_get_ex(SSobject, "CfgStatus", &cfg_json);
    tempobject.actl_mode = json_object_get_int(actl_json);
    tempobject.rollcount = json_object_get_int(rollcount_json);
    tempobject.cfgStatus = json_object_get_int(cfg_json);
    return tempobject;
}

struct json_object *TargetStatusToJsonObject(TargetStatus * data)
{
    struct json_object *tempobject;
    struct json_object *noOfTarget;
    struct json_object *rollcount;

    tempobject = json_object_new_object();
    noOfTarget = json_object_new_int(data->noOfTarget);
    rollcount = json_object_new_int(data->rollcount);

    json_object_object_add(tempobject, "NoOfTarget", noOfTarget);
    json_object_object_add(tempobject, "Rollcount", rollcount);

    return tempobject;
    
}

TargetStatus JsonObjectToTargetStatus(struct json_object * data)
{
    TargetStatus tempobject;
    struct json_object *TSobject;
    struct json_object *noOfTarget_json;
    struct json_object *rollcount_json;
    TSobject = json_object_new_object();
    json_object_object_get_ex(data, "TargetStatus", &TSobject);
    json_object_object_get_ex(TSobject, "NoOfTarget", &noOfTarget_json);
    json_object_object_get_ex(TSobject, "Rollcount", &rollcount_json);
    tempobject.noOfTarget = json_object_get_int(noOfTarget_json);
    tempobject.rollcount = json_object_get_int(rollcount_json);
    return tempobject;
}

struct json_object *TargetInfoToJsonObject(TargetInfo * data)
{
    struct json_object *tempobject;
    struct json_object *index_json;
    struct json_object *rcs_json;
    struct json_object *range_json;
    struct json_object *azimuth_json;
    struct json_object *vrel_json;
    struct json_object *rollcount_json;
    struct json_object *snr_json;

    tempobject = json_object_new_object();
    index_json = json_object_new_int(data->index);
    rcs_json = json_object_new_double(data->rcs);
    range_json = json_object_new_double(data->range);
    azimuth_json = json_object_new_int(data->azimuth);
    vrel_json = json_object_new_double(data->vrel);
    rollcount_json = json_object_new_int(data->rollCount);
    snr_json = json_object_new_int(data->SNR);

    json_object_object_add(tempobject, "Index", index_json);
    json_object_object_add(tempobject, "RCS", rcs_json);
    json_object_object_add(tempobject, "Range", range_json);
    json_object_object_add(tempobject, "Azimuth", azimuth_json);
    json_object_object_add(tempobject, "Vrel", vrel_json);
    json_object_object_add(tempobject, "Rollcount", rollcount_json);
    json_object_object_add(tempobject, "SNR", snr_json);

    return tempobject;   
}

TargetInfo *JsonObjectToTargetInfo(struct json_object * data)
{
    TargetInfo temp;
    TargetInfo *tempobject = &temp;
    
    struct json_object *TIobject;
    struct json_object *index_json;
    struct json_object *rcs_json;
    struct json_object *range_json;
    struct json_object *azimuth_json;
    struct json_object *vrel_json;
    struct json_object *rollcount_json;
    struct json_object *snr_json;
    
    TIobject = json_object_new_object();
    json_object_object_get_ex(data, "TargetInfos", &TIobject);

    json_object_object_get_ex(TIobject, "Index", &index_json);
    json_object_object_get_ex(TIobject, "RCS", &rcs_json);
    json_object_object_get_ex(TIobject, "Range", &range_json);
    json_object_object_get_ex(TIobject, "Azimuth", &azimuth_json);
    json_object_object_get_ex(TIobject, "Vrel", &vrel_json);
    json_object_object_get_ex(TIobject, "Rollcount", &rollcount_json);
    json_object_object_get_ex(TIobject, "SNR", &snr_json);

    tempobject->index = json_object_get_int(index_json);
    tempobject->rcs = json_object_get_double(rcs_json);
    tempobject->range = json_object_get_double(range_json);
    tempobject->azimuth = json_object_get_int(azimuth_json);
    tempobject->vrel = json_object_get_double(vrel_json);
    tempobject->rollCount = json_object_get_int(rollcount_json);
    tempobject->SNR = json_object_get_int(snr_json);

    return tempobject;
}





