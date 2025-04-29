#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "utils.h"

time_t timestamp_to_unix(Datetime t) {
    struct tm tm = {
        .tm_sec = t.s,
        .tm_min = t.mm,
        .tm_hour = t.h,
        .tm_mday = t.d,
        .tm_mon = t.m - 1,
        .tm_year = t.y - 1900,
    };
    return mktime(&tm);
}

Datetime time_to_timestamp(time_t t) {
    struct tm *tm_info = localtime(&t);
    Datetime timestamp = {
        .m = tm_info->tm_mon + 1,
        .d = tm_info->tm_mday,
        .y = tm_info->tm_year + 1900,
        .h = tm_info->tm_hour,
        .mm = tm_info->tm_min,
        .s = tm_info->tm_sec
    };
    return timestamp;
}

int is_same_day(Datetime a, Datetime b) {
    return a.y == b.y && a.m == b.m && a.d == b.d;
}

void display_timestamp(Datetime t) {
    printf("%04d-%02d-%02d %02d:%02d:%02d.%03d\t", t.y, t.m, t.d, t.h, t.mm, t.s, t.ms);
}

int is_csv_file(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if (dot && !strcmp(dot, ".csv")) {
        return 1;
    }
    return 0;
}

char **get_csv_files(const char **folders, size_t size, size_t *files_size) {
    char **files = malloc(MAX_FILES * sizeof(char *));
    size_t files_count = 0;
    for (size_t i = 0; i < size; ++i) {
        const char *folder_path = folders[i];
        DIR *dir;
        struct dirent *entry;
        dir = opendir(folder_path);
        if (!dir) {
            perror("Error opening folder\n");
            return NULL;
        }
        while ((entry = readdir(dir)) != NULL) {
            if (is_csv_file(entry->d_name) && files_count < MAX_FILES) {
                char path[512];
                snprintf(path, sizeof(path), "%s/%s", folder_path, entry->d_name);
                files[files_count] = strdup(path);
                ++files_count;
            }
        }
        closedir(dir);
    }
    qsort(files, files_count, sizeof(char *), compare_strings);
    *files_size = files_count;
    return files;
}

int compare_strings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}
