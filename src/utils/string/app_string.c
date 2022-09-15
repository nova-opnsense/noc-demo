#include "app_string.h"
#include "stdint.h"
#include "stdio.h"

char * upcase_to_lowcase(char * str_in){
    int str_in_size = strlen(str_in);
    char * str_ret = (char*) malloc(str_in_size);
    strcpy(str_ret,str_in);
    for (size_t i = 0; i < str_in_size; i++)
    {
        /* code */
        if(str_ret[i] >= 'A' && str_ret[i] <= 'Z'){
            str_ret[i] = str_ret[i] + 32; // 'A' is 65 and 'a' is 97
        }
    }
    return str_ret;
}

char *app_strdup(char* str){
    if(!str){
        return NULL;
    }
    
    return strdup(str);
}

char *app_strstr(char* str1 , char* str2){
    if(!str1 || !str2){
        return NULL;
    }
    return strstr(str1,str2);
}

char * app_strndup(char* str , int size){
    if(!str){
        return NULL;
    }
    return strndup(str,size);
}