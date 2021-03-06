/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: sensordata.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "sensordata.pb-c.h"
void   sensor_version_message__init
                     (SensorVersionMessage         *message)
{
  static SensorVersionMessage init_value = SENSOR_VERSION_MESSAGE__INIT;
  *message = init_value;
}
size_t sensor_version_message__get_packed_size
                     (const SensorVersionMessage *message)
{
  assert(message->base.descriptor == &sensor_version_message__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t sensor_version_message__pack
                     (const SensorVersionMessage *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &sensor_version_message__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t sensor_version_message__pack_to_buffer
                     (const SensorVersionMessage *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &sensor_version_message__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
SensorVersionMessage *
       sensor_version_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (SensorVersionMessage *)
     protobuf_c_message_unpack (&sensor_version_message__descriptor,
                                allocator, len, data);
}
void   sensor_version_message__free_unpacked
                     (SensorVersionMessage *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &sensor_version_message__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   sensor_status_message__init
                     (SensorStatusMessage         *message)
{
  static SensorStatusMessage init_value = SENSOR_STATUS_MESSAGE__INIT;
  *message = init_value;
}
size_t sensor_status_message__get_packed_size
                     (const SensorStatusMessage *message)
{
  assert(message->base.descriptor == &sensor_status_message__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t sensor_status_message__pack
                     (const SensorStatusMessage *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &sensor_status_message__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t sensor_status_message__pack_to_buffer
                     (const SensorStatusMessage *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &sensor_status_message__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
SensorStatusMessage *
       sensor_status_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (SensorStatusMessage *)
     protobuf_c_message_unpack (&sensor_status_message__descriptor,
                                allocator, len, data);
}
void   sensor_status_message__free_unpacked
                     (SensorStatusMessage *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &sensor_status_message__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   target_status_message__init
                     (TargetStatusMessage         *message)
{
  static TargetStatusMessage init_value = TARGET_STATUS_MESSAGE__INIT;
  *message = init_value;
}
size_t target_status_message__get_packed_size
                     (const TargetStatusMessage *message)
{
  assert(message->base.descriptor == &target_status_message__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t target_status_message__pack
                     (const TargetStatusMessage *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &target_status_message__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t target_status_message__pack_to_buffer
                     (const TargetStatusMessage *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &target_status_message__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
TargetStatusMessage *
       target_status_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (TargetStatusMessage *)
     protobuf_c_message_unpack (&target_status_message__descriptor,
                                allocator, len, data);
}
void   target_status_message__free_unpacked
                     (TargetStatusMessage *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &target_status_message__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   target_info_message__init
                     (TargetInfoMessage         *message)
{
  static TargetInfoMessage init_value = TARGET_INFO_MESSAGE__INIT;
  *message = init_value;
}
size_t target_info_message__get_packed_size
                     (const TargetInfoMessage *message)
{
  assert(message->base.descriptor == &target_info_message__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t target_info_message__pack
                     (const TargetInfoMessage *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &target_info_message__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t target_info_message__pack_to_buffer
                     (const TargetInfoMessage *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &target_info_message__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
TargetInfoMessage *
       target_info_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (TargetInfoMessage *)
     protobuf_c_message_unpack (&target_info_message__descriptor,
                                allocator, len, data);
}
void   target_info_message__free_unpacked
                     (TargetInfoMessage *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &target_info_message__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   sensor_data_message__init
                     (SensorDataMessage         *message)
{
  static SensorDataMessage init_value = SENSOR_DATA_MESSAGE__INIT;
  *message = init_value;
}
size_t sensor_data_message__get_packed_size
                     (const SensorDataMessage *message)
{
  assert(message->base.descriptor == &sensor_data_message__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t sensor_data_message__pack
                     (const SensorDataMessage *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &sensor_data_message__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t sensor_data_message__pack_to_buffer
                     (const SensorDataMessage *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &sensor_data_message__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
SensorDataMessage *
       sensor_data_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (SensorDataMessage *)
     protobuf_c_message_unpack (&sensor_data_message__descriptor,
                                allocator, len, data);
}
void   sensor_data_message__free_unpacked
                     (SensorDataMessage *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &sensor_data_message__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor sensor_version_message__field_descriptors[5] =
{
  {
    "dataType",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(SensorVersionMessage, datatype),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "result",
    2,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_BOOL,
    0,   /* quantifier_offset */
    offsetof(SensorVersionMessage, result),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "master",
    3,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(SensorVersionMessage, master),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "second",
    4,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(SensorVersionMessage, second),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "step",
    5,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(SensorVersionMessage, step),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned sensor_version_message__field_indices_by_name[] = {
  0,   /* field[0] = dataType */
  2,   /* field[2] = master */
  1,   /* field[1] = result */
  3,   /* field[3] = second */
  4,   /* field[4] = step */
};
static const ProtobufCIntRange sensor_version_message__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 5 }
};
const ProtobufCMessageDescriptor sensor_version_message__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "SensorVersionMessage",
  "SensorVersionMessage",
  "SensorVersionMessage",
  "",
  sizeof(SensorVersionMessage),
  5,
  sensor_version_message__field_descriptors,
  sensor_version_message__field_indices_by_name,
  1,  sensor_version_message__number_ranges,
  (ProtobufCMessageInit) sensor_version_message__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor sensor_status_message__field_descriptors[3] =
{
  {
    "actl_mode",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(SensorStatusMessage, actl_mode),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "rollcount",
    2,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(SensorStatusMessage, rollcount),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "cfgStatus",
    3,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(SensorStatusMessage, cfgstatus),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned sensor_status_message__field_indices_by_name[] = {
  0,   /* field[0] = actl_mode */
  2,   /* field[2] = cfgStatus */
  1,   /* field[1] = rollcount */
};
static const ProtobufCIntRange sensor_status_message__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor sensor_status_message__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "SensorStatusMessage",
  "SensorStatusMessage",
  "SensorStatusMessage",
  "",
  sizeof(SensorStatusMessage),
  3,
  sensor_status_message__field_descriptors,
  sensor_status_message__field_indices_by_name,
  1,  sensor_status_message__number_ranges,
  (ProtobufCMessageInit) sensor_status_message__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor target_status_message__field_descriptors[2] =
{
  {
    "noOfTarget",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(TargetStatusMessage, nooftarget),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "rollcount",
    2,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(TargetStatusMessage, rollcount),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned target_status_message__field_indices_by_name[] = {
  0,   /* field[0] = noOfTarget */
  1,   /* field[1] = rollcount */
};
static const ProtobufCIntRange target_status_message__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor target_status_message__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "TargetStatusMessage",
  "TargetStatusMessage",
  "TargetStatusMessage",
  "",
  sizeof(TargetStatusMessage),
  2,
  target_status_message__field_descriptors,
  target_status_message__field_indices_by_name,
  1,  target_status_message__number_ranges,
  (ProtobufCMessageInit) target_status_message__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor target_info_message__field_descriptors[7] =
{
  {
    "index",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(TargetInfoMessage, index),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "rcs",
    2,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_FLOAT,
    0,   /* quantifier_offset */
    offsetof(TargetInfoMessage, rcs),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "range",
    3,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_FLOAT,
    0,   /* quantifier_offset */
    offsetof(TargetInfoMessage, range),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "azimuth",
    4,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(TargetInfoMessage, azimuth),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "vrel",
    5,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_FLOAT,
    0,   /* quantifier_offset */
    offsetof(TargetInfoMessage, vrel),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "rollCount",
    6,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(TargetInfoMessage, rollcount),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "SNR",
    7,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(TargetInfoMessage, snr),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned target_info_message__field_indices_by_name[] = {
  6,   /* field[6] = SNR */
  3,   /* field[3] = azimuth */
  0,   /* field[0] = index */
  2,   /* field[2] = range */
  1,   /* field[1] = rcs */
  5,   /* field[5] = rollCount */
  4,   /* field[4] = vrel */
};
static const ProtobufCIntRange target_info_message__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 7 }
};
const ProtobufCMessageDescriptor target_info_message__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "TargetInfoMessage",
  "TargetInfoMessage",
  "TargetInfoMessage",
  "",
  sizeof(TargetInfoMessage),
  7,
  target_info_message__field_descriptors,
  target_info_message__field_indices_by_name,
  1,  target_info_message__number_ranges,
  (ProtobufCMessageInit) target_info_message__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor sensor_data_message__field_descriptors[5] =
{
  {
    "sensorVersion",
    1,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(SensorDataMessage, sensorversion),
    &sensor_version_message__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "sensorStatus",
    2,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(SensorDataMessage, sensorstatus),
    &sensor_status_message__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "targetStatus",
    3,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(SensorDataMessage, targetstatus),
    &target_status_message__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "targetInfo",
    4,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(SensorDataMessage, targetinfo),
    &target_info_message__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "tinfosize",
    5,
    PROTOBUF_C_LABEL_REQUIRED,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(SensorDataMessage, tinfosize),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned sensor_data_message__field_indices_by_name[] = {
  1,   /* field[1] = sensorStatus */
  0,   /* field[0] = sensorVersion */
  3,   /* field[3] = targetInfo */
  2,   /* field[2] = targetStatus */
  4,   /* field[4] = tinfosize */
};
static const ProtobufCIntRange sensor_data_message__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 5 }
};
const ProtobufCMessageDescriptor sensor_data_message__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "SensorDataMessage",
  "SensorDataMessage",
  "SensorDataMessage",
  "",
  sizeof(SensorDataMessage),
  5,
  sensor_data_message__field_descriptors,
  sensor_data_message__field_indices_by_name,
  1,  sensor_data_message__number_ranges,
  (ProtobufCMessageInit) sensor_data_message__init,
  NULL,NULL,NULL    /* reserved[123] */
};
