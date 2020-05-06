#include "main.h"

void main(int argc, char **argv)
{
    SensorData **sensorData_ptr;
    sensorData_ptr = malloc(DATA_TESTED*sizeof(SensorData *));
    for (int i = 0; i<DATA_TESTED; i++) {
        sensorData_ptr[i] = malloc(sizeof(SensorData));
    } 
    SensorData **sensorDataTemp_ptr;
    sensorDataTemp_ptr = malloc(DATA_TESTED*sizeof(SensorData *));
    for (int i = 0; i<DATA_TESTED; i++) {
        sensorDataTemp_ptr[i] = malloc(sizeof(SensorData));
        initData(sensorDataTemp_ptr[i]);
    } 
    
    uint64_t result_time_json, result_time_cbor, result_time_protobuf, result_time_xdr;
    int err_json = 0, err_cbor = 0, err_protobuf = 0, err_xdr = 0;

    struct timespec start, stop;
    clockid_t clk_id;
    uint64_t timer_start;
    uint64_t timer_stop;

    clk_id = CLOCK_MONOTONIC;

    printf (" ====== BENCHMARKING DATA SERIALIZATION ====== \n");
    printf("data tested : %i\n", (int)DATA_TESTED);

    generateData(sensorData_ptr);

#if JSON

    struct json_object *my_json;

    if(clock_gettime(clk_id, &start) == -1){
        perror("clock gettime start");
        exit(EXIT_FAILURE);
    }
    
    for (int i=0; i<DATA_TESTED; i++) {
        my_json = json_object_new_object();
        parse_to_json(sensorData_ptr[i], my_json);             
        json_to_sensorData(my_json, sensorDataTemp_ptr[i]);
        json_object_put(my_json);
    }
    
    if(clock_gettime(clk_id, &stop) == -1){
        perror("clock gettime stop");
        exit(EXIT_FAILURE);
    }
    timer_start = start.tv_sec*1000000000 + start.tv_nsec;
    timer_stop = stop.tv_sec*1000000000 + stop.tv_nsec;
    result_time_json = timer_stop - timer_start;

    for (int i=0; i<DATA_TESTED; i++) {
        if (verification(sensorData_ptr[i], sensorDataTemp_ptr[i])) {
            printf("err att : %i\n", i);
            err_json ++;
        }
    }
    printf("# JSON :\n");
    printResult(err_json, result_time_json);

#endif

#if CBOR 

    cbor_item_t *my_cbor;

    if(clock_gettime(clk_id, &start) == -1){
        perror("clock gettime start");
        exit(EXIT_FAILURE);
    } 

    for (int i=0; i<DATA_TESTED; i++) {
        my_cbor = cbor_new_definite_map(4);
        parse_to_cbor(sensorData_ptr[i], my_cbor);
        cbor_to_sensorData(my_cbor, sensorDataTemp_ptr[i]);
        cbor_decref(&my_cbor);
    }

    if(clock_gettime(clk_id, &stop) == -1){
        perror("clock gettime start");
        exit(EXIT_FAILURE);
    }
    timer_start = start.tv_sec*1000000000 + start.tv_nsec;
    timer_stop = stop.tv_sec*1000000000 + stop.tv_nsec;
    result_time_cbor = timer_stop - timer_start;

    //printf("Azimuth 1 : %i\n", sensorData_ptr[20]->tInfo.azimuth);
    //printf("Azimuth 2 : %i\n", sensorDataTemp_ptr[20]->tInfo.azimuth);
    for (int i=0; i<DATA_TESTED; i++) {
        if (verification(sensorData_ptr[i], sensorDataTemp_ptr[i])) {
            printf("err att : %i\n", i);
            err_cbor ++;
        }
    }
    printf("# CBOR :\n");
    printResult(err_cbor, result_time_cbor);

#endif

#if PROTOBUF 
        if(clock_gettime(clk_id, &start) == -1){
            perror("clock gettime start");
            exit(EXIT_FAILURE);
        } 
        if(clock_gettime(clk_id, &stop) == -1){
            perror("clock gettime start");
            exit(EXIT_FAILURE);
        }

        result_time_protobuf += (stop.tv_sec - start.tv_sec)*1000000000 + (stop.tv_nsec - start.tv_nsec);

#endif

#if XDR 
        if(clock_gettime(clk_id, &start) == -1){
            perror("clock gettime start");
            exit(EXIT_FAILURE);
        } 
        if(clock_gettime(clk_id, &stop) == -1){
            perror("clock gettime start");
            exit(EXIT_FAILURE);
        }

        result_time_protobuf += (stop.tv_sec - start.tv_sec)*1000000000 + (stop.tv_nsec - start.tv_nsec);

#endif

for (int i = 0; i<DATA_TESTED; i++) {
        free(sensorData_ptr[i]);
        free(sensorDataTemp_ptr[i]);
    }    
    free(sensorData_ptr);
    free(sensorDataTemp_ptr);
}

