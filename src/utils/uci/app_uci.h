#ifndef __EZM_UCI_H__
#define __EZM_UCI_H__


char* app_uci_add(char * args , char* section);
char* app_uci_set(char* args , char * value);
char* app_uci_get(char * args);
char* app_uci_delete(char* args );
char* app_uci_rename(char * args , char * new_name);
char* app_uci_add_list(char * args , char * list);
char* app_uci_del_list(char *args ,  char * list);
char *uci_show(char *args);
char* app_uci_commit(char *args);


#endif