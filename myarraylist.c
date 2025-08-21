#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "myarraylist.h"

void al_init(array_t *L, unsigned cap)
{
    L->length = 0;
    L->capacity = cap;
    L->data = malloc(sizeof(char*) * cap);
}


void al_destroy(array_t *L)
{
    for(int i=0; i<L->length; i++){
        free(L->data[i]);
    }
    free(L->data);
}

void al_append(array_t *L, const char *item)
{
    if (L->length == L->capacity) {
        L->capacity *= 2;
        char **temp = realloc(L->data, L->capacity * sizeof(char *));
        if (!temp) {
            perror("realloc failed");
            return;
        }
        L->data = temp;
    }

    L->data[L->length] = strdup(item);
    if (!L->data[L->length]) {
        perror("strdup failed");
        return;
    }

    L->length++;
}
