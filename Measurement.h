#ifndef __MEASUREMENT_H
#define __MEASUREMENT_H

#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h> 
#include <stddef.h>
#include <stdbool.h>
#include <time.h>
typedef struct {
	uint16_t id;
	uint8_t status;
	uint32_t timestamp;
	size_t sample_count;
	int16_t samples[];
} Measurement;


Measurement* Measurement_Create(uint16_t id, size_t count);

void Measurement_Destroy(Measurement **item);



#endif
