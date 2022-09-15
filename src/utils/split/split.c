#include "string.h"
#include "utils/log/log.h"
#include "utils/string/app_string.h"

char **split(char *str, char *pattern, int *size)
{
   // Extract the first token
   char **ret = NULL;
   int now_size = 0;
   char *token = strtok(str, pattern);
   // loop through the string to extract all other tokens
   while (token != NULL)
   {
      now_size++;
      ret = (char **)realloc(ret, now_size * sizeof(char *));
      ret[now_size - 1] = app_strdup(token);
      token = strtok(NULL, pattern);
   }
   *size = now_size;
   return ret;
}

void split_free(char **split_ret, size_t size)
{
   for (size_t i = 0; i < size; i++)
   {
      free(split_ret[i]);
   }
   free(split_ret);
}