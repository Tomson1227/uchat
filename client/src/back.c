#include "back.h"

/* Static functons */
static void *back_handler(void *ptr);

/* Function definitions */

void back_setup(t_chat *chat) {
    printf("In Back Setup\n");
    printf("Back ptr in back: %p\n", (void*)(chat->config->back_thread));
    if(0 != pthread_create(&(chat->config->back_thread), NULL, back_handler, chat)) {
        fprintf(stderr, "[BACK ERR]    Failed to create back_thread\n");
        exit(2);
    } else {
        printf("[BACK]    Back_thread created\n");
    }
}

static void *back_handler(void *ptr) {
    t_chat *chat = (t_chat *)ptr;
    /*Some routine */
    while(1) {
        if (!QueueisEmpty(chat->config->queue_recv)) {
            printf("[BACK]    New response from server\n");
            char *dq = deQueue(chat->config->queue_recv);
            process_rs_client(dq, chat);
            free(dq);
            dq = NULL;
        }
    }
}
