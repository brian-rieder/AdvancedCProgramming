#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

size_t my_strlen(const char * str)
{
	int len = 0;
	while(str[len] != '\0')
		++len;
	return len;
}

int my_countchar(const char * str, char ch)
{
	int i = 0, count = 0;
	while(str[i++] != '\0')
		if(str[i] == ch)
			++count;
	return count;
}

char * my_strchr(const char * str, int ch)
{
	int i = 0;
	while(str[i] != '\0') {
		if(str[i] == ch)
			return (char *) &str[i];
		++i;
	}
	if(str[i] == ch)
		return (char *) &str[i];
	return NULL;
}

char * my_strrchr(const char * str, int ch)
{
	
}

int main(int argc, char * * argv)
{
	const char * str = "Hello World!";
	printf("'%s'\n", my_strrchr(str, 'o')); // prints "'orld!'\n"
	printf("'%s'\n", my_strrchr(str, 'z')); // prints "'(null)'\n"
	                                        // i.e., my_strrchr(str, 'z') == NULL
	printf("'%s'\n", my_strrchr(str, '\0')); // prints "''\n" *
	return 0;
}
