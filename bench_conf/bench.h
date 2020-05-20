#pragma once

typedef struct {
    int numberData;
    int timer;
    int cpu;
} benchConfigT;

benchConfigT* benchParseArgs(int argc, char* argv[]);

