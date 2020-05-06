#include "main.h"

void main(int argc, char **argv)
{
    /*
    ssize_t size;
    int buffer_size =3*14;
    int sockfd[NUMBER_PORT], index, ret;   
    int port[NUMBER_PORT] = {1111};
    uint8_t buffer[200];
    dataUDP *data;
    data = malloc(sizeof(dataUDP));
*/
    //SensorData sensorData[DATA_TESTED] = (SensorData *)malloc(DATA_TESTED*sizeof(SensorData));
    SensorData **sensorData_ptr;
    sensorData_ptr = malloc(DATA_TESTED*sizeof(SensorData *));
    for (int i = 0; i<DATA_TESTED; i++) {
        sensorData_ptr[i] = malloc(sizeof(SensorData));
    } 
    //SensorData sensorDataTemp[DATA_TESTED] = (SensorData *)malloc(DATA_TESTED*sizeof(SensorData));
    SensorData **sensorDataTemp_ptr;
    sensorDataTemp_ptr = malloc(DATA_TESTED*sizeof(SensorData *));
    for (int i = 0; i<DATA_TESTED; i++) {
        sensorDataTemp_ptr[i] = malloc(sizeof(SensorData));
        initData(sensorDataTemp_ptr[i]);

        sensorDataTemp_ptr[i]->tInfo=malloc(1*sizeof(TargetInfo));

        initTargetInfo(sensorDataTemp_ptr[i]->tInfo);
        //sensorDataTemp_ptr[i]->tInfo=malloc(1*sizeof(TargetInfo));
    } 
    
    cbor_item_t *my_cbor;
    uint64_t result_time_json, result_time_cbor, result_time_protobuf, result_time_xdr;
    int err_json = 0, err_cbor = 0, err_protobuf = 0, err_xdr = 0;

    struct timespec start, stop;
    clockid_t clk_id;
    uint64_t timer_start;
    uint64_t timer_stop;

    clk_id = CLOCK_MONOTONIC;

    printf (" ====== BENCHMARKING DATA SERIALIZATION ====== \n");
/*
    printf("Befor catch Data\n");
    printf("\tazimuth at 20 : %i\n", sensorData[20].tInfo->azimuth);
    printf("\tazimuth at 44 : %i\n", sensorData[44].tInfo->azimuth);
    printf("\tazimuth at 91 : %i\n", sensorData[91].tInfo->azimuth);
*/
    catchDataFromUdp(sensorData_ptr);
/*
    // Initialize sockets UDP
    for(index = 0; index<NUMBER_PORT; index ++) {
        initServerUDP(port[index], &sockfd[index]);
    }
*/

/*
    // Begin the loop and run for each data reveived on the udp port
    for(int i=0; i<DATA_TESTED; i++) {
        for(index = 0; index<NUMBER_PORT; index ++) {
            data->index = index;
            data->buffer = buffer;
            data->port = port[index];
            data->sockfd = &sockfd[index];
            receiveFromUDP(data);
        }
        initData(sensorData_ptr);
        initData(sensorDataTemp_ptr);
        parseRawBuffer(data->buffer, buffer_size, sensorData_ptr);
*/

/*
        TargetInfo *myTargetInfos = sensorData.tInfo;
        SensorVersion mySensorVersion = sensorData.version;
        SensorStatus mySensorStatus = sensorData.sStatus;
        TargetStatus myTargetStatus = sensorData.tStatus;
*/
#if JSON 
        //json_object *my_json[DATA_TESTED];
        //json_object_iter my_json[DATA_TESTED];
        json_object *my_json;
        
        //sensorData_ptr = sensorData;
        //sensorDataTemp_ptr = sensorDataTemp;
        
        if(clock_gettime(clk_id, &start) == -1){
            perror("clock gettime start");
            exit(EXIT_FAILURE);
        }
        for (int i=0; i<DATA_TESTED; i++) {
            my_json = json_object_new_object();
            parse_to_json(sensorData_ptr[i], my_json);             
            json_to_sensorData(my_json, sensorDataTemp_ptr[i]);
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

        
        
    
//printf("azimuth at 20 : %i\n", sensorDataTemp_ptr[20].tInfo->azimuth);
       // printf("Azimuth 3 : %i\n",sensorDataTemp_ptr[20].tInfo->azimuth);
        //
//        printf("Azimuth 4 : %i\n",sensorDataTemp_ptr[20].tInfo->azimuth);
        

        /* DEBUG */
/*
        printf("befor serialization :\n");
        printf("azimuth at 20 : %i\n", sensorData_ptr[20].tInfo->azimuth);
        printf("azimuth at 44 : %i\n", sensorData_ptr[44].tInfo->azimuth);
        printf("azimuth at 91 : %i\n", sensorData_ptr[91].tInfo->azimuth);
*/

//        printf("after serialization :\n");
/*
        printf("azimuth at 20 : %i\n", sensorDataTemp_ptr[20].tInfo->azimuth);
        printf("azimuth at 44 : %i\n", sensorDataTemp_ptr[44].tInfo->azimuth);
        printf("azimuth at 91 : %i\n", sensorDataTemp_ptr[91].tInfo->azimuth);
        */

        for (int i = 0; i<DATA_TESTED; i++) {
            free(sensorData_ptr[i]);
            //free(sensorDataTemp_ptr[i]->tInfo);
            free(sensorDataTemp_ptr[i]);
        }    
        free(sensorData_ptr);
        free(sensorDataTemp_ptr);

        /* --- */
/*
        for (int i=0; i<DATA_TESTED; i++) {
            if (verification(sensorData[i], sensorDataTemp[i])) {
                printf("err att : %i\n", i);
                err_json ++;
            }
        } 
*/
#endif

#if CBOR 
        if(clock_gettime(clk_id, &start) == -1){
            perror("clock gettime start");
            exit(EXIT_FAILURE);
        } 
        timer_start = start.tv_sec*1000000000 + start.tv_nsec;
        my_cbor = cbor_new_definite_map(4);
        parse_to_cbor(sensorData_ptr, my_cbor);
        cbor_to_sensorData(my_cbor, sensorDataTemp_ptr);

        if(clock_gettime(clk_id, &stop) == -1){
            perror("clock gettime start");
            exit(EXIT_FAILURE);
        }
        timer_stop = stop.tv_sec*1000000000 + stop.tv_nsec;

        result_time_cbor += timer_start - timer_stop;
        if (verification(sensorData, sensorDataTemp)) {
           err_cbor ++;
        }

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
/*
    }

    free(data);
*/
/*
free(sensorData);
free(sensorDataTemp);
    printf("----- Result Benchmarking -----\n");
    printf("Data tested : %d\n", DATA_TESTED);
    printf("\n\n");
*/
#if JSON
    printf("# JSON :\n");
    printResult(err_json, result_time_json);
#endif
/*
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
*/

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

void catchDataFromUdp(SensorData **sensorData_ptr)
{
    ssize_t size;
    int buffer_size =3*14;
    int sockfd[NUMBER_PORT], index, ret;   
    int port[NUMBER_PORT] = {1111};
    uint8_t buffer[200];
    dataUDP *data;
    data = malloc(sizeof(dataUDP));

    // Initialize sockets UDP
    for (index = 0; index<NUMBER_PORT; index ++) {
        initServerUDP(port[index], &sockfd[index]);
    }

    // Begin the loop and run for each data received on the udp port
    for(int i=0; i<DATA_TESTED; i++) {
        for(index = 0; index<NUMBER_PORT; index ++) {
            data->index = index;
            data->buffer = buffer;
            data->port = port[index];
            data->sockfd = &sockfd[index];
            receiveFromUDP(data);
        }
        initData(sensorData_ptr[i]);
        parseRawBuffer(data->buffer, buffer_size, sensorData_ptr[i]);
        //sensorData_ptr++;
    }
    free(data);
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
    int16_t azimuth = sensorData_2->tInfo->azimuth;
    //TargetInfo *targetInfo_2 = sensorData_2->tInfo;
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
    if(azimuth != sensorData_2->tInfo->azimuth)
        return 1;
    if(sensorData_1->tInfo->index != sensorData_2->tInfo->index)
        return 1;
    if(sensorData_1->tInfo->range != sensorData_2->tInfo->range)
        return 1;
    if(sensorData_1->tInfo->rcs != sensorData_2->tInfo->rcs)
        return 1;
    if(sensorData_1->tInfo->rollCount != sensorData_2->tInfo->rollCount)
        return 1;
    if(sensorData_1->tInfo->SNR != sensorData_2->tInfo->SNR)
        return 1;
    if(sensorData_1->tInfo->vrel != sensorData_2->tInfo->vrel)
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