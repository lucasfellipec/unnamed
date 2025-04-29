#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "times_and_trades.h"

Times_And_Trades *generate_times_and_trades(const char *path, size_t *times_and_trades_size) {
    FILE *f = fopen(path, "r");
    if (!f) {
        perror("Error opening file\n");
        return NULL;
    }
    char line_buffer[1024];
    if (!fgets(line_buffer, sizeof(line_buffer), f)) {
        fclose(f);
        fprintf(stderr, "Empty file\n");
        return NULL;
    }
    Times_And_Trades *data = malloc(TIMES_AND_TRADES_SIZE * sizeof(Times_And_Trades));
    int rows = 0;
    int count = 0;
    while (fgets(line_buffer, sizeof(line_buffer), f) && rows < TIMES_AND_TRADES_SIZE) {
        char *cols[7];
        int col = 0;
        char *start = line_buffer;
        cols[0] = start;
        for (char *p = line_buffer; *p && col < 7; ++p) {
            if (*p == '\t') {
                *p = '\0';
                ++col;
                if (col < 7) {
                    cols[col] = p + 1;
                }
            } else if (*p == '\n' || *p == '\r') {
                *p = '\0';
            }
        }
        Datetime t;
        if (sscanf(cols[0], "%d.%d.%d", &t.y, &t.m, &t.d) != 3) {
            continue;
        }
        if (sscanf(cols[1], "%d:%d:%d.%d", &t.h, &t.mm, &t.s, &t.ms) != 4) {
            continue;
        }
        if (!strcmp(cols[4], "")) {
            continue;
        }
        float last = atof(cols[4]);
        float volume = atof(cols[5]);
        int flag = atoi(cols[6]);
        if (!((flag & 32) && (flag & 64))) {
            data[rows].timestamp = t;
            data[rows].price = last;
            data[rows].volume = volume;
            data[rows].real_volume = (double)volume * last;
            data[rows].flag = flag;
            data[rows].index = count++;
            if (flag & 32) {
                data[rows].agg = 'C';
            }
            if (flag & 64) {
                data[rows].agg = 'V';
            }
            ++rows;
        }
    }

    fclose(f);
    *times_and_trades_size = rows;
    return data;
}
