#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include "utils/log/log.h"
#include "utils/timer/timer.h"
#include "utils/cmd/cmd.h"
#include <fcntl.h>
#include "utils/file/file.h"

#define CMD_TIME 200

void timeout_cb(size_t timer_id, void *user_data)
{
    bool *flag = (bool *)(user_data);
    // log_info("timer cb");
    (*flag) = true;
}

cmd_ret_t execl_timeout(int timeout_ms, const char *cmd)
{
    cmd_ret_t ret;
    FILE *fp;
    /* Open the command for reading. */
    char *new_cmd = (char *)malloc(strlen(cmd) + 10);
    // Append stderr to stdout with 2>&1
    sprintf(new_cmd, "%s 2>&1", cmd);
    fp = popen(new_cmd, "r");
    if (fp == NULL)
    {
        pclose(fp);
        log_error("Failed to run command: %s \n", new_cmd);
    }
    fcntl(fp, F_SETFL, fcntl(fp, F_GETFL) | O_NONBLOCK);
    /* Read the output a line at a time - output it. */
    bool *timeout_flag = (bool *)malloc(sizeof(bool));
    *timeout_flag = false;
    size_t time_id = start_timer(timeout_ms, timeout_cb, TIMER_SINGLE_SHOT, timeout_flag);
    const int buf_size = 4096;
    int curr_size = buf_size;
    char *out = malloc(curr_size);
    char c;
    int i = 0;
    do
    {
        // log_info("1");
        c = fgetc(fp);
        if (feof(fp))
        {
            break;
        }
        if (curr_size - i < 5)
        {
            curr_size += buf_size;
            out = (char *)realloc(out, curr_size);
        }
        out[i++] = c;
    } while (!(*timeout_flag));
    stop_timer(time_id);
    pclose(fp);
    out[i] = 0;
    if (*timeout_flag)
    {
        log_info("timeout");
    }
    ret.err = (*timeout_flag) ? CMD_TIMEOUT : CMD_OK;
    ret.str = out;
    free(new_cmd);
    free(timeout_flag);
    return ret;
}

void execl_non_timeout(const char *cmd)
{
    FILE *fp;
    char *new_cmd = (char *)malloc(strlen(cmd) + 10);
    // Append stderr to stdout with 2>&1
    sprintf(new_cmd, "%s 2>&1", cmd);
    /* Open the command for reading. */
    fp = popen(cmd, "r");
    if (fp == NULL)
    {
        pclose(fp);
        log_error("Failed to run command: %s \n", cmd);
    }
    pclose(fp);
    return;
}

void execl_file(const char *path_to_file)
{
    int nb_lines;
    char **lines = read_line(path_to_file, &nb_lines);
    cmd_ret_t ret;
    if (nb_lines == 0 || lines == NULL)
    {
        log_error("file %s is empty or isn't exist", path_to_file);
    }
    for (size_t i = 0; i < nb_lines; i++)
    {
        /* code */
        execl_non_timeout(lines[i]);
    }
    return;
}