#pragma once

typedef struct {
    int numberData;
    int timer;
    int cpu;
    int freq;
} benchConfigT;

benchConfigT* benchParseArgs(int argc, char* argv[]);

