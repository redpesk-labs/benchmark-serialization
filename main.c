#include "bench.h"
#include "bench-option.h"

int DATA_TESTED = 1000000;


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
	printf("Example of data tested :\n");
	printStructure(&sensorData);
	printf("\n\n");

	if (config->timer) benchOptionTimer(sensorData, sensorDataTemp);
	if (config->cpu) benchOptionCpu(sensorData, sensorDataTemp, config->freq);

	free(config);

	return EXIT_SUCCESS;
}
