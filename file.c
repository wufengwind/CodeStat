#include "file.h"

#define MAXSIZE 1000

unsigned int one_file_rows(char *dir_path, unsigned int dir_path_length, const char *filename) {
    unsigned int counts = 0;
    size_t full_path_length = dir_path_length + strlen(filename) + 1;
    char *full_path = (char *) malloc(sizeof(char) * full_path_length);
    snprintf(full_path, full_path_length, "%s%s", dir_path, filename);
    FILE *fp = fopen(full_path, "r");
    char buffer[MAXSIZE];
    if (fp == NULL) {
        printf("%s open fail!\n", full_path);
    } else {
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            if (buffer[0] != '\0' && buffer[0] != '\n') {
                counts += 1;
            }
        }
    }
    fclose(fp);
    free(full_path);
    return counts;
}

bool check_dir(const char *dir_path, const char *filename) {
    struct stat file_stat;
    unsigned int file_path_length = strlen(dir_path) + strlen(filename) + 2;
    char *file_path = (char *) malloc(sizeof(char) * file_path_length);
    snprintf(file_path, file_path_length, "%s/%s", dir_path, filename);
    if (stat(file_path, &file_stat) == -1) {
        perror("Failed to get file status.");
        exit(EXIT_FAILURE);
    }
    free(file_path);
    return S_ISDIR(file_stat.st_mode);
}

/*初始化文件链表*/
FileLinkList *file_list_init() {
    FileLinkList *file_list = (FileLinkList *) malloc(sizeof(FileLinkList)); //头节点分配内存
    if (file_list == NULL) {
        perror("Failed to allocate memory.");
        exit(EXIT_FAILURE);
    }
    file_list->head = NULL;
    return file_list;
}

/*创建文件节点*/
FileNode *create_file_node(char *file_name, char *suffix_v) {
    FileNode *file_node = (FileNode *) malloc(sizeof(FileNode));
    if (file_node == NULL) {
        perror("Failed to allocate memory.");
        exit(EXIT_FAILURE);
    }
    file_node->next = NULL;
    file_node->file_name = strdup(file_name);
    file_node->suffix_v = strdup(suffix_v);
    return file_node;
}

/*文件链表中插入新节点——头插*/
void file_list_insert(FileLinkList *file_list, FileNode *file_node) {
    file_node->next = file_list->head; //使头节点成为新节点的后继节点
    file_list->head = file_node;       //新节点更新为头节点
}

/*销毁链表*/
void file_list_destroy(FileLinkList *file_list) {
    FileNode *node = file_list->head;//声明node指针指向头节点
    while (node != NULL) {
        FileNode *temp = node;//暂存node
        node = node->next;//node后移
        free(temp->file_name);//回收为文件名分配的内存
        free(temp->suffix_v);//回收为后缀分配的内存
        free(temp);//回收前一个节点的内存
    }
    file_list->head = NULL;//重置链表指针为空
}

/*打印file list*/
void print_file_list(FileLinkList *file_list) {
    FileNode *node = file_list->head;
    while (node != NULL) {
        printf("FILENAME:%s\tSUFFIX:%s\n", node->file_name, node->suffix_v);
        node = node->next;
    }
}

/*打开文件夹句柄*/
DIR *open_dir(char *dir_path) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Failed to open dir.");
        exit(EXIT_FAILURE);
    }
    return dir;
}


bool endswith_one(const char *file_name, const char *suffix) {
    size_t file_name_len = strlen(file_name);
    size_t suffix_len = strlen(suffix);
    if (file_name_len < suffix_len)
        return false;
    return strncmp(file_name + file_name_len - suffix_len, suffix, suffix_len) == 0;
}

bool endswith_all(const char *file_name, char *suffix[], const unsigned int suffix_num, char **suffix_v) {
    for (int i = 0; i < suffix_num; i++) {
        if (endswith_one(file_name, suffix[i])) {
            *suffix_v = (char *) suffix[i];
            return true;
        }
    }
    return false;
}

void compute_rows(char *dir_path, ClassLinkList *class_list, FileLinkList *file_list) {
    FileNode *file_node = file_list->head;
    ClassNode *class_node;
    char *file_name;
    unsigned int rows;
    unsigned int dir_path_length = strlen(dir_path);
    while (file_node != NULL) {
        file_name = file_node->file_name;
        rows = one_file_rows(dir_path, dir_path_length, file_name);
        class_node = search_node_class(class_list, file_node->suffix_v);
        class_node->count += rows;
        file_node = file_node->next;
    }
}

void get_c_files_recursive(char *suffix[], char *dir_path, const char *relative_path, const unsigned int suffix_num, FileLinkList *file_link_list, int *count) {
    *count++;
    DIR *dir = open_dir(dir_path);  //打开文件夹
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        char *filename = entry->d_name;
        if (strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0) {
            continue;  // 忽略 . 和 .. 目录
        }
        size_t full_path_length = strlen(dir_path) + strlen(filename) + 2; //要存'/'和'\0'
        char *full_path = (char *) malloc(full_path_length * sizeof(char));
        snprintf(full_path, full_path_length, "%s/%s", dir_path, filename);
        size_t next_relative_path_length = strlen(relative_path) + strlen(filename) + 2;//要存'/'和'\0'
        char *next_relative_path = (char *) malloc(next_relative_path_length * sizeof(char));
        snprintf(next_relative_path, next_relative_path_length, "%s/%s", relative_path, filename);
        if (check_dir(dir_path, filename)) {
            get_c_files_recursive(suffix, full_path, next_relative_path, suffix_num, file_link_list, count);
        } else {
            char *suffix_v;
            if (endswith_all(filename, suffix, suffix_num, &suffix_v)) {
                FileNode *file_node = create_file_node(next_relative_path, suffix_v);
                file_list_insert(file_link_list, file_node);
                free(next_relative_path);
            }
        }
    }
    closedir(dir);
}

void get_c_files(char *suffix[], char *dir_path, const unsigned int suffix_num, FileLinkList *file_link_list) {
    int count = 0;
    get_c_files_recursive(suffix, dir_path, "", suffix_num, file_link_list, &count);
}

