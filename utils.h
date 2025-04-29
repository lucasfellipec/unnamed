#ifndef UTILS_H_
#define UTILS_H_

#include <time.h>

#define MAX_FILES 4096

typedef struct {
    int h;
    int mm;
    int s;
    int ms;
} Time;

typedef struct {
    int m;
    int d;
    int y;
    int h;
    int mm;
    int s;
    int ms;
} Datetime;

extern time_t timestamp_to_unix(Datetime t);
extern Datetime time_to_timestamp(time_t t);
extern int is_same_day(Datetime a, Datetime b);
extern void display_timestamp(Datetime t);

extern int is_csv_file(const char *filename);
extern char **get_csv_files(const char **folders, size_t size, size_t *files_size);
extern int compare_strings(const void *a, const void *b);

#endif // UTILS_H_
