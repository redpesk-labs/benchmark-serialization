#include "parse-xdr.h"

/// @brief Serialize a SensorVersion object using xdr, if xdr->s_op=ENCODE.
/// @brief Deserialize a SensorVersion object using xdr, if xdr->s_op=DECODE.
/// @param[in/out] sensorVersion 
/// @param[out/out] xdr 
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int xdr_sensorVersion(XDR* xdr, SensorVersion* sensorVersion)
{
	bool_t temp = sensorVersion->result;
	if (!xdr_uint8_t(xdr, &sensorVersion->dataType)) {
		return EXIT_FAILURE;
	}
	if (!xdr_bool(xdr, &temp)) {
		return EXIT_FAILURE;
	}
	if (!xdr_uint8_t(xdr, &sensorVersion->master))
		return EXIT_FAILURE;
	if (!xdr_uint8_t(xdr, &sensorVersion->second))
		return EXIT_FAILURE;
	if (!xdr_uint8_t(xdr, &sensorVersion->step))
		return EXIT_FAILURE;

	if (xdr->x_op == XDR_DECODE) {
		sensorVersion->result = temp;
	}

	return EXIT_SUCCESS;
}

/// @brief Serialize a SensorStatus object using xdr, if xdr->s_op=ENCODE.
/// @brief Deserialize a SensorStatus object using xdr, if xdr->s_op=DECODE.
/// @param[in/out] sensorStatus 
/// @param[out/out] xdr 
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int xdr_sensorStatus(XDR* xdr, SensorStatus* sensorStatus)
{
	if (!xdr_uint8_t(xdr, &sensorStatus->actl_mode))
		return EXIT_FAILURE;
	if (!xdr_uint8_t(xdr, &sensorStatus->rollcount))
		return EXIT_FAILURE;
	if (!xdr_uint8_t(xdr, &sensorStatus->cfgStatus))
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

/// @brief Serialize a TargetStatus object using xdr, if xdr->s_op=ENCODE.
/// @brief Deserialize a TargetStatus object using xdr, if xdr->s_op=DECODE.
/// @param[in/out] targetStatus 
/// @param[out/out] xdr 
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int xdr_targetStatus(XDR* xdr, TargetStatus* targetStatus)
{
	if (!xdr_uint8_t(xdr, &targetStatus->noOfTarget))
		return EXIT_FAILURE;
	if (!xdr_uint8_t(xdr, &targetStatus->rollcount))
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

/// @brief Serialize a TargetInfo object using xdr, if xdr->s_op=ENCODE.
/// @brief Deserialize a TargetInfo object using xdr, if xdr->s_op=DECODE.
/// @param[in/out] targetInfo 
/// @param[out/out] xdr 
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int xdr_targetInfo(XDR* xdr, TargetInfo* targetInfo)
{
	if (!xdr_uint8_t(xdr, &targetInfo->index))
		return EXIT_FAILURE;
	if (!xdr_float(xdr, &targetInfo->rcs))
		return EXIT_FAILURE;
	if (!xdr_float(xdr, &targetInfo->range))
		return EXIT_FAILURE;
	if (!xdr_int16_t(xdr, &targetInfo->azimuth))
		return EXIT_FAILURE;

	if (!xdr_float(xdr, &targetInfo->vrel))
		return EXIT_FAILURE;
	if (!xdr_uint8_t(xdr, &targetInfo->rollCount))
		return EXIT_FAILURE;
	if (!xdr_int8_t(xdr, &targetInfo->SNR))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

/// @brief Serialize a SensorData object using xdr, if xdr->s_op=ENCODE.
/// @brief Deserialize a SensorData object using xdr, if xdr->s_op=DECODE.
/// @param[in/out] sensorData 
/// @param[out/out] xdr 
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int xdr_sensorData(SensorData* sensorData, XDR* xdr)
{
	if (xdr_sensorVersion(xdr, &sensorData->version)) {
		printf("Error during parsing Sensor Version\n");
		return EXIT_FAILURE;
	}

	if (xdr_sensorStatus(xdr, &sensorData->sStatus)) {
		printf("Error during parsing Sensor Status\n");
		return EXIT_FAILURE;
	}
		
	if (xdr_targetStatus(xdr, &sensorData->tStatus)) {
		printf("Error during parsing Target Status\n");
		return EXIT_FAILURE;
	}
		
	if (xdr_targetInfo(xdr, &sensorData->tInfo)) {
		printf("Error during parsing TargetInfo\n");
		return EXIT_FAILURE;
	}	

	if (!xdr_uint8_t(xdr, &sensorData->tInfoSize)) {
		printf("Error during parsing target InfoSize\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

// ---------- Serializer interface --------------------------------------------

/// @brief Initialize a context for xdr serialisation
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int xdr_init(void* ctx)
{
	// Nothing to do
	return EXIT_SUCCESS;
}

/// @brief Cleanup a context of xdr serialisation.
/// @param[in] ctx Context object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int xdr_cleanup(void* ctx)
{
	// Nothing to do
	return EXIT_SUCCESS;
}

/// @brief Print a string representation of the serialized data to the console.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to print.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int xdr_print(void* ctx, void* data)
{
	if (!data) return EXIT_FAILURE;
	//printf("%s", json_object_to_json_string_ext((struct json_object*)data, JSON_C_TO_STRING_PRETTY));
	return EXIT_SUCCESS;
}

/// @brief Free the allocated serialized object.
/// @param[in] ctx Context object.
/// @param[in] data Serialized data to free.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int xdr_freeobject(void* ctx, void* data)
{		
	//if (data) /*free object*/;
	return EXIT_SUCCESS;
}

/// @brief Serialize an object.
/// @param[in] ctx Context object.
/// @param[in] input Object to serialize.
/// @param[out] output Serialization result if any.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int xdr_serialize(void* ctx, SensorData input, void** output)
{
	if (!output) return EXIT_FAILURE;
	((XDR *)output)->x_op = XDR_ENCODE;
	xdr_setpos(((XDR *)output), 0);	
	return xdr_sensorData(&input, ((XDR *)output));
}


/// @brief Deserialize an object.
/// @param[in] ctx Context object.
/// @param[in] input Data to deserialize.
/// @param[out] output Deserialized object.
/// @return @c EXIT_SUCCESS or @c EXIT_FAILURE.
int xdr_deserialize(void* ctx, void* input, SensorData* output)
{
	if (!input || !output) return EXIT_FAILURE;
	((XDR *)input)->x_op = XDR_DECODE;
	xdr_setpos(((XDR *)input), 0);
	return xdr_sensorData(output, ((XDR *)input));
}

/// @brief Get the xdr serializer.
/// @param[out] s The serializer.
void xdr_get_serializer(Serializer* s)
{
	if (s)
	{
		s->context		= NULL;
		s->init			= xdr_init;
		s->cleanup		= xdr_cleanup;
		s->print		= xdr_print;
		s->freeobject	= xdr_freeobject;
		s->serialize	= xdr_serialize;
		s->deserialize	= xdr_deserialize;
	}
}
