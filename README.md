# Serialization benchmarking

The aim's project has to compare different data serialization between :

    1.JSON in map
    2.JSON in Array
    3.CBOR in map
    4.CBOR in Array
    5.XDR
    6.protobuf(TODO)

It will take the SP70C data example.

## explanation

### data receive

It will generate data possibly sent by a SP70C sensor:

`0xaa 0xaa 0x0a 0x06 0x01 0x11 0x00 0x00 0x00 0x00 0x00 0x00 0x55 0x55 0xaa 0xaa 0x0b 0x07 0x01 0x02 0x00 0x00 0x00 0x00 0x00 0x00 0x55 0x55 0xaa 0xaa 0x0c 0x07 0x01 0x6b 0x01 0x86 0x2a 0x02 0xbf 0x79 0x55 0x55`

### data parse

Then it will parse data into a structure of SP70C data:

| SensorData struct         | SensorVersion struct | SensorStatus struct | TargetStatus struct | TargetInfo struct |
| ------------------------- |--------------------- | ------------------- | ------------------- | ----------------- |
|typedef struct<br/> {<br/>**SensorVersion** version;<br/>**SensorStatus** sStatus;<br/> **TargetStatus** tStatus; <br/> **TargetInfo** *tInfo; <br/> uint8_t tInfoSize; <br/> } SensorData;|typedef struct<br/>{<br/>uint8_t dataType;<br/>bool result;<br/>uint8_t master;<br/>uint8_t second;<br/>uint8_t step;<br/>}SensorVersion;|typedef struct<br/>{<br/>uint8_t actl_mode;<br/>uint8_t rollcount;<br/>uint8_t cfgStatus;<br/>}SensorStatus;|typedef struct<br/>{<br/>uint8_t noOfTarget;<br/>uint8_t rollcount;<br/>}TargetStatus;|typedef struct<br/>{<br/>uint8_t  index;<br/>float  rcs;<br/>float range;<br/>int16_t  azimuth;<br/>float vrel;<br/>uint8_t  rollCount;<br/>int8_t  SNR;<br/>}TargetInfo;|

And store `DATA_TESTED` times the same value in a buffer. It will be our input test vector for the benchmark.

### serialization

After that it will test all different serializations and de-serializations. We use the **MONOTONIC CLOCK** to calculate the time passed in each case. Before to print the result, it will **verify** if all data is well serialized.

## How use it

You can go to the file **config-parsing.h** to change macros to change which serialization would be tested, and the number of data tested.
To have a better result the `DATA_TESTED` macro is set to 1000000.

create a build folder:

    ```console
    cd build
    cmake ..
    make
    ./benchmark
    ```

You can also use Valgrind to have a profiling:

    ```console
    valgrind --tool=callgrind ./benchmark
    ```

## RESULT EXAMPLE

    ```
    ====== BENCHMARKING DATA SERIALIZATION ====== 
    data tested : 1000000
    # JSON MAP:
        Data : OK
        Time during the serialization: 4540.577748 ms


    # JSON ARRAY :
        Data : OK
        Time during the serialization: 1720.860678 ms


    # CBOR MAP:
        Data : OK
        Time during the serialization: 3496.097179 ms


    # CBOR ARRAY:
        Data : OK
        Time during the serialization: 2252.337715 ms


    # XDR :
        Data : OK
        Time during the serialization: 281.472919 ms

    ```

## Note

libjsonc: [https://github.com/json-c/json-c](https://github.com/json-c/json-c)  `apt install libjson-c-dev`

libcbor: [https://github.com/PJK/libcbor](https://github.com/PJK/libcbor) `apt install libcbor-dev`

libprotobuf: `apt install libprotobuf-c-dev`
