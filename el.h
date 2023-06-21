typedef struct _el_system_t el_system_t;

el_system_t *
el_create();

void
el_free(el_system_t *L);

typedef void (*action_t)(char *output, int pos);

void
el_add_symbol(el_system_t *L, char *symbol, action_t action);

void
el_add_rule(el_system_t *L, char *input, char *output);

char *
el_perform(el_system_t *L, char *input, int steps);

void
el_enact(el_system_t *L, char *output);
