#include "libmx.h"

// char *mx_itoa(int number)
// {
//     char *str = (char *)malloc(12);
// 	int count = 0;
// 	unsigned int num;
//     int flag = 0;

// 	if(number & 0x80000000) {
//         flag = 1;
// 		str[count++] = '-';
// 		num = (number ^ 0xFFFFFFFF) + 1;
// 	}
// 	else {
// 		num = (unsigned) number;
//     }

// 	if(num == 0) {
// 		str[count++] = '0';
//     }

// 	for(; num != 0; num /= 10) {
// 		str[count++] = ((num % 10) + '0');
//     }

//     for(int i = 0; (i + flag) < (count + flag) / 2; ++i) {
//             char temp = str[i + flag];
//             str[i + flag] = str[count - i - 1];
//             str[count - i - 1] = temp;
//         }

//     str[count] = '\0';

//     return str;
// }
char *mx_itoa(long long number) {
    char *str = (char *)malloc(21);
	int count = 0;
	unsigned long long num;
    int flag = 0;

	if(number & 0x8000000000000000) {
        flag = 1;
		str[count++] = '-';
		num = (number ^ 0xFFFFFFFFFFFFFFFF) + 1;
	}
	else {
		num = (unsigned long long) number;
    }

	if(num == 0) {
		str[count++] = '0';
    }

	for(; num != 0; num /= 10) {
		str[count++] = ((num % 10) + '0');
    }

    for(int i = 0; (i + flag) < (count + flag) / 2; ++i) {
            char temp = str[i + flag];
            str[i + flag] = str[count - i - 1];
            str[count - i - 1] = temp;
        }

    str[count] = '\0';

    return str;
}
