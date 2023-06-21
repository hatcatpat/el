#include "el.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _kv_elem_t
{
    char *key;
    void *value;
} kv_elem_t;

typedef struct _kv_t
{
    kv_elem_t *elems;
    size_t size, length;
} kv_t;

kv_t *kv_create(size_t size);
void kv_free(kv_t *kv);
void *kv_find(kv_t *kv, char *key);
void **kv_append(kv_t *kv, char *key);

kv_t *
kv_create(size_t size)
{
    kv_t *kv = malloc(sizeof(kv_t));
    if(kv == NULL)
        return NULL;

    kv->length = 0;
    kv->size = size;
    kv->elems = NULL;

    return kv;
}

void
kv_free(kv_t *kv)
{
    if(kv->elems != NULL)
        {
            int i;
            for(i = 0; i < kv->length; ++i)
                {
                    free(kv->elems[i].key);
                    free(kv->elems[i].value);
                }
            free(kv->elems);
        }
    free(kv);
}

void *
kv_find(kv_t *kv, char *key)
{
    int i;
    for(i = 0; i < kv->length; ++i)
        if(strcmp(kv->elems[i].key, key) == 0)
            return kv->elems[i].value;
    return NULL;
}

void **
kv_append(kv_t *kv, char *key)
{
    kv_elem_t *elem = kv_find(kv, key);
    if(elem != NULL)
        return elem->value;

    kv->length++;
    kv->elems = realloc(kv->elems, kv->length * sizeof(kv_elem_t));
    elem = &kv->elems[kv->length - 1];

    elem->key = malloc(strlen(key));
    strcpy(elem->key, key);

    elem->value = malloc(kv->size);
    return elem->value;
}

struct _el_system_t
{
    kv_t *symbols; /* char * -> action_t */
    kv_t *rules; /* char * -> char * */
    char *output;
};

el_system_t *
el_create()
{
    el_system_t *L = malloc(sizeof(el_system_t));
    if(L == NULL)
        return NULL;

    L->output = NULL;
    L->symbols = kv_create(sizeof(action_t));
    L->rules = kv_create(sizeof(char *));

    return L;
}

void
el_free(el_system_t *L)
{
    if(L->output != NULL)
        free(L->output);
    kv_free(L->rules);
    kv_free(L->symbols);
    free(L);
}

void
el_add_symbol(el_system_t *L, char *symbol, action_t action)
{
    *(action_t*)kv_append(L->symbols, symbol) = action;
}

void
el_add_rule(el_system_t *L, char *input, char *output)
{
    *(char**)kv_append(L->rules, input) = output;
}

char *
step(el_system_t *L, char *input, int depth)
{
    char *output = NULL, *rule = NULL, *sub = NULL;
    char symbol[2] = { 0, 0 };
    int j, i = 0;

    while(i < strlen(input))
        {
            for(j = 0; j < L->rules->length; ++j)
                if(strstr(input + i, L->rules->elems[j].key) == input + i)
                    {
                        sub = L->rules->elems[j].key;
                        rule = *(char**)L->rules->elems[j].value;

                        if(output == NULL)
                            {
                                output = malloc(strlen(rule) + 1);
                                strcpy(output, rule);
                            }
                        else
                            {
                                output = realloc(output, strlen(output) + strlen(rule) + 1);
                                strcat(output, rule);
                            }

                        break;
                    }

            if(sub == NULL)
                {
                    symbol[0] = input[i];

                    if(output == NULL)
                        {
                            output = malloc(2);
                            strcpy(output, symbol);
                        }
                    else
                        {
                            output = realloc(output, strlen(output) + 2);
                            strcat(output, symbol);
                        }

                    i++;
                }
            else
                {
                    i += strlen(sub);
                    sub = NULL;
                }
        };

    free(input);

    printf("output :: %i, \"%s\"\n", depth, output);

    return output;
}

char *
el_perform(el_system_t *L, char *input, int steps)
{
    int i;
    char *output = malloc(strlen(input));
    strcpy(output, input);

    printf("input :: \"%s\"\n", input);

    for(i = 0; i < steps; ++i)
        output = step(L, output, i);

    if(L->output != NULL)
        free(L->output);

    L->output = output;

    return L->output;
}

void
el_enact(el_system_t *L, char *output)
{
    int i;
    char symbol[2] = { 0, 0 };
    action_t action = NULL;

    for(i = 0; i < strlen(output); ++i)
        {
            symbol[0] = output[i];
            action = *(action_t*)kv_find(L->symbols, symbol);
            if(action != NULL)
                action(output, i);
        }
}
