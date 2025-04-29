#include <stdio.h>
#include <stdlib.h>

#include "candles.h"
#include "utils.h"

Candle *generate_candles(size_t *cs_size, Times_And_Trades *data, size_t rows, int timeframe) {
    time_t current_interval_start = 0;
    int count = 0;
    int price = 0;
    int open = 0;
    int high = (int)-1e9;
    int low = (int)1e9;
    int close = 0;
    double real_volume_sum = 0;
    Color color;
    size_t j = 0;

    Candle *cs = malloc(CANDLES_SIZE * sizeof(Candle));

    for (size_t i = 0; i < rows; ++i) {
        time_t row_time = timestamp_to_unix(data[i].timestamp);
        time_t interval_start = (row_time/timeframe) * timeframe;
        if (interval_start != current_interval_start) {
            if (count > 0) {
                cs[j].timestamp = time_to_timestamp(current_interval_start);
                cs[j].open = (float)open/10.0;
                cs[j].high = (float)high/10.0;
                cs[j].low = (float)low/10.0;
                cs[j].close = (float)close/10.0;
                cs[j].real_volume = real_volume_sum;
                color = (open > close ? RED : open < close ? GREEN : DOJI);
                cs[j].color = color;
                ++j;
            }
            current_interval_start = interval_start;
            price = (int)(data[i].price * 10);
            open = price;
            high = (int)-1e9;
            low = (int)1e9;
            real_volume_sum = 0;
            count = 0;
        }
        price = (int)(data[i].price * 10);
        if (price > high) {
            high = price;
        }
        if (price < low) {
            low = price;
        }
        real_volume_sum += data[i].real_volume;
        close = price;
        ++count;
    }
    if (count > 0) {
        cs[j].timestamp = time_to_timestamp(current_interval_start);
        cs[j].open = (float)open/10.0;
        cs[j].high = (float)high/10.0;
        cs[j].low = (float)low/10.0;
        cs[j].close = (float)close/10.0;
        cs[j].real_volume = real_volume_sum;
        color = (open > close ? RED : open < close ? GREEN : DOJI);
        cs[j].color = color;
        ++j;
    }

    *cs_size = j;

    return cs;
}

void calculate_SMA(Candle *cs, size_t candles_size, int sma_period) {
    for (size_t i = sma_period; i < candles_size; ++i) {
        double sum = 0.0;
        for (int j = 0; j < sma_period; ++j) {
            sum += cs[i - j].high;
        }
        cs[i].indicators.SMA = sum/sma_period;
    }
}

void calculate_EMA(Candle *cs, size_t candles_size, int exp_ma_period) {
    /*double smoothing_factor = 2.0/(exp_ma_period + 1);
    cs[0].exp_ma = cs[0].close;
    for (size_t i = 1; i < candles_size; ++i) {
        cs[i].exp_ma = (cs[i].close * smoothing_factor) + (cs[i - 1].exp_ma * (1 - smoothing_factor));
    }*/
    // EMA as SMA for while...
    for (size_t i = exp_ma_period; i < candles_size; ++i) {
        double sum = 0.0;
        for (int j = 0; j < exp_ma_period; ++j) {
            sum += cs[i - j].close;
        }
        cs[i].indicators.EMA = sum/exp_ma_period;
    }
}

void display_candles(Candle *cs, size_t candles_size) {
    printf("date\ttimestamp\topen\thigh\tlow\tclose\treal_volume\tcolor\tSMA\tEMA\n");
    for (size_t i = 0; i < candles_size; ++i) {
        printf("%04d.%02d.%02d\t%02d:%02d:%02d.%03d\t%.1f\t%.1f\t%.1f\t%.1f\t%.1lf\t%d\t%.1f\t%.1f\n", cs[i].timestamp.y, cs[i].timestamp.m, cs[i].timestamp.d, cs[i].timestamp.h, cs[i].timestamp.mm, cs[i].timestamp.s, cs[i].timestamp.ms, cs[i].open, cs[i].high, cs[i].low, cs[i].close, cs[i].real_volume, cs[i].color, cs[i].indicators.SMA, cs[i].indicators.EMA);
    }
}