/**
 * @brief print the error message
 * @param msg char *
 * @retval none
 * **/
void error(char *msg) {
    perror(msg);
    exit(0);
}

void generateData(SensorData **sensorData_ptr)
{
   int buffer_size =3*14;
   uint8_t buffer[3*14] = {0xaa, 0xaa, 0x0a, 0x06, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x55,
    0xaa, 0xaa, 0xb, 0x07, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x55,
    0xaa, 0xaa, 0x0c, 0x07, 0x01, 0x6b, 0x01, 0x86, 0x2a, 0x02, 0xbf, 0x79, 0x55, 0x55};
    for(int i=0; i<DATA_TESTED; i++) {
        initData(sensorData_ptr[i]);
        parseRawBuffer(buffer, buffer_size, sensorData_ptr[i]);
    }
}

void debug_parse(struct json_object *data_json)
{
    struct json_object *SVobject;
    struct json_object *SSobject;
    struct json_object *TSobject;
    struct json_object *TIobject;

    struct json_object *datatType;
    struct json_object *rollCountSensor;
    struct json_object *rollcountTarget;
    struct json_object *azimuth;
    json_object_object_get_ex(data_json, "SensorVersion", &SVobject);
    json_object_object_get_ex(SVobject, "DataType", &datatType);
    json_object_object_get_ex(data_json, "SensorStatus", &SSobject);
    json_object_object_get_ex(SSobject, "Rollcount", &rollCountSensor);
    json_object_object_get_ex(data_json, "TargetStatus", &TSobject);
    json_object_object_get_ex(TSobject, "Rollcount", &rollcountTarget);
    json_object_object_get_ex(data_json, "TargetInfos", &TIobject);
    json_object_object_get_ex(TIobject, "Azimuth", &azimuth);
    
    
    printf(" ===== DEBUG PARSE JSON =====\n");
    printf("SensorVersion - datatType : %s\n", json_object_get_string(datatType));
    printf("SensorStatus - Rollcount : %s\n", json_object_get_string(rollCountSensor));
    printf("TargetStatus - Rollcount : %s\n", json_object_get_string(rollcountTarget));
    printf("TargetInfos - Azimuth : %s\n", json_object_get_string(azimuth));
    printf(" ===== END DEBUG PARSE JSON =====\n");
}

int verification(SensorData *sensorData_1, SensorData *sensorData_2)
{

    TargetInfo targetInfo_1 = sensorData_1->tInfo, targetInfo_2 = sensorData_2->tInfo;
    SensorVersion sensorVersion_1 = sensorData_1->version, sensorVersion_2 = sensorData_2->version;
    SensorStatus sensorStatus_1 = sensorData_1->sStatus, sensorStatus_2 = sensorData_2->sStatus;
    TargetStatus targetStatus_1 = sensorData_1->tStatus, targetStatus_2 = sensorData_2->tStatus;

    // sensor version 
    if (sensorVersion_1.dataType != sensorVersion_2.dataType)
        return 1;
    if (sensorVersion_1.master != sensorVersion_2.master)
        return 1;
    if (sensorVersion_1.result != sensorVersion_2.result)
        return 1;
    if (sensorVersion_1.second != sensorVersion_2.second)
        return 1;
    if (sensorVersion_1.step != sensorVersion_2.step)
        return 1;

    // sensor status
    if (sensorStatus_1.actl_mode != sensorStatus_2.actl_mode)
        return 1;
    if (sensorStatus_1.cfgStatus != sensorStatus_2.cfgStatus)
        return 1;
    if (sensorStatus_1.rollcount != sensorStatus_2.rollcount)
        return 1;

    // Target Status
    if (targetStatus_1.noOfTarget != targetStatus_2.noOfTarget)
        return 1;
    if (targetStatus_1.rollcount != targetStatus_2.rollcount)
        return 1; 

    // Target Info
    if(targetInfo_1.azimuth != targetInfo_2.azimuth)
        return 1;
    if(targetInfo_1.index != targetInfo_2.index)
        return 1;
    if(targetInfo_1.range != targetInfo_2.range)
        return 1;
    if(targetInfo_1.rcs != targetInfo_2.rcs)
        return 1;
    if(targetInfo_1.rollCount != targetInfo_2.rollCount)
        return 1;
    if(targetInfo_1.SNR != targetInfo_2.SNR)
        return 1;
    if(targetInfo_1.vrel != targetInfo_2.vrel)
        return 1;
    return 0;
    
}

void printResult(int err, double time) 
{
    if (err == 0){
        printf("\tData : OK\n");
        printf("\tTime during the serialization: %f ns\n", time);
    }else {
        printf("\tData: NOK\n");
        printf("\tNumber of error : %i", err);
    }
    printf("\n\n");
}