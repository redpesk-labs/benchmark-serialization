#include "config.h"
#include "data.h"
//#include "sp70c-data-handle.h"
//#include "string.h"
#include <pthread.h>
#include <semaphore.h>
#include "serializer.h"

#define TIME_RESOLUTION 1000000000ULL

sem_t mutex;
int fd[2];

typedef struct
{
	Serializer serializer;
	SensorData* data;
	SensorData* dataTemp;
}Data_Thread;


/// @brief fill a SensorData object.
/// @param[out] senorData_ptr The pointer to save data generated.
void generateData(SensorData* sensorData_ptr)
{
	//parseRawBuffer(sensorData_ptr);

	sensorData_ptr->version.dataType = 1;
	sensorData_ptr->version.result = 2;
	sensorData_ptr->version.master = 3;
	sensorData_ptr->version.second = 4;
	sensorData_ptr->version.step = 15;

	sensorData_ptr->sStatus.actl_mode = 3;
	sensorData_ptr->sStatus.rollcount = 2;
	sensorData_ptr->sStatus.cfgStatus = 1;

	sensorData_ptr->tStatus.noOfTarget= 10;
	sensorData_ptr->tStatus.rollcount= 11;

	sensorData_ptr->tInfo.index= 12;
	sensorData_ptr->tInfo.rcs= 4.5;
	sensorData_ptr->tInfo.range= 1.35;
	sensorData_ptr->tInfo.azimuth= -6;
	sensorData_ptr->tInfo.vrel= 0.02;
	sensorData_ptr->tInfo.rollCount= 3;
	sensorData_ptr->tInfo.SNR= -8;

	sensorData_ptr->tInfoSize = 1;
}

/// @brief Print byte to byte data into SensorData object.
/// @param[in] sd SensorData object to print.
void printByHexa(SensorData* sd)
{
	printf("Sensor Version by hexa :");
	for (int index =0; index<sizeof(SensorVersion); index++) {
		printf(" %02x", ((char *)&sd->version)[index]);
	}
	printf("\n");
	printf("Sensor Status by hexa :");
	for (int index =0; index<sizeof(SensorStatus); index++) {
		printf(" %02x", ((char *)&sd->sStatus)[index]);
	}
	printf("\n");
	printf("Target Status by hexa :");
	for (int index =0; index<sizeof(TargetStatus); index++) {
		printf(" %02x", ((char *)&sd->tStatus)[index]);
	}
	printf("\n");
	printf("Target Info by hexa :");
	for (int index =0; index<sizeof(TargetInfo); index++) {
		printf(" %02x", ((char *)&sd->tInfo)[index]);
	}
	printf("\n");
}

/// @brief compare 2 SensorData objects with memcmp.
/// @param[in] sd1 First SensorData object to compare.
/// @param[in] sd2 Second SensorData object to compare.
/// @retval err The number of err catch by memcmp.
int verification(SensorData* sd1, SensorData* sd2)
{
	int err=0;
	//printByHexa(sd1);
	//printByHexa(sd2); 
	err+= memcmp(sd1, sd2, sizeof(SensorData));
	return err;
}

/// @brief Print result of a specific bench.
/// @param[in] err The number of erreur happend during a bench.
/// @param[in] time Thee time during a bench.
void printResult(int err, uint64_t time)
{
	if (err == 0) {
		printf("\tData : OK\n");
		printf("\tTime during the serialization: %f ms\n", (double)time / 1000000);
	}
	else {
		printf("\tData: NOK\n");
		printf("\tNumber of error : %i", err);
	}
	printf("\n\n");
}

static void* serialize(void* data)
{
	int index;
	for (index = 0; index<DATA_TESTED; index++) {
		//sem_wait(&mutex);
		void* result;
		((Data_Thread* )data)->serializer.serialize(((Data_Thread* )data)->serializer.context, *((Data_Thread* )data)->data, &result);
		//sem_post(&mutex);
	}
	printf("join\n");
}

void* deserialize(Data_Thread data)
{	
	((Data_Thread* )data)->serializer.deserialize(((Data_Thread* )data)->serializer.context, result, ((Data_Thread* )data)->dataTemp);
	((Data_Thread* )data)->serializer.freeobject(((Data_Thread* )data)->serializer.context, result);
}

