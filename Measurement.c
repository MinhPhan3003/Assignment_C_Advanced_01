#include "Measurement.h"

//Create a record with flexible array member 
Measurement* Measurement_Create(uint16_t id, size_t count) {

	size_t total_size = sizeof(Measurement) + (count * sizeof(uint16_t));

	Measurement* buff = calloc(1,total_size);

	Measurement* temp = buff;
	if (temp == NULL) {
		printf("Created buffer for ID:%d fail\n", id);
		return NULL;
	}
	else {
		printf("ID:%d allocated bytes on Heap = %zu\n",id, total_size);
		buff->id = id;
		buff->sample_count = count;
		buff->timestamp = 0;
		buff->status = 0;
		return buff;
	}
}

//Destroy a record 
void Measurement_Destroy(Measurement** item) {
	//Check if pointer is not NULL 
	if (item != NULL && *item != NULL) {
		free(*item);
		*item = NULL;
	}
	else
		printf("item is already NULL");
}
