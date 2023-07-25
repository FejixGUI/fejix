#include <fejix/core/map.h>
#include <fejix/core/utils.h>

#include <malloc.h>
#include <string.h>
#include <stdbool.h>


#define MAX_LOAD_FACTOR 0.618f
#define MIN_LOAD_FACTOR (MAX_LOAD_FACTOR/4.0f)


struct fj_map {
    struct fj_map_node ** buckets;
    uint32_t buckets_count;
    uint32_t elements_count;
};

struct fj_map_node {
    struct fj_map_node * next;
    struct fj_map_element element;
};


static uint32_t hash(fj_id_t key)
{
    uint32_t x = key;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}


static fj_id_t get_key(struct fj_map_node * node)
{
    return node->element.key;
}


static uint32_t get_bucket_index(fj_id_t key, uint32_t buckets_count)
{
    return hash(key) % buckets_count;
}


static struct fj_map_node ** get_bucket(struct fj_map * map, fj_id_t key)
{
    uint32_t index = get_bucket_index(key, map->buckets_count);
    return &map->buckets[index];
}


static struct fj_map_node * get_tail_node(struct fj_map_node * list_head)
{
    struct fj_map_node * node = list_head;

    while (node->next != NULL) {
        node = node->next;
    }

    return node;
}


static void insert_node_to_bucket(
    struct fj_map_node ** bucket,
    struct fj_map_node * node
)
{
    struct fj_map_node * next = *bucket;
    *bucket = node;
    node->next = next;
}


static void remove_node_from_bucket(
    struct fj_map_node ** bucket,
    struct fj_map_node * prev_node,
    struct fj_map_node * node
)
{
    if (prev_node == NULL) {
        *bucket = node->next;
    } else {
        prev_node->next = node->next;
    }

    node->next = NULL;
}


static void clear_buckets(struct fj_map * map)
{
    memset(map->buckets, 0, map->buckets_count * sizeof(struct fj_map_node *));
}


static void raw_insert(struct fj_map * map, struct fj_map_node * node)
{
    struct fj_map_node ** bucket = get_bucket(map, get_key(node));
    insert_node_to_bucket(bucket, node);
}


static void raw_find(
    struct fj_map_node * list_head,
    fj_id_t key,
    struct fj_map_node ** found_node,
    struct fj_map_node ** found_previous_node
)
{
    *found_node = NULL;
    *found_previous_node = NULL;

    if (list_head == NULL) {
        return;
    }

    struct fj_map_node * previous_node = NULL;
    struct fj_map_node * current_node = list_head;

    while (current_node != NULL && get_key(current_node) != key) {
        previous_node = current_node;
        current_node = current_node->next;
    }

    if (current_node != NULL && get_key(current_node) == key) {
        *found_node = current_node;
        *found_previous_node = previous_node;
    }
}


static struct fj_map_node * raw_remove(struct fj_map * map, fj_id_t key)
{
    struct fj_map_node ** bucket = get_bucket(map, key);
    struct fj_map_node * prev_node;
    struct fj_map_node * node;
    raw_find(*bucket, key, &node, &prev_node);

    if (node == NULL) {
        return NULL;
    }

    remove_node_from_bucket(bucket, prev_node, node);

    return node;
}


static struct fj_map_node * extract_nodes(struct fj_map * map)
{
    struct fj_map_node * head_node = NULL;
    struct fj_map_node * tail_node = NULL;

    for (uint32_t i = 0; i < map->buckets_count; i++) {
        struct fj_map_node * node = map->buckets[i];

        if (node == NULL) {
            continue;
        }

        if (head_node == NULL) {
            head_node = node;
        }

        if (tail_node != NULL) {
            tail_node->next = node;
        }

        tail_node = get_tail_node(node);
    }

    return head_node;
}


static void reinsert_nodes(struct fj_map * map, struct fj_map_node * list_head)
{
    clear_buckets(map);

    struct fj_map_node * node = list_head;

    while (node != NULL) {
        struct fj_map_node * next = node->next;
        raw_insert(map, node);
        node = next;
    }
}


static void free_nodes(struct fj_map_node * list_head)
{
    struct fj_map_node * node = list_head;

    while (node != NULL) {
        struct fj_map_node * next_node = node->next;
        free(node);
        node = next_node;
    }
}


static float get_load_factor(struct fj_map * map)
{
    return (float) map->elements_count / (float) map->buckets_count;
}


static bool map_needs_to_grow(float load_factor)
{
    return load_factor > MAX_LOAD_FACTOR;
}


