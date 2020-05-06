#ifndef STRUCT_PARSING_H
#define STRUCT_PARSING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> 
#include <time.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include <json-c/json.h>
#include <cbor.h>

typedef struct
{
    int index;
    uint8_t *buffer;
    int port;
    int *sockfd;
} dataUDP;

typedef struct
{
   uint8_t  index;
   float  rcs;
   float range;
   int16_t  azimuth;
   float vrel;
   uint8_t  rollCount;
   int8_t  SNR;
}TargetInfo;

typedef struct
{
   uint8_t dataType;
   bool result;
   uint8_t master;
   uint8_t second;
   uint8_t step;
}SensorVersion;

typedef struct
{
   uint8_t actl_mode;
   uint8_t rollcount;
   uint8_t cfgStatus;
   
}SensorStatus;

typedef struct
{
   uint8_t noOfTarget;
   uint8_t rollcount;
   
}TargetStatus;

typedef struct {
    SensorVersion version;
    SensorStatus sStatus;
    TargetStatus tStatus;
    TargetInfo tInfo;
    uint8_t tInfoSize;    
} SensorData;


#endif