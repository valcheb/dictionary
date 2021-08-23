#include "../dictionary.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define TEST_DICT_SIZE 3
#define TEST_STRING_SIZE 11
static bool test_dict_is_empty()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE];
    uint8_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    return dict_is_empty(&dict) == true;
}

static bool test_dict_error_add_to_full()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE];
    uint8_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    uint8_t test_key = 1;
    uint8_t test_value = 77;
    for (uint8_t i = 0; i < TEST_DICT_SIZE; i++)
    {
        if (!dict_add(&dict, &test_key, (uint8_t *)&i))
        {
            return false;
        }
        test_key++;
    }

    return dict_add(&dict, &test_key, &test_value) == false && dict_is_full(&dict) == true;
}

static bool test_dict_error_add_same_key()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE];
    uint8_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    uint8_t test_key_1 = 1;
    uint8_t test_value_1 = 77;
    if (!dict_add(&dict, &test_key_1, &test_value_1))
    {
        return false;
    }

    uint8_t test_key_2 = test_key_1;
    uint8_t test_value_2 = 78;

    return dict_add(&dict, &test_key_2, &test_value_2) == false && dict.elem_number == 1;
}

static bool test_dict_key_existence()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE];
    uint8_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    uint8_t test_key_existence = 1;
    uint8_t test_key_not_existence = 2;
    uint8_t test_value = 77;

    if (!dict_add(&dict, &test_key_existence, &test_value))
    {
        return false;
    }

    return dict_is_key_existence(&dict, &test_key_existence) == true && dict_is_key_existence(&dict, &test_key_not_existence) == false;
}

static bool test_dict_add_get_8bit_value()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE];
    uint8_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    uint8_t test_key = 1;
    uint8_t test_value = 77;
    uint8_t ret_value;

    if (!dict_add(&dict, &test_key, &test_value))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key, &ret_value))
    {
        return false;
    }

    return ret_value == test_value && dict.elem_number == 1;
}

static bool test_dict_add_get_32bit_value()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE];
    uint32_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    uint8_t test_key = 1;
    uint32_t test_value = 777777777;
    uint32_t ret_value;

    if (!dict_add(&dict, &test_key, (uint8_t *)&test_value))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key, (uint8_t *)&ret_value))
    {
        return false;
    }

    return ret_value == test_value && dict.elem_number == 1;
}

typedef struct
{
    int a;
    int b;
} test_dict_struct_t;
static bool test_dict_add_get_struct_value()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE];
    test_dict_struct_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    uint8_t test_key = 1;
    test_dict_struct_t test_value = {77, 78};
    test_dict_struct_t ret_value;

    if (!dict_add(&dict, &test_key, (uint8_t *)&test_value))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key, (uint8_t *)&ret_value))
    {
        return false;
    }

    return ret_value.a == test_value.a && ret_value.b == test_value.b && dict.elem_number == 1;
}

typedef void (*test_dict_func_fptr_t)(void);
static void test_dict_func()
{
    ;
}
static bool test_dict_add_get_func_value()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE];
    test_dict_func_fptr_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    uint8_t test_key = 1;
    test_dict_func_fptr_t test_value = test_dict_func;
    test_dict_func_fptr_t ret_value;

    if (!dict_add(&dict, &test_key, (uint8_t *)&test_value))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key, (uint8_t *)&ret_value))
    {
        return false;
    }

    return ret_value == test_value && dict.elem_number == 1;
}

static bool test_dict_add_get_two_8bit_value()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE];
    uint8_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    uint8_t test_key_1 = 1;
    uint8_t test_value_1 = 77;
    uint8_t ret_value1;
    uint8_t test_key_2 = 2;
    uint8_t test_value_2 = 81;
    uint8_t ret_value_2;

    if (!dict_add(&dict, &test_key_1, &test_value_1))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key_1, &ret_value1))
    {
        return false;
    }

    if (!dict_add(&dict, &test_key_2, &test_value_2))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key_2, &ret_value_2))
    {
        return false;
    }

    return ret_value1 == test_value_1 && ret_value_2 == test_value_2 && dict.elem_number == 2;
}

static bool test_dict_add_get_two_32bit_value()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE];
    uint32_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    uint8_t test_key_1 = 1;
    uint32_t test_value_1 = 7777777;
    uint32_t ret_value_1;
    uint8_t test_key_2 = 2;
    uint32_t test_value_2 = 7777778;
    uint32_t ret_value_2;

    if (!dict_add(&dict, &test_key_1, (uint8_t *)&test_value_1))
    {
        return false;
    }

    if (!dict_add(&dict, &test_key_2, (uint8_t *)&test_value_2))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key_1, (uint8_t *)&ret_value_1))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key_2, (uint8_t *)&ret_value_2))
    {
        return false;
    }

    return ret_value_1 == test_value_1 && ret_value_2 == test_value_2 && dict.elem_number == 2;
}

