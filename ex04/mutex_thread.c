/*
* 시스템 프로그래밍 Lab4 실습 4번
* 파일 이름: mutex_thread.c
* 20213004 안수진
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_CLIENTS 10

typedef struct {
    char message[256];
    pthread_mutex_t mutex;
    pthread_cond_t cond_var;
} shared_data_t;

shared_data_t shared_data = {"", PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};

void* client_thread(void* arg) {
    shared_data_t* data = (shared_data_t*)arg;

    pthread_mutex_lock(&data->mutex);
    printf("Client: Received message: %s\n", data->message);
    pthread_mutex_unlock(&data->mutex);

    return NULL;
}

void* server_thread(void* arg) {
    shared_data_t* data = (shared_data_t*)arg;

    pthread_mutex_lock(&data->mutex);
    snprintf(data->message, sizeof(data->message), "Hello from server!");
    printf("Server: Broadcasted message\n");
    pthread_cond_broadcast(&data->cond_var);
    pthread_mutex_unlock(&data->mutex);

    return NULL;
}

int main() {
    pthread_t clients[MAX_CLIENTS];
    pthread_t server;

    pthread_create(&server, NULL, server_thread, &shared_data);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        pthread_create(&clients[i], NULL, client_thread, &shared_data);
    }

    pthread_join(server, NULL);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        pthread_join(clients[i], NULL);
    }

    pthread_mutex_destroy(&shared_data.mutex);
    pthread_cond_destroy(&shared_data.cond_var);

    return 0;
}
