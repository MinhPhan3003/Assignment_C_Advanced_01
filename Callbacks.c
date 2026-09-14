#include "Callbacks.h"


void Print_Measurement_Callback(const Measurement* item) {
    printf("ID:%u\n", (item->id));

    printf("status:%u\n", (item->status));

    time_t t = (time_t)(item)->timestamp;
    printf("timestamp %s", ctime(&t));

    printf("sample count:%zu\n", (item)->sample_count);
}

void Average_Measurement_Callback(const Measurement* item) {
    const int16_t* p = (item->samples);
    int64_t sum = 0;

    if (item == NULL || item->sample_count == 0) {
        return;
    }
    for (size_t i = 0; i < (item->sample_count);i++) {
        sum = sum + *(p + i);
    }

    double avg = (double)sum / (double)(item->sample_count);
    printf("the avg value of ID:%u is %.1f\n", (item->id), avg);
}

void CheckStatus_Measurement_Callback(const Measurement* item) {
    if (item == NULL) {
        return;
    }

    // Checking status flag
    if (item->status == 1) {
        printf(" Record ID:%u status normal.\n", item->id);
    }
    else {
        printf(" [ERROR] Record ID:%u ", item->id);
    }
}
