#include "config.h"
#include "data.h"
#include "sp70c-data-handle.h"
#include "string.h"

#define TIME_RESOLUTION 1000000000ULL

void generateData(SensorData* sensorData_ptr)
{
	parseRawBuffer(sensorData_ptr);
}

int verification(SensorData* sd1, SensorData* sd2)
{
	return memcmp(sd1, sd2, sizeof(SensorData *));
}

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

	printf(" ====== BENCHMARKING DATA SERIALIZATION ====== \n");
	printf("data tested : %i\n", (int)DATA_TESTED);

	generateData(&sensorData);

#ifdef BENCH_JSON
	// Initiate result values
	uint64_t result_time_json;
	int err_json = 0;
	int option = MAP;

	Serializer json;
	memset(&json, 0, sizeof(json));
	jsonc_get_serializer(&json);

	#ifdef BENCH_JSON_ARRAY	
		option = ARRAY;
	#endif

	json.context = &option;
	if (clock_gettime(clk_id, &start) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < DATA_TESTED; i++) {
		void* result;
		json.serialize(json.context, sensorData, &result);
		json.deserialize(json.context, result, &sensorDataTemp);
		json.freeobject(json.context, result);
	}

	if (clock_gettime(clk_id, &stop) == -1) {
		perror("clock gettime stop");
		exit(EXIT_FAILURE);
	}

	json.cleanup(json.context);

	timer_start = start.tv_sec * TIME_RESOLUTION + start.tv_nsec;
	timer_stop = stop.tv_sec * TIME_RESOLUTION + stop.tv_nsec;
	result_time_json = timer_stop - timer_start;

	if (verification(&sensorData, &sensorDataTemp)) {
		printf("output differs from input\n");
		err_json++;
	}

	printf("# JSON ");
#ifdef BENCH_JSON_ARRAY	
		printf("Array ");	
#endif
	printf(":\n");
	printResult(err_json, result_time_json);

#endif // BENCH_JSON

#ifdef BENCH_CBOR 
	// Initiate result values
	uint64_t result_time_cbor;
	int err_cbor = 0;
	cbor_item_t* my_cbor;

	if (clock_gettime(clk_id, &start) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < DATA_TESTED; i++) {
		my_cbor = cbor_new_definite_map(4);
		parse_to_cbor(&sensorData, my_cbor);
		cbor_to_sensorData(my_cbor, &sensorDataTemp);
		cbor_decref(&my_cbor);
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
	printf("# CBOR :\n");
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

	if (clock_gettime(clk_id, &start) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < DATA_TESTED; i++) {

		my_xdr_ptr->x_op = XDR_ENCODE;
		xdr_setpos(my_xdr_ptr, 0);
		xdr_sensorData(&sensorData, my_xdr_ptr);

		my_xdr_ptr->x_op = XDR_DECODE;
		xdr_setpos(my_xdr_ptr, 0);
		xdr_sensorData(&sensorDataTemp, my_xdr_ptr);

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
    uint8_t buf[1024];      // Buffer to store serialized data
    uint8_t *buf_ptr = buf;             
    size_t length;          // Length of serialized data
    SensorDataMessage *msg;
	uint64_t result_time_protobuf;
	int err_protobuf = 0;

	if (clock_gettime(clk_id, &start) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < DATA_TESTED; i++) {
        // Encode
        length = parse_to_protobuf(&sensorData, buf_ptr); 
        //Decode
        protobuf_to_sensorData(buf_ptr, &sensorDataTemp, length);
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
#endif

#if BENCH_DEBUG
	float floatToParse_1 = 5.0;
	float floatToParse_2 = 10.0;
	float floatParsed_1 = 0.0;
	float floatParsed_2 = 0.0;
	unsigned int len = 40 * 32;
	char buffer_xdr[len];

	XDR my_xdr;
	XDR* my_xdr_ptr = &my_xdr;
	int err = 0;
	SensorVersion* test_struct, * temp_struct;
	test_struct = malloc(sizeof(SensorVersion));
	temp_struct = malloc(sizeof(SensorVersion));
	test_struct->dataType = 1;
	test_struct->result = 1;
	test_struct->master = 2;
	test_struct->second = 3;
	test_struct->step = 4;

	temp_struct->dataType = 0;
	temp_struct->result = 0;
	temp_struct->master = 0;
	temp_struct->second = 0;
	temp_struct->step = 0;

	xdrmem_create(my_xdr_ptr, buffer_xdr, len, XDR_ENCODE);
	err = addSensorVersionToXdr(my_xdr_ptr, test_struct);
	if (err == 0)
		printf("Error au parsing\n");
	err = 0;
	printf("Position: %i\n", xdr_getpos(my_xdr_ptr));
	/* xdr_uint8_t(&my_xdr, &test_struct.dataType);
	bool_t temp = test_struct.result;
	xdr_bool(&my_xdr, &temp);
	xdr_uint8_t(&my_xdr, &test_struct.master);
	xdr_uint8_t(&my_xdr, &test_struct.second);
	xdr_uint8_t(&my_xdr, &test_struct.step); */

	my_xdr_ptr->x_op = XDR_DECODE;
	xdr_setpos(my_xdr_ptr, 0);
	err = addSensorVersionToXdr(my_xdr_ptr, temp_struct);
	if (err == 0)
		printf("Error au dé-parsing\n");
	err = 0;
	/* xdr_uint8_t(&my_xdr, &temp_struct.dataType);
	xdr_bool(&my_xdr, &temp);
	temp_struct.result = temp;
	xdr_uint8_t(&my_xdr, &temp_struct.master);
	xdr_uint8_t(&my_xdr, &temp_struct.second);
	xdr_uint8_t(&my_xdr, &temp_struct.step); */

	printf("dataType : %i\n", temp_struct->dataType);
	printf("result : %i\n", temp_struct->result);
	printf("master : %i\n", temp_struct->master);
	printf("second : %i\n", temp_struct->second);
	printf("step : %i\n", temp_struct->step);


#endif
	return 0;
}
