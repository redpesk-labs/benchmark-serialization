#include "parse-c.h"

/// @brief Serialize a SensorData object using memcpy.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
char* c_serialize_SensorData(SensorData input)
{
	char *temp;
	temp = malloc(1024*sizeof(char));
    memcpy(temp, &input, sizeof(SensorData));
    return temp;
}

/// @brief Deserialize a SensorData object using memcpy.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int c_deserialize_SensorData(char* input, SensorData* output)
{
    memcpy(output, input, sizeof(SensorData));
	free(input);
    return EXIT_SUCCESS;
}

// ---------- Serializer interface --------------------------------------------

/// @brief Initialize a context for c serialisation
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int c_init(void* ctx)
{

	return EXIT_SUCCESS;
}

/// @brief Cleanup a context of cbor serialisation.
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int c_cleanup(void* ctx)
{
	// Nothing to do
	return EXIT_SUCCESS;
}

/// @brief Print a string representation of the serialized data to the console.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to print.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int c_print(void* ctx, void* data)
{
	if (!data) return EXIT_FAILURE;
	printf("%s", (char* )data);
	return EXIT_SUCCESS;
}

/// @brief Free the allocated serialized object.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to free.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int c_freeobject(void* ctx, void* data)
{		
	if (data) /*free(data)*/;
	return EXIT_SUCCESS;
}

/// @brief Serialize an object.
/// @param[in] ctx Context object.
/// @param[in] input Object to serialize.
/// @param[out] output Serialization result if any.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int c_serialize(void* ctx, SensorData input, void** output)
{
	if (!output) return EXIT_FAILURE;
    *output = c_serialize_SensorData(input);
    return EXIT_SUCCESS;
}


/// @brief Deserialize an object.
/// @param[in] ctx Context object.
/// @param[in] input Data to deserialize.
/// @param[out] output Deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int c_deserialize(void* ctx, void* input, SensorData* output)
{
	if (!input || !output) return EXIT_FAILURE;
	return c_deserialize_SensorData((char* )input, output);
}

/// @brief Get the cbor serializer.
/// @param[out] s The serializer.
void c_get_serializer(Serializer* s)
{
	if (s)
	{
		s->context		= NULL;
		s->init			= c_init;
		s->cleanup		= c_cleanup;
		s->print		= c_print;
		s->freeobject	= c_freeobject;
		s->serialize	= c_serialize;
		s->deserialize	= c_deserialize;
	}
}