#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

#define ADDRESS     "35.212.161.165"
#define CLIENTID    "Laptop"
#define TOPIC       "home/project-2"
#define QOS         1
#define TIMEOUT     10000L

int main(int argc, char* argv[]) {
    printf("Attempting to initialize client...\n");
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc; // response code
    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS) {
        printf("Client initialization unsuccessful, rc=%d\n", rc);
        exit(EXIT_FAILURE);
    }
    conn_opts.keepAliveInterval = 20; // keep alive to 20 seconds
    conn_opts.cleansession = 1; // make it clean
    conn_opts.connectTimeout = 5; // timeout in 5 seconds
    printf("Client initialization successful\n");
    printf("Attempting to connect to broker...\n");
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Connection unsuccessful, timeout=5s, rc=%d\n", rc);
        exit(EXIT_FAILURE);
    }
    printf("Connection successful\n");
    printf("Type '1' to send payload 'ON'\nType '2' to send payload 'OFF'\nType '3' to quit\n");

    MQTTClient_message msg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    char ch = '0';
    while (ch != '3') {
        ch = getchar(); // char entered
        char* payload_msg;
        if (ch == '1') {
            payload_msg = "ON";
        } else if (ch == '2') {
            payload_msg = "OFF";
        } else {
            continue;
        }
        msg.payload = payload_msg;
        msg.payloadlen = (int) strlen(payload_msg);
        msg.qos = QOS;
        msg.retained = 0;
        if ((rc = MQTTClient_publishMessage(client, TOPIC, &msg, &token)) != MQTTCLIENT_SUCCESS) {
            printf("Failed to publish message, rc=%d\n", rc);
            exit(EXIT_FAILURE);
        }
        MQTTClient_waitForCompletion(client, token, TIMEOUT);
        printf("Sending payload '%s'\n", payload_msg);
    }
    if ((rc = MQTTClient_disconnect(client, 10000)) != MQTTCLIENT_SUCCESS) printf("Failed to disconnect, rc=%d\n", rc);
    MQTTClient_destroy(&client);
    return rc;
}