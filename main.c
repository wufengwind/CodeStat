#include "file.h"
#include "str.h"

int main() {
    int suffix_num;
    unsigned int total_count = 0;
    puts("Please input directory path.");
    char *dir_path = (char *) malloc(sizeof(char) * INPUT_LENGTH);
    s_gets(dir_path);
    puts("Please input suffix like '.c' , split by space.");
    char *suffix_str = (char *) malloc(sizeof(char) * INPUT_LENGTH);
    s_gets(suffix_str);
    char **suffix = split_str_by_space_to_arr(suffix_str, &suffix_num);
    ClassLinkList *class_link_list = create_class_list(suffix, suffix_num);
    ClassLinkList *map_link_list = class_list_init();
    FileLinkList *file_link_list = file_list_init();
    get_c_files(suffix, dir_path, suffix_num, file_link_list);
    compute_rows(dir_path, class_link_list, file_link_list);
    map_result(class_link_list, map_link_list, &total_count);
    print_class_list(map_link_list);
    printf("Total:%d\n", total_count);
    free(suffix_str);
    free(dir_path);
    class_list_destroy(class_link_list);
    class_list_destroy(map_link_list);
    file_list_destroy(file_link_list);
}
