#ifndef LIBMX_H
#define LIBMX_H

#ifdef __linux__
    #include <malloc.h> // For Linux
    #define malloc_size malloc_usable_size
#else
    #include <malloc/malloc.h> //For macOS
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

//structures
typedef struct s_list {
    void *data;
    struct s_list *next;
}              t_list;

//Basic. Utils pack //DONE
void mx_printint(int n); //done (100/100)
void mx_printchar(char c); //done (100/100)
void mx_printstr(const char *s); //done (100/100)
void mx_print_strarr(char **arr, const char *delim); //done (100/100)
char *mx_nbr_to_hex(unsigned long nbr); //done (100/100)
int mx_bubble_sort(int *arr, int size); //done (100/100)
int mx_quicksort(int *arr, int left, int right); //done (100/100)
//char *mx_itoa(int number); //done (100/100)

//Advanced. Utils pack //DONE
void mx_print_unicode(wchar_t c); //done (100/100)
double mx_pow(double n, unsigned int pow); //done (100/100)
int mx_sqrt(int x); //done (100/100)
unsigned long mx_hex_to_nbr(const char *hex); //done (100/100)
void mx_foreach(int* arr, int size, void(*f)(int)); //done (100/100)
int mx_binary_search(char **arr, int size, const char *s, int *count); //done (100/100)

//Basic. Strings pack
int mx_strlen(const char *s); //done (100/100)
void mx_swap_char(char *s1, char *s2); //done (100/100)
char *mx_strcpy(char *dst, const char *src); //done (100/100)
int mx_strcmp(const char *s1, const char *s2); //done (100/100)
char *mx_strcat(char *restrict s1, const char *restrict s2); //done (100/100)
char *mx_strnew(const int size); //done (100/100)
char *mx_strdup(const char *s1); //done (100/100)
char *mx_strjoin(const char *s1, const char *s2); //(75/100) need to be modyfied
void mx_strdel(char **str); //done (100/100)
void mx_del_strarr(char ***arr); //done (100/100)
char *mx_file_to_str(const char *file); //done (100/100)
int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd); //(90/100) it's Ok(Timeout problem)

//Advanced. Strings pack //DONE
char *mx_strncpy(char *dst, const char *src, int len); //done (100/100)
void mx_str_reverse(char *s); //done (100/100)
char *mx_strndup(const char *s1, size_t n); //done (100/100)
char *mx_strstr(const char *haystack, const char *needle); //done (100/100)
int mx_count_words(const char *str, char c); //done (100/100)
int mx_count_substr(const char *str, const char *sub); //done (100/100)
int mx_get_char_index(const char *str, char c); //done (100/100)
int mx_get_substr_index(const char *str, const char *sub); //done (100/100)
char *mx_strtrim(const char *str); //done (100/100)
char *mx_del_extra_spaces(const char *str); //done (100/100)
char **mx_strsplit(const char *s, char c); //done (100/100)
char *mx_replace_substr(const char *str, const char* sub, const char *replace); //done (100/100)

//Basic. Memory pack //DONE
void *mx_memset(void *b, int c, size_t len); //done (100/100)
void *mx_memcpy(void *restrict dst, const void *restrict src, size_t n); //done (100/100)
int mx_memcmp(const void *s1, const void *s2, size_t n); //done (100/100)
void *mx_realloc(void *ptr, size_t size); //done (100/100)

//Advanced. Memory pack
void *mx_memmove(void *dst, const void *src, size_t len); //done (100/100)
void *mx_memrchr(const void *s, int c, size_t n); //done (100/100)
void *mx_memccpy(void *restrict dst, const void *restrict src,int c, size_t n); //done (100/100)
void *mx_memchr(const void *s, int c, size_t n); //done (100/100)
void *mx_memmem(const void *big, size_t big_len, const void *little, size_t little_len); //(100/100)

//Basic. List pack //DONE
t_list *mx_create_node(void *data); //done (100/100)
void mx_push_front(t_list **list, void *data); //done (100/100)
void mx_push_back(t_list **list, void *data); //done (100/100)
void mx_pop_front(t_list **head); //done (100/100)
void mx_pop_back(t_list **head); //done (100/100)

//Advanced. List pack //DONE
int mx_list_size(t_list *list); //done (100/100)
t_list *mx_sort_list(t_list *lst, bool(*cmp)(void*, void*)); //done (100/100)

//My functions
int mx_tolower(int c);
bool mx_isdigit(char c);
bool mx_isspace(char c);
int mx_atoi(const char *str);
void mx_print_error(char *error);
int mx_strncmp(const char *src, const char *str, size_t n);
void mx_print_list(char ***list, char *dilim1, char *dilim2);
char *mx_itoa(long long number);
char *mx_path_build(char *str1, char *str2, char *str3);

#endif /* LIBMX_H */
