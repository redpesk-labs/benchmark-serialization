#include "wrap-jsmn.h"
#include <stdio.h>
#include "jsmn.h"

/// @brief Serialize a SensorVersion object using json representation.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
int jsmn_serialize_SensorVersion(SensorVersion *input, int opt, char *output, int len)
{
    int r;
    switch (opt)
    {
    case ARRAY:
        r = snprintf(output, (size_t)len, "%u,%d,%u,%u,%u",
                     input->dataType, input->result, input->master, input->second, input->step);
        break;
    
    case MAP:
    default:
        r = snprintf(output, (size_t)len, "{\"dataType\":%u,\"result\":%d,\"master\":%u,\"second\":%u,\"step\":%u}",
                     input->dataType, input->result, input->master, input->second, input->step);
        break;
    }
    return r;
} 

/// @brief Serialize a SensorStatus object using json representation.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
int jsmn_serialize_SensorStatus(SensorStatus *input, int opt, char *output, int len)
{
    int r;
    switch (opt)
    {
    case ARRAY:
        r = snprintf(output, (size_t)len, "%u,%u,%u",
                    input->actl_mode, input->rollcount, input->cfgStatus);
        break;
    default:
    case MAP:
        r = snprintf(output, (size_t)len, "{\"actlMode\":%u,\"rollcount\":%u,\"cfgStatus\":%u}",
                    input->actl_mode, input->rollcount, input->cfgStatus);
    }
    return r;
}

/// @brief Serialize a TargetStatus object using json representation.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
int jsmn_serialize_TargetStatus(TargetStatus *input, int opt, char *output, int len)
{
    int r;
    switch (opt)
    {
    case ARRAY:
        r = snprintf(output, (size_t)len, "%u,%u",
                     input->noOfTarget, input->rollcount);
        break;
    case MAP:
    default:
        r = snprintf(output, (size_t)len, "{\"noOfTarget\":%u,\"rollcount\":%u}",
                     input->noOfTarget, input->rollcount);
        break;
    }
    return r;
}

/// @brief Serialize a TargetInfo object using json representation.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
int jsmn_serialize_TargetInfo(TargetInfo *input, int opt, char *output, int len)
{
    int r;
    switch (opt)
    {
    case ARRAY:
        r = snprintf(output, (size_t)len, "%u,%f,%f,%i,%f,%u,%i",
                     input->index, input->rcs, input->range, input->azimuth, input->vrel, input->rollCount, input->SNR);
        break;
    case MAP:
    default:
        r = snprintf(output, (size_t)len, "{\"index\":%u,\"rcs\":%f,\"range\":%f,\"azimuth\":%i,\"vrel\":%f,\"rollcount\":%u,\"snr\":%i}",
                     input->index, input->rcs, input->range, input->azimuth, input->vrel, input->rollCount, input->SNR);
        break;
    }
    return r;
}

