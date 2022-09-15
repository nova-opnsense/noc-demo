#pragma once
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"



void app_free(void * ptr);
bool app_mem_empty(void * ptr , size_t size);