#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unnamed.h"
#include "candles.h"
#include "utils.h"
#include "result.h"

#define SPREAD 0.5
#define TAKE_PROFIT 20
#define STOP_LOSS 20

Result result[8192] = {0};
size_t result_size = 0;

void unnamed(Candle *cs, size_t candles_size) {
    for (size_t i = 1; i < candles_size; ++i) {
        double SMA = cs[i - 1].indicators.SMA;
        double EMA = cs[i - 1].indicators.EMA;

        int ctd_i = 0;
        double entry_price = 0;
        double take_profit = 0;
        double stop_loss = 0;

        int Can = 0;
        int Hit = 0;

        if (SMA - EMA > 0) {
            entry_price = cs[i - 1].high + SPREAD;
            for (size_t g = 0; g < cs[i].ctd_size; ++g) {
                if (cs[i].ctd[g].price > entry_price) {
                    ctd_i = cs[i].ctd[g].i;
                    result[result_size].Exact_Entry_Time = cs[i].ctd[g].timestamp;
                    Can = 1;
                    break;
                }
            }
            if (Can) {
                take_profit = entry_price + TAKE_PROFIT;
                stop_loss = entry_price - STOP_LOSS;

                result[result_size].Entry_Time = cs[i].timestamp;
                result[result_size].Entry_Price = entry_price;
                result[result_size].Take_Profit = take_profit;
                result[result_size].Stop_Loss = stop_loss;
                result[result_size].Take_Profit_Pts = TAKE_PROFIT;
                result[result_size].Stop_Loss_Pts = STOP_LOSS;
                result[result_size].Dir = 'C';

                for (size_t c = i; c < candles_size; ++c) {
                    if (cs[c].is_last_candle) {
                        result[result_size].Pts = (result[result_size].Dir == 'C' ? cs[c].close - entry_price : entry_price - cs[c].close);
                        strcpy(result[result_size].Result, (result[result_size].Pts > 0 ? "TP" : "SL"));
                        result[result_size].Exit_Time = cs[c].timestamp;
                        break;
                    }
                    for (size_t n = (c == i ? ctd_i : 0); n < cs[c].ctd_size; ++n) {
                        if (cs[c].ctd[n].price > take_profit) {
                            strcpy(result[result_size].Result, "TP");
                            result[result_size].Exit_Time = cs[c].ctd[n].timestamp;
                            result[result_size].Pts = TAKE_PROFIT;
                            Hit = 1;
                            break;
                        } else if (cs[c].ctd[n].price < stop_loss) {
                            strcpy(result[result_size].Result, "SL");
                            result[result_size].Exit_Time = cs[c].ctd[n].timestamp;
                            result[result_size].Pts = STOP_LOSS * -1;
                            Hit = 1;
                            break;
                        }
                    }
                    if (Hit) {
                        break;
                    }
                }
                ++result_size;
            }
        }
    }
}
