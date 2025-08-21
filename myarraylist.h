typedef struct {
    unsigned length;
    unsigned capacity;
    int *data;
} array_t;


void al_init(array_t *, unsigned);
void al_destroy(array_t * al);

void al_append(array_t * al, char * item);