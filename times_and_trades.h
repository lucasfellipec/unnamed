#ifndef TT_H_
#define TT_H_

#include "utils.h"

#define TIMES_AND_TRADES_SIZE 2000000

typedef struct {
    Datetime timestamp;
    int index;
    float price;
    double real_volume;
    float volume;
    char agg;
    int flag;
} Times_And_Trades;

extern Times_And_Trades *generate_times_and_trades(const char *path, size_t *times_and_trades_size);

#endif // TT_H_
