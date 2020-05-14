# Serialization benchmarking

The aim's project has to compare different data serialization between :

    1.JSON in map
    2.JSON in Array
    3.CBOR in map
    4.CBOR in Array
    5.XDR
    6.protobuf

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

To have a significant bench, it is recommended to create a binary for each serialization techno.

## How use it

You can go to the file **config-parsing.h** to change macros to change which serialization would be tested, and the number of data tested.
To have a better result the `DATA_TESTED` macro is set to 1000000.

create a build folder:

    cd build
    cmake [-DBENCH_OPTION=ON] ..
    make
    ./benchmark

You can also use Valgrind to have a profiling:

    valgrind --tool=callgrind ./benchmark

### -DOPTION

Here is the list of different options to configure the bench :

    1. -DBENCH_JSON=ON
    2. -DBENCH_JSON=ON -DBENCH_JSON_ARRAY=ON
    3. -DBENCH_CBOR=ON
    4. -DBENCH_CBOR=ON -DBENCH_CBOR_ARRAY=ON
    5. -DBENCH_XDR=ON
    6. -DBENCH_PROTOBUF=ON

## RESULT EXAMPLE

    ====== BENCHMARKING DATA SERIALIZATION ====== 

    data tested : 1000000

    # JSON MAP:
        Data : OK
        Time during the serialization: 4352.010288 ms


    # JSON ARRAY :
        Data : OK
        Time during the serialization: 1711.330259 ms


    # CBOR MAP:
        Data : OK
        Time during the serialization: 3100.252335 ms


    # CBOR ARRAY:
        Data : OK
        Time during the serialization: 3016.003068 ms


    # PROTOBUF :
        Data : OK
        Time during the serialization: 1451.426229 ms


    # XDR :
        Data : OK
        Time during the serialization: 313.980669 ms



## Note

libjsonc: [https://github.com/json-c/json-c](https://github.com/json-c/json-c)  `apt install libjson-c-dev`

libcbor: [https://github.com/PJK/libcbor](https://github.com/PJK/libcbor) `apt install libcbor-dev`

lib nlohman/lson: [https://github.com/nlohmann/json](https://github.com/nlohmann/json) `sudo apt install nlohmann-json-dev`
