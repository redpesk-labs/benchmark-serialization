#include "bench-option.h"
#include "statgrab.h"

extern int DATA_TESTED;

sem_t mutexParse;
sem_t mutexData;
bool isFinished;
char buffer[2048];

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

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
		printf("\tData : NOK\n");
		printf("\tpercentage of data serialized: %i %%\n", dataSerialized);
		printf("\tpercentage of data parsed: %i %%\n", dataParsed);
	}
	else {
		printf("\tData: OK\n");
		printf("\tPercentage of CPU using : %f %%", cpu);
	}
	printf("\n\n");
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
    SensorData data = *((thdata*) input)->dataToSerialize;
    void* result;

    while (!isFinished)
    {  
        // Set cancellation Enable
        state = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        if (state != 0)
            perror("pthread_setcancellation");
        // Wait data available
        sem_wait(&mutexData);
        // Set cancellation Enable
        state = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        if (state != 0)
            perror("pthread_setcancellation");

        /* Serialization of the data */
        ((thdata* )input)->s->serialize(((thdata* )input)->s->context, data, &result);
        ((thdata*) input)->countSerialize ++;
        pthread_mutex_lock(&mut);
        (((thdata*) input)->buffer) = (char* )result;
        pthread_mutex_unlock(&mut);

        sem_getvalue(&mutexParse, &semVal);
        if (semVal == 0)
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
        // Wait data serilized available
        sem_wait(&mutexParse);
        // Set cancellation Enable
        state = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        if (state != 0)
            perror("pthread_setcancellation");
        
        /* Parsing of he data */
        pthread_mutex_lock(&mut);
        ((thdata* )input)->s->deserialize(((thdata* )input)->s->context, ((thdata*) input)->buffer, ((thdata*) input)->dataSerialized);
        ((thdata* )input)->s->freeobject(((thdata* )input)->s->context, ((thdata*) input)->buffer);
        pthread_mutex_unlock(&mut);
        ((thdata*) input)->countParse++;
    }
    
    pthread_exit(EXIT_SUCCESS);
}

/// @brief Main function of the cpu percentage option
/// @param[in] sensorData data, to serialized
/// @param[in] sensorDataTemp, Data to parse
int benchOptionCpu(SensorData sensorData, SensorData sensorDataTemp, int freq)
{

	memset(&buffer, 0, sizeof(buffer));

    int ret;
    int index;
    int state;
    void* res;
    int resultParse;
    int resultSerialize;
    thdata dataThread;

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

    /***************************************************/
    /****************REFERENCE C************************/
    Serializer c;
	memset(&c, 0, sizeof(c));
	c_get_serializer(&c);

    /* Initiate data to thread */
    dataThread.s = &c;
    dataThread.buffer = buffer;
    dataThread.dataToSerialize = &sensorData;
    dataThread.dataSerialized = &sensorDataTemp;

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
    isFinished = false;
    for (index=0; index<DATA_TESTED-1; index++) {
        sem_post(&mutexData);
        usleep(freq);
    }
    sem_post(&mutexData);
    isFinished = true;
    cpu_stats = sg_get_cpu_stats_diff(&entries);
    cpu_percents = sg_get_cpu_percents_r(cpu_stats, &entries);
    percentage = cpu_percents->user;
    sg_free_mem_stats(cpu_percents);
    sg_shutdown();


    state = pthread_cancel(thread_emission);
    if (state != 0)
        perror("pthread cancelled emission");
    /* Wait thread to finish */   
    state = pthread_join(thread_emission, &res);
    if (state != 0)
        perror("pthread join");
    if (res == PTHREAD_CANCELED)
        printf("thread emmission was cancelled\n");
    state = pthread_join(thread_emission, NULL);

    state = pthread_cancel(thread_reception);
    if (state != 0)
        perror("pthread cancelled reception");
    /* Wait thread to finish */   
    state = pthread_join(thread_reception, &res);
    if (state != 0)
        perror("thread join");
    if (res == PTHREAD_CANCELED)
        printf("thread reception was cancelled\n");
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
    /********************JSON C*************************/

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

    state = pthread_cancel(thread_emission);
    if (state != 0)
        perror("pthread cancelled emission");
    /* Wait thread to finish */   
    state = pthread_join(thread_emission, &res);
    if (state != 0)
        perror("pthread join");
    if (res == PTHREAD_CANCELED)
        printf("thread emmission was cancelled\n");
    state = pthread_join(thread_emission, NULL);

    state = pthread_cancel(thread_reception);
    if (state != 0)
        perror("pthread cancelled reception");
    /* Wait thread to finish */   
    state = pthread_join(thread_reception, &res);
    if (state != 0)
        perror("thread join");
    if (res == PTHREAD_CANCELED)
        printf("thread reception was cancelled\n");
    state = pthread_join(thread_reception, NULL);

    /* Verification of parsing data */
    if (memcmp(&sensorData, &sensorDataTemp, sizeof(SensorData))) printf("Error copy\n");
	memset(&sensorDataTemp, 0, sizeof(SensorData));

    /* Print Result */
    resultSerialize = (int) (dataThread.countSerialize*100)/DATA_TESTED;
    resultParse = (int) (dataThread.countParse*100)/DATA_TESTED;
	printf("# JSON-C ");
#ifdef BENCH_JSON_ARRAY	
		printf("Array ");	
#endif
	printf(":\n");
    percentageResult(resultSerialize, resultParse, percentage);
#endif // BENCH_JSON

    return EXIT_SUCCESS;
}