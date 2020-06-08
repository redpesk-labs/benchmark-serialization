#include "bench-option.h"

extern int DATA_TESTED;

sem_t mutexParse;
sem_t mutexData;
sem_t mutexEndParse;
pthread_mutex_t condMutex = PTHREAD_MUTEX_INITIALIZER;
bool is_finished;
bool is_xdr;
bool is_ref;
bool is_protobuf;
char buffer[2048];

typedef struct str_thdata
{
    Serializer* s;
    SensorData* dataToSerialize;
    SensorData* dataSerialized;
    void* buffer;
    int countSerialize;
    int countParse;
} thdata;



/// @brief thread function to serialize data
/// @param[in] input, pthread_data
static void* serialize(void* input)
{    
    // Set cancellation Disable
    int state;
    int semVal;
    bool endBench = false;
    
    // Initialize data to serialize:
    ((thdata*) input)->countSerialize = 0;
    Serializer *s = ((thdata* )input)->s;
    SensorData* data;

    while (1)
    {  
        // Wait previous data parsed:
        //printf("Waiting end of Parsing\n");
        sem_wait(&mutexEndParse);
        // Wait data to serilized:
        //printf("Waiting Data\n");
        sem_wait(&mutexData);

        pthread_mutex_lock(&condMutex);
        data = ((thdata*) input)->dataToSerialize;
        pthread_mutex_unlock(&condMutex);

        // Serialization of the data:
        if (is_xdr || is_protobuf) {
            s->serialize(s->context, data, (((thdata*) input)->buffer));
        }else if (is_ref) { // Passage du pointeur
            (((thdata*) input)->buffer) = data;
        }else {
            s->serialize(s->context, data, &(((thdata*) input)->buffer));
        }
        ((thdata*) input)->countSerialize ++;

        //printf("End Serialization\n");
        sem_post(&mutexParse);

        pthread_mutex_lock(&condMutex);
        //endBench = ((thdata* )input)->endBench;
        endBench = is_finished;
        pthread_mutex_unlock(&condMutex);
        if (endBench){
            //printf("Bench end: Serialization\n");
            break;
        }
        usleep(1);  
    }
    pthread_exit(EXIT_SUCCESS);
}

/// @brief thread function to parse data
/// @param[in] input, pthread_data
void* parse(void* input)
{	
    int state;
    bool endBench = false;
    SensorData* result;
    // Initialize data to parse:
    ((thdata*) input)->countParse = 0;
    Serializer* s = ((thdata* )input)->s;

    while (1) {
        // Wait data serialized available:
        //printf("Wait data to parse\n");
        sem_wait(&mutexParse);

/*         pthread_mutex_lock(&condMutex);
        result = (((thdata*) input)->buffer);
        pthread_mutex_unlock(&condMutex); */

        // Parsing of he data:
        if (is_ref) { // Passage du pointeur
            ((thdata*) input)->dataSerialized = ((thdata*) input)->buffer;
            ((thdata*) input)->countParse++;
        } else {
            s->deserialize(s->context, ((thdata*) input)->buffer, ((thdata*) input)->dataSerialized);
            s->freeobject(s->context, ((thdata*) input)->buffer);
            ((thdata*) input)->countParse++;
        }
        //printf("end of parsing\n");
        sem_post(&mutexEndParse);

        pthread_mutex_lock(&condMutex);
        //endBench = ((thdata* )input)->endBench;
        endBench = is_finished;
        pthread_mutex_unlock(&condMutex);
        if (endBench){
            //printf("Bench end: Parsing\n");
            break;
        }
        usleep(1);
    }    
    
    pthread_exit(EXIT_SUCCESS);
}

