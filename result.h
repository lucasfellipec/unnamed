#ifndef RESULT_H_
#define RESULT_H_

#include "utils.h"

typedef struct {
    Datetime Entry_Time;
    Datetime Exact_Entry_Time;
    Datetime Exit_Time;
    char Dir;
    float Entry_Price;
    float Exit_Price;
    float Pts;
    float Take_Profit;
    float Stop_Loss;
    float Take_Profit_Pts;
    float Stop_Loss_Pts;
    char Result[3];
} Result;

extern void display_strategy_result(Result *result, size_t result_size);

#endif // RESULT_H_
