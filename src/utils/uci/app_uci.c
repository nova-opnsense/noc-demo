#include "stdio.h"
#include "app_uci.h"
#include "utils/cmd/cmd.h"
#include "utils/log/log.h"

char *app_uci_add(char *args, char *section)
{
    char cmd[200];
    sprintf(cmd, "uci add %s %s", args, section);
    cmd_ret_t ret = execl_timeout(2000, cmd);
    log_debug(cmd);
    if (ret.err == CMD_TIMEOUT)
    {
        log_error("%s timeout", cmd);
    }
    if (strcmp(ret.str, ""))
    {
        return ret.str;
    }
    else
    {
        return NULL;
    }
}

char *app_uci_set(char *args, char *value)
{
    char cmd[200];
    sprintf(cmd, "uci set %s=%s", args, value);
    cmd_ret_t ret = execl_timeout(2000, cmd);
    log_debug(cmd);
    if (ret.err == CMD_TIMEOUT)
    {
        log_error("%s timeout", cmd);
    }
    if (strcmp(ret.str, ""))
    {
        return ret.str;
    }
    else
    {
        return NULL;
    }
}

char *app_uci_get(char *args)
{
    char cmd[200];
    sprintf(cmd, "uci get %s", args);
    cmd_ret_t ret = execl_timeout(2000, cmd);
    log_debug(cmd);
    if (ret.err == CMD_TIMEOUT)
    {
        log_error("%s timeout", cmd);
        return NULL;
    }
    if (strcmp(ret.str, ""))
    {
        return ret.str;
    }
    else
    {
        return NULL;
    }
}

char *app_uci_delete(char *args)
{
    char cmd[200];
    sprintf(cmd, "uci delete %s", args);
    cmd_ret_t ret = execl_timeout(2000, cmd);
    log_debug(cmd);
    if (ret.err == CMD_TIMEOUT)
    {
        log_error("%s timeout", cmd);
    }
    if (strcmp(ret.str, ""))
    {
        return ret.str;
    }
    else
    {
        return NULL;
    }
}

char *app_uci_rename(char *args, char *new_name)
{
    char cmd[200];
    sprintf(cmd, "uci get %s", args);
    cmd_ret_t ret = execl_timeout(2000, cmd);
    log_debug(cmd);
    if (ret.err == CMD_TIMEOUT)
    {
        log_error("%s timeout", cmd);
    }
    if (strcmp(ret.str, ""))
    {
        return ret.str;
    }
    else
    {
        return NULL;
    }
}
char *app_uci_add_list(char *args, char *list)
{
    char cmd[200];
    sprintf(cmd, "uci add_list %s=%s", args, list);
    cmd_ret_t ret = execl_timeout(2000, cmd);
    log_debug(cmd);
    if (ret.err == CMD_TIMEOUT)
    {
        log_error("%s timeout", cmd);
    }
    if (strcmp(ret.str, ""))
    {
        return ret.str;
    }
    else
    {
        return NULL;
    }
}
char *app_uci_del_list(char *args, char *list)
{
    char cmd[200];
    if (list)
    {
        sprintf(cmd, "uci del_list %s=%s", args, list);
    }
    else
    {
        sprintf(cmd, "uci del_list %s", args);
    }

    cmd_ret_t ret = execl_timeout(2000, cmd);
    log_debug(cmd);
    if (ret.err == CMD_TIMEOUT)
    {
        log_error("%s timeout", cmd);
    }
    if (strcmp(ret.str, ""))
    {
        return ret.str;
    }
    else
    {
        return NULL;
    }
}
char *uci_show(char *args)
{
    char cmd[200];
    sprintf(cmd, "uci show %s", args);
    cmd_ret_t ret = execl_timeout(2000, cmd);
    log_debug(cmd);
    if (ret.err == CMD_TIMEOUT)
    {
        log_error("%s timeout", cmd);
        return NULL;
    }
    if (strcmp(ret.str, ""))
    {
        return ret.str;
    }
    else
    {
        return NULL;
    }
}

char *app_uci_commit(char *args)
{
    char cmd[200];
    sprintf(cmd, "uci commit %s", args);
    cmd_ret_t ret = execl_timeout(2000, cmd);
    log_debug(cmd);
    if (ret.err == CMD_TIMEOUT)
    {
        log_error("%s timeout", cmd);
    }
    if (strcmp(ret.str, ""))
    {
        return ret.str;
    }
    else
    {
        return NULL;
    }
}