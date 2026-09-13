#include "Callbacks.h"


void Print_Measurement_Callback(const Measurement* item) {
	printf("ID:%u\n", (item->id));
	printf("status:%u\n", (item->status));
	printf("timestamp:%d\n", (item)->timestamp);
	printf("sample count:%zu\n", (item)->sample_count);
}

void Average_Measurement_Callback(const Measurement* item) {
	const int16_t* p = (item->samples);
	int16_t sum = 0;

	if (item->sample_count == 0) {
		return;
	}
	for (size_t i = 0; i < (item->sample_count);i++){
		sum = sum + *(p+i);
	}

	double avg = (double)sum / (item->sample_count);
	printf("the avg value of ID:%u is %.1f\n",(item->id), avg);
}	

void CheckStatus_Measurement_Callback(const Measurement* item) {
    if (item == NULL) {
        return;
    }

    // Checking status flag
    if (item->status == 1) {
        printf(" Record ID:%u status normal.\n",item->id);
    }
    else {        
        printf(" [ERROR] Record ID:%u ",item->id);
    }
}
