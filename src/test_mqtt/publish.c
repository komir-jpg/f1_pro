#include <mosquitto.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define RESET "\x1B[0m"
#define RED "\x1b[31m"
#define GRN "\x1b[32m"
#define YLW "\x1b[33m"
#define MAG "\x1b[35m"
#define CYN "\x1b[36m"

// Ctrl+C handler to stop sending messages
static volatile sig_atomic_t keep_running = 1;
void handle_sigint() { keep_running = 0; }
bool is_connected = false;

void on_connect(struct mosquitto *mosq, void *obj, int rc) {
  if (rc == 0) is_connected = true;
  printf(GRN "connected to broker whith code %d\n" RESET, rc);
}
void on_publish(struct mosquitto *mosq, void *obj, int id) {
  printf(CYN "message sent with ID: %d\n" RESET, id);
}

int main() {
  signal(SIGINT, handle_sigint);
  struct mosquitto *client;
  char *message = "message for the subscriber";

  mosquitto_lib_init();
  client = mosquitto_new(NULL, true, NULL);
  if (!client) {
    fprintf(stderr, RED "creation of new mosquitto instance failed\n" RESET);
  }

  // set the callback functions
  mosquitto_connect_callback_set(client, on_connect);
  mosquitto_publish_callback_set(client, on_publish);

  int tls_status = mosquitto_tls_set(
      client, "/etc/ssl/certs/ca-certificates.crt", NULL, NULL, NULL, NULL);

  if (tls_status != MOSQ_ERR_SUCCESS) {
    fprintf(stderr, "TLS setup error %s\n", mosquitto_strerror(tls_status));
    return -1;
  }

  printf(MAG
         "connecting to EMQX on port:8883 (unauth, "
         "encrypted)\n" RESET);
  if (mosquitto_connect(client, "broker.emqx.io", 8883, 30) !=
      MOSQ_ERR_SUCCESS) {
    fprintf(stderr, RED "cannot connect to the broker\n" RESET);
    return -1;
  }

  mosquitto_loop_start(client);

  // send the message to mosquitto every second we send a new message
  while (keep_running) {
    if (is_connected) {
      printf(YLW "Sending message to mqtt broker\n" RESET);
      if (mosquitto_publish(client, NULL, "test/f1/telemetry", strlen(message),
                            message, 0, false) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, RED "error occurred while sending message\n" RESET);
      }
    }
    sleep(1);
  }

  mosquitto_loop_stop(client, true);
  mosquitto_lib_cleanup();
}
