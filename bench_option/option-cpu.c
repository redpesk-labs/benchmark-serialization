#include "bench-option.h"
#include "statgrab.h"

extern int DATA_TESTED;

sem_t mutexParse;
sem_t mutexData;
sem_t mutexEndParse;;
bool isFinished;
bool is_xdr;
bool is_protobuf;
char buffer[2048];
char bufferRef[2048];

//pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

typedef struct str_thdata
{
    Serializer* s;
    SensorData* dataToSerialize;
    SensorData* dataSerialized;
    void* buffer;
    int countSerialize;
    int countParse;
} thdata;

/// @brief Print process stats, usefull for debug.
/// @param[in] proc_stats The project stat of the library statgrab.
int show_proc_stats(sg_process_stats* proc_stats)
{
    printf("process name: %s\n",     proc_stats->process_name);
    printf("process title: %s\n",    proc_stats->proctitle);
    printf("pid: %i\n",              proc_stats->pid);
    printf("parent: %i\n",           proc_stats->parent);
    printf("pgid: %i\n",             proc_stats->pgid);
    printf("sessid: %i\n",           proc_stats->sessid);

    printf("uid: %d\n",              proc_stats->uid);
    printf("euid: %d\n",             proc_stats->euid);
    printf("gid: %d\n",              proc_stats->gid);
    printf("egid: %d\n",             proc_stats->egid);

    printf("start time: %ld\n",       proc_stats->start_time);
    printf("time spent: %ld\n",       proc_stats->time_spent);
    printf("cpu percent: %f\n",      proc_stats->cpu_percent);

    printf("nice: %i\n",             proc_stats->nice);
    printf("state: %i\n",            (int) proc_stats->state);

    return EXIT_SUCCESS;
}

/// @brief Print result of a specific bench.
/// @param[in] The percentage of data serialized.
/// @param[in] The percentage of data parsed.
/// @param[in] The percentage of CPU using by the user during all transaction.
void percentageResult(int dataSerialized, int dataParsed, float cpu)
{
	if (dataSerialized != 100 || dataParsed != 100) {
		printf("\t/!\\ Data : NOK\n");
		printf("\tpercentage of data serialized: %i %%\n", dataSerialized);
		printf("\tpercentage of data parsed: %i %%\n", dataParsed);
		printf("\tPercentage of CPU using : %f %%\n", cpu);
	}
	else {
		printf("\tData: OK\n");
		printf("\tPercentage of CPU using : %f %%\n", cpu);
	}
	printf("\n");
}

/// @brief thread function to serialize data
/// @param[in] input, pthread_data
static void* serialize(void* input)
{    
    // Set cancellation Disable
    int state;
    int semVal;
    state = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    if (state != 0)
        perror("pthread_setcancellation");
    
    // Initialize data to serialize
    ((thdata*) input)->countSerialize = 0;
    Serializer *s = ((thdata* )input)->s;
    SensorData* data = ((thdata*) input)->dataToSerialize;
    void* result;

    while (!isFinished)
    {  
        // Set cancellation Enable
        state = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        if (state != 0)
            perror("pthread_setcancellation");
        // Wait data available
        sem_wait(&mutexEndParse);
        sem_wait(&mutexData);
        // Set cancellation Enable
        state = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        if (state != 0)
            perror("pthread_setcancellation");

        /* Serialization of the data */
        //pthread_mutex_lock(&mut);
        if (is_xdr || is_protobuf) {
            ((thdata* )input)->s->serialize(((thdata* )input)->s->context, data, (((thdata*) input)->buffer));
        }else {
            ((thdata* )input)->s->serialize(((thdata* )input)->s->context, data, &(((thdata*) input)->buffer));
        }
        ((thdata*) input)->countSerialize ++;
        //pthread_mutex_unlock(&mut);
        sem_post(&mutexParse);
        usleep(1);  
    }
    pthread_exit(EXIT_SUCCESS);
}

