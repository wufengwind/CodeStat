#ifndef CODESTAT_STR_H
#define CODESTAT_STR_H
#define INPUT_LENGTH 1000
#define INITIAL_SIZE 10
#define INCREMENT_SIZE 5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void *s_gets(char *);

/*返回指向指针数组的第一个元素的指针，也就是指向指针的指针*/
char **split_str_by_space_to_arr(char *, int *);

#endif //CODESTAT_STR_H
