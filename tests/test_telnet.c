#include "time.h"
#include "msg_def.h"
#include "logger.h"

#define PUBLISH_INTERVAL    5
void  test_telnet(){
    time_t prev_time = time(NULL);
    // while(1){
    //     if(time(NULL) - prev_time  > PUBLISH_INTERVAL){
    //         emt_send_action(req);
    //         prev_time = time(NULL);
    //     }
    // }
    // emt_send_action(req);
    va_list args;
    char *new_cmd = create_cmd(SECTION_DEBUG, DEBUG_LEVEL, args);
    log_info("cmd: %s",new_cmd);
    free_cmd(new_cmd);
    return 0;
}