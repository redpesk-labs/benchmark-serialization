# Serialization benchmarking

The aim's project has to compare different data serialization between :

    1.JSON with JSON-c (in Array and Map)
    2.JSON with FASTJSON (in Array and Map)
    3.Json with nlohmann/json libcpp (in MAP)
    4.Json in this string representation
    5.CBOR (in Array and Map)
    6.XDR
    7.protobuf

It will take the SP70C data example.

## explanation

### data receive

It will generate data possibly sent by a SP70C sensor into a structure of SP70C data:

| SensorData struct         | SensorVersion struct | SensorStatus struct | TargetStatus struct | TargetInfo struct |
| ------------------------- |--------------------- | ------------------- | ------------------- | ----------------- |
|typedef struct<br/> {<br/>**SensorVersion** version;<br/>**SensorStatus** sStatus;<br/> **TargetStatus** tStatus; <br/> **TargetInfo** *tInfo; <br/> uint8_t tInfoSize; <br/> } SensorData;|typedef struct<br/>{<br/>uint8_t dataType;<br/>bool result;<br/>uint8_t master;<br/>uint8_t second;<br/>uint8_t step;<br/>}SensorVersion;|typedef struct<br/>{<br/>uint8_t actl_mode;<br/>uint8_t rollcount;<br/>uint8_t cfgStatus;<br/>}SensorStatus;|typedef struct<br/>{<br/>uint8_t noOfTarget;<br/>uint8_t rollcount;<br/>}TargetStatus;|typedef struct<br/>{<br/>uint8_t  index;<br/>float  rcs;<br/>float range;<br/>int16_t  azimuth;<br/>float vrel;<br/>uint8_t  rollCount;<br/>int8_t  SNR;<br/>}TargetInfo;|

And it will test `DATA_TESTED` times the same value . It will be our input test vector for the benchmark.

### serialization

After that it will test all different serializations and de-serializations. We use the **MONOTONIC CLOCK** to calculate the time passed in each case. Before to print the result, it will **verify** if all data is well serialized.

To have a significant bench, it will create a binary for each serialization techno. In this case, each lib will not interface with another.

## How use it

You can go to the file **config-parsing.h** to change macros to change which serialization would be tested, and the number of data tested.
To have a better result the `DATA_TESTED` macro is set to 1000000.

create a build folder:

    cd build
    cmake [-DBENCH_OPTION=ON] ..
    make
    ./benchmark_<serialization>

You can also use Valgrind to have a profiling:

    valgrind --tool=callgrind ./benchmark_<serialization>

### -DOPTION

Here is the list of different options to configure the bench :

    1. -DBENCH_JSON=ON
    2. -DBENCH_JSON=ON -DBENCH_JSON_ARRAY=ON
    3. -DBENCH_FASTJSON=ON
    4. -DBENCH_FASTJSON=ON -DBENCH_FASTJSON_ARRAY=ON
    5. -DBENCH_JSONCPP=ON
    6. -DBENCH_JSONSTRING=ON
    7. -DBENCH_CBOR=ON
    8. -DBENCH_CBOR=ON -DBENCH_CBOR_ARRAY=ON
    9. -DBENCH_XDR=ON
    10. -DBENCH_PROTOBUF=ON

### OPTION MAIN

Different possible option are able:

    1. --d=<NUMBER OF DATA>             : to run with a custom data number
    2. --t                              : to run a benchmark showing the time during the parsing
    3. --c=<FREQUENCE in nanosecond>    : to run a benchmark showing the percentage of CPU taken during the parsing

## RESULT EXAMPLE

    ====== BENCHMARKING DATA SERIALIZATION ====== 

    data tested : 1000000

    # JSON-C :
        Data : OK
        Time during the serialization: 4830.279633 ms

    # FASTJSON :
        Data : OK
        Time during the serialization: 4325.356560 ms

    # CBOR:
        Data : OK
        Time during the serialization: 3329.877594 ms

    # JSON-C Array :
        Data : OK
        Time during the serialization: 1738.954999 ms

    # FASTJSON Array :
        Data : OK
        Time during the serialization: 2517.455459 ms

    # JSON-CPP :
        Data : OK
        Time during the serialization: 34840.732209 ms

    # CBOR ARRAY:
        Data : OK
        Time during the serialization: 2044.524197 ms

    # PROTOBUF :
        Data : OK
        Time during the serialization: 1531.751197 ms

    # XDR :
        Data : OK
        Time during the serialization: 346.656860 ms

## Note

libjsonc: [https://github.com/json-c/json-c](https://github.com/json-c/json-c)  `apt install libjson-c-dev`

libcbor: [https://github.com/PJK/libcbor](https://github.com/PJK/libcbor) `apt install libcbor-dev`

lib nlohman/json: [https://github.com/nlohmann/json](https://github.com/nlohmann/json) `sudo apt install nlohmann-json-dev`
