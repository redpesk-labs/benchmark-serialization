#include "config.h"
#include "data.h"

#include <iostream>

#define TIME_RESOLUTION 1000000000ULL

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

	//Generate data to parse
	generateData(&sensorData);

	printf(" ====== BENCHMARKING DATA SERIALIZATION ====== \n\n");
	printf("data tested : %i\n\n", (int)DATA_TESTED);
#ifdef BENCH_JSONCPP
	// Initiate result values
	uint64_t result_time_jsoncpp;
	int err_jsoncpp = 0;
	int option = MAP;

	Serializer jsoncpp;
	memset(&jsoncpp, 0, sizeof(jsoncpp));
	ParseJsonCpp parseJsonCpp_object;
    parseJsonCpp_object.jsoncpp_get_serializer(&jsoncpp);

#ifdef BENCH_JSONCPP_ARRAY	
		option = ARRAY;
#endif

	jsoncpp.context = &option;
	if (clock_gettime(clk_id, &start) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}
	void* result_json;
	memset(&result_json, 0, sizeof(nlohmann::json));
	for (int i = 0; i < DATA_TESTED; i++) {
		
		jsoncpp.serialize(jsoncpp.context, sensorData, &result_json);
		jsoncpp.deserialize(jsoncpp.context, &result_json, &sensorDataTemp);
		jsoncpp.freeobject(jsoncpp.context, &result_json);
	}

	if (clock_gettime(clk_id, &stop) == -1) {
		perror("clock gettime stop");
		exit(EXIT_FAILURE);
	}

	jsoncpp.cleanup(jsoncpp.context);

	timer_start = start.tv_sec * TIME_RESOLUTION + start.tv_nsec;
	timer_stop = stop.tv_sec * TIME_RESOLUTION + stop.tv_nsec;
	result_time_jsoncpp = timer_stop - timer_start;

	if (verification(&sensorData, &sensorDataTemp)) {
		printf("output differs from input\n");
		err_jsoncpp++;
	}

	printf("# JSON-CPP ");
#ifdef BENCH_JSONCPP_ARRAY	
		printf("Array ");	
#endif
	printf(":\n");
	printResult(err_jsoncpp, result_time_jsoncpp);

#endif // BENCH_JSONCPP

}