#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define DICT_INVALID_SEARCH -1

typedef struct
{
    uint8_t *key;
    uint8_t *value;
} dict_elem_t;

typedef struct
{
    dict_elem_t *buf;
    size_t size;
    size_t elem_number;
    size_t key_size;
    size_t value_size;
} dict_t;

bool dict_init(dict_t *dict, dict_elem_t *src_buf, size_t size,
               uint8_t * keys, uint8_t *values,
               size_t key_size, size_t value_size);
bool dict_add(dict_t *dict, const uint8_t *key, const uint8_t *value);
bool dict_get(const dict_t *dict, const uint8_t *key, uint8_t *ret_value);
bool dict_delete(dict_t *dict, const uint8_t *key);
bool dict_update(dict_t *dict, const uint8_t *key, const uint8_t *value);
bool dict_is_key_existence(const dict_t *dict, const uint8_t *key);
bool dict_is_full(const dict_t *dict);
bool dict_is_empty(const dict_t *dict);

#endif /*DICTIONARY_H_*/