/// @brief Main function of the cpu percentage option
/// @param[in] sensorData data, to serialized
/// @param[in] sensorDataTemp, Data to parse
int benchOptionCpu(SensorData sensorData, SensorData sensorDataTemp, int freq)
{
    printf("option:\t\t CPU\n");
	printf("data tested :\t %i struct C\n", DATA_TESTED);
	printf("freq chosen :\t %i µs\n\n", freq);

    int ret;
    int index;
    int state;
    void* res;
    int resultParse;
    int resultSerialize;
    thdata dataThread;
    is_xdr = false;
    is_protobuf = false;
    is_finished = false;

    float percentage = 0;
    double count = 0;

    // Initiate value for stats proc:
    size_t entries;
    memset(&entries, 0, sizeof(size_t));
    sg_cpu_stats* cpu_stats ;
    sg_cpu_percents* cpu_percents;
    sg_cpu_percent_source cps;
    memset(&cps, 0, sizeof(sg_cpu_percent_source));

	// Initiate 2 threads, for emission and reception:
	pthread_t thread_emission;
	pthread_t thread_reception;

	// Initiate the semaphore mutex:
	sem_init(&mutexParse, 0, 0);
    sem_init(&mutexData, 0, 0);
    sem_init(&mutexEndParse, 0, 1);

    /***************************************************/
    /****************REFERENCE C************************/
    is_ref = true;
    SensorData data_temp;

    Serializer c;
	memset(&c, 0, sizeof(c));
	c_get_serializer(&c);

    // Initiate data to thread:
    dataThread.s = &c;
    dataThread.buffer = &data_temp;
    dataThread.dataToSerialize = &sensorData;
    dataThread.dataSerialized = &sensorDataTemp;

    pthread_mutex_lock(&condMutex);
    is_finished = false;
    pthread_mutex_unlock(&condMutex);

    // Thread creation:
	ret = pthread_create(&thread_emission, NULL, serialize, (void*) &dataThread);
	if(ret){
		perror("pthread_emission_create");
	}
	ret = pthread_create(&thread_reception, NULL, parse, (void*) &dataThread);
	if(ret){
		perror("pthread_reception_create");
	}

    // catch cpu percentage during 2 process:
    sg_init(1);	
    cpu_stats = sg_get_cpu_stats(&entries);
    //sem_post(&mutexData);
    for (index = 0; index < DATA_TESTED - 1; index++) {
        pthread_mutex_lock(&condMutex);
		generateData(&sensorData);
        pthread_mutex_unlock(&condMutex);  
        sem_post(&mutexData);
        usleep(freq);
    }

    pthread_mutex_lock(&condMutex);
    is_finished = true;
    pthread_mutex_unlock(&condMutex);

    usleep(freq);
    //printf("LAST MUTEX DATA SEND\n");
    sem_post(&mutexData);

    state = pthread_join(thread_emission, NULL);
    state = pthread_join(thread_reception, NULL);

    cpu_stats = sg_get_cpu_stats_diff(&entries);
    cpu_percents = sg_get_cpu_percents_r(cpu_stats, &entries);
    percentage = cpu_percents->user;
    sg_free_mem_stats(cpu_percents);
    sg_shutdown();


    sensorDataTemp = *dataThread.dataSerialized;
    // Verification of parsing data:
    if (memcmp(&sensorData, &sensorDataTemp, sizeof(SensorData))) printf("Error copy\n");
	memset(&sensorDataTemp, 0, sizeof(SensorData));

    // Print Result:
    resultSerialize = (int) (dataThread.countSerialize*100)/DATA_TESTED;
    resultParse = (int) (dataThread.countParse*100)/DATA_TESTED;
    printf("# REFERENCE C :\n");
    printResultCPU(resultSerialize, resultParse, percentage);

    is_ref=false;
    /***************************************************/
    /********************TECHNO TO TEST*****************/

	memset(&buffer, 0, sizeof(buffer));

    #ifdef BENCH_JSON
	int option_parse = MAP;	

	Serializer json;
	memset(&json, 0, sizeof(json));
	jsonc_get_serializer(&json);

	#ifdef BENCH_JSON_ARRAY	
		option_parse = ARRAY;
	#endif
	json.context = &option_parse;

    // Initiate data to thread:
    dataThread.s = &json;
    dataThread.buffer = buffer;
    dataThread.dataToSerialize = &sensorData;
    dataThread.dataSerialized = &sensorDataTemp;

    #endif // BENCH_JSON

    #ifdef BENCH_FASTJSON
	int option_parse = MAP;	

	Serializer fastjson;
	memset(&fastjson, 0, sizeof(fastjson));
	fastjson_get_serializer(&fastjson);

	#ifdef BENCH_FASTJSON_ARRAY	
		option_parse = ARRAY;
	#endif
	fastjson.context = &option_parse;

    // Initiate data to thread:
    dataThread.s = &fastjson;
    dataThread.buffer = buffer;
    dataThread.dataToSerialize = &sensorData;
    dataThread.dataSerialized = &sensorDataTemp;
    #endif // BENCH_FASTJSON

    #ifdef BENCH_JSONSTRING
	int option_parse = MAP;	

	Serializer jsonstring;
	memset(&jsonstring, 0, sizeof(jsonstring));
	jsonstring_get_serializer(&jsonstring);
    #ifdef BENCH_JSONSTRING_ARRAY	
		option_parse = ARRAY;	
    #endif
	jsonstring.context = &option_parse;

    // Initiate data to thread:
    dataThread.s = &jsonstring;
    dataThread.buffer = buffer;
    dataThread.dataToSerialize = &sensorData;
    dataThread.dataSerialized = &sensorDataTemp;
    #endif // BENCH_JSONSTRING    

    #ifdef BENCH_CBOR
	int option_parse = MAP;	

	Serializer cborc;
	memset(&cborc, 0, sizeof(cborc));
	cborc_get_serializer(&cborc);

    #ifdef BENCH_CBOR_ARRAY	
		option_parse = ARRAY;	
    #endif
	cborc.context = &option_parse;

    // Initiate data to thread:
    dataThread.s = &cborc;
    dataThread.buffer = buffer;
    dataThread.dataToSerialize = &sensorData;
    dataThread.dataSerialized = &sensorDataTemp;
    #endif // BENCH_CBOR   

    #ifdef BENCH_XDR
    is_xdr = true;
	int option_parse = MAP;	

	XDR my_xdr;
	XDR* my_xdr_ptr = &my_xdr;
	char buffer_xdr[1280];
	unsigned long len =1280;
	xdrmem_create(&my_xdr, buffer, 2048, XDR_ENCODE);

	Serializer xdr;
	memset(&xdr, 0, sizeof(xdr));
	xdr_get_serializer(&xdr);

	xdr.context = &option_parse;

    // Initiate data to thread:
    dataThread.s = &xdr;
    dataThread.buffer = my_xdr_ptr;
    dataThread.dataToSerialize = &sensorData;
    dataThread.dataSerialized = &sensorDataTemp;
    #endif // BENCH_XDR

    #ifdef BENCH_PROTOBUF
    is_protobuf = true;
    uint8_t buf[1024]; 
	void *result_protobuf = buf;
    size_t length;  
	Serializer protobuf;
	memset(&protobuf, 0, sizeof(protobuf));
	protobuf_get_serializer(&protobuf);

	protobuf.context = &length;

    // Initiate data to thread:
    dataThread.s = &protobuf;
    dataThread.buffer = result_protobuf;
    dataThread.dataToSerialize = &sensorData;
    dataThread.dataSerialized = &sensorDataTemp;
    #endif // BENCH_PROTOBUF

    // Thread creation:
    pthread_mutex_lock(&condMutex);
    is_finished = false;
    pthread_mutex_unlock(&condMutex);

	sem_init(&mutexParse, 0, 0);
    sem_init(&mutexData, 0, 0);

	ret = pthread_create(&thread_emission, NULL, serialize, (void*) &dataThread);
	if(ret){
		perror("pthread_emission_create");
	}
	ret = pthread_create(&thread_reception, NULL, parse, (void*) &dataThread);
	if(ret){
		perror("pthread_reception_create");
	}

    // catch cpu percentage during 2 process:
    sg_init(1);	
    cpu_stats = sg_get_cpu_stats(&entries);
    for (index=0; index<DATA_TESTED-1; index++) {

        pthread_mutex_lock(&condMutex);
		generateData(&sensorData);
        pthread_mutex_unlock(&condMutex);

        sem_post(&mutexData);
        usleep(freq);
    }

    pthread_mutex_lock(&condMutex);
    is_finished = true;
    pthread_mutex_unlock(&condMutex);
    usleep(freq);
    sem_post(&mutexData);

    cpu_stats = sg_get_cpu_stats_diff(&entries);
    cpu_percents = sg_get_cpu_percents_r(cpu_stats, &entries);
    percentage = cpu_percents->user;
    sg_free_mem_stats(cpu_percents);
    sg_shutdown();

    pthread_join(thread_emission, NULL);
    pthread_join(thread_reception, NULL);

    // Verification of parsing data:
    if (memcmp(&sensorData, &sensorDataTemp, sizeof(SensorData))) printf("Error copy\n");
	memset(&sensorDataTemp, 0, sizeof(SensorData));

    // Print Result:
    resultSerialize = (int) (dataThread.countSerialize*100)/DATA_TESTED;
    resultParse = (int) (dataThread.countParse*100)/DATA_TESTED;

    #ifdef BENCH_JSON
	printf("# JSON-C ");
    #ifdef BENCH_JSON_ARRAY	
		printf("Array ");	
    #endif
	printf(":\n");
    #endif // BENCH_JSON

    #ifdef BENCH_FASTJSON
	printf("# FASTJSON ");
    #ifdef BENCH_FASTJSON_ARRAY	
		printf("Array ");	
    #endif
	printf(":\n");
    #endif // BENCH_FASTJSON

    #ifdef BENCH_JSONSTRING
	printf("# JSONSTRING ");
    #ifdef BENCH_JSONSTRING_ARRAY	
		printf("Array ");
	#endif
	printf(":\n");
    #endif // BENCH_JSONSTRING

    #ifdef BENCH_CBOR
	printf("# CBOR ");
    #ifdef BENCH_CBOR_ARRAY	
		printf("ARRAY ");	
    #endif
	printf(":\n");
    #endif // BENCH_CBOR 

    #ifdef BENCH_XDR
	printf("# XDR ");
	printf(":\n");
    #endif // BENCH_JSONSTRING

    #ifdef BENCH_PROTOBUF
	printf("# PROTOBUF ");
	printf(":\n");
    #endif // BENCH_PROTOBUF
    printResultCPU(resultSerialize, resultParse, percentage);

    return EXIT_SUCCESS;
}