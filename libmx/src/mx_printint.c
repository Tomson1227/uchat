#include "libmx.h"

void mx_printint(int n)
{
	char arr[10] = {0};
	unsigned int num;
	int i = 0;
	
	if(n & 0x80000000) {
		mx_printchar('-');
		num = (n ^ 0xFFFFFFFF) + 1;
	}
	else
		num = (unsigned) n;
	
	if(num == 0)
		mx_printchar('0');

	for(; num != 0; num /= 10, ++i)
		arr[i] = ((num % 10) + '0');
	
	for(; i > 0;)
		mx_printchar(arr[--i]);
}