/// @brief Serialize a SensorData object using json representation
/// @param[in] input The object to serialize..
/// @param[in] opt serialization option (MAP / ARRAY).
/// @return The serialized result.
char* jsmn_serialize_SensorData(SensorData* input, int opt)
{
    static const char *strings[2][6] = {
        { "[[", "],[", "],[", "],[", "],", "]" },
        { "{\"sensorversion\":", ",\"sensorstatus\":", ",\"targetstatus\":", ",\"targetinfo\":", ",\"targetinfosize\":", "}" }
    };
    int size = 4000, count = 0, r, idx;
    char* output;

    output = malloc((size_t)size);

    idx = opt == ARRAY ? 0 : 1;
    r = snprintf(&output[count], (size_t)(size - count), strings[idx][0]);
    count = r < 0 ? count : count+r <= size ? count+r : size;

    r = jsmn_serialize_SensorVersion(&input->version, opt, &output[count], (size_t)(size - count));

    count = r < 0 ? count : count+r <= size ? count+r : size;
    r = snprintf(&output[count], (size_t)(size - count), strings[idx][1]);
    count = r < 0 ? count : count+r <= size ? count+r : size;

    r = jsmn_serialize_SensorStatus(&input->sStatus, opt, &output[count], (size_t)(size - count));

    count = r < 0 ? count : count+r <= size ? count+r : size;
    r = snprintf(&output[count], (size_t)(size - count), strings[idx][2]);
    count = r < 0 ? count : count+r <= size ? count+r : size;

    r = jsmn_serialize_TargetStatus(&input->tStatus, opt, &output[count], (size_t)(size - count));

    count = r < 0 ? count : count+r <= size ? count+r : size;
    r = snprintf(&output[count], (size_t)(size - count), strings[idx][3]);
    count = r < 0 ? count : count+r <= size ? count+r : size;

    r = jsmn_serialize_TargetInfo(&input->tInfo, opt, &output[count], (size_t)(size - count));

    count = r < 0 ? count : count+r <= size ? count+r : size;
    r = snprintf(&output[count], (size_t)(size - count), strings[idx][4]);
    count = r < 0 ? count : count+r <= size ? count+r : size;

    r = snprintf(&output[count], (size_t)(size - count), "%u", input->tInfoSize);

    count = r < 0 ? count : count+r <= size ? count+r : size;
    r = snprintf(&output[count], (size_t)(size - count), strings[idx][5]);
    count = r < 0 ? count : count+r <= size ? count+r : size;

    output = realloc(output, (size_t)(1 + count));
    return output;
}

int jsmn_cmp_string(char* input, jsmntok_t* t, char* s){
	if (t->type == JSMN_STRING && strncmp(input + t->start, s, t->end - t->start) == 0 ) {
		return 1;
	}
	return 0;
}


/// @brief Serialize a SensorData object using json representation
/// @param[in] input The string to parse.
/// @param[out] output dezerialized sensorversion.
/// @param[in] t tokens from jsmn library.
/// @param[in] n number of tokens for the struct.
/// @param[in] i index of the current token.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE .
int jsmn_deserialize_SensorVersion(char* input, SensorVersion* output, jsmntok_t* t, int n, int i)
{
	int index;
	char temp[10];
	memset(temp, '\0',sizeof(temp));
	for (index = i; index < i+2*n; index +=2) {
		if (strncmp(input + t[index].start, "da", 2) == 0) { // DataType
			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->dataType = (uint8_t) atoi(temp);
			memset(temp, '\0',sizeof(temp));
		} else if (strncmp(input + t[index].start, "re", 2) == 0) { // result
			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->result = (bool) atoi(temp);
			memset(temp, '\0',sizeof(temp));
		} else if (strncmp(input + t[index].start, "ma", 2) == 0) { // master
			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->master = (uint8_t) atoi(temp);
			memset(temp, '\0',sizeof(temp));
		} else if (strncmp(input + t[index].start, "se", 2) == 0) { // second
			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->second = (uint8_t) atoi(temp);
			memset(temp, '\0',sizeof(temp));
		} else if (strncmp(input + t[index].start, "st", 2) == 0) { // step
			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->step = (uint8_t) atoi(temp);
		}
	}
}

/// @brief Serialize a SensorData object using json representation
/// @param[in] input The string to parse.
/// @param[out] output dezerialized sensorstatus.
/// @param[in] t tokens from jsmn library.
/// @param[in] n number of tokens for the struct.
/// @param[in] i index of the current token.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE .
int jsmn_deserialize_SensorStatus(char* input, SensorStatus* output, jsmntok_t* t, int n, int i)
{
	int index;
	char temp[10];
	memset(temp, '\0',sizeof(temp));
	for (index = i; index < i+2*n; index +=2) {
		if (strncmp(input + t[index].start, "ac", 2) == 0) { // actl_mode
			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->actl_mode = (uint8_t) atoi(temp);
			memset(temp, '\0',sizeof(temp));
		} else if (strncmp(input + t[index].start, "ro", 2) == 0) { // rollcount
			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->rollcount = (uint8_t) atoi(temp);
			memset(temp, '\0',sizeof(temp));
		} else if (strncmp(input + t[index].start, "cf", 2) == 0) { // cfg
			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->cfgStatus = (uint8_t) atoi(temp);
		} 
	}
}