/// @brief Main function for the bench.
/// @retval @c EXIT_SUCCESS or @c EXIT_FAILURE
int main()
{
	// Allocate memory 
	SensorData sensorData;
	memset(&sensorData, 0, sizeof(SensorData));
	SensorData sensorDataTemp;
	memset(&sensorDataTemp, 0, sizeof(SensorData));

	// Initiate values for our clock
	struct timespec start, stop;
	clockid_t clk_id;
	uint64_t timer_start;
	uint64_t timer_stop;
	clk_id = CLOCK_MONOTONIC;

	// Initiate 2 threads, for emission and reception
	pthread_t thread_emission;
	pthread_t thread_reception;

	//Initiate the semaphore
	sem_init(&mutex, 0, 1);

	//Generate data to parse
	generateData(&sensorData);

	//Prepare data for threads
 	Data_Thread data_serialize;
	memset(&data_serialize, 0, sizeof(data_serialize));
	data_serialize.data = &sensorData;
	data_serialize.dataTemp = &sensorDataTemp;
	
 	Data_Thread data_deserialize;
	memset(&data_deserialize, 0, sizeof(data_deserialize));
	data_deserialize.data = &sensorDataTemp;  

	printf(" ====== BENCHMARKING DATA SERIALIZATION ====== \n\n");
	printf("data tested : %i\n\n", (int)DATA_TESTED);

	/* Reference in C */
	int result_time_ref=0;
	int err_ref=0;
	int ret;
	char* buffer_ref;
	memset(&buffer_ref, 0, sizeof(sensorData));
	
/* 	if (clock_gettime(clk_id, &start) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < DATA_TESTED; i++) {
		// copy data to send
		memcpy(&buffer_ref, &sensorData, sizeof(SensorData));
		//copy data received
		memcpy(&sensorDataTemp, &buffer_ref, sizeof(SensorData));
	}

	if (clock_gettime(clk_id, &stop) == -1) {
		perror("clock gettime stop");
		exit(EXIT_FAILURE);
	}
	if (verification(&sensorData, &sensorDataTemp)) {
		printf("output differs from input\n");
		err_ref++;
	}
	timer_start = start.tv_sec * TIME_RESOLUTION + start.tv_nsec;
	timer_stop = stop.tv_sec * TIME_RESOLUTION + stop.tv_nsec;
	result_time_ref = timer_stop - timer_start;
	printf("# Reference C: \n");
	printResult(err_ref, result_time_ref); */



//#ifdef BENCH_JSON
	// Initiate result values
	uint64_t result_time_json;
	int err_json = 0;
	int option = MAP;

	Serializer json;
	memset(&data_serialize.serializer, 0, sizeof(json));
	memset(&data_deserialize.serializer, 0, sizeof(json));

	jsonc_get_serializer(&data_serialize.serializer);
	jsonc_get_serializer(&data_deserialize.serializer);

	#ifdef BENCH_JSON_ARRAY	
		option = ARRAY;
	#endif

	//json.context = &option;
	data_serialize.serializer.context = &option;
	if (clock_gettime(clk_id, &start) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}
	ret = pthread_create(&thread_emission, NULL, serialize, &data_serialize);
	if(ret){
		perror("pthread_create");
	}
	ret = pthread_create(&thread_reception, NULL, deserialize, &data_deserialize);
	if(ret){
		perror("pthread_create");
	}
/* 	for (int i = 0; i < DATA_TESTED; i++) {
		void* result_json;
		json.serialize(json.context, sensorData, &result_json);
		json.deserialize(json.context, result_json, &sensorDataTemp);
		json.freeobject(json.context, result_json);
	} */
	pthread_join(thread_emission, NULL);
	pthread_join(thread_reception, NULL);
	if (clock_gettime(clk_id, &stop) == -1) {
		perror("clock gettime stop");
		exit(EXIT_FAILURE);
	}
	timer_start = start.tv_sec * TIME_RESOLUTION + start.tv_nsec;
	timer_stop = stop.tv_sec * TIME_RESOLUTION + stop.tv_nsec;
	result_time_json = timer_stop - timer_start;

	if (verification(&sensorData, &sensorDataTemp)) {
		printf("output differs from input\n");
		err_json++;
	}

	printf("# JSON-C ");
#ifdef BENCH_JSON_ARRAY	
		printf("Array ");	
#endif
	printf(":\n");
	printResult(err_json, result_time_json);

//#endif // BENCH_JSON

#ifdef BENCH_FASTJSON
	// Initiate result values
	uint64_t result_time_fastjson;
	int err_fastjson = 0;
	int option_fastjson = MAP;

	Serializer fastjson;
	memset(&fastjson, 0, sizeof(fastjson));
	fastjson_get_serializer(&fastjson);

	#ifdef BENCH_FASTJSON_ARRAY	
		option_fastjson = ARRAY;
	#endif

	fastjson.context = &option_fastjson;
	if (clock_gettime(clk_id, &start) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < DATA_TESTED; i++) {
		void* result_fastjson;
		fastjson.serialize(fastjson.context, sensorData, &result_fastjson);
		fastjson.deserialize(fastjson.context, result_fastjson, &sensorDataTemp);
		fastjson.freeobject(fastjson.context, result_fastjson);
	}

	if (clock_gettime(clk_id, &stop) == -1) {
		perror("clock gettime stop");
		exit(EXIT_FAILURE);
	}

	fastjson.cleanup(fastjson.context);

	timer_start = start.tv_sec * TIME_RESOLUTION + start.tv_nsec;
	timer_stop = stop.tv_sec * TIME_RESOLUTION + stop.tv_nsec;
	result_time_fastjson = timer_stop - timer_start;

	if (verification(&sensorData, &sensorDataTemp)) {
		printf("output differs from input\n");
		err_fastjson++;
	}

	printf("# FASTJSON ");
#ifdef BENCH_FASTJSON_ARRAY	
		printf("Array ");	
#endif
	printf(":\n");
	printResult(err_fastjson, result_time_fastjson);

#endif // BENCH_FASTJSON

#ifdef BENCH_CBOR 
	// Initiate result values
	uint64_t result_time_cbor;
	int err_cbor = 0;
	int option_cbor = MAP;

	Serializer cborc;
	memset(&cborc, 0, sizeof(cborc));
	cborc_get_serializer(&cborc);
	
#ifdef BENCH_CBOR_ARRAY	
		option_cbor = ARRAY;	
#endif
	cborc.context = &option_cbor;
	//cborc.init(cborc.context);

	if (clock_gettime(clk_id, &start) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}
	void* result_cbor;
	for (int i = 0; i < DATA_TESTED; i++) {
		cborc.serialize(cborc.context, sensorData, &result_cbor);
		cborc.deserialize(cborc.context, result_cbor, &sensorDataTemp);
		cborc.freeobject(cborc.context, result_cbor);
	}
	

	if (clock_gettime(clk_id, &stop) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}

	timer_start = start.tv_sec * TIME_RESOLUTION + start.tv_nsec;
	timer_stop = stop.tv_sec * TIME_RESOLUTION + stop.tv_nsec;
	result_time_cbor = timer_stop - timer_start;

	if (verification(&sensorData, &sensorDataTemp)) {
		printf("output differs from input\n");
		err_cbor++;
	}
	printf("# CBOR");
#ifdef BENCH_CBOR_ARRAY	
		printf(" Array");	
#endif
	printf(":\n");
	printResult(err_cbor, result_time_cbor);

#endif // BENCH_CBOR

#ifdef BENCH_XDR
	// Initiate result values
	uint64_t result_time_xdr;
	int err_xdr = 0;

	XDR my_xdr;
	XDR* my_xdr_ptr = &my_xdr;
	char buffer_xdr[40 * 32];
	unsigned long len = 40 * 32;
	xdrmem_create(my_xdr_ptr, buffer_xdr, len, XDR_ENCODE); 

	Serializer xdr;
	memset(&xdr, 0, sizeof(xdr));
	xdr_get_serializer(&xdr);

	//void* result;
	//result = (void **)my_xdr_ptr;
	if (clock_gettime(clk_id, &start) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < DATA_TESTED; i++) {
		xdr.serialize(xdr.context, sensorData, (void **)my_xdr_ptr);
		xdr.deserialize(xdr.context, my_xdr_ptr, &sensorDataTemp);
	}
	if (clock_gettime(clk_id, &stop) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}

	timer_start = start.tv_sec * TIME_RESOLUTION + start.tv_nsec;
	timer_stop = stop.tv_sec * TIME_RESOLUTION + stop.tv_nsec;
	result_time_xdr = timer_stop - timer_start;

	if (verification(&sensorData, &sensorDataTemp)) {
		printf("output differs from input\n");
		err_xdr++;
	}
	printf("# XDR :\n");
	printResult(err_xdr, result_time_xdr);

#endif // BENCH_XDR

#ifdef BENCH_PROTOBUF
//protoc-c --c_out=. sensordata.proto

    uint8_t buf[1024];      // Buffer to store serialized data            
    size_t length;          // Length of serialized data
	uint64_t result_time_protobuf;
	int err_protobuf = 0;

	Serializer protobuf;
	memset(&protobuf, 0, sizeof(protobuf));
	protobuf_get_serializer(&protobuf);
	protobuf.context = &length;
	if (clock_gettime(clk_id, &start) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}

	void *result_protobuf = buf;
	for (int i = 0; i < DATA_TESTED; i++) {
		protobuf.serialize(protobuf.context, sensorData,(void **)result_protobuf);
		protobuf.deserialize(protobuf.context, result_protobuf, &sensorDataTemp);
		/*
        // Encode
        length = protobuf_serialize_sensorData(&sensorData, buf_ptr); 
        //Decode
        protobuf_deserialize_sensorData(buf_ptr, &sensorDataTemp, length);
		*/
	}

	if (clock_gettime(clk_id, &stop) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}
	timer_start = start.tv_sec * TIME_RESOLUTION + start.tv_nsec;
	timer_stop = stop.tv_sec * TIME_RESOLUTION + stop.tv_nsec;
	result_time_protobuf = timer_stop - timer_start;

	if (verification(&sensorData, &sensorDataTemp)) {
		printf("output differs from input\n");
		err_protobuf++;
	}
	printf("# PROTOBUF :\n");
	printResult(err_protobuf, result_time_protobuf);
#endif //BENCH_PROTOBUF

#if BENCH_DEBUG

#endif //BENCH_DEBUG

	return EXIT_SUCCESS;
}
