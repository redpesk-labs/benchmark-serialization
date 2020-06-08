#pragma once

#include "data.h"
#include "config.h"
//#include "sp70c-data-handle.h"
//#include "string.h"
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h> 
#include "serializer.h"

#include "statgrab.h"

#define TIME_RESOLUTION 1000000000ULL

void printByHexa(SensorData* sd);

void printStructure(SensorData* data);

int verification(SensorData* sd1, SensorData* sd2);

void generateData(SensorData* sensorData_ptr);

void printResultTimer(int err, uint64_t time);

void printResultCPU(int dataSerialized, int dataParsed, float cpu);

int show_proc_stats(sg_process_stats* proc_stats);

int benchOptionTimer(SensorData sensorData, SensorData sensorDataTemp);

int benchOptionCpu(SensorData sensorData, SensorData sensorDataTemp, int freq);