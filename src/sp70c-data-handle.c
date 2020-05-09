#include "config.h"
#include "sp70c-data-handle.h"

//Sets everything to 0
int initTargetInfo(TargetInfo* tInfo)
{
	tInfo->azimuth = 0;
	tInfo->index = 0;
	tInfo->range = 0;
	tInfo->rcs = 0;
	tInfo->rollCount = 0;
	tInfo->SNR = 0;
	tInfo->vrel = 0;
	return 0;
}


//Sets everything to 0
int initSensorVersion(SensorVersion* sVersion)
{
	sVersion->dataType = 0;
	sVersion->master = 0;
	sVersion->result = 0;
	sVersion->second = 0;
	sVersion->step = 0;
	return 0;
}

//Sets everything to 0
int initSensorStatus(SensorStatus* sStatus)
{
	sStatus->actl_mode = 0;
	sStatus->cfgStatus = 0;
	sStatus->rollcount = 0;
	return 0;
}

int initTargetStatus(TargetStatus* tStatus)
{
	tStatus->noOfTarget = 0;
	tStatus->rollcount = 0;
	return 0;
}

//Sets everything to 0
int initData(SensorData* sensorData)
{
	initTargetStatus(&sensorData->tStatus);
	initSensorVersion(&sensorData->version);
	initSensorStatus(&sensorData->sStatus);
	initTargetInfo(&sensorData->tInfo);
	sensorData->tInfoSize = 0;
	return 0;
}

int parseRawBuffer(uint8_t* buffer, size_t size, SensorData* sensorData)
{
	int index, tInfoIndex;
	uint16_t messageID;
	uint8_t parsingBuffer[PARSING_BUFFER_SIZE];

	for (index = 0; index < size; index++) {
		if (buffer[index] == 0xaa && buffer[index + 1] == 0xaa) {
			messageID = (uint16_t)(((uint16_t)buffer[index + 3] * 0x100 + (uint16_t)buffer[index + 2]));
			if (buffer[index + PARSING_BUFFER_SIZE - 1] == 0x55 && buffer[index + PARSING_BUFFER_SIZE - 2] == 0x55) {

				for (int i = 0; i < PARSING_BUFFER_SIZE; i++)
				{
					parsingBuffer[i] = buffer[index + i];
				}
				switch (messageID)
				{
				case 0x400: // if Sensor Version message ID
					parseSensorVersion(parsingBuffer, sensorData);
					index += PARSING_BUFFER_SIZE - 1;
					break;

				case 0x60A:// if Sensor Status message ID
					parseSensorStatus(parsingBuffer, sensorData);
					index += PARSING_BUFFER_SIZE - 1;
					break;

				case 0x70B:// if Sensor Target status message ID
					if (parseTargetStatus(parsingBuffer, sensorData) > 0) {
						tInfoIndex = index + PARSING_BUFFER_SIZE;
						sensorData->tInfoSize = 0;
						parseTargetInfo(buffer + tInfoIndex, sensorData); //Parse each target info received after the 0x70B message
					}
					index += PARSING_BUFFER_SIZE * (sensorData->tStatus.noOfTarget + 1) - 1;
					break;
				default:
					continue;
					break;
				}
			}
		}
	}
	return 0;
}


//Parse data that matched the SensorVersion message ID
int parseSensorVersion(uint8_t* buffer, SensorData* sensorData)
{
	SensorVersion sensorVersion;
	sensorVersion.dataType = (uint8_t)(buffer[4] & 0x7F);
	sensorVersion.result = (bool)((buffer[4] >> 7) & 0x1);
	sensorVersion.master = (uint8_t)(buffer[5]);
	sensorVersion.second = (uint8_t)(buffer[6]);
	sensorVersion.step = (uint8_t)(buffer[7]);

	sensorData->version = sensorVersion;
	return 0;
}

