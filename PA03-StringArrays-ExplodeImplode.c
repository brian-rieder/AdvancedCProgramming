// Author: Brian Rieder
// Title: PA03-StringArrays-ExplodeImplode.c

#include <string.h>
#include "answer03.h"

char * strcat_ex(char * * dest, int * n, const char * src)
{
    int dest_len = *dest == NULL ? 0 : strlen(*dest);
    int src_len = strlen(src);
    if((*dest == NULL) || (src_len + dest_len + 1 > *n)) {
        *n = 1 + 2 * (src_len + dest_len);
        char * new_dest = malloc(*n * sizeof(char));	
        *new_dest = '\0';
        if(*dest != NULL)
            strcpy(new_dest, *dest);
        free(*dest);
        *dest = new_dest;
    }
    strcat(*dest, src);
    return *dest;
}

char * * explode(const char * str, const char * delims, int * arrLen)
{
    const char * itr, * last;
    // Find number of strings
    *arrLen = 1;
    itr = str;
    while(*itr) {
       if(strchr(delims, *itr))
           (*arrLen)++;
       itr++;
    }
    char * * output_arr = malloc(*arrLen * sizeof(char *));
    int arrInd = 0;
    int len; // for the length of strings
    last = str;
    itr = str;
    while(*itr) {
        if(strchr(delims, *itr) != NULL) {
            len = itr - last;
            output_arr[arrInd] = malloc((len+1) * sizeof(char));
            memcpy(output_arr[arrInd], last, len);
            output_arr[arrInd][len] = '\0';
            last = itr + 1;
            ++arrInd;
        }
        ++itr;
    }
    // Handle the final string
    len = itr - last;
    output_arr[arrInd] = malloc((len+1) * sizeof(char));
    memcpy(output_arr[arrInd], last, len);
    output_arr[arrInd][len] = '\0';

    return arr;
}

char * implode(char * * strArr, int len, const char * glue)
{
    char * ret = NULL;
    int n = 0;
    int i;
    for(i = 0; i < len; ++i) {
        if(i > 0)
            strcat_ex(&ret, &n, glue);
        strcat_ex(&ret, &n, strArr[i]);
    }
    return ret;
}

static int strcmpvoid(const void * a, const void * b)
{
    return strcmp(*((const char * *) a), *((const char * *) b));
}

void sortStringArray(char * * arrString, int len)
{
    qsort(arrString, len, sizeof(char*), strcmpvoid);
}

static int charcmpvoid(const void * a, const void * b)
{
    return (int) *((const char *) a) - (int) *((const char *) b);
}

void sortStringCharacters(char * str)
{
    qsort(str, strlen(str), sizeof(char), charcmpvoid);
}

void destroyStringArray(char * * strArr, int len)
{
    if(strArr == NULL) return;
    int i;
    for(i = 0; i < len; ++i) 
        free(strArr[i]);
    free(strArr);
}
