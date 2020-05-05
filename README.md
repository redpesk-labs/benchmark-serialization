# Serialization benchmarking

The aim's project has to compare different data serialization between :

    1.JSON
    2.protobuf
    3.CBOR
    4.XDR

It will take the SP70C data example.

## explanation

### data receive

It will listen data through an UDP socket at 127.0.0.1:1111

`0xaa 0xaa 0x0a 0x06 0x01 0x11 0x00 0x00 0x00 0x00 0x00 0x00 0x55 0x55 0xaa 0xaa 0x0b 0x07 0x01 0x02 0x00 0x00 0x00 0x00 0x00 0x00 0x55 0x55 0xaa 0xaa 0x0c 0x07 0x01 0x6b 0x01 0x86 0x2a 0x02 0xbf 0x79 0x55 0x55`

### data parse

Then it will parse data into a structure of SP70C data:

| SensorData struct         | SensorVersion struct | SensorStatus struct | TargetStatus struct | TargetInfo struct |
| ------------------------- |--------------------- | ------------------- | ------------------- | ----------------- |
|typedef struct<br/> {<br/>**SensorVersion** version;<br/>**SensorStatus** sStatus;<br/> **TargetStatus** tStatus; <br/> **TargetInfo** *tInfo; <br/> uint8_t tInfoSize; <br/> } SensorData;|typedef struct<br/>{<br/>uint8_t dataType;<br/>bool result;<br/>uint8_t master;<br/>uint8_t second;<br/>uint8_t step;<br/>}SensorVersion;|typedef struct<br/>{<br/>uint8_t actl_mode;<br/>uint8_t rollcount;<br/>uint8_t cfgStatus;<br/>}SensorStatus;|typedef struct<br/>{<br/>uint8_t noOfTarget;<br/>uint8_t rollcount;<br/>}TargetStatus;|typedef struct<br/>{<br/>uint8_t  index;<br/>float  rcs;<br/>float range;<br/>int16_t  azimuth;<br/>float vrel;<br/>uint8_t  rollCount;<br/>int8_t  SNR;<br/>}TargetInfo;|

### serialization

After that it will test all different serializations and de-serializations. We use the **MONOTONIC CLOCK** to calculate the time passed in each case. Before to print the result, it will **verify** if all data is well serialized.

## How use it

Firstly you have to run the generator-udp project. you can find it at : [git@git.ovh.iot:vlefebvre/generator-udp.git](git@git.ovh.iot:vlefebvre/generator-udp.git)
It will send data to the right udp port.
And then run this project. You can go to the file **config-parsing.h** to change macro to change which serialization would be tested, and the number of data tested.
