
#include <stdio.h>
#include <stdlib.h>
#include "Measurement.h"
#include "Repository.h"
#include "Callbacks.h"


static int read_long(const char* prompt, long* out) {
    printf("%s", prompt);
    if (scanf("%ld", out) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return 0;
    }
    return 1;
}

static int read_char(const char* prompt, char* out) {
    printf("%s", prompt);
    if (scanf(" %c", out) != 1) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return 0;
    }
    return 1;
}

// Create a record, fill its samples, then add it to storage.
void action_add(Measurement*** storage, size_t* size, size_t* capacity) {
    long id;
    long count;
    char choice;

    if (!read_long("Enter ID: ", &id)) {
        printf("  [Error] Invalid ID input.\n");
        return;
    }
    if (id < 0 || id > 65535) {
        printf("  [Error] ID must be between 0 and 65535.\n");
        return;
    }

    if (!read_long("Enter sample count: ", &count)) {
        printf("  [Error] Invalid sample count input.\n");
        return;
    }

    if (count < 0) {
        printf("  [Error] Sample count cannot be negative.\n");
        return;
    }

    Measurement* m = Measurement_Create((uint16_t)id, (size_t)count);
    if (m == NULL) {
        printf("  [Error] Create failed.\n");
        return;
    }

    // Fill the flexible array member
    for (size_t i = 0; i < (size_t)count; i++) {
        long v;
        while (1) {
            char prompt[32];
            snprintf(prompt, sizeof(prompt), "sample[%zu] = ", i);

            if (!read_long(prompt, &v)) {
                printf("  [ERROR] Please enter a valid number.\n");
                continue;  
            }
            if (v < -32768 || v > 32767) {
                printf("  [ERROR] Value out of range\n");
                continue;  
            }
            break;  
        }
        m->samples[i] = (int16_t)v;
    }

    do {
        printf("Do you want to add this Measurement to Storage? (y/n): ");
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y') {
            if (Repository_Add(storage, size, capacity, m)) {
                printf("-> Added ID:%ld (size=%zu, capacity=%zu)\n", id, *size, *capacity);
            }
            else {
                printf(" [ERROR] ADD FAILED.\n");
                Measurement_Destroy(&m);
            }
            break;
        }

        if (choice == 'n' || choice == 'N') {
            printf("Measurement discarded.\n");
            Measurement_Destroy(&m);
            break;
        }
        printf("Please enter y or n.\n");

    } while (1);
}

int main(void) {
    Measurement** storage = NULL;   // start empty; Repository_Add allocates lazily
    size_t size = 0;
    size_t capacity = 0;
    long choice;

    do {
        printf("\n=========== MEASUREMENT MANAGER ===========\n");
        printf(" 1. Create Measurement and add to Storage\n");
        printf(" 2. Find Record by ID\n");
        printf(" 3. Delete Record by ID\n");
        printf(" 4. Print all Records\n");
        printf(" 5. Average of each Record\n");
        printf(" 6. Check status of each Record\n");
        printf(" 7. Show size / capacity\n");
        printf(" 0. Exit\n");
        printf("-------------------------------------------\n");

        if (!read_long("Enter choice = ", &choice)) {
            printf("[ERROR] Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            //Create new record and add to storage
        case 1:
            action_add(&storage, &size, &capacity);
            break;

            //Search a record base on ID
        case 2: {
            long id;

            if (!read_long("Enter ID to find: ", &id)) {
                printf("  [Error] Invalid ID input.\n");
                break;
            }
            if (id < 0 || id > 65535) {
                printf("  [Error] ID must be between 0 and 65535.\n");
                break;
            }

            Measurement* found = Repository_Find(storage, size, (uint16_t)id);
            if (found != NULL)
                printf(" -> Found ID:%u\n", found->id);
            else
                printf(" -> ID:%ld not found (miss).\n", id);
            break;
        }
              //Delete a record base on ID
        case 3: {
            long id;

            if (!read_long("Enter ID to delete: ", &id)) {
                printf("  [Error] Invalid ID input.\n");
                break;
            }
            if (id < 0 || id > 65535) {
                printf("  [Error] ID must be between 0 and 65535.\n");
                break;
            }

            if (Repository_Delete(&storage, &size, (uint16_t)id))
                printf(" -> Deleted ID:%ld (size=%zu)\n", id, size);
            else
                printf(" -> ID:%ld not found.\n", id);
            break;
        }

        case 4:
            Repository_ForEach(storage, size, Print_Measurement_Callback);
            break;

        case 5:
            Repository_ForEach(storage, size, Average_Measurement_Callback);
            break;

        case 6:
            Repository_ForEach(storage, size, CheckStatus_Measurement_Callback);
            break;

        case 7:
            printf("  size=%zu | capacity=%zu\n", size, capacity);
            break;

        case 0:
            printf("Cleaning up and exiting...\n");
            break;

        default:
            printf("[Error] Invalid choice.\n");
        }
    } while (choice != 0);

    // Final cleanup
    Repository_Clear(&storage, &size, &capacity);
    printf("All memory freed.\n");
    return 0;
}
