#include "parse-json.h"


void parse_to_json(SensorData *sensorData, json_object *my_json, enum option my_option)
{  
    struct json_object *tempobject;
    struct json_object *SVobject;
    struct json_object *SSobject;
    struct json_object *TSobject;
    struct json_object *TIobject;

    SVobject = SensorVersionToJsonObject(&sensorData->version, my_option);
    SSobject = SensorStatusToJsonObject(&sensorData->sStatus, my_option);
    TSobject = TargetStatusToJsonObject(&sensorData->tStatus, my_option);
    TIobject = TargetInfoToJsonObject(&sensorData->tInfo, my_option);
    
    switch (my_option) {
        case ARRAY:
            json_object_array_add(my_json, SVobject);
            json_object_array_add(my_json, SSobject);
            json_object_array_add(my_json, TSobject);
            json_object_array_add(my_json, TIobject);
            break;
        
        default:
            json_object_object_add(my_json, "SensorVersion", SVobject);
            json_object_object_add(my_json, "SensorStatus", SSobject);
            json_object_object_add(my_json, "TargetStatus", TSobject);
            json_object_object_add(my_json, "TargetInfos", TIobject);
            break;
    }

}

void json_to_sensorData(json_object *my_json, SensorData *sensorData, enum option my_option)
{
    SensorVersion sensor_version;
    SensorStatus sensor_status;
    TargetStatus target_status;
    TargetInfo target_info;

    sensor_version = JsonObjectToSensorVersion(my_json, my_option);
    sensor_status = JsonObjectToSensorStatus(my_json, my_option);
    target_status = JsonObjectToTargetStatus(my_json, my_option);
    target_info = JsonObjectToTargetInfo(my_json, my_option);

    //printf("Azimmuth 1: %i\n", target_info->azimuth);

    sensorData->version = sensor_version;
    sensorData->sStatus = sensor_status;
    sensorData->tStatus = target_status;
    sensorData->tInfo = target_info;
    sensorData->tInfoSize=0;
}

struct json_object *SensorVersionToJsonObject(SensorVersion * data, enum option my_option)
{
    struct json_object *tempobject;
    struct json_object *dataType_json;
    struct json_object *result_json;
    struct json_object *master_json;
    struct json_object *second_json;
    struct json_object *step_json;
    
    dataType_json = json_object_new_int(data->dataType);
    result_json = json_object_new_boolean(data->result);
    master_json = json_object_new_int(data->master);
    second_json = json_object_new_int(data->second);
    step_json = json_object_new_int(data->step);

    switch (my_option) {
        case ARRAY :
            tempobject = json_object_new_array();
            json_object_array_add(tempobject, dataType_json);
            json_object_array_add(tempobject, result_json);
            json_object_array_add(tempobject, master_json);
            json_object_array_add(tempobject, second_json);
            json_object_array_add(tempobject, step_json);
            break;
        default :
            tempobject = json_object_new_object();
            json_object_object_add(tempobject, "DataType", dataType_json);
            json_object_object_add(tempobject, "Result", result_json);
            json_object_object_add(tempobject, "Master", master_json);
            json_object_object_add(tempobject, "Second", second_json);
            json_object_object_add(tempobject, "Step", step_json);
            break;
    }
    

    return tempobject;
}

SensorVersion JsonObjectToSensorVersion(struct json_object * data, enum option my_option)
{
    SensorVersion tempobject;
    struct json_object *SVobject_json;
    struct json_object *dataType_json;
    struct json_object *result_json;
    struct json_object *master_json;
    struct json_object *second_json;
    struct json_object *step_json;

    switch (my_option) {
        case ARRAY :
            SVobject_json = json_object_array_get_idx(data, 0);
            dataType_json = json_object_array_get_idx(SVobject_json, 0);
            result_json = json_object_array_get_idx(SVobject_json, 1);
            master_json = json_object_array_get_idx(SVobject_json, 2);
            second_json = json_object_array_get_idx(SVobject_json, 3);
            step_json = json_object_array_get_idx(SVobject_json, 4);
            break;
        default :
            json_object_object_get_ex(data, "SensorVersion", &SVobject_json);
            json_object_object_get_ex(SVobject_json, "DataType", &dataType_json);
            json_object_object_get_ex(SVobject_json, "Result", &result_json);
            json_object_object_get_ex(SVobject_json, "Master", &master_json);
            json_object_object_get_ex(SVobject_json, "Second", &second_json);
            json_object_object_get_ex(SVobject_json, "Step", &step_json);
            break;
    }
    

    tempobject.dataType = json_object_get_int(dataType_json);
    tempobject.result = json_object_get_boolean(result_json);
    tempobject.master = json_object_get_int(master_json);
    tempobject.second = json_object_get_int(second_json);
    tempobject.step = json_object_get_int(step_json);
    return tempobject;
}

