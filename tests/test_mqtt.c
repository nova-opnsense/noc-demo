
#include "mqtt_handle.h"
#include "time.h"

int test_mqtt(){
    mqtt_connect(MOSQUITTO_HOST,MOSQUITTO_PORT,60);
    mqtt_loop();
    clock_t prev_clock = clock();
    while(1){
        if(clock() - prev_clock  > PUBLISH_INTERVAL){
            mqtt_publish_message(NULL,"test",4,"payload",1,false);
            prev_clock = clock();
        }
    }
    mqtt_clean();
}