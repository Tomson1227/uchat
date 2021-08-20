#include "uchat.h"

void upload_old_dialogs(char **dialogs, int *ids, int n, t_chat *chat) {
    for (int i = 0; i < n; i++) {
        create_dialog(ids[i], dialogs[i], chat);
    }
}
