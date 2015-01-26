// Author: Brian Rieder
// Title: PA04-CatLite.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

void printHelpMessage()
{
    printf("Usage: cat-lite [--help] [FILE]...\n"
	   "With no FILE, or when FILE is -, read standard input.\n"
	   "\n"
	   "Examples:\n"
	   "  cat-lite README   Print the file README to standard output.\n"
	   "  cat-lite f - g    Print f's contents, then standard input, \n"
	   "                    then g's contents.\n"
	   "  cat-lite          Copy standard input to standard output.\n"
	   "\n");
}

int processFile(const char * filename)
{
	int isStdin = (strcmp(filename, "-") == 0);
	FILE * file_ptr = isStdin ? stdin : fopen(filename, "r");
	if(file_ptr == NULL)
		return FALSE;
	char letter;
	while((letter = fgetc(file_ptr)) != EOF)
		fputc(letter, stdout);
	if(!isStdin)
		fclose(file_ptr);
	return TRUE;
}

int main(int argc, char * * argv)
{
	int i;
	for(i = 1; i < argc; ++i) {
		if(strcmp(argv[i], "--help") == 0) {
			printHelpMessage();
			return EXIT_SUCCESS;
		}
	}
	for(i = 1; i < argc; ++i) {
		if(!processFile(argv[i])) {
			fprintf(stderr, "Unable to open %s\n", argv[i]);
			return EXIT_FAILURE;
		}
	}
	if(argc == 1) {
		processFile("-");
		return EXIT_SUCCESS;
	}
	return EXIT_SUCCESS;
}
