#include "bench-option.h"
#include "statgrab.h"

extern int DATA_TESTED;

sem_t mutexParse;
sem_t mutexData;
bool isFinished;

typedef struct str_thdata
{
    SensorData* data;
    char* buffer;
    int count;
} thdata;

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
}

/// @brief Print result of a specific bench.
/// @param[in] The percentage of data serialized.
/// @param[in] The percentage of data parsed.
/// @param[in] The percentage of CPU using by the user during all transaction.
void percentageResult(int dataSerialized, int dataParsed, float cpu)
{
	if (dataSerialized != 100 || dataParsed != 100) {
		printf("\tData : NOK\n");
		printf("\tpercentage f data serialized: %i %%\n", dataSerialized);
		printf("\tpercentage f data parsed: %i %%\n", dataParsed);
	}
	else {
		printf("\tData: OK\n");
		printf("\tPercentage of CPU using : %f %%", cpu);
	}
	printf("\n\n");
}

static void* serialize(void* input)
{    
    ((thdata*) input)->count = 0;
    while (!isFinished)
    {  
        sem_wait(&mutexData);
        //printf("Data to serialize received\n");
        memcpy(((thdata*) input)->buffer, ((thdata*) input)->data, sizeof(SensorData));
        ((thdata*) input)->count ++;
        sem_post(&mutexParse);
        usleep(1);
        
    }
    //printf("No more data to serialized\n");
    pthread_exit(EXIT_SUCCESS);
}

void* parse(void* input)
{	
    ((thdata*) input)->count = 0;
    while (!isFinished) {
        sem_wait(&mutexParse);
        //printf("Data to parse received\n");
        memcpy(((thdata*) input)->data, ((thdata*) input)->buffer, sizeof(SensorData));
        ((thdata*) input)->count++;
        
    }
    //printf("No more data to parse\n");
    pthread_exit(EXIT_SUCCESS);
}

int benchOptionCpu(SensorData sensorData, SensorData sensorDataTemp, int freq)
{
    int ret;
    int index;
    int resultParse;
    int resultSerialize;
    thdata dataToSerialize;
    thdata dataToParse;

    printf("option:\t\t CPU\n");
	printf("data tested :\t %i struct C\n", DATA_TESTED);
	printf("frequency :\t %i ns\n\n", freq);

    float percentage = 0;
    double count = 0;

    /* Initiate value for stats proc */
    size_t entries;
    memset(&entries, 0, sizeof(size_t));
    sg_cpu_stats* cpu_stats = malloc(sizeof(sg_cpu_stats));
    memset(cpu_stats, 0, sizeof(sg_cpu_stats));
    sg_cpu_percent_source cps;
    memset(&cps, 0, sizeof(sg_cpu_percent_source));

	/* Initiate 2 threads, for emission and reception */
	pthread_t thread_emission;
	pthread_t thread_reception;

	/* Initiate the semaphore mutex */
	sem_init(&mutexParse, 0, 0);
    sem_init(&mutexData, 0, 0);

	/* Reference in C */
	char buffer_ref[1024];
	memset(&buffer_ref, 0, sizeof(sensorData));

    /* Initiate data to thread */
    dataToSerialize.buffer = buffer_ref;
    dataToSerialize.data = &sensorData;
    dataToParse.buffer = buffer_ref;
    dataToParse.data = &sensorDataTemp;

    /* Thread creation */
	ret = pthread_create(&thread_emission, NULL, serialize, (void*) &dataToSerialize);
	if(ret){
		perror("pthread_emission_create");
	}
	ret = pthread_create(&thread_reception, NULL, parse, (void*) &dataToParse);
	if(ret){
		perror("pthread_reception_create");
	}

    sg_init(1);	
    cpu_stats = sg_get_cpu_stats(&entries);
    isFinished = false;
    for (index=0; index<DATA_TESTED; index++) {
        sem_post(&mutexData);
        usleep(freq);
    }
    isFinished = true;
    sem_getvalue(&mutexData, &ret);
    if (!ret){
        sem_post(&mutexData);
    }
    cpu_stats = sg_get_cpu_stats_diff(&entries);
    percentage = (sg_get_cpu_percents_r(cpu_stats, &entries))->user;
    sg_shutdown();


    /* Wait thread to finish */   
    pthread_join(thread_emission, NULL);
    pthread_join(thread_reception, NULL);

    /* Verification of parsing data */
    if (memcmp(&sensorData, &sensorDataTemp, sizeof(SensorData))) printf("Error copy\n");
	memset(&sensorDataTemp, 0, sizeof(SensorData));

    /* Print Result */
    resultSerialize = (int) (dataToSerialize.count*100)/DATA_TESTED;
    resultParse = (int) (dataToParse.count*100)/DATA_TESTED;
    printf("# REFERENCE C :\n");
    percentageResult(resultSerialize, resultParse, percentage);

    return EXIT_SUCCESS;
}