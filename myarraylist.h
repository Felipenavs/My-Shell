#ifndef MYARRAYLIST_H
#define MYARRAYLIST_H

/**
 * @struct array_t
 * @brief  A simple dynamic array (array list) of strings.
 *
 * This structure manages a resizable array of `char *` elements.
 * - `length`   : how many items are currently stored
 * - `capacity` : how many items can be stored before resizing
 * - `data`     : pointer to the actual array of string pointers
 */
typedef struct 
{
    unsigned length;
    unsigned capacity;
    char ** data;
} array_t;

/**
 * @brief Initialize a dynamic array.
 * 
 * @param al       Pointer to the array_t to initialize.
 * @param capacity Initial number of elements the array can hold.
 * @return -1 on failure (e.g. malloc failed).
 */
int al_init(array_t * al, unsigned capacity);

/**
 * @brief Free all memory used by the array.
 * 
 * @param al Pointer to the array_t to destroy.
 */
void al_destroy(array_t * al);

/**
 * @brief Append a string pointer to the dynamic array.
 * 
 * @param L    Pointer to the array_t to append to.
 * @param item Pointer to the string to add.
 * @return -1 on failure (e.g. malloc failed).
 */
int al_append(array_t * L, char * item);

#endif
