#include "str.h"

void *s_gets(char *st) {
    int i = 0;
    fgets(st, INPUT_LENGTH, stdin);
    if (st) {
        while (st[i] != '\n' && st[i] != '\0')
            i++;
        if (st[i] == '\n')
            st[i] = '\0';
        else
            while (getchar() != '\n')
                continue;
    }
    //return ret_val;
}

char **split_str_by_space_to_arr(char *input_str, int *suffix_num) {
    unsigned capacity = INITIAL_SIZE;
    char **result = (char **) malloc(capacity * sizeof(char *));
    if (result == NULL) {
        perror("Failed to allocate memory.");
        exit(EXIT_FAILURE);
    }
    char *token;
    const char *delimiter = " ";
    int count = 0;
    token = strtok(input_str, delimiter);
    while (token != NULL) {
        if (count >= capacity) {
            capacity += INCREMENT_SIZE;
            result = (char **) realloc(result, capacity * sizeof(char *));
            if (result == NULL) {
                perror("Failed to allocate memory.");
                exit(EXIT_FAILURE);
            }
        }
        result[count] = strdup(token);
        if (result[count] == NULL) {
            perror("Failed to allocate memory.");
            exit(EXIT_FAILURE);
        }
        count++;
        token = strtok(NULL, delimiter);
    }
    if (count >= capacity) {
        capacity += INCREMENT_SIZE;
        result = (char **) realloc(result, capacity * sizeof(char *));
        if (result == NULL) {
            perror("Failed to allocate memory.");
            exit(EXIT_FAILURE);
        }
    }
    result[count] = NULL;
    *suffix_num = count;
    return result;
}