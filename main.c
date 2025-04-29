#include <stdio.h>
#include <stdlib.h>

#include "times_and_trades.h"
#include "utils.h"
#include "candles.h"
#include "unnamed.h"

#define CANDLES_MAX_SIZE 1000000

const char *folders[] = {
    "/home/rabbithole/projects/streamlabs/bases/2025.01"
};

int main(int argc, char *argv[]) {
    Candle *candles = malloc(CANDLES_MAX_SIZE * sizeof(Candle));
    size_t candles_size = 0;

    int timeframe = atoi(argv[1]);
    int SMA_period = atoi(argv[2]);
    int EMA_period = atoi(argv[3]);

    size_t folders_size = sizeof(folders)/sizeof(folders[0]);
    size_t files_size = 0;
    char **files = get_csv_files(folders, folders_size, &files_size);

    for (size_t i = 0; i < 1; ++i) {
        size_t times_and_trades_size = 0;
        size_t cs_size = 0;

        Times_And_Trades *times_and_trades = generate_times_and_trades(files[i], &times_and_trades_size);
        Candle *cs = generate_candles(&cs_size, times_and_trades, times_and_trades_size, timeframe);

        for (size_t c = 0; c < cs_size; ++c) {
            candles[candles_size++] = cs[c];
        }

        free(cs);
        free(times_and_trades);
        free(files[i]);
    }

    calculate_SMA(candles, candles_size, SMA_period);
    calculate_EMA(candles, candles_size, EMA_period);

    unnamed(candles, candles_size); // Strategy

    free(candles);
    free(files);

    return 0;
}
