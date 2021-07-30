#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <getopt.h>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

static const char *user_id;
static size_t file_size_limit = 2 * 1024 * 1024;
static const char *invalid_exe_name[] = {
    "r2",
    "frida-trace",
    ""
};

typedef struct lnode {
    const char *node;
    struct lnode *next;
} list_node_t;

typedef list_node_t *list_t;

static list_t list_start;
static list_t list_end;

static int check_node(const char *file_name, struct stat *status) {
    if (status->st_mode & S_IXUSR) {
        // Executable file name should not in deny list.
        for(int i = 0; invalid_exe_name[i][0] != '\0'; i ++) {
            if (!strcmp(file_name, invalid_exe_name[i])) {
                return 2;
            }
        }
    }
    // File size should not bigger than 2MiB
    if (status->st_size > file_size_limit) {
        return 1;
    }
    return 0;
}

static const char* invalid_type_str(int type) {
    switch (type) {
    case 1:
        return "INVALID_BIG_FILE";
    case 2:
        return "INVALID_EXE_NAME";
    default:
        return "";
    }
}

static void list_node(const char *prefix, const char *file_name) {
    size_t name_size = strlen(prefix);
    size_t entry_size = strlen(file_name);
    char *node_name = malloc(name_size + entry_size + 2);

    strcpy(node_name, prefix);
    node_name[name_size] = '/';
    strcpy (&node_name[name_size + 1], file_name);
    node_name[name_size + 1 + entry_size] = 0;

    struct stat status;
    if(stat(node_name, &status)) {
        //fprintf(stderr, "Can't access \"%s\": %s\n", node_name, strerror(errno));
        return;
    }

    mode_t stat_mode = status.st_mode;
    off_t stat_size = status.st_size;
    time_t stat_mtime = status.st_mtime;

    int check_result = check_node(file_name, &status);
    if(check_result) {
        printf ("%s:%s:%s\n", user_id, node_name, invalid_type_str(check_result));
    }
	
    if(S_ISDIR(stat_mode) && 
        strcmp(file_name, ".") && 
        strcmp(file_name, "..")) {
        list_t tmp = malloc(sizeof(list_node_t));
        tmp->node = node_name;
        tmp->next = NULL;
        list_end->next = tmp;
        list_end = tmp;
    }
}

static void list_dir(const char *name) {
    DIR *dir = opendir (name);
    if (dir == NULL) {
        //fprintf(stderr, "Can't access dir \"%s\": %s\n", name, strerror(errno));
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        const char *entry_name = entry->d_name;
        list_node(name, entry_name);
    }
    closedir(dir);
}

static void list_main(const char *name) {
    struct stat status;
    if(stat(name, &status)) {
        //fprintf(stderr, "Can't access \"%s\": %s\n", name, strerror(errno));
        return;
    }
    if(S_ISDIR(status.st_mode)) {
        list_start = malloc(sizeof(list_node_t));
        list_start->node = name;
        list_start->next = NULL;
        list_end = list_start;
        while (list_start) {
            list_dir(list_start->node);
            list_start = list_start->next;
        }
    } else {
        list_node("", name);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Directory is needed here.");
        exit(1);
    }
    user_id = argv[1];
    list_main(argv[1]);
    return 0;
}