#include "wrap-jsmn.h"
#include <stdio.h>




// ---------- Serializer interface --------------------------------------------

/// @brief Initialize a context for JSON serialisation
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsomn_init(void* ctx)
{
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
	//printf("%s", json_object_to_json_string_ext((struct json_object*)data, JSON_C_TO_STRING_PRETTY));
	return EXIT_SUCCESS;
}

/// @brief Free the allocated serialized object.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to free.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsmn_freeobject(void* ctx, void* data)
{
    if (!data) return EXIT_FAILURE;
	//if (data) return json_object_put((struct json_object*)data) ? EXIT_SUCCESS : EXIT_FAILURE;
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
	return jsmn_deserialize_SensorData((struct json_object*)input, output, opt);
}

/// @brief Get the jsmn serializer.
/// @param[out] s The serializer.
void jsmn_get_serializer(Serializer* s)
{
	if (s)
	{
		s->context		= NULL;
		s->init			= jsmn_init;
		s->cleanup		= jsmn_cleanup;
		s->print		= jsmn_print;
		s->freeobject	= jsmn_freeobject;
		s->serialize	= jsmn_serialize;
		s->deserialize	= jsmn_deserialize;
	}
}
