#include "utils/file/file.h"
#include "utils/log/log.h"
#include "utils/split/split.h"
#include "stdlib.h"
#include "stdio.h"
#include <unistd.h>
#include <sys/stat.h>
#include "pthread.h"

static pthread_mutex_t mutex_lock;

cJSON *load_json_from_file(char *path_to_file)
{
    // read file
    char *data = read_data(path_to_file);

    cJSON *error_table_json = cJSON_Parse(data);
    free(data);

    return error_table_json;
}

int write_data(char *data, int data_size, char *path_to_file, int mode) // mode is append or overwrite
{
    pthread_mutex_lock(&mutex_lock);
    FILE *fp;
    if (mode == TRUNCATE)
    {
        fp = fopen(path_to_file, "w");
    }
    else if (mode == APPEND)
    {
        fp = fopen(path_to_file, "a");
    }
    else
    {
        log_error("Mode must in range[%d-%d] to write to file: %d", APPEND, TRUNCATE, mode);
        return -1;
    }

    if (fp == NULL)
    {
        log_error("Cannot read file: %s", path_to_file);
        return -1;
    }
    fwrite(data, 1, data_size, fp);
    fclose(fp);
    pthread_mutex_unlock(&mutex_lock);
    return 0;
}

char *read_data(char *path_to_file)
{
    size_t len;
    char *data;
    pthread_mutex_lock(&mutex_lock);
    FILE *f = fopen(path_to_file, "r");
    if (f == NULL)
    {
        log_error("Cannot read file: %s", path_to_file);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    data = (char *)malloc(len + 1);
    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);
    pthread_mutex_unlock(&mutex_lock);
    return data;
}

char **read_line(char *path_to_file, int *nb_lines)
{
    int num_lines;
    char *file_data = read_data(path_to_file);
    if (file_data == NULL)
    {
        log_error("Cannot read file %s", path_to_file);
        return NULL;
    }
    char **lines = split(file_data, "\n", &num_lines);
    if (lines == NULL)
    {
        log_error("Cannot split file %s with multiple line", path_to_file);
        return NULL;
    }
    *nb_lines = num_lines;
    free(file_data);
    return lines;
}

bool check_file_exist(char *path_to_file)
{
    pthread_mutex_lock(&mutex_lock);
    FILE *f = fopen(path_to_file, "r");
    if (f == NULL)
    {
        log_error("Cannot read file: %s", path_to_file);
        return 0;
    }
    pthread_mutex_unlock(&mutex_lock);
    return 1;
}