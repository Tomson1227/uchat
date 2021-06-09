#include "libmx.h"

void mx_print_error(char *error)
{
    write(2, error, mx_strlen(error));
    exit(1);
}
