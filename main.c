#include "main.h"

void main(int argc, char **argv)
{
    ssize_t size;
    int buffer_size =3*14;
    int sockfd[NUMBER_PORT], index, ret;
    
    int port[NUMBER_PORT] = {1111};
    uint8_t buffer[200];

    dataUDP *data;
    data = malloc(sizeof(dataUDP));

    SensorData *sensorData;
    sensorData=(SensorData *)malloc(sizeof(SensorData));
    SensorData *sensorDataTemp;
    sensorDataTemp=(SensorData *)malloc(sizeof(SensorData));
    json_object *my_json;

    double result_time_json = 0, result_time_cbor = 0, result_time_protobuf = 0, result_time_xdr = 0;
    int err_json = 0, err_cbor = 0, err_protobuf = 0, err_xdr = 0;
    struct timespec start, stop;
    clockid_t clk_id;

    clk_id = CLOCK_MONOTONIC;

    printf (" ====== BENCHMARKING DATA SERIALIZATION ====== \n");

    // Initialize sockets UDP
    for(index = 0; index<NUMBER_PORT; index ++) {
        initServerUDP(port[index], &sockfd[index]);
    }

    // Begin the loop and run for each data reveived on the udp port
    for(int i=0; i<DATA_TESTED; i++) {
        for(index = 0; index<NUMBER_PORT; index ++) {
            data->index = index;
            data->buffer = buffer;
            data->port = port[index];
            data->sockfd = &sockfd[index];
            receiveFromUDP(data);
        }
        initData(sensorData);
        initData(sensorDataTemp);
        parseRawBuffer(data->buffer, buffer_size, sensorData);

        TargetInfo *myTargetInfos = sensorData->tInfo;
        SensorVersion mySensorVersion = sensorData->version;
        SensorStatus mySensorStatus = sensorData->sStatus;
        TargetStatus myTargetStatus = sensorData->tStatus;

#if JSON 
        if(clock_gettime(clk_id, &start) == -1){
            perror("clock gettime start");
            exit(EXIT_FAILURE);
        }

        my_json = json_object_new_object();
        parse_to_json(sensorData, my_json);             
        json_to_sensorData(my_json, sensorDataTemp);

        if(clock_gettime(clk_id, &stop) == -1){
            perror("clock gettime start");
            exit(EXIT_FAILURE);
        }

        result_time_json += (stop.tv_sec - start.tv_sec)*1000000000 + (stop.tv_nsec - start.tv_nsec);

        if (!verification(*sensorData, *sensorDataTemp)) {
           err_json ++;
        } 
#endif

#if CBOR 
        if(clock_gettime(clk_id, &start) == -1){
            perror("clock gettime start");
            exit(EXIT_FAILURE);
        } 
        if(clock_gettime(clk_id, &stop) == -1){
            perror("clock gettime start");
            exit(EXIT_FAILURE);
        }

        result_time_cbor += (stop.tv_sec - start.tv_sec)*1000000000 + (stop.tv_nsec - start.tv_nsec);

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

    }

    free(data);
    free(sensorData);
    free(sensorDataTemp);

    printf("----- Result Benchmarking -----\n");
    printf("Data tested : %d\n", DATA_TESTED);
    printf("\n\n");

#if JSON
    printf("# JSON :\n");
    printResult(err_json, result_time_json/DATA_TESTED);
#endif
#if CBOR
    printf("# CBOR :\n");
    printResult(err_cbor, result_time_cbor/DATA_TESTED);
#endif
#if PROTOBUF
    printf("# PROTOBUF :\n");
    printResult(err_protobuf, result_time_protobuf/DATA_TESTED);
#endif
#if XDR
    printf("# XDR :\n");
    printResult(err_xdr, result_time_xdr/DATA_TESTED);
#endif

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

int verification(SensorData sensorData_1, SensorData sensorData_2)
{
    // sensor version
    if (sensorData_1.version.dataType != sensorData_2.version.dataType)
        return 1;
    if (sensorData_1.version.master != sensorData_2.version.master)
        return 1;
    if (sensorData_1.version.result != sensorData_2.version.result)
        return 1;
    if (sensorData_1.version.second != sensorData_2.version.second)
        return 1;
    if (sensorData_1.version.step != sensorData_2.version.step)
        return 1;
    
    // sensor status
    if (sensorData_1.sStatus.actl_mode != sensorData_2.sStatus.actl_mode)
        return 1;
    if (sensorData_1.sStatus.cfgStatus != sensorData_2.sStatus.cfgStatus)
        return 1;
    if (sensorData_1.sStatus.rollcount != sensorData_2.sStatus.rollcount)
        return 1;

    // Target Status
    if (sensorData_1.tStatus.noOfTarget != sensorData_2.tStatus.noOfTarget)
        return 1;
    if (sensorData_1.tStatus.rollcount != sensorData_2.tStatus.rollcount)
        return 1;

    // Target Info
    if(sensorData_1.tInfo->azimuth != sensorData_2.tInfo->azimuth)
        return 1;
    if(sensorData_1.tInfo->index != sensorData_2.tInfo->index)
        return 1;
    if(sensorData_1.tInfo->range != sensorData_2.tInfo->range)
        return 1;
    if(sensorData_1.tInfo->rcs != sensorData_2.tInfo->rcs)
        return 1;
    if(sensorData_1.tInfo->rollCount != sensorData_2.tInfo->rollCount)
        return 1;
    if(sensorData_1.tInfo->SNR != sensorData_2.tInfo->SNR)
        return 1;
    if(sensorData_1.tInfo->vrel != sensorData_2.tInfo->vrel)
        return 1;
    return 0;
}

void printResult(int err, double time) {
    if (err == 0){
        printf("\tData : OK\n");
        printf("\tTime during the serialization: %f ns\n", time);
    }else {
        printf("\tData: NOK\n");
    }
    printf("\n\n");
}