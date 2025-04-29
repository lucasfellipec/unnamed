#include <stdio.h>

#include "result.h"

void display_strategy_result(Result *result, size_t result_size) {
    printf("Entry_Time\tExact_Entry_Time\tExit_Time\tDir\tEntry_Price\tExit_Price\tPts\tTP_Price\tSL_Price\tTP_Pts\tSL_Pts\tResult\n");
    for (size_t i = 0; i < result_size; ++i) {
        display_timestamp(result[i].Entry_Time);
        display_timestamp(result[i].Exact_Entry_Time);
        display_timestamp(result[i].Exit_Time);
        printf("%c\t", result[i].Dir);
        printf("%.1f\t", result[i].Entry_Price);
        printf("%.1f\t", result[i].Exit_Price);
        printf("%.1f\t", result[i].Pts);
        printf("%.1f\t", result[i].Take_Profit);
        printf("%.1f\t", result[i].Stop_Loss);
        printf("%.1f\t", result[i].Take_Profit_Pts);
        printf("%.1f\t", result[i].Stop_Loss_Pts);
        printf("%s\n", result[i].Result);
    }
}
