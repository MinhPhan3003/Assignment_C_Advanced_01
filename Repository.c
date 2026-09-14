#include "Repository.h"

// size stand for number of ptr in current
bool Repository_Add(Measurement*** storage, size_t* size, size_t* capacity, Measurement* item) {
    if (storage == NULL || size == NULL || capacity == NULL || item == NULL) {
        return false;
    }

    //Check if there is the same created ID 
    for (size_t i = 0; i < *size; i++) {
        if ((*(*storage + i))->id == item->id) {
            printf("[WARNING] ID:%u already exists in storage\n", item->id);
            return false;
        }
    }

    size_t new_capacity;
    Measurement** temp;

    if (*size >= *capacity) {
        if (*capacity == 0) {
            new_capacity = 4;
        }
        else {
            new_capacity = *capacity * 2;
        }
        temp = realloc(*storage, (sizeof(Measurement*) * new_capacity));
        if (temp == NULL) {
            return false;
        }

        //Update the capacity and item's adress
        *storage = temp;
        *capacity = new_capacity;
    }

    //Adding item to the storage
    (*storage)[*size] = item;
    (*size)++;

    return true;
}
bool Repository_Delete(Measurement*** storage, size_t* size, uint16_t id) {
    if (storage==NULL|| *storage == NULL) {
        return false;
    }


    for (size_t i = 0; i < *size; i++) {
        if ((*(*storage + i)) != NULL) {
            if ((*(*storage + i))->id == id) {
                Measurement_Destroy((*storage + i));

                //Move all remain records from the delete record position
                for (size_t j = i; j < (*size - 1); j++) {
                    (*storage)[j] = (*storage)[j + 1];
                }
                (*storage)[*size - 1] = NULL;
                (*size)--;

                return true;
            }
        }
    }
    return false;
}

Measurement* Repository_Find(Measurement** storage, size_t size, uint16_t id) {
    if (storage == NULL) {
        printf("FINDING RECORD FAIL");
        return NULL;
    }

    else {
        for (size_t i = 0; i < size; i++)
        {
            if ((*(storage + i)) != NULL && (*(storage + i))->id == id) {
                printf("There is a record that you need\n");
                return *(storage + i);
            }
        }
    }
    return NULL;
}

void Repository_ForEach(Measurement** storage, size_t size, void (*callback)(const Measurement*)) {
    if (storage == NULL || size == 0 || callback == NULL) {
        printf("Invalid input\n");
        return;
    }
    for (size_t i = 0; i < size; i++) {
        if (*(storage + i) != NULL) {
            callback(*(storage + i));
        }
    }
}

void Repository_Clear(Measurement*** storage, size_t* size, size_t* capacity) {
    if (storage == NULL || *storage == NULL) return;
    for (size_t i = 0; i < *size; i++) {
        Measurement_Destroy(&(*storage)[i]);
    }
    free(*storage);
    *storage = NULL;
    *size = 0;
    *capacity = 0;
}
