// Author: Brian Rieder
// Title: PA04-EchoLite.c

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * * argv)
{
	int i;
	for(i = 1; i < argc; ++i) {
		if(i > 1)
			printf(" ");
		printf("%s", argv[i]);
	}
	printf("\n");
	return EXIT_SUCCESS;
}
