#pragma once
#ifndef __APP_SPLIT__
#define __APP_SPLIT__
#include "string.h"

char** split(char * str,char * patern, int *size);
void split_free(char **split_ret ,size_t size);
#endif