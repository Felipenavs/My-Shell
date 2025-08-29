#ifndef MYARRAYLIST_H
#define MYARRAYLIST_H

/**
 * @struct array_t
 * @brief  A simple dynamic array (array list) of strings.
 *
 * This structure manages a resizable array of `char *` elements.
 * - `length`   : number of items currently stored
 * - `capacity` : number of items that can be stored before resizing
 * - `data`     : pointer to the array of string pointers
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
 * @return 0 on success, -1 on failure (e.g., malloc failed).
 */
int al_init(array_t * al, unsigned capacity);

/**
 * @brief Free all memory used by the array.
 * 
 * This frees the internal storage for string pointers, 
 * but does not free the strings themselves.
 * 
 * @param al Pointer to the array_t to destroy.
 */
void al_destroy(array_t * al);

/**
 * @brief Append a string pointer to the dynamic array.
 * 
 * The array automatically grows if more capacity is needed.
 * 
 * @param L    Pointer to the array_t to append to.
 * @param item Pointer to the string to add.
 * @return 0 on success, -1 on failure (e.g., malloc failed).
 */
int al_append(array_t * L, char * item);

/**
 * @brief Ensure the array data is NULL-terminated.
 *
 * This appends a NULL pointer to the end of the array's data.
 * Useful when passing the array as arguments to functions that 
 * expect NULL-terminated lists (e.g., execv).
 *
 * @param L Pointer to the array_t to modify.
 * @return 0 on success, -1 on failure (e.g., malloc failed).
 */
int al_null_terminator(array_t * L);

#endif

