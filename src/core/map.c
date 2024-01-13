#include <fejix/map.h>
#include <fejix/malloc.h>
#include <fejix/utils.h>

#include <string.h>


#define MAX_LOAD_FACTOR (0.618f)
#define MIN_LOAD_FACTOR (MAX_LOAD_FACTOR/4.0f)


static
uintptr_t get_key(struct fj_map_node * node)
{
    return node->element.key;
}


static
uint32_t get_bucket_index(uintptr_t key, uint32_t bucket_count)
{
    return fj_uintptr_hash32(key) % bucket_count;
}


static
struct fj_map_node ** get_bucket(struct fj_map const * map, uintptr_t key)
{
    uint32_t index = get_bucket_index(key, map->bucket_count);
    return &map->buckets[index];
}


static
struct fj_map_node * get_tail_node(struct fj_map_node * list_head)
{
    struct fj_map_node * node = list_head;

    while (node->next != NULL) {
        node = node->next;
    }

    return node;
}


static
void insert_node_to_bucket(
    struct fj_map_node ** bucket,
    struct fj_map_node * node
)
{
    struct fj_map_node * next = *bucket;
    *bucket = node;
    node->next = next;
}


static
void remove_node_from_bucket(
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


static
void clear_buckets(struct fj_map * map)
{
    memset(map->buckets, 0, sizeof(*map->buckets) * map->bucket_count);
}


static
void raw_insert(struct fj_map * map, struct fj_map_node * node)
{
    struct fj_map_node ** bucket = get_bucket(map, get_key(node));
    insert_node_to_bucket(bucket, node);
}


static
void find_node_in_bucket(
    struct fj_map_node ** bucket,
    uintptr_t key,
    struct fj_map_node * FJ_NULLABLE FJ_OUT * found_node,
    struct fj_map_node * FJ_NULLABLE FJ_OUT * found_previous_node
)
{
    *found_node = NULL;
    *found_previous_node = NULL;

    if (bucket == NULL) {
        return;
    }

    struct fj_map_node * previous_node = NULL;
    struct fj_map_node * current_node = *bucket;

    while (current_node != NULL && get_key(current_node) != key) {
        previous_node = current_node;
        current_node = current_node->next;
    }

    if (current_node != NULL && get_key(current_node) == key) {
        *found_node = current_node;
        *found_previous_node = previous_node;
    }
}


static
struct fj_map_node * raw_remove(struct fj_map * map, uintptr_t key)
{
    struct fj_map_node ** bucket = get_bucket(map, key);

    struct fj_map_node * node;
    struct fj_map_node * prev_node;
    find_node_in_bucket(bucket, key, &node, &prev_node);

    if (node == NULL) {
        return NULL;
    }

    remove_node_from_bucket(bucket, prev_node, node);

    return node;
}


static
struct fj_map_node * extract_nodes(struct fj_map * map)
{
    if (map->buckets == NULL) {
        return NULL;
    }

    struct fj_map_node * head_node = NULL;
    struct fj_map_node * tail_node = NULL;

    for (uint32_t i = 0; i < map->bucket_count; i++) {
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

    clear_buckets(map);

    return head_node;
}


static
void reinsert_nodes(struct fj_map * map, struct fj_map_node * list_head)
{
    struct fj_map_node * node = list_head;

    while (node != NULL) {
        struct fj_map_node * next = node->next;
        raw_insert(map, node);
        node = next;
    }
}


static
void free_nodes(struct fj_map_node * list_head)
{
    struct fj_map_node * node = list_head;

    while (node != NULL) {
        struct fj_map_node * next_node = node->next;
        fj_free(node);
        node = next_node;
    }
}


static
float get_load_factor(struct fj_map * map)
{
    return (float) map->element_count / (float) map->bucket_count;
}


static
fj_bool_t map_needs_to_grow(float load_factor)
{
    return load_factor > MAX_LOAD_FACTOR;
}


static
fj_bool_t map_needs_to_shrink(float load_factor)
{
    return load_factor < MIN_LOAD_FACTOR;
}


static
fj_bool_t map_is_validated(float load_factor)
{
    return !map_needs_to_grow(load_factor)
        && !map_needs_to_shrink(load_factor);
}


static
fj_err_t resize_buckets(struct fj_map * map, uint32_t bucket_count)
{
    FJ_INIT_ERRORS

    struct fj_map_node ** buckets = NULL;

    FJ_TRY fj_realloc_uninit(
        (void *) &map->buckets, bucket_count, sizeof(*map->buckets)
    );

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    map->buckets = buckets;
    map->bucket_count = bucket_count;

    return FJ_OK;
}


static
fj_err_t resize_map(struct fj_map * map, fj_bool_t grow)
{
    if (grow) {
        return resize_buckets(map, map->bucket_count * 2);
    }

    return resize_buckets(map, FJ_MAX(1, map->bucket_count / 2));
}


static
fj_err_t rehash(struct fj_map * map, fj_bool_t grow)
{
    FJ_INIT_ERRORS

    struct fj_map_node * list_head = extract_nodes(map);

    FJ_TRY resize_map(map, grow);

    if (FJ_FAILED) {
        free_nodes(list_head);
        fj_free(map->buckets);
        fj_free(map);

        return FJ_LAST_ERROR;
    }

    reinsert_nodes(map, list_head);

    return FJ_OK;
}


static
fj_err_t validate_map(struct fj_map * map)
{
    float load_factor = get_load_factor(map);

    if (!map_is_validated(load_factor)) {
        return rehash(map, map_needs_to_grow(load_factor));
    }

    return FJ_OK;
}


static
fj_err_t map_remove(struct fj_map * map, uintptr_t key)
{
    struct fj_map_node * node = raw_remove(map, key);

    if (node == NULL) {
        return FJ_OK;
    }

    fj_free(node);

    map->element_count--;

    return validate_map(map);
}


static
struct fj_map_element * map_find(struct fj_map const * map, uintptr_t key)
{
    struct fj_map_node ** bucket = get_bucket(map, key);

    struct fj_map_node * prev_node;
    struct fj_map_node * node;
    find_node_in_bucket(bucket, key, &node, &prev_node);

    if (node == NULL) {
        return NULL;
    }

    return &node->element;
}


/** Returns true if the value was updated, false if the record does not exist
    in the map. */
static
fj_bool_t map_update(struct fj_map * map, uintptr_t key, void * value)
{
    struct fj_map_element * element = map_find(map, key);

    if (element == NULL) {
        return false;
    }

    element->value = value;

    return true;
}


static
fj_err_t map_insert(struct fj_map * map, uintptr_t key, void * value)
{
    FJ_INIT_ERRORS

    struct fj_map_node * node = NULL;

    FJ_TRY fj_alloc_zeroed((void *) &node, sizeof(*node));

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    node->element.key = key;
    node->element.value = value;
    raw_insert(map, node);

    map->element_count++;

    return validate_map(map);
}


static
void bucket_foreach_node(
    struct fj_map_node ** bucket,
    fj_map_foreach_callback_t callback,
    void * data
)
{
    struct fj_map_node * node = *bucket;

    while (node != NULL) {
        fj_bool_t should_continue = callback(&node->element, data);

        if (!should_continue) {
            return;
        }

        node = node->next;
    }
}


fj_err_t fj_map_init(struct fj_map * map)
{
    FJ_INIT_ERRORS

    *map = (struct fj_map) { 0 };

    FJ_TRY fj_realloc_zeroed(
        (void *) &map->buckets,
        0,
        1,
        sizeof(*map->buckets)
    );

    if (FJ_FAILED) {
        return FJ_LAST_ERROR;
    }

    map->bucket_count = 1;

    return FJ_OK;
}


void fj_map_deinit(struct fj_map * map)
{
    struct fj_map_node * list_head = extract_nodes(map);

    if (list_head != NULL) {
        free_nodes(list_head);
    }

    if (map->buckets != NULL) {
        fj_free(map->buckets);
    }
}


fj_err_t fj_map_set(struct fj_map * map, uintptr_t key, void * value)
{
    if (value == NULL) {
        return map_remove(map, key);
    }

    if (map_update(map, key, value)) {
        return FJ_OK;
    }

    return map_insert(map, key, value);
}


void * fj_map_get(struct fj_map const * map, uintptr_t key)
{
    struct fj_map_element * element = map_find(map, key);

    if (element == NULL) {
        return NULL;
    }

    return element->value;
}


void fj_map_foreach(
    struct fj_map const * map,
    fj_map_foreach_callback_t callback,
    void * callback_data
)
{
    for (uint32_t i = 0; i < map->bucket_count; i++) {

        if (map->buckets[i] == NULL) {
            continue;
        }

        bucket_foreach_node(&map->buckets[i], callback, callback_data);
    }
}
