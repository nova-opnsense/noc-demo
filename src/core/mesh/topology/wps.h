#ifndef __WPS_H__
#define __WPS_H__

void add_device_check( void * user_data);
void remove_device_check( void * user_data);
void timeout_check(void * user_data);
void polling_wps( void * user_data);

#endif