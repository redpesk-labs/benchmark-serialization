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

#define TIME_RESOLUTION 1000000000ULL

int benchOptionTimer(SensorData sensorData, SensorData sensorDataTemp);

int benchOptionCpu(SensorData sensorData, SensorData sensorDataTemp, int freq);