#include <fejix/core/map.h>
#include <fejix/core/utils.h>

#include <malloc.h>
#include <string.h>


#define MAX_LOAD_FACTOR 0.618f
#define MIN_LOAD_FACTOR (MAX_LOAD_FACTOR/4.0f)


static uint32_t hash(fj_id_t key)
{
    uint32_t x = key;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}


static fj_id_t get_key(fj_map_node_t * node)
{
    return node->element.key;
}


static uint32_t get_bucket_index(fj_id_t key, uint32_t buckets_count)
{
    return hash(key) % buckets_count;
}


static fj_map_node_t ** get_bucket(fj_map_t * map, fj_id_t key)
{
    uint32_t index = get_bucket_index(key, map->buckets_count);
    return &map->buckets[index];
}


static fj_map_node_t * get_tail_node(fj_map_node_t * list_head)
{
    fj_map_node_t * node = list_head;

    while (node->next != NULL) {
        node = node->next;
    }

    return node;
}


static void insert_node_to_bucket(
    fj_map_node_t ** bucket,
    fj_map_node_t * node
)
{
    fj_map_node_t * next = *bucket;
    *bucket = node;
    node->next = next;
}


static void remove_node_from_bucket(
    fj_map_node_t ** bucket,
    fj_map_node_t * prev_node,
    fj_map_node_t * node
)
{
    if (prev_node == NULL) {
        *bucket = node->next;
    } else {
        prev_node->next = node->next;
    }

    node->next = NULL;
}


static void clear_buckets(fj_map_t * map)
{
    memset(map->buckets, 0, map->buckets_count * sizeof(fj_map_node_t *));
}


static void raw_insert(fj_map_t * map, fj_map_node_t * node)
{
    fj_map_node_t ** bucket = get_bucket(map, get_key(node));
    insert_node_to_bucket(bucket, node);
}


static void raw_find(
    fj_map_node_t * list_head,
    fj_id_t key,
    fj_map_node_t ** found_node,
    fj_map_node_t ** found_previous_node
)
{
    *found_node = NULL;
    *found_previous_node = NULL;

    if (list_head == NULL) {
        return;
    }

    fj_map_node_t * previous_node = NULL;
    fj_map_node_t * current_node = list_head;

    while (current_node != NULL && get_key(current_node) != key) {
        previous_node = current_node;
        current_node = current_node->next;
    }

    if (current_node != NULL && get_key(current_node) == key) {
        *found_node = current_node;
        *found_previous_node = previous_node;
    }
}


static fj_map_node_t * raw_remove(fj_map_t * map, fj_id_t key)
{
    fj_map_node_t ** bucket = get_bucket(map, key);
    fj_map_node_t * prev_node;
    fj_map_node_t * node;
    raw_find(*bucket, key, &node, &prev_node);

    if (node == NULL) {
        return NULL;
    }

    remove_node_from_bucket(bucket, prev_node, node);

    return node;
}


