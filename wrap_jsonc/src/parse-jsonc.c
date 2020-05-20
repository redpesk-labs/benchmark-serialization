#include "parse-jsonc.h"
#include <stdio.h>

/// @brief Serialize a SensorVersion object using json-c.
/// @param[in] sv The object to serialize.
/// @return The serialized result.
struct json_object* jsonc_serialize_SensorVersion(SensorVersion sv, enum Option_t opt)
{
	struct json_object* out;
	if (opt == ARRAY) {
		out = json_object_new_array();
		json_object_array_add(out, json_object_new_int(sv.dataType));
        json_object_array_add(out, json_object_new_boolean(sv.result));
        json_object_array_add(out, json_object_new_int(sv.master));
        json_object_array_add(out, json_object_new_int(sv.second));
        json_object_array_add(out, json_object_new_int(sv.step));
	} else {
		out = json_object_new_object();
		json_object_object_add(out, "dataType", json_object_new_int(sv.dataType));
		json_object_object_add(out, "result", json_object_new_boolean(sv.result));
		json_object_object_add(out, "master", json_object_new_int(sv.master));
		json_object_object_add(out, "second", json_object_new_int(sv.second));
		json_object_object_add(out, "step", json_object_new_int(sv.step));
	}
	return out;
}

/// @brief Deserialize a SensorVersion object using json-c.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int jsonc_deserialize_SensorVersion(struct json_object* input, SensorVersion* output, enum Option_t opt)
{

	if (opt == ARRAY) {

		output->dataType = (uint8_t)json_object_get_int(json_object_array_get_idx(input, 0));
		output->result = (bool)json_object_get_boolean(json_object_array_get_idx(input, 1));
		output->master = (uint8_t)json_object_get_int(json_object_array_get_idx(input, 2));
		output->second = (uint8_t)json_object_get_int(json_object_array_get_idx(input, 3));
		output->step = (uint8_t)json_object_get_int(json_object_array_get_idx(input, 4));

	} else {

		struct json_object* tmp;

		if (!json_object_object_get_ex(input, "dataType", &tmp)) return EXIT_FAILURE;
		output->dataType = (uint8_t)json_object_get_int(tmp); // TODO: check value boundaries before casting.

		if (!json_object_object_get_ex(input, "result", &tmp)) return EXIT_FAILURE;
		output->result = (bool)json_object_get_boolean(tmp); // TODO: check value boundaries before casting.

		if (!json_object_object_get_ex(input, "master", &tmp)) return EXIT_FAILURE;
		output->master = (uint8_t)json_object_get_int(tmp); // TODO: check value boundaries before casting.

		if (!json_object_object_get_ex(input, "second", &tmp)) return EXIT_FAILURE;
		output->second = (uint8_t)json_object_get_int(tmp); // TODO: check value boundaries before casting.

		if (!json_object_object_get_ex(input, "step", &tmp)) return EXIT_FAILURE;
		output->step = (uint8_t)json_object_get_int(tmp); // TODO: check value boundaries before casting.

	}

	return EXIT_SUCCESS;
}

/// @brief Serialize a SensorStatus object using json-c.
/// @param[in] ss The object to serialize.
/// @return The serialized result.
struct json_object* jsonc_serialize_SensorStatus(SensorStatus ss, enum Option_t opt)
{
	struct json_object* out;

	if (opt == ARRAY) {
		out = json_object_new_array();
		json_object_array_add(out, json_object_new_int(ss.actl_mode));
		json_object_array_add(out, json_object_new_boolean(ss.rollcount));
		json_object_array_add(out, json_object_new_int(ss.cfgStatus));
	} else  {
		out = json_object_new_object();
		json_object_object_add(out, "actl_mode", json_object_new_int(ss.actl_mode));
		json_object_object_add(out, "rollcount", json_object_new_boolean(ss.rollcount));
		json_object_object_add(out, "cfgStatus", json_object_new_int(ss.cfgStatus));
	}

	return out;
}

