#include <mosquitto.h>
#include <stdio.h>

#define RESET "\x1B[0m"
#define RED "\x1b[31m"
#define GRN "\x1b[32m"
#define YLW "\x1b[33m"
#define MAG "\x1b[35m"
#define CYN "\x1b[36m"

void on_log(struct mosquitto *mosq, void *obj, int level, const char *str) {
  printf(CYN "[LOG] %s\n" RESET, str);
}

// mosquitto uses a callback function when a new connection is enstablished
// usefull for connectig to new topics
void on_connect(struct mosquitto *mosq, void *obj, int rc) {
  printf(GRN "connected to broker with code %d\n" RESET, rc);
  if (rc == 0) {
    // all good we can subscribe
    // now we listen to test/f1/telemety
    mosquitto_subscribe(mosq, NULL, "test/f1/telemetry", 0);
  }
}

// callback to message recived
void on_message(struct mosquitto *mosq, void *obj,
                const struct mosquitto_message *msg) {
  printf(YLW "message recived:\n" RESET);
  printf("[message] id: %d\n", msg->mid);
  printf("[message] " MAG "payload topic: %s\n" RESET, msg->topic);
  printf("[message] " CYN "payload len: %d\n" RESET, msg->payloadlen);
  printf("[message] payload: %s\n", (char *)msg->payload);
}

int main() {
  struct mosquitto *client;
  mosquitto_lib_init();

  // set a new client
  client = mosquitto_new("mirko_f1_test_001", true, NULL);
  if (!client) {
    fprintf(stderr, RED "creation of new mosquitto instance failed\n" RESET);
  }

  // set the callbacks functions
  mosquitto_connect_callback_set(client, on_connect);
  mosquitto_message_callback_set(client, on_message);
  mosquitto_log_callback_set(client, on_log);

  // TLS connection
  int tls_status = mosquitto_tls_set(
      client, "/etc/ssl/certs/ca-certificates.crt", NULL, NULL, NULL, NULL);

  if (tls_status != MOSQ_ERR_SUCCESS) {
    fprintf(stderr, "TLS setup error %s\n", mosquitto_strerror(tls_status));
    return -1;
  }

  // connect to broker
  printf(MAG
         "connecting to EMQX on port:8883 (unauth, "
         "encrypted)\n" RESET);
  if (mosquitto_connect(client, "broker.emqx.io", 8883, 30) !=
      MOSQ_ERR_SUCCESS) {
    fprintf(stderr, RED "cannot connect to the broker\n RESET");
    return -1;
  }
  // loop NOT thread safe the call is blocking
  // TODO implement a bit mask error handler
  mosquitto_loop_forever(client, -1, 1);
  mosquitto_destroy(client);
  mosquitto_lib_cleanup();
}
