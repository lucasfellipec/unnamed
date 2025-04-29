#ifndef CANDLES_H_
#define CANDLES_H_

#include "utils.h"
#include "times_and_trades.h"
#include "indicators.h"

#define CANDLES_SIZE 600

typedef enum {
    RED,
    GREEN,
    DOJI,
} Color;

typedef struct {
    Datetime timestamp;
    double open;
    double high;
    double low;
    double close;
    double real_volume;
    Color color;

    Indicators indicators;
} Candle;

extern Candle *generate_candles(size_t *cs_size, Times_And_Trades *data, size_t rows, int timeframe);

extern void calculate_SMA(Candle *cs, size_t candles_size, int P);
extern void calculate_EMA(Candle *cs, size_t candles_size, int P);

extern void display_candles(Candle *cs, size_t candles_size);

#endif // CANDLES_H_
