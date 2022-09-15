#ifndef DOTENV_DOTENV_H
#define DOTENV_DOTENV_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @param path Can be a directory containing a file named .env, or the path of the env file itself
     * @param overwrite Existing variables will be overwritten
     * @return 0 on success, -1 if can't open the file
     */
    int env_load(const char *path, bool overwrite);

    char *env_get_force(const char *name);
    char *env_get(const char *name, char *def);
    int env_get_int(const char *name, int def);

#ifdef __cplusplus
}
#endif

#endif
