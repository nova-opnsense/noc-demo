#include "stdlib.h"
#include "stdio.h"
#include <cjson/cJSON.h>
#include "stdbool.h"

#define APPEND 0
#define TRUNCATE 1

#define FILE_TYPE 0
#define DIR_TYPE 1

cJSON *load_json_from_file(char *path_to_file);
int write_data(char *data, int data_size, char *path_to_file, int mode);
char *read_data(char *path_to_file);
char **read_line(char *path_to_file, int *nb_lines);
bool check_file_exist(char *path_to_file);