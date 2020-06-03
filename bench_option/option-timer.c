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
	printf("target info size : %02x", sd->tInfoSize);
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
	printf("data tested :\t %i struct C\n\n", DATA_TESTED);

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
	SensorData *ptr;
	memset(&buffer_ref, 0, sizeof(sensorData));
	
 	if (clock_gettime(clk_id, &start) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < DATA_TESTED; i++) {
		ptr = &sensorData;
		sensorDataTemp = *ptr;
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

	// Initiate result values
	uint64_t result_time;
	int err = 0;	
	Serializer s;
	memset(&s, 0, sizeof(s));
	int option_parse = MAP;

#ifdef BENCH_JSON
	jsonc_get_serializer(&s);
	#ifdef BENCH_JSON_ARRAY	
		option_parse = ARRAY;
	#endif
	s.context = &option_parse;
#endif // BENCH_JSON

#ifdef BENCH_FASTJSON
	fastjson_get_serializer(&s);
	#ifdef BENCH_FASTJSON_ARRAY	
		option_parse = ARRAY;
	#endif
	s.context = &option_parse;
#endif //BENCH_FAST_JSON

#ifdef BENCH_JSONSTRING
	jsonstring_get_serializer(&s);
	#ifdef BENCH_JSONSTRING_ARRAY	
		option_parse = ARRAY;
	#endif
	s.context = &option_parse;
#endif //BENCH_FAST_JSON

#ifdef BENCH_CBOR
	cborc_get_serializer(&s);
	#ifdef BENCH_CBOR_ARRAY	
		option_parse = ARRAY;
	#endif
	s.context = &option_parse;
#endif // BENCH_CBOR

#ifdef BENCH_XDR
	XDR my_xdr;
	XDR* result = &my_xdr;
	char buffer_xdr[40 * 32];
	unsigned long len = 40 * 32;
	xdrmem_create(result, buffer_xdr, len, XDR_ENCODE); 

	xdr_get_serializer(&s);
#endif // BENCH_XDR

#ifdef BENCH_PROTOBUF
//protoc-c --c_out=. sensordata.proto

    uint8_t buf[1024];      // Buffer to store serialized data            
    size_t length;          // Length of serialized data
	protobuf_get_serializer(&s);
	s.context = &length;
	void* result = buf;
#endif


	if (clock_gettime(clk_id, &start) == -1) {
		perror("clock gettime start");
		exit(EXIT_FAILURE);
	}
 	for (int i = 0; i < DATA_TESTED; i++) {
		#if defined(BENCH_XDR) || defined(BENCH_PROTOBUF)
		s.serialize(s.context, &sensorData, (void **)result);
		s.deserialize(s.context, result, &sensorDataTemp);
		#else
		void* result;
		s.serialize(s.context, &sensorData, &result);
		//s.print(s.context, result);
		s.deserialize(s.context, result, &sensorDataTemp);
		s.freeobject(s.context, result);
		#endif

	} 
	if (clock_gettime(clk_id, &stop) == -1) {
		perror("clock gettime stop");
		exit(EXIT_FAILURE);
	}
	timer_start = start.tv_sec * TIME_RESOLUTION + start.tv_nsec;
	timer_stop = stop.tv_sec * TIME_RESOLUTION + stop.tv_nsec;
	result_time = timer_stop - timer_start;

	if (verification(&sensorData, &sensorDataTemp)) {
		printf("output differs from input\n");
		err++;
	}

#ifdef BENCH_JSON
	printf("## JSON-C ");
	#ifdef BENCH_JSON_ARRAY	
			printf("Array ");	
	#endif
#endif

#ifdef BENCH_FASTJSON
	printf("## FASTJSON ");
	#ifdef BENCH_FASTJSON_ARRAY	
		printf("Array ");	
	#endif
#endif

#ifdef BENCH_JSONSTRING
	printf("## JSONSTRING ");	
	#ifdef BENCH_JSONSTRING_ARRAY	
		printf("array ");
	#endif
#endif

#ifdef BENCH_CBOR
	printf("## CBOR ");	
	#ifdef BENCH_CBOR_ARRAY	
		printf("array ");
	#endif
#endif

#ifdef BENCH_XDR
	printf("## XDR");
#endif

#ifdef BENCH_PROTOBUF
	printf("## PROTOBUF ");
#endif

	printf(":\n");
	printResult(err, result_time);

#if BENCH_DEBUG
	printf("Practice makes progress, not perfection.\n");
#endif //BENCH_DEBUG
}