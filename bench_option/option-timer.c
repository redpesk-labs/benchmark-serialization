#include "bench-option.h"
extern int DATA_TESTED;
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

int benchOptionTimer(SensorData sensorData, SensorData sensorDataTemp)
{
    printf("option:\t\t TIMER\n");
	printf("data tested :\t %i struct C\n", DATA_TESTED);

	/* Initiate values for our clock */
	struct timespec start, stop;
	clockid_t clk_id;
	uint64_t timer_start;
	uint64_t timer_stop;
	clk_id = CLOCK_MONOTONIC;

	/* Reference in C */
	int result_time_ref=0;
	int err_ref=0;
	int ret;
	char buffer_ref[1024];
	memset(&buffer_ref, 0, sizeof(sensorData));
	
 	if (clock_gettime(clk_id, &start) == -1) {
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
	printResult(err_ref, result_time_ref);
	memset(&sensorDataTemp, 0, sizeof(SensorData));


	
#ifdef BENCH_JSON
	// Initiate result values
	uint64_t result_time_json;
	int err_json = 0;
	int option_parse = MAP;

	Serializer json;
	memset(&json, 0, sizeof(json));
	jsonc_get_serializer(&json);

	#ifdef BENCH_JSON_ARRAY	
		option_parse = ARRAY;
	#endif

	json.context = &option_parse;
	if (clock_gettime(clk_id, &start) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}
 	for (int i = 0; i < DATA_TESTED; i++) {
		void* result_json;
		json.serialize(json.context, sensorData, &result_json);
		json.deserialize(json.context, result_json, &sensorDataTemp);
		json.freeobject(json.context, result_json);
	} 
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

#endif // BENCH_JSON

#ifdef BENCH_FASTJSON
	// Initiate result values
	uint64_t result_time_fastjson;
	int err_fastjson = 0;
	int option_parse_fastjson = MAP;

	Serializer fastjson;
	memset(&fastjson, 0, sizeof(fastjson));
	fastjson_get_serializer(&fastjson);

	#ifdef BENCH_FASTJSON_ARRAY	
		option_parse_fastjson = ARRAY;
	#endif

	fastjson.context = &option_parse_fastjson;
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

#ifdef BENCH_JSONSTRING

	uint64_t result_time_jsonstring;
	int err_jsonstring = 0;

	Serializer jsonstring;
	memset(&jsonstring, 0, sizeof(jsonstring));
	jsonstring_get_serializer(&jsonstring);

	if (clock_gettime(clk_id, &start) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}
	
	for (int i = 0; i < DATA_TESTED; i++) {
		void* result_jsonstring;
		jsonstring.serialize(jsonstring.context, sensorData, &result_jsonstring);
		jsonstring.deserialize(jsonstring.context, result_jsonstring, &sensorDataTemp);
		jsonstring.freeobject(jsonstring.context, result_jsonstring);
	}

	if (clock_gettime(clk_id, &stop) == -1) {
		perror("clock gettime stop");
		exit(EXIT_FAILURE);
	}

	jsonstring.cleanup(jsonstring.context);

	timer_start = start.tv_sec * TIME_RESOLUTION + start.tv_nsec;
	timer_stop = stop.tv_sec * TIME_RESOLUTION + stop.tv_nsec;
	result_time_jsonstring = timer_stop - timer_start;

	if (verification(&sensorData, &sensorDataTemp)) {
		printf("output differs from input\n");
		err_jsonstring++;
	}

	printf("# jsonstring ");
	printf(":\n");
	printResult(err_jsonstring, result_time_jsonstring);

#endif // BENCH_JSONSTRING

#ifdef BENCH_CBOR 
	// Initiate result values
	uint64_t result_time_cbor;
	int err_cbor = 0;
	int option_parse_cbor = MAP;

	Serializer cborc;
	memset(&cborc, 0, sizeof(cborc));
	cborc_get_serializer(&cborc);
	
#ifdef BENCH_CBOR_ARRAY	
		option_parse_cbor = ARRAY;	
#endif
	cborc.context = &option_parse_cbor;
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

}