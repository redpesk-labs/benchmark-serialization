#include "parse-xdr.h"

void xdr_sensorData(SensorData* sensorData, XDR* xdr)
{

	if (!xdr_sensorVersion(xdr, &sensorData->version))
		printf("Error during parsing Sensor Version\n");
	if (!xdr_sensorStatus(xdr, &sensorData->sStatus))
		printf("Error during parsing Sensor Status\n");
	if (!xdr_targetStatus(xdr, &sensorData->tStatus))
		printf("Error during parsing Target Status\n");
	if (!xdr_targetInfo(xdr, &sensorData->tInfo))
		printf("Error during parsing TargetInfo\n");

}

int xdr_sensorVersion(XDR* xdr, SensorVersion* sensorVersion)
{
	bool_t temp = sensorVersion->result;
	if (!xdr_uint8_t(xdr, &sensorVersion->dataType)) {
		return 0;
	}
	if (!xdr_bool(xdr, &temp)) {
		return 0;
	}
	if (!xdr_uint8_t(xdr, &sensorVersion->master))
		return 0;
	if (!xdr_uint8_t(xdr, &sensorVersion->second))
		return 0;
	if (!xdr_uint8_t(xdr, &sensorVersion->step))
		return 0;

	if (xdr->x_op == XDR_DECODE) {
		sensorVersion->result = temp;
	}

	return 1;
}

int xdr_sensorStatus(XDR* xdr, SensorStatus* sensorStatus)
{
	if (!xdr_uint8_t(xdr, &sensorStatus->actl_mode))
		return 0;
	if (!xdr_uint8_t(xdr, &sensorStatus->rollcount))
		return 0;
	if (!xdr_uint8_t(xdr, &sensorStatus->cfgStatus))
		return 0;
	return 1;
}

int xdr_targetStatus(XDR* xdr, TargetStatus* targetStatus)
{
	if (!xdr_uint8_t(xdr, &targetStatus->noOfTarget))
		return 0;
	if (!xdr_uint8_t(xdr, &targetStatus->rollcount))
		return 0;
	return 1;
}

int xdr_targetInfo(XDR* xdr, TargetInfo* targetInfo)
{
	if (!xdr_uint8_t(xdr, &targetInfo->index))
		return 0;
	if (!xdr_float(xdr, &targetInfo->rcs))
		return 0;
	if (!xdr_float(xdr, &targetInfo->range))
		return 0;
	if (!xdr_int16_t(xdr, &targetInfo->azimuth))
		return 0;

	if (!xdr_float(xdr, &targetInfo->vrel))
		return 0;
	if (!xdr_uint8_t(xdr, &targetInfo->rollCount))
		return 0;
	if (!xdr_int8_t(xdr, &targetInfo->SNR))
		return 0;

	return 1;
}
