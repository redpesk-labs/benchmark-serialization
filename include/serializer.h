#pragma once

#include "data.h"

typedef struct
{
	void* context;

	int (*init)			(void* ctx);
	int (*cleanup)		(void* ctx);
	int (*print)		(void* ctx, void* data);
	int (*freeobject)	(void* ctx, void* data);
	int (*serialize)	(void* ctx, SensorData* input, void** output);
	int (*deserialize)	(void* ctx, void* input, SensorData* output);

} Serializer;