struct json_object *SensorStatusToJsonObject(SensorStatus * data, enum option my_option)
{
    struct json_object *tempobject;
    struct json_object *actlMode_json;
    struct json_object *rollCount_json;
    struct json_object *cfgStatus_json;

    actlMode_json = json_object_new_int(data->actl_mode);
    rollCount_json = json_object_new_int(data->rollcount);
    cfgStatus_json = json_object_new_int(data->cfgStatus);

    switch (my_option) {
        case ARRAY :
            tempobject = json_object_new_array();
            json_object_array_add(tempobject, actlMode_json);
            json_object_array_add(tempobject, rollCount_json);
            json_object_array_add(tempobject, cfgStatus_json);
            break;
        default :
            tempobject = json_object_new_object();
            json_object_object_add(tempobject, "Actl_mode", actlMode_json);
            json_object_object_add(tempobject, "Rollcount", rollCount_json);
            json_object_object_add(tempobject, "CfgStatus", cfgStatus_json);
            break;
    }

    return tempobject;
}

SensorStatus JsonObjectToSensorStatus(struct json_object * data, enum option my_option)
{
    SensorStatus tempobject;
    struct json_object *SSobject;
    struct json_object *actl_json;
    struct json_object *rollcount_json;
    struct json_object *cfg_json;

    switch (my_option) {
        case ARRAY :
            SSobject = json_object_array_get_idx(data, 1);
            actl_json = json_object_array_get_idx(SSobject, 0);
            rollcount_json = json_object_array_get_idx(SSobject, 1);
            cfg_json = json_object_array_get_idx(SSobject, 2);
            break;
        default :
            json_object_object_get_ex(data, "SensorStatus", &SSobject);
            json_object_object_get_ex(SSobject, "Actl_mode", &actl_json);
            json_object_object_get_ex(SSobject, "Rollcount", &rollcount_json);
            json_object_object_get_ex(SSobject, "CfgStatus", &cfg_json);
            break;
    }

    tempobject.actl_mode = json_object_get_int(actl_json);
    tempobject.rollcount = json_object_get_int(rollcount_json);
    tempobject.cfgStatus = json_object_get_int(cfg_json);
    return tempobject;
}

struct json_object *TargetStatusToJsonObject(TargetStatus * data, enum option my_option)
{
    struct json_object *tempobject;
    struct json_object *noOfTarget;
    struct json_object *rollcount;
    
    noOfTarget = json_object_new_int(data->noOfTarget);
    rollcount = json_object_new_int(data->rollcount);
    switch (my_option) {
        case ARRAY :
            tempobject = json_object_new_array();
            json_object_array_add(tempobject, noOfTarget);
            json_object_array_add(tempobject, rollcount);
            break;
        default :
            tempobject = json_object_new_object();
            json_object_object_add(tempobject, "NoOfTarget", noOfTarget);
            json_object_object_add(tempobject, "Rollcount", rollcount);
            break;
    }
    return tempobject;
    
}

TargetStatus JsonObjectToTargetStatus(struct json_object * data, enum option my_option)
{
    TargetStatus tempobject;
    struct json_object *TSobject;
    struct json_object *noOfTarget_json;
    struct json_object *rollcount_json;

    switch (my_option) {
    case ARRAY :
        TSobject = json_object_array_get_idx(data, 2);
        noOfTarget_json = json_object_array_get_idx(TSobject, 0);
        rollcount_json = json_object_array_get_idx(TSobject, 1);
        break;
    default :
        json_object_object_get_ex(data, "TargetStatus", &TSobject);
        json_object_object_get_ex(TSobject, "NoOfTarget", &noOfTarget_json);
        json_object_object_get_ex(TSobject, "Rollcount", &rollcount_json);
        break;
    }


    tempobject.noOfTarget = json_object_get_int(noOfTarget_json);
    tempobject.rollcount = json_object_get_int(rollcount_json);
    return tempobject;
}