/// @brief thread function to parse data
/// @param[in] input, pthread_data
void* parse(void* input)
{	
    // Set cancellation Disable
    int state;
    state = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    if (state != 0)
        perror("pthread_setcancellation");
    // Initialize data to parse
    ((thdata*) input)->countParse = 0;
    Serializer* s = ((thdata* )input)->s;

    while (!isFinished) {
        // Set cancellation Enable
        state = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        if (state != 0)
            perror("pthread_setcancellation");
        // Wait data serialized available
        sem_wait(&mutexParse);
        // Set cancellation Enable
        state = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        if (state != 0)
            perror("pthread_setcancellation");
        /* Parsing of he data */
        //pthread_mutex_lock(&mut);
        ((thdata* )input)->s->deserialize(((thdata* )input)->s->context, ((thdata*) input)->buffer, ((thdata*) input)->dataSerialized);
        ((thdata* )input)->s->freeobject(((thdata* )input)->s->context, ((thdata*) input)->buffer);
        //pthread_mutex_unlock(&mut);
        sem_post(&mutexEndParse);
        usleep(1);
        ((thdata*) input)->countParse++;
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
    memset(&bufferRef, 0, sizeof(bufferRef));

    int ret;
    int index;
    int state;
    void* res;
    int resultParse;
    int resultSerialize;
    thdata dataThread;
    is_xdr = false;
    is_protobuf = false;

    float percentage = 0;
    double count = 0;

    /* Initiate value for stats proc */
    size_t entries;
    memset(&entries, 0, sizeof(size_t));
    sg_cpu_stats* cpu_stats ;
    sg_cpu_percents* cpu_percents;
    //memset(cpu_stats, 0, sizeof(sg_cpu_stats));
    sg_cpu_percent_source cps;
    memset(&cps, 0, sizeof(sg_cpu_percent_source));

	/* Initiate 2 threads, for emission and reception */
	pthread_t thread_emission;
	pthread_t thread_reception;

	/* Initiate the semaphore mutex */
	sem_init(&mutexParse, 0, 0);
    sem_init(&mutexData, 0, 0);
    sem_init(&mutexEndParse, 0, 1);

    /***************************************************/
    /****************REFERENCE C************************/
    Serializer c;
	memset(&c, 0, sizeof(c));
	c_get_serializer(&c);

    /* Initiate data to thread */
    dataThread.s = &c;
    dataThread.buffer = bufferRef;
    dataThread.dataToSerialize = &sensorData;
    dataThread.dataSerialized = &sensorDataTemp;

    isFinished = false;
    /* Thread creation */
	ret = pthread_create(&thread_emission, NULL, serialize, (void*) &dataThread);
	if(ret){
		perror("pthread_emission_create");
	}
	ret = pthread_create(&thread_reception, NULL, parse, (void*) &dataThread);
	if(ret){
		perror("pthread_reception_create");
	}

    /* catch cpu percentage during 2 process */
    sg_init(1);	
    cpu_stats = sg_get_cpu_stats(&entries);
        sem_post(&mutexData);
    for (index=1; index<DATA_TESTED; index++) {
        usleep(freq);
        sem_post(&mutexData);
    }
    isFinished = true;
    cpu_stats = sg_get_cpu_stats_diff(&entries);
    cpu_percents = sg_get_cpu_percents_r(cpu_stats, &entries);
    percentage = cpu_percents->user;
    sg_free_mem_stats(cpu_percents);
    sg_shutdown();


    state = pthread_cancel(thread_emission);
    /* Wait thread to finish */   
    state = pthread_join(thread_emission, &res);
    if (state != 0)
        perror("pthread join");
    state = pthread_join(thread_emission, NULL);

    state = pthread_cancel(thread_reception);
    /* Wait thread to finish */   
    state = pthread_join(thread_reception, &res);
    if (state != 0)
        perror("thread join");
    state = pthread_join(thread_reception, NULL);

    /* Verification of parsing data */
    if (memcmp(&sensorData, &sensorDataTemp, sizeof(SensorData))) printf("Error copy\n");
	memset(&sensorDataTemp, 0, sizeof(SensorData));

    /* Print Result */
    resultSerialize = (int) (dataThread.countSerialize*100)/DATA_TESTED;
    resultParse = (int) (dataThread.countParse*100)/DATA_TESTED;
    printf("# REFERENCE C :\n");
    percentageResult(resultSerialize, resultParse, percentage);

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

    /* Initiate data to thread */
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

    /* Initiate data to thread */
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

    /* Initiate data to thread */
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

    /* Initiate data to thread */
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

    /* Initiate data to thread */
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

    /* Initiate data to thread */
    dataThread.s = &protobuf;
    dataThread.buffer = result_protobuf;
    dataThread.dataToSerialize = &sensorData;
    dataThread.dataSerialized = &sensorDataTemp;
    #endif // BENCH_PROTOBUF

    /* Thread creation */
    isFinished = false;
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

    /* catch cpu percentage during 2 process */
    sg_init(1);	
    cpu_stats = sg_get_cpu_stats(&entries);
    for (index=0; index<DATA_TESTED; index++) {
        sem_post(&mutexData);
        usleep(freq);
    }
    isFinished = true;
    cpu_stats = sg_get_cpu_stats_diff(&entries);
    cpu_percents = sg_get_cpu_percents_r(cpu_stats, &entries);
    percentage = cpu_percents->user;
    sg_free_mem_stats(cpu_percents);
    sg_shutdown();

    /* Wait/inform emission thread to finish */
    state = pthread_cancel(thread_emission);
    state = pthread_join(thread_emission, NULL);
    if (state != 0)
        perror("pthread join");

    /* Wait/inform emission thread to finish */
    state = pthread_cancel(thread_reception);
    state = pthread_join(thread_reception, NULL);
    if (state != 0)
        perror("thread join"); 

    /* Verification of parsing data */
    if (memcmp(&sensorData, &sensorDataTemp, sizeof(SensorData))) printf("Error copy\n");
	memset(&sensorDataTemp, 0, sizeof(SensorData));

    /* Print Result */
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
    percentageResult(resultSerialize, resultParse, percentage);

    return EXIT_SUCCESS;
}