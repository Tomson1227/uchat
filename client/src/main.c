#include "uchat.h"

int login_window(int argc, char *argv[]);
int chat_window(int argc, char **argv);


int main(int argc, char *argv[]) {

    check_args(argc, argv);
    t_config *config = setup_address(argv[1], argv[2]);
    client_connect(config);
    launch_threads(config);
    config->send_buf = malloc(sizeof(char) * 128);

    int num = 0;
    while (1) {
        //simulate data interchange
        sprintf(config->send_buf, "Sending request no %d", num);
        config->send_singal = 1;
        num++;
        while(0 == config->send_singal) {
            //wait for signal being sent
        }
        sleep(1);
    }

    //login_window(argc, argv);
    //chat_window(argc, argv);
    pthread_join(config->send_thread, NULL);
    pthread_join(config->recv_thread, NULL);

    return 0;
}