//Parse data that matched the SensorStatus message ID
int parseSensorStatus(uint8_t* buffer, SensorData* sensorData)
{
	SensorStatus sensorStatus;
	sensorStatus.actl_mode = (uint8_t)(buffer[4] & 0x7F);
	sensorStatus.rollcount = (uint8_t)(buffer[5] & 0x3);
	sensorStatus.cfgStatus = (uint8_t)((buffer[5] >> 4) & 0xF);

	sensorData->sStatus = sensorStatus;
	return 0;
}

//Parse data that matched the TargetStatus message ID
int parseTargetStatus(uint8_t* buffer, SensorData* sensorData)
{
	TargetStatus targetStatus;
	targetStatus.noOfTarget = (uint8_t)(buffer[4]);
	targetStatus.rollcount = (uint8_t)(buffer[5] & 0x3);
	/* if(targetStatus.noOfTarget>0){
		if(sensorData->tInfo){
			if(sensorData->tStatus.noOfTarget!=targetStatus.noOfTarget){
				sensorData->tInfo=(TargetInfo *)realloc(sensorData->tInfo,sizeof(TargetInfo)*targetStatus.noOfTarget);
			}
		}
		if(!sensorData->tInfo){
			sensorData->tInfo=(TargetInfo*)calloc(targetStatus.noOfTarget,sizeof(TargetInfo));
			if(!sensorData->tInfo){
				printf("ERROR ALLOCATING MEMORY\n");
			}
		 }
	}
	if(targetStatus.noOfTarget==0){
		if(sensorData->tInfo){
			free(sensorData->tInfo);
			sensorData->tInfo=NULL;
		}
	}
*/
	sensorData->tStatus = targetStatus;
	//printf("Number of targets : %d\n", sensorData->tStatus.noOfTarget);
	return sensorData->tStatus.noOfTarget;
}

//Parse data that matched the TargetInfo message ID
int parseTargetInfo(uint8_t* buffer, SensorData* sensorData)
{
	TargetInfo targetInfo;
	targetInfo.index = (uint8_t)buffer[4];
	targetInfo.rcs = (float)((float)(buffer[5]) * 0.5 - 50);
	targetInfo.range = (float)((float)(buffer[6]/**256*/ + buffer[7]) * 0.01);
	targetInfo.azimuth = (int16_t)(buffer[8] * 2 - 90);
	targetInfo.vrel = (float)(((float)((buffer[9] & 7)/**256*/ + buffer[10]) * 0.05) - 35);
	targetInfo.rollCount = (uint8_t)(buffer[9] >> 5);
	targetInfo.SNR = (int8_t)(buffer[11] - 127);
	/*     if(sensorData->tInfo && sensorData->tStatus.noOfTarget>0){
			targetInfoCopy(sensorData->tInfo+sensorData->tInfoSize,&targetInfo);
			sensorData->tInfoSize++;
		} */
	sensorData->tInfo = targetInfo;
	return 0;
}

//Parse all targets info according to the number of detected targets
int parseAllTargetInfo(uint8_t* buffer, SensorData* sensorData)
{
	uint8_t parsingBuffer[PARSING_BUFFER_SIZE];
	int noOfTargets = sensorData->tStatus.noOfTarget;
	for (int i = 0; i < noOfTargets; i++)
	{
		if (buffer[i * PARSING_BUFFER_SIZE + PARSING_BUFFER_SIZE - 1] == 0x55 && buffer[i * PARSING_BUFFER_SIZE + PARSING_BUFFER_SIZE - 2] == 0x55) {
			for (int j = 0; j < PARSING_BUFFER_SIZE; j++)
			{
				parsingBuffer[j] = buffer[i * PARSING_BUFFER_SIZE + j];
			}
			parseTargetInfo(parsingBuffer, sensorData);
		}
	}
	return 0;
}


//Copies a TargetInfo data into an other
int targetInfoCopy(TargetInfo* copy, TargetInfo* copied)
{
	copy->index = copied->index;
	copy->azimuth = copied->azimuth;
	copy->range = copied->range;
	copy->rcs = copied->rcs;
	copy->rollCount = copied->rollCount;
	copy->SNR = copied->SNR;
	copy->vrel = copied->vrel;
	return 0;
}