static bool test_dict_add_get_8bit_value_string_key()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE][TEST_STRING_SIZE];
    uint8_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    char test_key[TEST_STRING_SIZE] = "test_key";
    uint8_t test_value = 77;
    uint8_t ret_value;

    if (!dict_add(&dict, (uint8_t *)test_key, &test_value))
    {
        return false;
    }

    if (!dict_get(&dict, (uint8_t *)test_key, &ret_value))
    {
        return false;
    }

    return ret_value == test_value && dict.elem_number == 1;
}

static bool test_dict_add_get_two_8bit_value_string_key()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE][TEST_STRING_SIZE];
    uint8_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    char test_key_1[TEST_STRING_SIZE] = "test_key_1";
    uint8_t test_value_1 = 77;
    uint8_t ret_value_1;
    char test_key_2[TEST_STRING_SIZE] = "test_key_2";
    uint8_t test_value_2 = 78;
    uint8_t ret_value_2;

    if (!dict_add(&dict, (uint8_t *)test_key_1, &test_value_1))
    {
        return false;
    }

    if (!dict_get(&dict, (uint8_t *)test_key_1, &ret_value_1))
    {
        return false;
    }

    if (!dict_add(&dict, (uint8_t *)test_key_2, &test_value_2))
    {
        return false;
    }

    if (!dict_get(&dict, (uint8_t *)test_key_2, &ret_value_2))
    {
        return false;
    }

    return ret_value_1 == test_value_1 && ret_value_2 == test_value_2 && dict.elem_number == 2;
}

static bool test_dict_add_get_string_value_string_key()
{
    dict_t dict;
    char keys[TEST_DICT_SIZE][TEST_STRING_SIZE];
    char values[TEST_DICT_SIZE][TEST_STRING_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    char test_key[TEST_STRING_SIZE] = "test_key";
    char test_value[TEST_STRING_SIZE] = "test_value";
    char ret_value[sizeof(test_value)];

    if (!dict_add(&dict, (uint8_t *)test_key, (uint8_t *)test_value))
    {
        return false;
    }

    if (!dict_get(&dict, (uint8_t *)test_key, (uint8_t *)ret_value))
    {
        return false;
    }

    return strncmp(test_value, ret_value, sizeof(test_value)) == 0 && dict.elem_number == 1;
}

static bool test_dict_update_8bit()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE];
    uint8_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    uint8_t test_key = 1;
    uint8_t test_value_1 = 77;
    uint8_t test_value_2 = 78;
    uint8_t ret_value;

    if (!dict_add(&dict, &test_key, &test_value_1))
    {
        return false;
    }

    if (!dict_update(&dict, &test_key, &test_value_2))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key, &ret_value))
    {
        return false;
    }

    return ret_value == test_value_2 && dict.elem_number == 1;
}

static bool test_dict_update_with_smaller_string_value()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE];
    char values[TEST_DICT_SIZE][TEST_STRING_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    uint8_t test_key = 1;
    char test_value_1[TEST_STRING_SIZE] = "long_str";
    char test_value_2[TEST_STRING_SIZE] = "small";
    char ret_value[TEST_STRING_SIZE];

    if (!dict_add(&dict, &test_key, (uint8_t *)&test_value_1))
    {
        return false;
    }

    if (!dict_update(&dict, &test_key, (uint8_t *)&test_value_2))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key, (uint8_t *)&ret_value))
    {
        return false;
    }

    return strncmp(test_value_2, ret_value, TEST_STRING_SIZE) == 0 && dict.elem_number == 1;
}

static bool test_dict_has_no_zero_key()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE];
    uint8_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    uint8_t test_zero_key = 0;

    return dict_is_key_existence(&dict, &test_zero_key) == false;
}

static bool test_dict_delete_one_8bit()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE];
    uint8_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    uint8_t test_key_1 = 1;
    uint8_t test_key_2 = 2;
    uint8_t test_key_3 = 3;
    uint8_t test_value_1 = 77;
    uint8_t test_value_2 = 78;
    uint8_t test_value_3 = 79;
    uint8_t ret_value_1 = 0;
    uint8_t ret_value_2 = 0;
    uint8_t ret_value_3 = 0;

    if (!dict_add(&dict, &test_key_1, &test_value_1))
    {
        return false;
    }

    if (!dict_add(&dict, &test_key_2, &test_value_2))
    {
        return false;
    }

    if (!dict_add(&dict, &test_key_3, &test_value_3))
    {
        return false;
    }

    if (!dict_delete(&dict, &test_key_2))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key_1, &ret_value_1))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key_3, &ret_value_3))
    {
        return false;
    }

    if (dict_get(&dict, &test_key_2, &ret_value_2))
    {
        return false;
    }

    return ret_value_1 == test_value_1 && ret_value_3 == test_value_3 && dict.elem_number == 2;
}

