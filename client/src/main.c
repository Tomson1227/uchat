#include <stdio.h>

int login_window(int argc, char *argv[]);
int chat_window(int argc, char **argv);


int main(int argc, char *argv[]) {

    login_window(argc, argv);
    chat_window(argc, argv);


    return 0;
}
