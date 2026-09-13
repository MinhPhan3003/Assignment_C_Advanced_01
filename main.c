#include <stdio.h>
#include "Measurement.h"
#include "Repository.h"
#include "Callbacks.h"
int main(){

	Measurement** storage = NULL;
	Measurement* select = NULL;
	size_t size = 0;
	size_t capacity = 0;

	//============ TESTCASE for check create measurement ====================
	Measurement* m1;
	Measurement* m2;
	m1 = Measurement_Create(123, 5);

	if (m1 != NULL) {
		printf("m1 created successfully\n");
	}

	m2 = Measurement_Create(111, 2);

	//============== TESTCASE for adding new item ========

	if (Repository_Add(&storage, &size, &capacity, m1))
		printf("	Pointer'address: %p | ID: %u\n", (void*)storage[0], (storage[0]->id));

	if (Repository_Add(&storage, &size, &capacity, m2))
		printf("	Pointer'address: %p | ID: %u\n", (void*)storage[1], (storage[1]->id));
	
	//============== TESTCASE for destroy an item ====== 
	
	//Measurement_Destroy(&m1);
	//if (m1 == NULL) printf("The buffer of m1 has been destroyed");

	//============== TESTCASE for finding a record =========

	select = Repository_Find(storage, size, 123);
	select = Repository_Find(storage, size, 111);

	//============== TESTCASE for Callback ==================

	Repository_ForEach(storage, size, Average_Measurement_Callback);
}