/// @brief Serialize a SensorData object using json representation
/// @param[in] input The string to parse.
/// @param[out] output dezerialized targetstaus.
/// @param[in] t tokens from jsmn library.
/// @param[in] n number of tokens for the struct.
/// @param[in] i index of the current token.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE .
int jsmn_deserialize_TargetStatus(char* input, TargetStatus* output, jsmntok_t* t, int n, int i)
{
	int index;
	char temp[10];
	memset(temp, '\0',sizeof(temp));
	for (index = i; index < i+2*n; index +=2) {
		if (strncmp(input + t[index].start, "no", 2) == 0) { // noOfTarget
			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->noOfTarget = (uint8_t) atoi(temp);
			memset(temp, '\0',sizeof(temp));
		} else if (strncmp(input + t[index].start, "ro", 2) == 0) { // rollcount
			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->rollcount = (uint8_t) atoi(temp);
		}
	}
}

/// @brief Serialize a SensorData object using json representation
/// @param[in] input The string to parse.
/// @param[out] output dezerialized targetinfo.
/// @param[in] t tokens from jsmn library.
/// @param[in] n number of tokens for the struct.
/// @param[in] i index of the current token.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE .
int jsmn_deserialize_TargetInfo(char* input, TargetInfo* output, jsmntok_t* t, int n, int i)
{
	int index;
	char temp[10];
	memset(temp, '\0',sizeof(temp));
	for (index = i; index < i+2*n; index +=2) {
		if (strncmp(input + t[index].start, "in", 2) == 0) { // index

			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->index = (uint8_t) atoi(temp);
			memset(temp, '\0',sizeof(temp));

		} else if (strncmp(input + t[index].start, "rc", 2) == 0) { // rcs

			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->rcs = strtof(temp, NULL);
			memset(temp, '\0',sizeof(temp));

		} else if (strncmp(input + t[index].start, "ra", 2) == 0) { // range

			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->range = strtof(temp, NULL);
			memset(temp, '\0',sizeof(temp));

		} else if (strncmp(input + t[index].start, "az", 2) == 0) { // azimuth

			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->azimuth = (int16_t) atof(temp);
			memset(temp, '\0',sizeof(temp));
			
		} else if (strncmp(input + t[index].start, "vr", 2) == 0) { // vrel

			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->vrel = strtof(temp, NULL);
			memset(temp, '\0',sizeof(temp));

		} else if (strncmp(input + t[index].start, "ro", 2) == 0) { // rollcount

			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->rollCount = (uint8_t) atoi(temp);
			memset(temp, '\0',sizeof(temp));

		} else if (strncmp(input + t[index].start, "sn", 2) == 0) { // snr

			strncpy(temp, input + t[index+1].start, t[index+1].end - t[index+1].start);
			output->SNR = (int8_t) strtof(temp, NULL);

		}
	}
}

