#include "main.h"

void main(int argc, char **argv)
{
    // Allocate memory 
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
    
    // Initiate result values
    uint64_t result_time_json, result_time_json_array, result_time_cbor, result_time_cbor_array, result_time_protobuf, result_time_xdr;
    int err_json = 0, err_json_array = 0, err_cbor = 0, err_cbor_array = 0, err_protobuf = 0, err_xdr = 0;

    // Initiate values for our clock
    struct timespec start, stop;
    clockid_t clk_id;
    uint64_t timer_start;
    uint64_t timer_stop;
    clk_id = CLOCK_MONOTONIC;

    printf (" ====== BENCHMARKING DATA SERIALIZATION ====== \n\n");
    printf("data tested : %i\n\n", (int)DATA_TESTED);

    generateData(sensorData_ptr);

#if JSON_MAP

    struct json_object *my_json;

    if(clock_gettime(clk_id, &start) == -1){
        perror("clock gettime start");
        exit(EXIT_FAILURE);
    }
    
    for (int i=0; i<DATA_TESTED; i++) {
        my_json = json_object_new_object();
        parse_to_json(sensorData_ptr[i], my_json, MAP);             
        json_to_sensorData(my_json, sensorDataTemp_ptr[i], MAP);
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
    printf("# JSON MAP:\n");
    printResult(err_json, result_time_json);

#endif

#if JSON_ARRAY

    struct json_object *my_json_array;

    if(clock_gettime(clk_id, &start) == -1){
        perror("clock gettime start");
        exit(EXIT_FAILURE);
    }
    
    for (int i=0; i<DATA_TESTED; i++) {
        my_json_array = json_object_new_array();
        parse_to_json(sensorData_ptr[i], my_json_array, ARRAY);             
        json_to_sensorData(my_json_array, sensorDataTemp_ptr[i], ARRAY);
        json_object_put(my_json_array);
    }
    
    if(clock_gettime(clk_id, &stop) == -1){
        perror("clock gettime stop");
        exit(EXIT_FAILURE);
    }
    timer_start = start.tv_sec*1000000000 + start.tv_nsec;
    timer_stop = stop.tv_sec*1000000000 + stop.tv_nsec;
    result_time_json_array = timer_stop - timer_start;

    for (int i=0; i<DATA_TESTED; i++) {
        if (verification(sensorData_ptr[i], sensorDataTemp_ptr[i])) {
            printf("err att : %i\n", i);
            err_json_array ++;
        }
    }
    printf("# JSON ARRAY :\n");
    printResult(err_json_array, result_time_json_array);

#endif

#if CBOR_MAP 

    cbor_item_t *my_cbor;

    if(clock_gettime(clk_id, &start) == -1){
        perror("clock gettime start");
        exit(EXIT_FAILURE);
    } 

    for (int i=0; i<DATA_TESTED; i++) {
        my_cbor = cbor_new_definite_map(4);
        parse_to_cbor(sensorData_ptr[i], my_cbor, MAP);
        cbor_to_sensorData(my_cbor, sensorDataTemp_ptr[i], MAP);
        cbor_decref(&my_cbor);
    }

    if(clock_gettime(clk_id, &stop) == -1){
        perror("clock gettime start");
        exit(EXIT_FAILURE);
    }
    timer_start = start.tv_sec*1000000000 + start.tv_nsec;
    timer_stop = stop.tv_sec*1000000000 + stop.tv_nsec;
    result_time_cbor = timer_stop - timer_start;

    for (int i=0; i<DATA_TESTED; i++) {
        if (verification(sensorData_ptr[i], sensorDataTemp_ptr[i])) {
            printf("err att : %i\n", i);
            err_cbor ++;
        }
    }
    printf("# CBOR MAP:\n");
    printResult(err_cbor, result_time_cbor);

#endif

#if CBOR_ARRAY
    cbor_item_t temp;
    cbor_item_t *my_cbor_array = &temp;

    if(clock_gettime(clk_id, &start) == -1){
        perror("clock gettime start");
        exit(EXIT_FAILURE);
    } 
    my_cbor_array = cbor_new_definite_array(4);
    for (int i=0; i<DATA_TESTED; i++) {       
        parse_to_cbor(sensorData_ptr[i], my_cbor_array, ARRAY);
        cbor_to_sensorData((my_cbor_array), sensorDataTemp_ptr[i], ARRAY);
    }
    cbor_decref(&my_cbor_array);
    if(clock_gettime(clk_id, &stop) == -1){
        perror("clock gettime start");
        exit(EXIT_FAILURE);
    }
    timer_start = start.tv_sec*1000000000 + start.tv_nsec;
    timer_stop = stop.tv_sec*1000000000 + stop.tv_nsec;
    result_time_cbor_array = timer_stop - timer_start;

    for (int i=0; i<DATA_TESTED; i++) {
        if (verification(sensorData_ptr[i], sensorDataTemp_ptr[i])) {
            printf("err att : %i\n", i);
            err_cbor_array ++;
        }
    }
    printf("# CBOR ARRAY:\n");
    printResult(err_cbor_array, result_time_cbor_array);

#endif

#if PROTOBUF 
    uint8_t buf[1024];      // Buffer to store serialized data
    uint8_t *buf_ptr = buf;             
    size_t length;          // Length of serialized data
    SensorDataMessage *msg;

    if(clock_gettime(clk_id, &start) == -1){
        perror("clock gettime start");
        exit(EXIT_FAILURE);
    } 
    for (int i=0; i<DATA_TESTED; i++) {
        // Encode
        length = parse_to_protobuf(sensorData_ptr[i], buf_ptr); 
        //Decode
        protobuf_to_sensorData(buf_ptr, sensorDataTemp_ptr[i], length);
    }
    if(clock_gettime(clk_id, &stop) == -1){
        perror("clock gettime start");
        exit(EXIT_FAILURE);
    }

    timer_start = start.tv_sec*1000000000 + start.tv_nsec;
    timer_stop = stop.tv_sec*1000000000 + stop.tv_nsec;
    result_time_protobuf = timer_stop - timer_start;

    for (int i=0; i<DATA_TESTED; i++) {
        if (verification(sensorData_ptr[i], sensorDataTemp_ptr[i])) {
            printf("err att : %i\n", i);
            err_protobuf ++;
        }
    }
    printf("# PROTOBUF :\n");
    printResult(err_protobuf, result_time_protobuf);

#endif

#if XDR_MACRO 
    XDR my_xdr;
    XDR *my_xdr_ptr = &my_xdr;
    char buffer_xdr[40*32];
    unsigned long len = 40*32;

    xdrmem_create(my_xdr_ptr, buffer_xdr, len, XDR_ENCODE);

    if(clock_gettime(clk_id, &start) == -1){
        perror("clock gettime start");
        exit(EXIT_FAILURE);
    } 
    for (int i=0; i<DATA_TESTED; i++) {

        my_xdr_ptr->x_op = XDR_ENCODE;
        xdr_setpos(my_xdr_ptr, 0);
        xdr_sensorData(sensorData_ptr[i], my_xdr_ptr);

        my_xdr_ptr->x_op = XDR_DECODE;
        xdr_setpos(my_xdr_ptr, 0);
        xdr_sensorData(sensorDataTemp_ptr[i], my_xdr_ptr);
        
    }
    if(clock_gettime(clk_id, &stop) == -1){
        perror("clock gettime start");
        exit(EXIT_FAILURE);
    }

    timer_start = start.tv_sec*1000000000 + start.tv_nsec;
    timer_stop = stop.tv_sec*1000000000 + stop.tv_nsec;
    result_time_xdr = timer_stop - timer_start;

    for (int i=0; i<DATA_TESTED; i++) {
        if (verification(sensorData_ptr[i], sensorDataTemp_ptr[i])) {
            printf("err att : %i\n", i);
            err_xdr ++;
        }
    }
    printf("# XDR :\n");
    printResult(err_xdr, result_time_xdr);

#endif

#if DEBUG
    float floatToParse_1 = 5.0;
    float floatToParse_2 = 10.0;
    float floatParsed_1 = 0.0;
    float floatParsed_2 = 0.0;
    unsigned int len = 40*32;
    char buffer_xdr[len];
    
    XDR my_xdr;
    XDR *my_xdr_ptr = &my_xdr;
    int err =0;
    SensorVersion *test_struct, *temp_struct;
    test_struct = malloc(sizeof(SensorVersion));
    temp_struct = malloc(sizeof(SensorVersion));
    test_struct->dataType = 10;
    test_struct->result = 1;
    test_struct->master = 20;
    test_struct->second = 30;
    test_struct->step = 40;

    temp_struct->dataType = 0;
    temp_struct->result = 0;
    temp_struct->master = 0;
    temp_struct->second = 0;
    temp_struct->step = 0;

    // *** Serialize

    SensorVersionMessage msg = SENSOR_VERSION_MESSAGE__INIT;
    void *buf;             // Buffer to store serialized data
    size_t length;           // Length of serialized data
/*
    msg.datatype = test_struct->dataType;
    msg.result = test_struct->result;
    msg.master = test_struct->master;
    msg.second = test_struct->second;
    msg.step = test_struct->step;
    */
    msg.datatype = 2;
    msg.result = true;
    msg.master = 3;
    msg.second = 3;
    msg.step = 3;
    length = sensor_version_message__get_packed_size(&msg);
    buf = malloc(length);
    sensor_version_message__pack(&msg, buf);
    printf("Writting %li serialized bytes\n",length);
    


    uint8_t buf_temp[MAX_MSG_SIZE];
    SensorVersionMessage *msg_temp;
    size_t length_temp = length;
    /*
    for (int i = 0; i<length_temp; i++) {
        buf_temp[i] = &buf[i];
    }
    */
    msg_temp = sensor_version_message__unpack(NULL, length_temp, buf);
    if (msg_temp == NULL)
    {
        fprintf(stderr, "error unpacking incoming message\n");
        exit(1);
    }
    temp_struct->dataType = msg_temp->datatype;
    temp_struct->result = msg_temp->result;
    temp_struct->master = msg_temp->master;
    temp_struct->second = msg_temp->second;
    temp_struct->step = msg_temp->step;
    // *** END

    printf("dataType : %i\n", temp_struct->dataType);
    printf("result : %i\n", temp_struct->result);
    printf("master : %i\n", temp_struct->master);
    printf("second : %i\n", temp_struct->second);
    printf("step : %i\n", temp_struct->step);
  
free(buf);
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
        printf("\tTime during the serialization: %f ms\n", time/1000000);
    }else {
        printf("\tData: NOK\n");
        printf("\tNumber of error : %i", err);
    }
    printf("\n\n");
}