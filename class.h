#ifndef CODESTAT_CLASS_H
#define CODESTAT_CLASS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct SuffixMap {
    char *suffix_v;
    char *file_type;
} SuffixMap;

typedef struct ClassNode {
    struct ClassNode *next;
    char *class;
    unsigned int count;
} ClassNode;

typedef struct ClassLinkList {
    ClassNode *head;
} ClassLinkList;

/*初始化一个链表*/
ClassLinkList *class_list_init();

/*链表中插入新节点——头插*/
void class_list_insert(ClassLinkList *, char *class, unsigned int);

/*搜索特定class的节点*/
ClassNode *search_node_class(ClassLinkList *, char *);

/*销毁链表*/
void class_list_destroy(ClassLinkList *);

/*搜索映射表，匹配后缀和大类*/
char *get_map_dest(char *);

/*搜索map list，如果有该类型则累加count，否则插入该类型*/
ClassNode *search_map_node(char *, ClassLinkList *);

/*获取最终结果集*/
void map_result(ClassLinkList *, ClassLinkList *, unsigned int *);

/*创建存储文件类型和对应行数的链表*/
ClassLinkList *create_class_list(char *[], unsigned int);

/*打印class list*/
void print_class_list(ClassLinkList *);

#endif //CODESTAT_CLASS_H