/// @brief Deserialize a SensorStatus object using json-c.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int jsonc_deserialize_SensorStatus(struct json_object* input, SensorStatus* output, enum Option_t opt)
{

	if (opt == ARRAY) {

		output->actl_mode = (uint8_t)json_object_get_int(json_object_array_get_idx(input, 0));
		output->rollcount = (uint8_t)json_object_get_int(json_object_array_get_idx(input, 1));
		output->cfgStatus = (uint8_t)json_object_get_int(json_object_array_get_idx(input, 2));

	} else {
		struct json_object* tmp;

		if (!json_object_object_get_ex(input, "actl_mode", &tmp)) return EXIT_FAILURE;
		output->actl_mode = (uint8_t)json_object_get_int(tmp); // TODO: check value boundaries before casting.

		if (!json_object_object_get_ex(input, "rollcount", &tmp)) return EXIT_FAILURE;
		output->rollcount = (uint8_t)json_object_get_int(tmp); // TODO: check value boundaries before casting.

		if (!json_object_object_get_ex(input, "cfgStatus", &tmp)) return EXIT_FAILURE;
		output->cfgStatus = (uint8_t)json_object_get_int(tmp); // TODO: check value boundaries before casting.
	}
	return EXIT_SUCCESS;
}

/// @brief Serialize a TargetStatus object using json-c.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
struct json_object* jsonc_serialize_TargetStatus(TargetStatus ts, enum Option_t opt)
{
	struct json_object* out;

	if (opt == ARRAY) {

		out = json_object_new_array();
		json_object_array_add(out, json_object_new_int(ts.noOfTarget));
		json_object_array_add(out, json_object_new_boolean(ts.rollcount));

	} else {

		out = json_object_new_object();
		json_object_object_add(out, "noOfTarget", json_object_new_int(ts.noOfTarget));
		json_object_object_add(out, "rollcount", json_object_new_boolean(ts.rollcount));

	}

	return out;
}

/// @brief Deserialize a TargetStatus object using json-c.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int jsonc_deserialize_TargetStatus(struct json_object* input, TargetStatus* output, enum Option_t opt)
{

	if (opt == ARRAY) {
		output->noOfTarget = (uint8_t)json_object_get_int(json_object_array_get_idx(input, 0));
		output->rollcount = (uint8_t)json_object_get_int(json_object_array_get_idx(input, 1));
	} else {
		struct json_object* tmp;

		if (!json_object_object_get_ex(input, "noOfTarget", &tmp)) return EXIT_FAILURE;
		output->noOfTarget = (uint8_t)json_object_get_int(tmp); // TODO: check value boundaries before casting.

		if (!json_object_object_get_ex(input, "rollcount", &tmp)) return EXIT_FAILURE;
		output->rollcount = (uint8_t)json_object_get_int(tmp); // TODO: check value boundaries before casting.
	}

	return EXIT_SUCCESS;
}

/// @brief Serialize a TargetInfo object using json-c.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
struct json_object* jsonc_serialize_TargetInfo(TargetInfo ti, enum Option_t opt)
{
	struct json_object* out;

	if (opt == ARRAY) {
		out = json_object_new_array();
		json_object_array_add(out, json_object_new_int(ti.index));
		json_object_array_add(out, json_object_new_double(ti.rcs));
		json_object_array_add(out, json_object_new_double(ti.range));
		json_object_array_add(out, json_object_new_int(ti.azimuth));
		json_object_array_add(out, json_object_new_double(ti.vrel));
		json_object_array_add(out, json_object_new_int(ti.rollCount));
		json_object_array_add(out, json_object_new_int(ti.SNR));
	} else {
		out = json_object_new_object();
		json_object_object_add(out, "index", json_object_new_int(ti.index));
		json_object_object_add(out, "rcs", json_object_new_double(ti.rcs));
		json_object_object_add(out, "range", json_object_new_double(ti.range));
		json_object_object_add(out, "azimuth", json_object_new_int(ti.azimuth));
		json_object_object_add(out, "vrel", json_object_new_double(ti.vrel));
		json_object_object_add(out, "rollCount", json_object_new_int(ti.rollCount));
		json_object_object_add(out, "SNR", json_object_new_int(ti.SNR));
	}
	
