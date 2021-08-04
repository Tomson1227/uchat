#ifndef STRUCT_H
#define STRUCT_H

typedef struct s_ush t_ush;

typedef struct s_args {
    int number;
    char **value;
}              t_args;

typedef struct s_process {
    char *file;
    char *command;
    char **args;
    int status;
    pid_t pid;
    posix_spawn_file_actions_t *actions;
    posix_spawnattr_t *attr;
}              t_process;

typedef struct s_process_list {
    t_process *process;
    struct s_process_list *next_process;
    struct s_process_list *prev_process;
}              t_process_list;

typedef struct s_command_list {
    char *command;
    struct s_command_list *next_command;
    struct s_command_list *prev_command;
}              t_command_list;

typedef struct s_tab_func {
    uint16_t var_num;
    uint16_t line_index;
    int16_t var_index;
    uint16_t tab_press;
    char **bin_dirs;
    char **variants;
    char *last_arg;
    char *serch_arg;
    char *path; 
    bool command;
}              t_tab_func;


typedef struct s_shell_env_list {
    char *variable; 
    char *value; 
    bool exp;
    struct s_shell_env_list *next_list;
}              t_shell_env_list;

struct s_ush {
    char *prompt;
    char *home;
    char **args;
    char **bin_dirs;
    char **built_in;
    volatile int status;
    volatile int local_status;
    struct termios term;
    struct termios oterm;
    t_process *process;
    t_shell_env_list *env;
    t_command_list *command_list;
    t_process_list *process_list;
};

typedef struct s_line {
    char *line;
    char get_char;
    int position;
    t_tab_func *tab_func;
    t_command_list *last_command;
}              t_line;

#endif /* All struct */
