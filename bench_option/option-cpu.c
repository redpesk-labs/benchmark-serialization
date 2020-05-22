#include "bench-option.h"

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
         if(!sem_trywait(&mutexParse) ){
            //printf("Data to parse received\n");
            memcpy(((thdata*) input)->data, ((thdata*) input)->buffer, sizeof(SensorData));
            ((thdata*) input)->count++;
        }  
    }
    //printf("No more data to parse\n");
    pthread_exit(EXIT_SUCCESS);
}

int benchOptionCpu(SensorData sensorData, SensorData sensorDataTemp, int freq)
{
    int ret;
    int index;
    //int value;
    float resultParse;
    float resultSerialize;
    thdata dataToSerialize;
    thdata dataToParse;

    printf("option CPU\n\n");
	printf("data tested : %i\n\n", DATA_TESTED);
	printf("frequence : %i ns\n\n", freq);


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


	ret = pthread_create(&thread_emission, NULL, serialize, (void*) &dataToSerialize);
	if(ret){
		perror("pthread_emission_create");
	}
	ret = pthread_create(&thread_reception, NULL, parse, (void*) &dataToParse);
	if(ret){
		perror("pthread_reception_create");
	}

    isFinished = false;
    for (index=0; index<DATA_TESTED; index++) {
        sem_post(&mutexData);
        usleep(freq);
    }
    isFinished = true;
    
    pthread_join(thread_reception, NULL);

    if (memcmp(&sensorData, &sensorDataTemp, sizeof(SensorData))) printf("Error copy\n");

	memset(&sensorDataTemp, 0, sizeof(SensorData));
    resultSerialize = (dataToSerialize.count*100)/DATA_TESTED;
    resultParse = (dataToParse.count*100)/DATA_TESTED;
    printf("percentage of data serialized : %f %% \n", resultSerialize);
    printf("percentage of data parse : %f %% \n", resultParse);

    return EXIT_SUCCESS;
}