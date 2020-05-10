/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: sensordata.proto */

#ifndef PROTOBUF_C_sensordata_2eproto__INCLUDED
#define PROTOBUF_C_sensordata_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1002001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _SensorVersionMessage SensorVersionMessage;
typedef struct _SensorStatusMessage SensorStatusMessage;
typedef struct _TargetStatusMessage TargetStatusMessage;
typedef struct _TargetInfoMessage TargetInfoMessage;
typedef struct _SensorDataMessage SensorDataMessage;


/* --- enums --- */


/* --- messages --- */

struct  _SensorVersionMessage
{
  ProtobufCMessage base;
  uint32_t datatype;
  protobuf_c_boolean result;
  uint32_t master;
  uint32_t second;
  uint32_t step;
};
#define SENSOR_VERSION_MESSAGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sensor_version_message__descriptor) \
    , 0, 0, 0, 0, 0 }


struct  _SensorStatusMessage
{
  ProtobufCMessage base;
  uint32_t actl_mode;
  uint32_t rollcount;
  uint32_t cfgstatus;
};
#define SENSOR_STATUS_MESSAGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sensor_status_message__descriptor) \
    , 0, 0, 0 }


struct  _TargetStatusMessage
{
  ProtobufCMessage base;
  uint32_t nooftarget;
  uint32_t rollcount;
};
#define TARGET_STATUS_MESSAGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&target_status_message__descriptor) \
    , 0, 0 }


struct  _TargetInfoMessage
{
  ProtobufCMessage base;
  uint32_t index;
  float rcs;
  float range;
  uint32_t azimuth;
  float vrel;
  uint32_t rollcount;
  uint32_t snr;
};
#define TARGET_INFO_MESSAGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&target_info_message__descriptor) \
    , 0, 0, 0, 0, 0, 0, 0 }


struct  _SensorDataMessage
{
  ProtobufCMessage base;
  SensorVersionMessage *sensorversion;
  SensorStatusMessage *sensorstatus;
  TargetStatusMessage *targetstatus;
  TargetInfoMessage *targetinfo;
};
#define SENSOR_DATA_MESSAGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&sensor_data_message__descriptor) \
    , NULL, NULL, NULL, NULL }


/* SensorVersionMessage methods */
void   sensor_version_message__init
                     (SensorVersionMessage         *message);
size_t sensor_version_message__get_packed_size
                     (const SensorVersionMessage   *message);
size_t sensor_version_message__pack
                     (const SensorVersionMessage   *message,
                      uint8_t             *out);
size_t sensor_version_message__pack_to_buffer
                     (const SensorVersionMessage   *message,
                      ProtobufCBuffer     *buffer);
SensorVersionMessage *
       sensor_version_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sensor_version_message__free_unpacked
                     (SensorVersionMessage *message,
                      ProtobufCAllocator *allocator);
/* SensorStatusMessage methods */
void   sensor_status_message__init
                     (SensorStatusMessage         *message);
size_t sensor_status_message__get_packed_size
                     (const SensorStatusMessage   *message);
size_t sensor_status_message__pack
                     (const SensorStatusMessage   *message,
                      uint8_t             *out);
size_t sensor_status_message__pack_to_buffer
                     (const SensorStatusMessage   *message,
                      ProtobufCBuffer     *buffer);
SensorStatusMessage *
       sensor_status_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sensor_status_message__free_unpacked
                     (SensorStatusMessage *message,
                      ProtobufCAllocator *allocator);
/* TargetStatusMessage methods */
void   target_status_message__init
                     (TargetStatusMessage         *message);
size_t target_status_message__get_packed_size
                     (const TargetStatusMessage   *message);
size_t target_status_message__pack
                     (const TargetStatusMessage   *message,
                      uint8_t             *out);
size_t target_status_message__pack_to_buffer
                     (const TargetStatusMessage   *message,
                      ProtobufCBuffer     *buffer);
TargetStatusMessage *
       target_status_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   target_status_message__free_unpacked
                     (TargetStatusMessage *message,
                      ProtobufCAllocator *allocator);
/* TargetInfoMessage methods */
void   target_info_message__init
                     (TargetInfoMessage         *message);
size_t target_info_message__get_packed_size
                     (const TargetInfoMessage   *message);
size_t target_info_message__pack
                     (const TargetInfoMessage   *message,
                      uint8_t             *out);
size_t target_info_message__pack_to_buffer
                     (const TargetInfoMessage   *message,
                      ProtobufCBuffer     *buffer);
TargetInfoMessage *
       target_info_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   target_info_message__free_unpacked
                     (TargetInfoMessage *message,
                      ProtobufCAllocator *allocator);
/* SensorDataMessage methods */
void   sensor_data_message__init
                     (SensorDataMessage         *message);
size_t sensor_data_message__get_packed_size
                     (const SensorDataMessage   *message);
size_t sensor_data_message__pack
                     (const SensorDataMessage   *message,
                      uint8_t             *out);
size_t sensor_data_message__pack_to_buffer
                     (const SensorDataMessage   *message,
                      ProtobufCBuffer     *buffer);
SensorDataMessage *
       sensor_data_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   sensor_data_message__free_unpacked
                     (SensorDataMessage *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*SensorVersionMessage_Closure)
                 (const SensorVersionMessage *message,
                  void *closure_data);
typedef void (*SensorStatusMessage_Closure)
                 (const SensorStatusMessage *message,
                  void *closure_data);
typedef void (*TargetStatusMessage_Closure)
                 (const TargetStatusMessage *message,
                  void *closure_data);
typedef void (*TargetInfoMessage_Closure)
                 (const TargetInfoMessage *message,
                  void *closure_data);
typedef void (*SensorDataMessage_Closure)
                 (const SensorDataMessage *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor sensor_version_message__descriptor;
extern const ProtobufCMessageDescriptor sensor_status_message__descriptor;
extern const ProtobufCMessageDescriptor target_status_message__descriptor;
extern const ProtobufCMessageDescriptor target_info_message__descriptor;
extern const ProtobufCMessageDescriptor sensor_data_message__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_sensordata_2eproto__INCLUDED */
