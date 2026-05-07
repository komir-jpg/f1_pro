#include<mosquitto.h>
#include<stdio.h>

//mosquitto uses a callback function when a new connection is enstablished
//usefull for connectig to new topics
void on_connect(struct mosquitto *mosq, void *obj, int rc){
    printf("connected to broker with code %d\n", rc);
    if(rc == 0){
        //all good we can subscribe
        //now we listen to test/f1/telemety
        mosquitto_subscribe(mosq, NULL, "test/f1/telemety", 0);
    }
} 

//callback to message recived
void on_message(stuct mosquitto *mosq, void *obj, struct mosquitto message *msg){
    printf("message recived:\n");
    printf("message id: %d", msg->mid);
    printf("message payload topic: %s\n", msg->topic);
    printf("message payload len: %s\n", msg->payloadlen);
    printf("message payload: %s\n", (char*)msg->payload);
}

int main(){

    struct mosquitto *client;
    mosquitto_lib_init();

    //set a new client
    client = mosquitto_new(NULL, true, NULL);
    if(!client){fprintf(stderr,
            "creation of new mosquitto instance failed with ERRNO: %d", client);}
            

    //set the callbacks functions
    mosquitto_connect_callback_set(client, on_connect);
    mosquitto_message_callback_set(client, on_message);

    //connect to broker
    printf("connecting to test.mosquitto.org on port:1883 (unauth, unencrypted)\n");
    if(mosquitto_connect(client, "test/f1/telemety", 1883, 30) != MSQ_ERR_SUCCESS){
        fprintf(stderr, "cannot connect to the broker\n");
        return -1;
    }
    //loop NOT thread safe the call is blocking
    //TODO implement a bit mask error handler 
    mosquitto_loop(client, -1, 1);
    mosquitto_lib_cleanup();


}

