#include<mosquitto.h>
#include<stdio.h>

//mosquitto uses a callback function when a new connection is enstablished
//usefull for connectig to new topics

int main(){

    struct mosquitto *client;
    mosquitto_lib_init();

    //set a new client
    client = mosquitto_new(NULL, true, NULL);

    
    mosquitto_lib_cleanup();


}