/// @brief Deserialize a SensorData object using jsmn library.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int jsmn_deserialize_SensorData(char* input, SensorData* output, enum Option_t opt)
{
	int r;
	int temp;
	int index;
	int child;
	bool sv_f = false;
	bool ss_f = false;
	bool ts_f = false;
	bool ti_f = false;
	bool tif_f = false;
	char infoSize[10];

	jsmn_parser p;
	jsmntok_t t[50];
	jsmn_init(&p);
	r = jsmn_parse(&p, input, strlen(input), t, sizeof(t)/sizeof(t[0]));

	if (r < 0) {
		printf("error during parsing with library jsmn:\n");
		switch (r){
			case JSMN_ERROR_INVAL:
				printf("-- bad token, JSON string is corrupted\n");
				break;
			case JSMN_ERROR_NOMEM:
				printf("-- not enough tokens, JSON string is too large\n");
				break;
			case JSMN_ERROR_PART:
				printf("-- JSON string is too short, expecting more JSON data");
				break;
			default:
				break;
		}
		return EXIT_FAILURE;
	}
	
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Error - object expected!\n");
		return EXIT_FAILURE;
	}

	for (index = 1; index <r; index++) {
		if (!sv_f){
			if (jsmn_cmp_string(input, &t[index], "sensorversion")){
				sv_f = true;
				temp = t[index+1].size; // size of the json object 
				jsmn_deserialize_SensorVersion(input, &output->version, t, temp, index+2);
				index += 2*temp;
			}
		}
		if (!ss_f){
			if (jsmn_cmp_string(input, &t[index], "sensorstatus")){
				ss_f = true;
				temp = t[index+1].size; // size of the json object 
				jsmn_deserialize_SensorStatus(input, &output->sStatus, t, temp, index+2);
				index += 2*temp;
			}
		}
		if (!ts_f){
			if (jsmn_cmp_string(input, &t[index], "targetstatus")){
				ts_f = true;
				temp = t[index+1].size; // size of the json object 
				jsmn_deserialize_TargetStatus(input, &output->tStatus, t, temp, index+2);
				index += 2*temp;
			}
		}
		if (!ti_f){
			if (jsmn_cmp_string(input, &t[index], "targetinfo")){
				ti_f = true;
				temp = t[index+1].size; // size of the json object 
				jsmn_deserialize_TargetInfo(input, &output->tInfo, t, temp, index+2);
				index += 2*temp;
			}
		}
		if (!tif_f){
			if (jsmn_cmp_string(input, &t[index], "targetinfosize")){
				tif_f = true;
				strncpy(infoSize, input + t[index+1].start, t[index+1].end - t[index+1].start);
				output->tInfoSize = (uint8_t) atoi(infoSize);
			}
		}
	}
	return EXIT_SUCCESS;
}

// ---------- Serializer interface --------------------------------------------

/// @brief Initialize a context for JSON serialisation
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int my_jsmn_init(void* ctx)
{
	//jsmn_parser parser;
	//jsmn_init(&parser);
	return EXIT_SUCCESS;
}

/// @brief Cleanup a context of JSON serialisation.
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsmn_cleanup(void* ctx)
{
	// Nothing to do
	return EXIT_SUCCESS;
}

/// @brief Print a string representation of the serialized data to the console.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to print.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsmn_print(void* ctx, void* data)
{
	if (!data) return EXIT_FAILURE;
    printf("%s\n", (char* )data);
	return EXIT_SUCCESS;
}

/// @brief Free the allocated serialized object.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to free.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsmn_freeobject(void* ctx, void* data)
{
    if (data) {
        free(data);
		return EXIT_SUCCESS;
    }
	return EXIT_FAILURE;
}

/// @brief Serialize an object.
/// @param[in] ctx Context object.
/// @param[in] input Object to serialize.
/// @param[out] output Serialization result if any.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsmn_serialize(void* ctx, SensorData* input, void** output)
{
	if (!output) return EXIT_FAILURE;
	enum Option_t opt = *((int *)ctx);
	*output = jsmn_serialize_SensorData(input, opt);
	return EXIT_SUCCESS;
}


/// @brief Deserialize an object.
/// @param[in] ctx Context object.
/// @param[in] input Data to deserialize.
/// @param[out] output Deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsmn_deserialize(void* ctx, void* input, SensorData* output)
{
	if (!output){
		printf("Output not valid\n");
		return EXIT_FAILURE;
	}
	if (!input){
		printf("Input not valid\n");
		return EXIT_FAILURE;
	}
	//if (!input || !output) return EXIT_FAILURE;
	enum Option_t opt = *((int *)ctx);
	return jsmn_deserialize_SensorData((char*)input, output, opt);
}

/// @brief Get the jsmn serializer.
/// @param[out] s The serializer.
void jsmn_get_serializer(Serializer* s)
{
	if (s)
	{
		s->context		= NULL;
		s->init			= my_jsmn_init;
		s->cleanup		= jsmn_cleanup;
		s->print		= jsmn_print;
		s->freeobject	= jsmn_freeobject;
		s->serialize	= jsmn_serialize;
		s->deserialize	= jsmn_deserialize;
	}
}
