#ifndef __APP_CMD_H__
#define __APP_CMD_H__

typedef enum {
    CMD_OK,
    CMD_TIMEOUT
}cmd_err_t;

typedef struct {
    cmd_err_t err;
    char *str;
}cmd_ret_t;



cmd_ret_t execl_timeout(int timeout_ms, const char* cmd);
void execl_non_timeout(const char* cmd );
void execl_file(const char* path_to_file);

#endif