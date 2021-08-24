#include "dictionary.h"
#include <string.h>

#define DICT_INVALID_SEARCH -1

static int dict_search(const dict_t *dict, const uint8_t *key)
{
    for (size_t i = 0; i < dict->elem_number; i++)
    {
        if (memcmp(dict->buf[i].key, key, dict->key_size) == 0)
        {
            return (int)i;
        }
    }

    return DICT_INVALID_SEARCH;
}

bool dict_init(dict_t *dict, dict_elem_t *src_buf, size_t size,
               uint8_t *keys, uint8_t *values,
               size_t key_size, size_t value_size)
{
    dict->buf = src_buf;
    dict->size = size;
    dict->elem_number = 0;
    dict->key_size = key_size;
    dict->value_size = value_size;

    for (size_t i = 0; i < size; i++)
    {
        dict->buf[i].key = (uint8_t *)(keys + i * key_size);
        dict->buf[i].value = (uint8_t *)(values + i * value_size);
        memset(dict->buf[i].key, 0, key_size);
        memset(dict->buf[i].value, 0, value_size);
    }

    return true;
}

bool dict_add(dict_t *dict, const uint8_t *key, const uint8_t *value)
{
    if (dict_is_full(dict))
    {
        return false;
    }

    if (dict_is_key_existence(dict, key))
    {
        return false;
    }

    memcpy(dict->buf[dict->elem_number].key, key, dict->key_size);
    memcpy(dict->buf[dict->elem_number].value, value, dict->value_size);
    dict->elem_number++;

    return true;
}

bool dict_get(const dict_t *dict, const uint8_t *key, uint8_t *ret_value)
{
    int search_pos = dict_search(dict, key);
    if (search_pos == DICT_INVALID_SEARCH)
    {
        return false;
    }

    memcpy(ret_value, dict->buf[search_pos].value, dict->value_size);

    return true;
}

bool dict_delete(dict_t *dict, const uint8_t *key)
{
    int search_pos = dict_search(dict, key);
    if (search_pos == DICT_INVALID_SEARCH)
    {
        return false;
    }

    memmove(&dict->buf[search_pos], &dict->buf[search_pos + 1],
            (dict->elem_number - (size_t)(search_pos + 1))*sizeof(dict_elem_t));
    dict->elem_number--;

    return true;
}

bool dict_update(dict_t *dict, const uint8_t *key, const uint8_t *value)
{
    int search_pos = dict_search(dict, key);
    if (search_pos == DICT_INVALID_SEARCH)
    {
        return false;
    }

    memcpy(dict->buf[search_pos].key, key, dict->key_size);
    memcpy(dict->buf[search_pos].value, value, dict->value_size);

    return true;
}

bool dict_is_key_existence(const dict_t *dict, const uint8_t *key)
{
    return dict_search(dict, key) != DICT_INVALID_SEARCH;
}

bool dict_is_full(const dict_t *dict)
{
    return dict->elem_number >= dict->size;
}

bool dict_is_empty(const dict_t *dict)
{
    return dict->elem_number == 0;
}