struct json_object *TargetInfoToJsonObject(TargetInfo * data, enum option my_option)
{
    struct json_object *tempobject;
    struct json_object *index_json;
    struct json_object *rcs_json;
    struct json_object *range_json;
    struct json_object *azimuth_json;
    struct json_object *vrel_json;
    struct json_object *rollcount_json;
    struct json_object *snr_json;

    
    index_json = json_object_new_int(data->index);
    rcs_json = json_object_new_double(data->rcs);
    range_json = json_object_new_double(data->range);
    azimuth_json = json_object_new_int(data->azimuth);
    vrel_json = json_object_new_double(data->vrel);
    rollcount_json = json_object_new_int(data->rollCount);
    snr_json = json_object_new_int(data->SNR);

    switch (my_option) {
        case ARRAY :
            tempobject = json_object_new_array();
            json_object_array_add(tempobject, index_json);
            json_object_array_add(tempobject, rcs_json);
            json_object_array_add(tempobject, range_json);
            json_object_array_add(tempobject, azimuth_json);
            json_object_array_add(tempobject, vrel_json);
            json_object_array_add(tempobject, rollcount_json);
            json_object_array_add(tempobject, snr_json);
            break;
        default :
            tempobject = json_object_new_object();
            json_object_object_add(tempobject, "Index", index_json);
            json_object_object_add(tempobject, "RCS", rcs_json);
            json_object_object_add(tempobject, "Range", range_json);
            json_object_object_add(tempobject, "Azimuth", azimuth_json);
            json_object_object_add(tempobject, "Vrel", vrel_json);
            json_object_object_add(tempobject, "Rollcount", rollcount_json);
            json_object_object_add(tempobject, "SNR", snr_json);
            break;
    }

    return tempobject;   
}

TargetInfo JsonObjectToTargetInfo(struct json_object * data, enum option my_option)
{
    TargetInfo tempobject;
    
    struct json_object *TIobject;
    struct json_object *index_json;
    struct json_object *rcs_json;
    struct json_object *range_json;
    struct json_object *azimuth_json;
    struct json_object *vrel_json;
    struct json_object *rollcount_json;
    struct json_object *snr_json;
    
    switch (my_option) {
        case ARRAY :
            TIobject = json_object_array_get_idx(data, 3);
            index_json = json_object_array_get_idx(TIobject, 0);
            rcs_json = json_object_array_get_idx(TIobject, 1);
            range_json = json_object_array_get_idx(TIobject, 2);
            azimuth_json = json_object_array_get_idx(TIobject, 3);
            vrel_json = json_object_array_get_idx(TIobject, 4);
            rollcount_json = json_object_array_get_idx(TIobject, 5);
            snr_json = json_object_array_get_idx(TIobject, 6);
            break;
        
        default:
            json_object_object_get_ex(data, "TargetInfos", &TIobject);
            json_object_object_get_ex(TIobject, "Index", &index_json);
            json_object_object_get_ex(TIobject, "RCS", &rcs_json);
            json_object_object_get_ex(TIobject, "Range", &range_json);
            json_object_object_get_ex(TIobject, "Azimuth", &azimuth_json);
            json_object_object_get_ex(TIobject, "Vrel", &vrel_json);
            json_object_object_get_ex(TIobject, "Rollcount", &rollcount_json);
            json_object_object_get_ex(TIobject, "SNR", &snr_json);
            break;
    }


    tempobject.index = json_object_get_int(index_json);
    tempobject.rcs = json_object_get_double(rcs_json);
    tempobject.range = json_object_get_double(range_json);
    tempobject.azimuth = json_object_get_int(azimuth_json);
    tempobject.vrel = json_object_get_double(vrel_json);
    tempobject.rollCount = json_object_get_int(rollcount_json);
    tempobject.SNR = json_object_get_int(snr_json);

    return tempobject;
}