	return out;
}

/// @brief Deserialize a TargetInfo object using json-c.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int jsonc_deserialize_TargetInfo(struct json_object* input, TargetInfo* output, enum Option_t opt)
{

	if (opt == ARRAY) {

		output->index = (uint8_t)json_object_get_int(json_object_array_get_idx(input, 0));
		output->rcs = (float)json_object_get_double(json_object_array_get_idx(input, 1));
		output->range = (float)json_object_get_double(json_object_array_get_idx(input, 2));
		output->azimuth = (int16_t)json_object_get_int(json_object_array_get_idx(input, 3));
		output->vrel = (float)json_object_get_double(json_object_array_get_idx(input, 4));
		output->rollCount = (uint8_t)json_object_get_int(json_object_array_get_idx(input, 5));
		output->SNR = (int8_t)json_object_get_int(json_object_array_get_idx(input, 6));

	}else {

		struct json_object* tmp;

		if (!json_object_object_get_ex(input, "index", &tmp)) return EXIT_FAILURE;
		output->index = (uint8_t)json_object_get_int(tmp); // TODO: check value boundaries before casting.

		if (!json_object_object_get_ex(input, "rcs", &tmp)) return EXIT_FAILURE;
		output->rcs = (float)json_object_get_double(tmp); // TODO: check value boundaries before casting.

		if (!json_object_object_get_ex(input, "range", &tmp)) return EXIT_FAILURE;
		output->range = (float)json_object_get_double(tmp); // TODO: check value boundaries before casting.

		if (!json_object_object_get_ex(input, "azimuth", &tmp)) return EXIT_FAILURE;
		output->azimuth = (int16_t)json_object_get_int(tmp); // TODO: check value boundaries before casting.

		if (!json_object_object_get_ex(input, "vrel", &tmp)) return EXIT_FAILURE;
		output->vrel = (float)json_object_get_double(tmp); // TODO: check value boundaries before casting.

		if (!json_object_object_get_ex(input, "rollCount", &tmp)) return EXIT_FAILURE;
		output->rollCount = (uint8_t)json_object_get_int(tmp); // TODO: check value boundaries before casting.

		if (!json_object_object_get_ex(input, "SNR", &tmp)) return EXIT_FAILURE;
		output->SNR = (int8_t)json_object_get_int(tmp); // TODO: check value boundaries before casting.

	}

	return EXIT_SUCCESS;
}

/// @brief Serialize a SensorData object using json-c.
/// @param[in] ts The object to serialize.
/// @return The serialized result.
struct json_object* jsonc_serialize_SensorData(SensorData input, enum Option_t opt)
{
	json_object* out;
	if (opt == ARRAY) {
		out = json_object_new_array();
		json_object_array_add(out, jsonc_serialize_SensorVersion(input.version, opt));
		json_object_array_add(out, jsonc_serialize_SensorStatus(input.sStatus, opt));
		json_object_array_add(out, jsonc_serialize_TargetStatus(input.tStatus, opt));
		json_object_array_add(out, jsonc_serialize_TargetInfo(input.tInfo, opt));
		json_object_array_add(out, json_object_new_int(input.tInfoSize));		
	} else {
		out = json_object_new_object();
		json_object_object_add(out, "version", jsonc_serialize_SensorVersion(input.version, opt));
		json_object_object_add(out, "sStatus", jsonc_serialize_SensorStatus(input.sStatus, opt));
		json_object_object_add(out, "tStatus", jsonc_serialize_TargetStatus(input.tStatus, opt));
		json_object_object_add(out, "tInfo", jsonc_serialize_TargetInfo(input.tInfo, opt));
		json_object_object_add(out, "tInfoSize", json_object_new_int(input.tInfoSize));
	}

	return out;
}

