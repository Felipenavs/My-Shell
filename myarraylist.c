#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "myarraylist.h"

int al_init(array_t *L, unsigned cap)
{
    L->length = 0;
    L->capacity = cap;
    L->data = malloc(sizeof(char*) * cap);
    if(!L->data)
    {
        perror("malloc failed");
        return -1;
    }

    return 0;
}


void al_destroy(array_t *L)
{
    for(int i=0; i<L->length; i++){ //loops through the whole array and frees all the items
        free(L->data[i]);
    }
    free(L->data);
}

int al_append(array_t * L, char * item)
{
    if (L->length == L->capacity) 
    {
        L->capacity *= 2;
        char **temp = realloc(L->data, L->capacity * sizeof(char *));
        if (!temp)
        {
            perror("realloc failed");
            return -1;
        }
        L->data = temp;
    }
    

    L->data[L->length] = strdup(item);
    if (!L->data[L->length]) 
    {
        perror("strdup failed");
        return -1;
    }

    L->length++;
    return 0;
}

int al_null_terminator(array_t * L)
{
    if (L->length == L->capacity) 
    {
        L->capacity *= 2;
        char **temp = realloc(L->data, L->capacity * sizeof(char *));
        if (!temp)
        {
            perror("realloc failed");
            return -1;
        }
        L->data = temp;
    }
    
    L->data[L->length] = NULL;
    return 0;
}