static bool test_dict_delete_two_8bit()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE];
    uint8_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    uint8_t test_key_1 = 1;
    uint8_t test_key_2 = 2;
    uint8_t test_key_3 = 3;
    uint8_t test_value_1 = 77;
    uint8_t test_value_2 = 78;
    uint8_t test_value_3 = 79;
    uint8_t ret_value_1 = 0;
    uint8_t ret_value_2 = 0;
    uint8_t ret_value_3 = 0;

    if (!dict_add(&dict, &test_key_1, &test_value_1))
    {
        return false;
    }

    if (!dict_add(&dict, &test_key_2, &test_value_2))
    {
        return false;
    }

    if (!dict_add(&dict, &test_key_3, &test_value_3))
    {
        return false;
    }

    if (!dict_delete(&dict, &test_key_1))
    {
        return false;
    }

    if (!dict_delete(&dict, &test_key_2))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key_3, &ret_value_3))
    {
        return false;
    }

    if (dict_get(&dict, &test_key_1, &ret_value_1))
    {
        return false;
    }

    if (dict_get(&dict, &test_key_2, &ret_value_2))
    {
        return false;
    }

    return ret_value_3 == test_value_3 && dict.elem_number == 1;
}

static bool test_dict_add_after_delete_8bit()
{
    dict_t dict;
    uint8_t keys[TEST_DICT_SIZE];
    uint8_t values[TEST_DICT_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    uint8_t test_key_1 = 1;
    uint8_t test_key_2 = 2;
    uint8_t test_value_1 = 77;
    uint8_t test_value_2 = 78;
    uint8_t test_value_3 = 79;
    uint8_t ret_value_1 = 0;
    uint8_t ret_value_2 = 0;

    if (!dict_add(&dict, &test_key_1, &test_value_1))
    {
        return false;
    }

    if (!dict_add(&dict, &test_key_2, &test_value_2))
    {
        return false;
    }

    if (!dict_delete(&dict, &test_key_2))
    {
        return false;
    }

    if (!dict_add(&dict, &test_key_2, &test_value_3))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key_1, &ret_value_1))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key_2, &ret_value_2))
    {
        return false;
    }

    return ret_value_1 == test_value_1 && ret_value_2 == test_value_3 && dict.elem_number == 2;
}

static bool test_dict_delete_one_string_value()
{
    dict_t dict;
    char keys[TEST_DICT_SIZE][TEST_STRING_SIZE];
    char values[TEST_DICT_SIZE][TEST_STRING_SIZE];
    dict_elem_t buf[TEST_DICT_SIZE];
    dict_init(&dict, buf, TEST_DICT_SIZE,
              (uint8_t *)keys, (uint8_t *)values,
              sizeof(keys[0]), sizeof(values[0]));

    uint8_t test_key_1 = 1;
    uint8_t test_key_2 = 2;
    uint8_t test_key_3 = 3;
    char test_value_1[TEST_STRING_SIZE] = "string_1";
    char test_value_2[TEST_STRING_SIZE] = "string_2";
    char test_value_3[TEST_STRING_SIZE] = "string_3";
    char ret_value_1[TEST_STRING_SIZE] = "";
    char ret_value_2[TEST_STRING_SIZE] = "";
    char ret_value_3[TEST_STRING_SIZE] = "";

    if (!dict_add(&dict, &test_key_1, (uint8_t *)&test_value_1))
    {
        return false;
    }

    if (!dict_add(&dict, &test_key_2, (uint8_t *)&test_value_2))
    {
        return false;
    }

    if (!dict_add(&dict, &test_key_3, (uint8_t *)&test_value_3))
    {
        return false;
    }

    if (!dict_delete(&dict, &test_key_2))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key_1, (uint8_t *)&ret_value_1))
    {
        return false;
    }

    if (!dict_get(&dict, &test_key_3, (uint8_t *)&ret_value_3))
    {
        return false;
    }

    if (dict_get(&dict, &test_key_2, (uint8_t *)&ret_value_2))
    {
        return false;
    }

    return strncmp(test_value_1, ret_value_1, sizeof(test_value_1)) == 0 &&
           strncmp(test_value_3, ret_value_3, sizeof(test_value_3)) == 0 &&
           dict.elem_number == 2;
}

int main()
{
    assert(test_dict_is_empty() == true);
    assert(test_dict_add_get_8bit_value() == true);
    assert(test_dict_key_existence() == true);
    assert(test_dict_error_add_to_full() == true);
    assert(test_dict_add_get_32bit_value() == true);
    assert(test_dict_add_get_struct_value() == true);
    assert(test_dict_add_get_func_value() == true);
    assert(test_dict_add_get_two_8bit_value() == true);
    assert(test_dict_add_get_two_32bit_value() == true);
    assert(test_dict_error_add_same_key() == true);
    assert(test_dict_add_get_8bit_value_string_key() == true);
    assert(test_dict_add_get_two_8bit_value_string_key() == true);
    assert(test_dict_add_get_string_value_string_key() == true);
    assert(test_dict_update_8bit() == true);
    assert(test_dict_update_with_smaller_string_value() == true);
    assert(test_dict_has_no_zero_key() == true);
    assert(test_dict_delete_one_8bit() == true);
    assert(test_dict_delete_two_8bit() == true);
    assert(test_dict_add_after_delete_8bit() == true);
    assert(test_dict_delete_one_string_value() == true);

    printf("Tests passed\n");
}