static bool map_needs_to_shrink(float load_factor)
{
    return load_factor < MIN_LOAD_FACTOR;
}


static bool map_is_validated(float load_factor)
{
    return !map_needs_to_grow(load_factor)
        && !map_needs_to_shrink(load_factor);
}


static fj_result_t resize_buckets(struct fj_map * map, uint32_t buckets_count)
{
    size_t size = buckets_count * sizeof(struct fj_map_node *);
    map->buckets = realloc(map->buckets, size);

    if (map->buckets == NULL) {
        map->buckets_count = 0;
        map->elements_count = 0;
        return FJ_MALLOC_FAIL;
    }

    map->buckets_count = buckets_count;

    return FJ_OK;
}


static fj_result_t resize_map(struct fj_map * map, bool grow)
{
    if (grow) {
        return resize_buckets(map, map->buckets_count * 2);
    } else {
        return resize_buckets(map, fj_max(1, map->buckets_count / 2));
    }
}


static fj_result_t rehash(struct fj_map * map, bool grow)
{
    struct fj_map_node * list_head = extract_nodes(map);

    fj_result_t result = resize_map(map, grow);
    if (result != FJ_OK) {
        free_nodes(list_head);
        return result;
    }

    if (list_head != NULL) {
        reinsert_nodes(map, list_head);
    }

    return FJ_OK;
}


static fj_result_t validate_map(struct fj_map * map)
{
    float load_factor = get_load_factor(map);

    if (map_is_validated(load_factor)) {
        return FJ_OK;
    }

    return rehash(map, map_needs_to_grow(load_factor));
}


static fj_result_t map_remove(struct fj_map * map, fj_id_t key)
{
    struct fj_map_node * node = raw_remove(map, key);

    if (node == NULL) {
        return FJ_INTERNAL_FAIL;
    }

    free(node);

    map->elements_count--;

    return validate_map(map);
}


static struct fj_map_element * map_find(struct fj_map * map, fj_id_t key)
{
    struct fj_map_node ** bucket = get_bucket(map, key);

    struct fj_map_node * prev_node;
    struct fj_map_node * node;
    raw_find(*bucket, key, &node, &prev_node);

    if (node == NULL) {
        return NULL;
    }

    return &node->element;
}


static fj_result_t map_update(struct fj_map * map, fj_id_t key, fj_ptr_t value)
{
    struct fj_map_element * element = map_find(map, key);

    if (element != NULL) {
        element->value = value;
        return FJ_OK;
    }

    return FJ_INTERNAL_FAIL;
}


static fj_result_t map_insert(struct fj_map * map, fj_id_t key, fj_ptr_t value)
{
    struct fj_map_node * node = calloc(1, sizeof(struct fj_map_node));

    if (node == NULL) {
        return FJ_MALLOC_FAIL;
    }

    node->element.key = key;
    node->element.value = value;
    raw_insert(map, node);

    map->elements_count++;

    return validate_map(map);
}


struct fj_map * fj_map_new(void)
{
    struct fj_map * map = calloc(1, sizeof(struct fj_map));

    if (map == NULL) {
        return NULL;
    }

    map->buckets = calloc(1, sizeof(struct fj_map_node *));
    map->buckets_count = 1;

    if (map->buckets == NULL) {
        free(map);
        return NULL;
    }

    return map;
}


void fj_map_del(struct fj_map * map)
{
    if (map->buckets != NULL) {
        free(map->buckets);
    }

    free(map);
}


fj_result_t fj_map_set(struct fj_map * map, fj_id_t key, fj_ptr_t value)
{
    if (value == NULL) {
        return map_remove(map, key);
    }

    if (map_update(map, key, value) == FJ_OK) {
        return FJ_OK;
    }

    return map_insert(map, key, value);
}


fj_ptr_t fj_map_get(struct fj_map * map, fj_id_t key)
{
    struct fj_map_element * element = map_find(map, key);

    if (element == NULL) {
        return NULL;
    }

    return element->value;
}


void fj_map_foreach(
    struct fj_map * map,
    fj_map_foreach_callback_t callback,
    fj_ptr_t data
)
{
    for (uint32_t i = 0; i < map->buckets_count; i++) {

        if (map->buckets[i] == NULL) {
            continue;
        }

        struct fj_map_node * node = map->buckets[i];

        while (node != NULL) {
            if (callback(&node->element, data) == FJ_MAP_FOREACH_STOP) {
                return;
            }

            node = node->next;
        }
    }
}