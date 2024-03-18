#include "class.h"

#define SUFFIX ".c",".h",".txt"
#define SUFFIX_NUM 3

SuffixMap suffixMap[] = {
        {".c",     "C"},
        {".h",     "C/C++ Header"},
        {".cpp",   "C++"},
        {".hpp",   "C++"},
        {".cc",    "C++"},
        {".cxx",   "C++"},
        {".js",    "JavaScript"},
        {".html",  "HTML"},
        {".htm",   "HTML"},
        {".css",   "CSS"},
        {".py",    "Python"},
        {".java",  "Java"},
        {".cs",    "C#"},
        {".rb",    "Ruby"},
        {".sh",    "Shell"},
        {".yml",   "YAML"},
        {".yaml",  "YAML"},
        {".sql",   "SQL"},
        {".go",    "Go"},
        {".rs",    "Rust"},
        {".kt",    "Kotlin"},
        {".r",     "R"},
        {".ts",    "TypeScript"},
        {".php",   "PHP"},
        {".pl",    "Perl"},
        {".scala", "Scala"},
        {".swift", "Swift"}
};

/*初始化一个链表*/
ClassLinkList *class_list_init() {
    ClassLinkList *list = (ClassLinkList *) malloc(sizeof(ClassLinkList)); //头节点分配内存
    if (list == NULL) {
        perror("Failed to allocate memory.");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    return list;
}

/*链表中插入新节点——头插*/
void class_list_insert(ClassLinkList *list, char *class, unsigned int count) {
    ClassNode *node = (ClassNode *) malloc(sizeof(ClassNode));
    if (node == NULL) {
        perror("Failed to allocate memory.");
        exit(EXIT_FAILURE);
    }
    node->class = strdup(class);  //安全起见，使用class的副本赋值给node->class，这里的内存是动态分配的
    node->count = count;
    node->next = list->head; //使头节点成为新节点的后继节点
    list->head = node;       //新节点更新为头节点
}

/*搜索特定class的节点*/
ClassNode *search_node_class(ClassLinkList *list, char *class) {
    ClassNode *node = list->head;
    while (node != NULL) {
        if (strcmp(node->class, class) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

/*销毁链表*/
void class_list_destroy(ClassLinkList *list) {
    ClassNode *node = list->head;//声明node指针指向头节点
    while (node != NULL) {
        ClassNode *temp = node;//暂存node
        node = node->next;//node后移
        free(temp->class);//回收前一个节点的class的内存
        free(temp);//回收前一个节点的内存
    }
    list->head = NULL;//重置链表指针为空
}

/*创建存储文件类型和对应行数的链表*/
ClassLinkList *create_class_list(char *suffix[], const unsigned int suffix_num) {
    ClassLinkList *list = class_list_init(); //初始化链表
    ClassNode *node;
    char *current_suffix;
    for (int i = 0; i < suffix_num; i++) {
        current_suffix = *(suffix + i);
        if ((node = search_node_class(list, current_suffix)) == NULL) { //没有搜索到对应class的节点，头插
            class_list_insert(list, current_suffix, 0);
        }
    }
    return list;
}

/*搜索映射表，匹配后缀和大类*/
char *get_map_dest(char *class) {
    unsigned int map_num = sizeof(suffixMap) / sizeof(suffixMap[0]);
    for (int i = 0; i < map_num; i++) {
        if (strcmp(suffixMap[i].suffix_v, class) == 0) {
            return suffixMap[i].file_type;
        }
    }
    return "Unknown";
}

/*搜索map list，如果有该类型则累加count，否则插入该类型*/
ClassNode *search_map_node(char *map_dest, ClassLinkList *map_list) {
    ClassNode *map_node = map_list->head;
    while (map_node != NULL) {
        if (strcmp(map_node->class, map_dest) == 0) {
            return map_node;
        }
        map_node = map_node->next;
    }
    return NULL;
}

/*获取最终结果集*/
void map_result(ClassLinkList *class_list, ClassLinkList *map_list, unsigned int *total_count) {
    ClassNode *class_node = class_list->head;
    while (class_node != NULL) {  //遍历所有class节点
        char *suffix_v = class_node->class; //获取class值
        char *map_dest = get_map_dest(suffix_v); //获取class的映射大类
        ClassNode *map_node = search_map_node(map_dest, map_list); //寻找class对应的映射大类在大类链表中的节点
        if (map_node == NULL) {
            class_list_insert(map_list, map_dest, class_node->count); //大类链表中没有找到对应节点就直接插入一个新的大类节点
        } else {
            map_node->count += class_node->count; //大类链表中找到对应节点就把其count加上class节点的count
        }
        *total_count += class_node->count;
        class_node = class_node->next;
    }
}

/*打印class list*/
void print_class_list(ClassLinkList *list) {
    ClassNode *node = list->head;
    while (node != NULL) {
        printf("%s:%d\n", node->class, node->count);
        node = node->next;
    }
}

