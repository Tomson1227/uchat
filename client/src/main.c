#include "uchat.h"

int login_window(int argc, char *argv[]);
int chat_window(int argc, char **argv);


int main(int argc, char *argv[]) {
    // login_window(argc, argv);

    check_args(argc, argv);
    t_config *config = address_setup(argv[1], argv[2]);
    chat_window(argc, argv);
    connection_setup(config);
    
    
    //SPAMMER
    int num = 0;
    while (1) {
        char *temp = (char*)calloc(128, sizeof(char));
        temp = send_rq_log_in_client("USER", "PASSWORD");
        enQueue(config->queue_send, temp);
        printf("[SPAMMER]   Queued: %s\n", temp);
        sleep(1);
        // if (0 == num % 10) {
        //     //dequeue
        //     while(!QueueisEmpty(config->queue_send)) {
        //     char *dq = deQueue(config->queue_send);
        //     printf("[SPAMMER]   Dequeued: %s\n", dq);
        //     free(dq);
        //     }
        // }
    }
    
    pthread_join(config->conn_thread, NULL);

    return 0;
}
