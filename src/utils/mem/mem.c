#include "mem.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


void app_free(void * ptr){
    if(ptr){
        free(ptr);
    }
}

bool app_mem_empty(void * ptr , size_t size){
    bool result = true;
    unsigned char* end = ptr + size;
    unsigned char* i;

    for(i= ptr ; i<end; i++) {
        if(*i > 0) {
            result = false;
            break;
        }
    }
    return result;
}