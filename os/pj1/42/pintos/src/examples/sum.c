#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
#include"userprog/syscall.h"
#include"lib/user/syscall.h"

int main(int argc, char *argv[])
{
	int a, b, c, d;
	int pibo, sum;
	
	a = atoi(argv[1]);	
	b = atoi(argv[2]);	
	c = atoi(argv[3]);	
	d = atoi(argv[4]);

	pibo = pib(a);
	sum = sofi(a, b, c, d);
	printf("%d %d\n",pibo,sum);
	return EXIT_SUCCESS;
}