static fj_map_node_t * extract_nodes(fj_map_t * map)
{
    fj_map_node_t * head_node = NULL;
    fj_map_node_t * tail_node = NULL;

    for (uint32_t i = 0; i < map->buckets_count; i++) {
        fj_map_node_t * node = map->buckets[i];

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


static void reinsert_nodes(fj_map_t * map, fj_map_node_t * list_head)
{
    clear_buckets(map);

    fj_map_node_t * node = list_head;

    while (node != NULL) {
        fj_map_node_t * next = node->next;
        raw_insert(map, node);
        node = next;
    }
}


static void free_nodes(fj_map_node_t * list_head)
{
    fj_map_node_t * node = list_head;

    while (node != NULL) {
        fj_map_node_t * next_node = node->next;
        free(node);
        node = next_node;
    }
}


static float get_load_factor(fj_map_t * map)
{
    return (float) map->elements_count / (float) map->buckets_count;
}


static fj_bool_t map_needs_to_grow(float load_factor)
{
    return load_factor > MAX_LOAD_FACTOR;
}


static fj_bool_t map_needs_to_shrink(float load_factor)
{
    return load_factor < MIN_LOAD_FACTOR;
}


static fj_bool_t map_is_validated(float load_factor)
{
    return !map_needs_to_grow(load_factor)
        && !map_needs_to_shrink(load_factor);
}


static fj_result_t resize_buckets(fj_map_t * map, uint32_t buckets_count)
{
    size_t size = buckets_count * sizeof(fj_map_node_t *);
    map->buckets = realloc(map->buckets, size);

    if (map->buckets == NULL) {
        map->buckets_count = 0;
        map->elements_count = 0;
        return FJ_MALLOC_FAIL;
    }

    map->buckets_count = buckets_count;

    return FJ_OK;
}


static fj_result_t resize_map(fj_map_t * map, fj_bool_t grow)
{
    if (grow) {
        return resize_buckets(map, map->buckets_count * 2);
    } else {
        return resize_buckets(map, fj_max_u32(1, map->buckets_count / 2));
    }
}


static fj_result_t rehash(fj_map_t * map, fj_bool_t grow)
{
    fj_map_node_t * list_head = extract_nodes(map);

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


static fj_result_t validate_map(fj_map_t * map)
{
    float load_factor = get_load_factor(map);

    if (map_is_validated(load_factor)) {
        return FJ_OK;
    }

    return rehash(map, map_needs_to_grow(load_factor));
}


static fj_result_t map_update(fj_map_t * map, fj_id_t key, fj_ptr_t value)
{
    fj_map_element_t * element = fj_map_find(map, key);

    if (element != NULL) {
        element->value = value;
        return FJ_OK;
    }

    return FJ_INTERNAL_FAIL;
}


static void iter_go_to_next_node_in_bucket(fj_map_iter_t * iter)
{
    if (iter->current_node != NULL) {
        iter->current_node = iter->current_node->next;
    }
}


static fj_map_node_t ** find_next_used_bucket(
    fj_map_t * map,
    uint32_t start_index
)
{
    for (uint32_t index = start_index; index < map->buckets_count; index++) {
        if (map->buckets[index] != NULL) {
            return &map->buckets[index];
        }
    }

    return NULL;
}


static uint32_t get_bucket_search_start_index(fj_map_iter_t * iter)
{
    uint32_t start_index = iter->current_bucket_index;

    // If the iterator has already started
    if (iter->current_node != NULL) {
        // Search *after* the current index
        start_index++;
    }

    return start_index;
}


static void iter_go_to_next_bucket(fj_map_iter_t * iter)
{
    fj_map_t * map = iter->map;
    uint32_t start_index = get_bucket_search_start_index(iter);
    fj_map_node_t ** bucket = find_next_used_bucket(map, start_index);

    if (bucket == NULL) {
        iter->current_node = NULL;
        return;
    }

    iter->current_bucket_index = bucket - map->buckets;
    iter->current_node = *bucket;
}


fj_map_t * fj_map_new(void)
{
    fj_map_t * map = calloc(1, sizeof(fj_map_t));

    if (map == NULL) {
        return NULL;
    }

    map->buckets = calloc(1, sizeof(fj_map_node_t *));
    map->buckets_count = 1;

    if (map->buckets == NULL) {
        free(map);
        return NULL;
    }

    return map;
}


void fj_map_del(fj_map_t * map)
{
    if (map->buckets != NULL) {
        free(map->buckets);
    }

    free(map);
}


fj_result_t fj_map_insert(fj_map_t * map, fj_id_t key, fj_ptr_t value)
{
    if (map_update(map, key, value) == FJ_OK) {
        return FJ_OK;
    }

    fj_map_node_t * node = calloc(1, sizeof(fj_map_node_t));

    if (node == NULL) {
        return FJ_MALLOC_FAIL;
    }

    node->element.key = key;
    node->element.value = value;
    raw_insert(map, node);

    map->elements_count++;

    return validate_map(map);
}


fj_result_t fj_map_remove(fj_map_t * map, fj_id_t key)
{
    fj_map_node_t * node = raw_remove(map, key);

    if (node == NULL) {
        return FJ_INTERNAL_FAIL;
    }

    free(node);

    map->elements_count--;

    return validate_map(map);
}


fj_map_element_t * fj_map_find(fj_map_t * map, fj_id_t key)
{
    fj_map_node_t ** bucket = get_bucket(map, key);

    fj_map_node_t * prev_node;
    fj_map_node_t * node;
    raw_find(*bucket, key, &node, &prev_node);

    if (node == NULL) {
        return NULL;
    }

    return &node->element;
}


void fj_map_iter_init(fj_map_t * map, fj_map_iter_t * iter)
{
    iter->map = map;
    iter->current_bucket_index = 0;
    iter->current_node = NULL;
}


fj_map_element_t * fj_map_iter_next(fj_map_iter_t * iter)
{
    iter_go_to_next_node_in_bucket(iter);

    if (iter->current_node != NULL) {
        return &iter->current_node->element;
    }

    iter_go_to_next_bucket(iter);

    if (iter->current_node != NULL) {
        return &iter->current_node->element;
    }

    return NULL;
}