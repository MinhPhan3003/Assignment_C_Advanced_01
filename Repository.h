#ifndef __REPOSITORY_H
#define __REPOSITORY_H

#include <stdio.h>
#include <stdlib.h> 
#include <stdint.h> 
#include <stddef.h>
#include <stdbool.h>
#include "Measurement.h"

bool Repository_Add(Measurement*** storage, size_t* size, size_t* capacity, Measurement* item);

bool Repository_Delete(Measurement*** storage, size_t* size, uint16_t id);

Measurement* Repository_Find(Measurement** storage, size_t size, uint16_t id);	

void Repository_ForEach(Measurement** storage, size_t size, void (*callback)(const Measurement*));


#endif 