/// @brief Deserialize a SensorData object using json-c.
/// @param[in] input The object to deserialize.
/// @param[out] output The deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE
int jsonc_deserialize_SensorData(struct json_object* input, SensorData* output, enum Option_t opt)
{

	if (opt == ARRAY) {
		if (jsonc_deserialize_SensorVersion(json_object_array_get_idx(input, 0), &output->version, opt)) return EXIT_FAILURE;
		if (jsonc_deserialize_SensorStatus(json_object_array_get_idx(input, 1), &output->sStatus, opt)) return EXIT_FAILURE;
		if (jsonc_deserialize_TargetStatus(json_object_array_get_idx(input, 2), &output->tStatus, opt)) return EXIT_FAILURE;
		if (jsonc_deserialize_TargetInfo(json_object_array_get_idx(input, 3), &output->tInfo, opt)) return EXIT_FAILURE;
		output->tInfoSize = (uint8_t)json_object_get_int(json_object_array_get_idx(input, 4));
	} else {
		struct json_object* tmp;

		if (!json_object_object_get_ex(input, "version", &tmp)) return EXIT_FAILURE;
		if (jsonc_deserialize_SensorVersion(tmp, &output->version, opt)) return EXIT_FAILURE;

		if (!json_object_object_get_ex(input, "sStatus", &tmp)) return EXIT_FAILURE;
		if (jsonc_deserialize_SensorStatus(tmp, &output->sStatus, opt)) return EXIT_FAILURE;

		if (!json_object_object_get_ex(input, "tStatus", &tmp)) return EXIT_FAILURE;
		if (jsonc_deserialize_TargetStatus(tmp, &output->tStatus, opt)) return EXIT_FAILURE;

		if (!json_object_object_get_ex(input, "tInfo", &tmp)) return EXIT_FAILURE;
		if (jsonc_deserialize_TargetInfo(tmp, &output->tInfo, opt)) return EXIT_FAILURE;

		if (!json_object_object_get_ex(input, "tInfoSize", &tmp)) return EXIT_FAILURE;
		output->tInfoSize = (uint8_t)json_object_get_int(tmp); // TODO: check value boundaries before casting.
	}

	return EXIT_SUCCESS;
}

// ---------- Serializer interface --------------------------------------------

/// @brief Initialize a context for JSON serialisation
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsonc_init(void* ctx)
{
	return EXIT_SUCCESS;
}

/// @brief Cleanup a context of JSON serialisation.
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsonc_cleanup(void* ctx)
{
	// Nothing to do
	return EXIT_SUCCESS;
}

/// @brief Print a string representation of the serialized data to the console.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to print.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsonc_print(void* ctx, void* data)
{
	if (!data) return EXIT_FAILURE;
	printf("%s", json_object_to_json_string_ext((struct json_object*)data, JSON_C_TO_STRING_PRETTY));
	return EXIT_SUCCESS;
}

/// @brief Free the allocated serialized object.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to free.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsonc_freeobject(void* ctx, void* data)
{
	if (data) return json_object_put((struct json_object*)data) ? EXIT_SUCCESS : EXIT_FAILURE;
	return EXIT_FAILURE;
}

/// @brief Serialize an object.
/// @param[in] ctx Context object.
/// @param[in] input Object to serialize.
/// @param[out] output Serialization result if any.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsonc_serialize(void* ctx, SensorData input, void** output)
{
	if (!output) return EXIT_FAILURE;
	enum Option_t opt = *((int *)ctx);
	*output = jsonc_serialize_SensorData(input, opt);
	return EXIT_SUCCESS;
}


/// @brief Deserialize an object.
/// @param[in] ctx Context object.
/// @param[in] input Data to deserialize.
/// @param[out] output Deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int jsonc_deserialize(void* ctx, void* input, SensorData* output)
{
	if (!input || !output) return EXIT_FAILURE;
	enum Option_t opt = *((int *)ctx);
	return jsonc_deserialize_SensorData((struct json_object*)input, output, opt);
}

/// @brief Get the json-c serializer.
/// @param[out] s The serializer.
void jsonc_get_serializer(Serializer* s)
{
	if (s)
	{
		s->context		= NULL;
		s->init			= jsonc_init;
		s->cleanup		= jsonc_cleanup;
		s->print		= jsonc_print;
		s->freeobject	= jsonc_freeobject;
		s->serialize	= jsonc_serialize;
		s->deserialize	= jsonc_deserialize;
	}
}
