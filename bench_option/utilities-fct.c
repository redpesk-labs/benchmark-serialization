#include "bench-option.h"
/// @brief fill a SensorData object, to be fast in the generation, it will generate jut one rand value.
/// @param[out] senorData_ptr The pointer to save data generated.
void generateData(SensorData* sensorData_ptr)
{
    
    srand(time(NULL)); // initialisation de rand
    int randInt = rand()/(RAND_MAX/10);
    bool randBool = (int) ((float)rand()/RAND_MAX);
    float randFloat = randInt + 0.55;

	sensorData_ptr->version.dataType = (uint8_t) randInt + 1;
	sensorData_ptr->version.result = randBool;
	sensorData_ptr->version.master = (uint8_t) randInt + 3;
	sensorData_ptr->version.second = (uint8_t) randInt + 4;
	sensorData_ptr->version.step = (uint8_t) randInt + 5;

	sensorData_ptr->sStatus.actl_mode = (uint8_t) randInt + 6;
	sensorData_ptr->sStatus.rollcount = (uint8_t) randInt + 7;
	sensorData_ptr->sStatus.cfgStatus = (uint8_t) randInt + 8;

	sensorData_ptr->tStatus.noOfTarget= (uint8_t) randInt + 9;
	sensorData_ptr->tStatus.rollcount= (uint8_t) randInt + 10;

	sensorData_ptr->tInfo.index= (uint8_t) randInt + 11;
	sensorData_ptr->tInfo.rcs= randFloat;
	sensorData_ptr->tInfo.range= randFloat + 1.1;
	sensorData_ptr->tInfo.azimuth= (int16_t) randInt + 12;
	sensorData_ptr->tInfo.vrel= randFloat + 2.6;
	sensorData_ptr->tInfo.rollCount= (uint8_t) randInt + 13;
	sensorData_ptr->tInfo.SNR= -1 * (int8_t) randInt;

	sensorData_ptr->tInfoSize = 1;
}

/// @brief print th sensorStructure to debug
/// @param [in] the sensorData pointer to print
void printStructure(SensorData* data)
{
    printf("SensorVersion:\n");
    printf("\t dataType: %u\n", data->version.dataType);
    printf("\t result: %u\n", data->version.result);
    printf("\t master: %u\n", data->version.master);
    printf("\t second: %u\n", data->version.second);
    printf("\t step: %u\n", data->version.step);

    printf("SensorStatus:\n");
    printf("\t actl_mode: %u\n", data->sStatus.actl_mode);
    printf("\t rollcount: %u\n", data->sStatus.rollcount);
    printf("\t cfgStatus: %u\n", data->sStatus.cfgStatus);

    printf("TargetStatus:\n");
    printf("\t noOfTarget: %u\n", data->tStatus.noOfTarget);
    printf("\t rollcount: %u\n", data->tStatus.rollcount);

    printf("Targetinfo:\n");
    printf("\t index: %u\n", data->tInfo.index);
    printf("\t rcs: %f\n", data->tInfo.rcs);
    printf("\t range: %f\n", data->tInfo.range);
    printf("\t azimuth: %i\n", data->tInfo.azimuth);
    printf("\t vrel: %f\n", data->tInfo.vrel);
    printf("\t rollcount: %u\n", data->tInfo.rollCount);
    printf("\t SNR: %i\n", data->tInfo.SNR);

    printf("tInfoSize: %u\n", data->tInfoSize);
}

/// @brief Print byte to byte data into SensorData object.
/// @param[in] sd SensorData object to print.
void printByHexa(SensorData* sd)
{
	printf("Sensor Version by hexa :");
	for (int index =0; index<sizeof(SensorVersion); index++) {
		printf(" %02x", ((char *)&sd->version)[index]);
	}
	printf("\n");
	printf("Sensor Status by hexa :");
	for (int index =0; index<sizeof(SensorStatus); index++) {
		printf(" %02x", ((char *)&sd->sStatus)[index]);
	}
	printf("\n");
	printf("Target Status by hexa :");
	for (int index =0; index<sizeof(TargetStatus); index++) {
		printf(" %02x", ((char *)&sd->tStatus)[index]);
	}
	printf("\n");
	printf("Target Info by hexa :");
	for (int index =0; index<sizeof(TargetInfo); index++) {
		printf(" %02x", ((char *)&sd->tInfo)[index]);
	}
	printf("\n");
	printf("target info size : %02x", sd->tInfoSize);
	printf("\n");
}

/// @brief compare 2 SensorData objects with memcmp.
/// @param[in] sd1 First SensorData object to compare.
/// @param[in] sd2 Second SensorData object to compare.
/// @retval err The number of err catch by memcmp.
int verification(SensorData* sd1, SensorData* sd2)
{
	int err;
	float diff;
	if (memcmp(sd1, sd2, sizeof(SensorData)) != 0) {
		// printByHexa(sd1);
		// printByHexa(sd2); 
		// printf("\n\n");
		// diff = sd1->tInfo.range - sd2->tInfo.range;
		// printf("difference of range: %.10f\n", diff);
		// diff = sd1->tInfo.rcs - sd2->tInfo.rcs;
		// printf("difference of rcs: %.10f\n", diff);
		// diff = sd1->tInfo.vrel - sd2->tInfo.vrel;
		// printf("difference of vrel: %.10f\n", diff);
		if ( (sd1->tInfo.range - sd2->tInfo.range) > 0.00001 ||
				(sd1->tInfo.rcs - sd2->tInfo.rcs) > 0.00001 ||
				(sd1->tInfo.vrel - sd2->tInfo.vrel) > 0.00001) {
					return 1;
				}
	}
	return 0;
}

/// @brief Print result of a specific bench with timer option.
/// @param[in] err The number of erreur happend during a bench.
/// @param[in] time Thee time during a bench.
void printResultTimer(int err, uint64_t time)
{
	if (err == 0) {
		printf("\tData : OK\n");
		printf("\tTime during the serialization: %f ms\n", (double)time / 1000000);
	}
	else {
		printf("\tData: NOK\n");
		printf("\tNumber of error : %i", err);
	}
	printf("\n\n");
}

/// @brief Print result of a specific bench with option cpu.
/// @param[in] The percentage of data serialized.
/// @param[in] The percentage of data parsed.
/// @param[in] The percentage of CPU using by the user during all transaction.
void printResultCPU(int dataSerialized, int dataParsed, float cpu)
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