#ifndef CODESTAT_FILE_H
#define CODESTAT_FILE_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "class.h"

typedef struct FileNode {
    struct FileNode *next;
    char *file_name;
    char *suffix_v;
} FileNode;

typedef struct FileLinkList {
    FileNode *head;
} FileLinkList;

unsigned int one_file_rows(char *, unsigned int, const char *);

bool check_dir(const char *, const char *);

/*初始化文件链表*/
FileLinkList *file_list_init();

/*创建文件节点*/
FileNode *create_file_node(char *, char *);

/*文件链表中插入新节点——头插*/
void file_list_insert(FileLinkList *, FileNode *);

/*销毁链表*/
void file_list_destroy(FileLinkList *);

/*打印file list*/
void print_file_list(FileLinkList *);

/*打开文件夹句柄*/
DIR *open_dir(char *);

bool endswith_one(const char *, const char *);

bool endswith_all(const char *, char *[], unsigned int, char **);

void compute_rows(char *, ClassLinkList *, FileLinkList *);

void get_c_files_recursive(char *[], char *, const char *, unsigned int, FileLinkList *, int *);

void get_c_files(char *[], char *, unsigned int, FileLinkList *);

#endif //CODESTAT_FILE_H
