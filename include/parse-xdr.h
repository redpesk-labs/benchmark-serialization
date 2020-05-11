#pragma once

#include "data.h"
#include <rpc/xdr.h>

void xdr_sensorData(SensorData* sensorData, XDR* xdr);
int xdr_sensorVersion(XDR* xdr, SensorVersion* sensorVersion);
int xdr_sensorStatus(XDR* xdr, SensorStatus* sensorStatus);
int xdr_targetStatus(XDR* xdr, TargetStatus* targetStatus);
int xdr_targetInfo(XDR* xdr, TargetInfo* targetInfo);
