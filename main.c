#include "bench.h"
#include "bench-option.h"

int DATA_TESTED = 1000000;
/// @brief fill a SensorData object.
/// @param[out] senorData_ptr The pointer to save data generated.
void generateData(SensorData* sensorData_ptr)
{
	//parseRawBuffer(sensorData_ptr);

	sensorData_ptr->version.dataType = 1;
	sensorData_ptr->version.result = 2;
	sensorData_ptr->version.master = 3;
	sensorData_ptr->version.second = 4;
	sensorData_ptr->version.step = 15;

	sensorData_ptr->sStatus.actl_mode = 3;
	sensorData_ptr->sStatus.rollcount = 2;
	sensorData_ptr->sStatus.cfgStatus = 1;

	sensorData_ptr->tStatus.noOfTarget= 10;
	sensorData_ptr->tStatus.rollcount= 11;

	sensorData_ptr->tInfo.index= 12;
	sensorData_ptr->tInfo.rcs= 4.5;
	sensorData_ptr->tInfo.range= 1.35;
	sensorData_ptr->tInfo.azimuth= -6;
	sensorData_ptr->tInfo.vrel= 0.02;
	sensorData_ptr->tInfo.rollCount= 3;
	sensorData_ptr->tInfo.SNR= -8;

	sensorData_ptr->tInfoSize = 1;
}

/// @brief Main function for the bench.
/// @retval @c EXIT_SUCCESS or @c EXIT_FAILURE
int main(int argc, char* argv[])
{
	/* Allocate memory */
	SensorData sensorData;
	memset(&sensorData, 0, sizeof(SensorData));
	SensorData sensorDataTemp;
	memset(&sensorDataTemp, 0, sizeof(SensorData));

	/* Generate data to parse */
	generateData(&sensorData);

	/* Get main option */
	benchConfigT* config = benchParseArgs(argc, argv);
	if(!config) exit(0);

	if(config->numberData != 0) DATA_TESTED = config->numberData;

	printf(" ====== BENCHMARKING DATA SERIALIZATION ====== \n\n");

	if (config->timer) benchOptionTimer(sensorData, sensorDataTemp);
	if (config->cpu) printf("Option charge cpu chosen : --- SOON AVAILABLE ---\n");

	free(config);

	return EXIT_SUCCESS;
}
