#include "config.h"
#include "struct-parsing.h"
#include "sp70c-data-handle.h"

#define SZ_BUFFER 3*14
#define TIME_RESOLUTION 1000000000ULL

void generateData(SensorData** sensorData_ptr)
{
	int buffer_size = SZ_BUFFER;
	uint8_t buffer[SZ_BUFFER] = {
		0xaa, 0xaa, 0x0a, 0x06, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x55,
		0xaa, 0xaa, 0x0b, 0x07, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x55,
		0xaa, 0xaa, 0x0c, 0x07, 0x01, 0x6b, 0x01, 0x86, 0x2a, 0x02, 0xbf, 0x79, 0x55, 0x55
	};
	for (int i = 0; i < DATA_TESTED; i++) {
		initData(sensorData_ptr[i]);
		parseRawBuffer(buffer, buffer_size, sensorData_ptr[i]);
	}
}

int verification(SensorData* sensorData_1, SensorData* sensorData_2)
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
	if (targetInfo_1.azimuth != targetInfo_2.azimuth)
		return 1;
	if (targetInfo_1.index != targetInfo_2.index)
		return 1;
	if (targetInfo_1.range != targetInfo_2.range)
		return 1;
	if (targetInfo_1.rcs != targetInfo_2.rcs)
		return 1;
	if (targetInfo_1.rollCount != targetInfo_2.rollCount)
		return 1;
	if (targetInfo_1.SNR != targetInfo_2.SNR)
		return 1;
	if (targetInfo_1.vrel != targetInfo_2.vrel)
		return 1;
	return 0;

}

void printResult(int err, double time)
{
	if (err == 0) {
		printf("\tData : OK\n");
		printf("\tTime during the serialization: %f ms\n", time / 1000000);
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
	SensorData** sensorData_ptr;
	sensorData_ptr = malloc(DATA_TESTED * sizeof(SensorData*));
	for (int i = 0; i < DATA_TESTED; i++) {
		sensorData_ptr[i] = malloc(sizeof(SensorData));
	}
	SensorData** sensorDataTemp_ptr;
	sensorDataTemp_ptr = malloc(DATA_TESTED * sizeof(SensorData*));
	for (int i = 0; i < DATA_TESTED; i++) {
		sensorDataTemp_ptr[i] = malloc(sizeof(SensorData));
		initData(sensorDataTemp_ptr[i]);
	}

	// Initiate values for our clock
	struct timespec start, stop;
	clockid_t clk_id;
	uint64_t timer_start;
	uint64_t timer_stop;
	clk_id = CLOCK_MONOTONIC;

	printf(" ====== BENCHMARKING DATA SERIALIZATION ====== \n");
	printf("data tested : %i\n", (int)DATA_TESTED);

	generateData(sensorData_ptr);

#ifdef BENCH_JSON
	// Initiate result values
	uint64_t result_time_json;
	int err_json = 0;

	struct json_object* my_json;

	if (clock_gettime(clk_id, &start) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < DATA_TESTED; i++) {
		my_json = json_object_new_object();
		parse_to_json(sensorData_ptr[i], my_json);
		json_to_sensorData(my_json, sensorDataTemp_ptr[i]);
		json_object_put(my_json);
	}

	if (clock_gettime(clk_id, &stop) == -1) {
		perror("clock gettime stop");
		exit(EXIT_FAILURE);
	}
	timer_start = start.tv_sec * TIME_RESOLUTION + start.tv_nsec;
	timer_stop = stop.tv_sec * TIME_RESOLUTION + stop.tv_nsec;
	result_time_json = timer_stop - timer_start;

	for (int i = 0; i < DATA_TESTED; i++) {
		if (verification(sensorData_ptr[i], sensorDataTemp_ptr[i])) {
			printf("err att : %i\n", i);
			err_json++;
		}
	}
	printf("# JSON :\n");
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
		parse_to_cbor(sensorData_ptr[i], my_cbor);
		cbor_to_sensorData(my_cbor, sensorDataTemp_ptr[i]);
		cbor_decref(&my_cbor);
	}

	if (clock_gettime(clk_id, &stop) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}
	timer_start = start.tv_sec * TIME_RESOLUTION + start.tv_nsec;
	timer_stop = stop.tv_sec * TIME_RESOLUTION + stop.tv_nsec;
	result_time_cbor = timer_stop - timer_start;

	for (int i = 0; i < DATA_TESTED; i++) {
		if (verification(sensorData_ptr[i], sensorDataTemp_ptr[i])) {
			printf("err att : %i\n", i);
			err_cbor++;
		}
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
		xdr_sensorData(sensorData_ptr[i], my_xdr_ptr);

		my_xdr_ptr->x_op = XDR_DECODE;
		xdr_setpos(my_xdr_ptr, 0);
		xdr_sensorData(sensorDataTemp_ptr[i], my_xdr_ptr);

	}
	if (clock_gettime(clk_id, &stop) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}

	timer_start = start.tv_sec * TIME_RESOLUTION + start.tv_nsec;
	timer_stop = stop.tv_sec * TIME_RESOLUTION + stop.tv_nsec;
	result_time_xdr = timer_stop - timer_start;

	for (int i = 0; i < DATA_TESTED; i++) {
		if (verification(sensorData_ptr[i], sensorDataTemp_ptr[i])) {
			printf("err att : %i\n", i);
			err_xdr++;
		}
	}
	printf("# XDR :\n");
	printResult(err_xdr, result_time_xdr);

#endif // BENCH_XDR

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

	for (int i = 0; i < DATA_TESTED; i++) {
		free(sensorData_ptr[i]);
		free(sensorDataTemp_ptr[i]);
	}
	free(sensorData_ptr);
	free(sensorDataTemp_ptr);

	return 0;
}
