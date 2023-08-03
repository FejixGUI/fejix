#include <fejix/core/map.h>
#include <fejix/core/utils.h>

#include <string.h>


#define MAX_LOAD_FACTOR 0.618f
#define MIN_LOAD_FACTOR (MAX_LOAD_FACTOR/4.0f)


struct map_node;


/// Bucket is a head node of a list of nodes
typedef struct map_node * bucket_t;


struct fj_map {
    bucket_t * buckets;
    uint32_t buckets_count;
    uint32_t elements_count;
};

struct map_node {
    struct map_node * next;
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


static fj_id_t get_key(struct map_node * node)
{
    return node->element.key;
}


static uint32_t get_bucket_index(fj_id_t key, uint32_t buckets_count)
{
    return hash(key) % buckets_count;
}


static bucket_t * get_bucket(struct fj_map * map, fj_id_t key)
{
    uint32_t index = get_bucket_index(key, map->buckets_count);
    return &map->buckets[index];
}


static struct map_node * get_tail_node(struct map_node * list_head)
{
    struct map_node * node = list_head;

    while (node->next != NULL) {
        node = node->next;
    }

    return node;
}


static void insert_node_to_bucket(
    bucket_t * bucket,
    struct map_node * node
)
{
    struct map_node * next = *bucket;
    *bucket = node;
    node->next = next;
}


static void remove_node_from_bucket(
    bucket_t * bucket,
    struct map_node * prev_node,
    struct map_node * node
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
    memset(map->buckets, 0, map->buckets_count * sizeof(struct map_node *));
}


static void raw_insert(struct fj_map * map, struct map_node * node)
{
    bucket_t * bucket = get_bucket(map, get_key(node));
    insert_node_to_bucket(bucket, node);
}


static void raw_find(
    struct map_node * list_head,
    fj_id_t key,
    struct map_node ** found_node,
    struct map_node ** found_previous_node
)
{
    *found_node = NULL;
    *found_previous_node = NULL;

    if (list_head == NULL) {
        return;
    }

    struct map_node * previous_node = NULL;
    struct map_node * current_node = list_head;

    while (current_node != NULL && get_key(current_node) != key) {
        previous_node = current_node;
        current_node = current_node->next;
    }

    if (current_node != NULL && get_key(current_node) == key) {
        *found_node = current_node;
        *found_previous_node = previous_node;
    }
}


static struct map_node * raw_remove(struct fj_map * map, fj_id_t key)
{
    bucket_t * bucket = get_bucket(map, key);

    struct map_node * list_head = *bucket;
    struct map_node * prev_node;
    struct map_node * node;
    raw_find(list_head, key, &node, &prev_node);

    if (node == NULL) {
        return NULL;
    }

    remove_node_from_bucket(bucket, prev_node, node);

    return node;
}


static struct map_node * extract_nodes(struct fj_map * map)
{
    if (map->buckets == NULL) {
        return NULL;
    }

    struct map_node * head_node = NULL;
    struct map_node * tail_node = NULL;

    for (uint32_t i = 0; i < map->buckets_count; i++) {
        struct map_node * node = map->buckets[i];

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


static void reinsert_nodes(struct fj_map * map, struct map_node * list_head)
{
    struct map_node * node = list_head;

    while (node != NULL) {
        struct map_node * next = node->next;
        raw_insert(map, node);
        node = next;
    }
}


static void free_nodes(struct map_node * list_head)
{
    struct map_node * node = list_head;

    while (node != NULL) {
        struct map_node * next_node = node->next;
        fj_free(node);
        node = next_node;
    }
}


static float get_load_factor(struct fj_map * map)
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


static fj_err_t resize_buckets(struct fj_map * map, uint32_t buckets_count)
{
    size_t bucket_size = sizeof(struct map_node *);
    bucket_t * buckets = fj_realloc(map->buckets, buckets_count, bucket_size);

    if (buckets == NULL) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    map->buckets = buckets;
    map->buckets_count = buckets_count;

    return FJ_OK;
}


static fj_err_t resize_map(struct fj_map * map, fj_bool_t grow)
{
    if (grow) {
        return resize_buckets(map, map->buckets_count * 2);
    }

    return resize_buckets(map, fj_max(1, map->buckets_count / 2));
}


static fj_err_t rehash(struct fj_map * map, fj_bool_t grow)
{
    struct map_node * list_head = extract_nodes(map);

    fj_err_t e = resize_map(map, grow);

    if (e != FJ_OK) {
        free_nodes(list_head);
        fj_free(map->buckets);
        fj_free(map);

        return e;
    }

    reinsert_nodes(map, list_head);

    return FJ_OK;
}


static fj_err_t validate_map(struct fj_map * map)
{
    float load_factor = get_load_factor(map);

    if (!map_is_validated(load_factor)) {
        return rehash(map, map_needs_to_grow(load_factor));
    }

    return FJ_OK;
}


static fj_err_t map_remove(struct fj_map * map, fj_id_t key)
{
    struct map_node * node = raw_remove(map, key);

    if (node == NULL) {
        return FJ_OK;
    }

    fj_free(node);

    map->elements_count--;

    return validate_map(map);
}


static struct fj_map_element * map_find(struct fj_map * map, fj_id_t key)
{
    struct map_node * list_head = *get_bucket(map, key);

    struct map_node * prev_node;
    struct map_node * node;
    raw_find(list_head, key, &node, &prev_node);

    if (node == NULL) {
        return NULL;
    }

    return &node->element;
}


static fj_bool_t map_update(struct fj_map * map, fj_id_t key, fj_ptr_t value)
{
    struct fj_map_element * element = map_find(map, key);

    if (element == NULL) {
        return false;
    }

    element->value = value;

    return true;
}


static fj_err_t map_insert(struct fj_map * map, fj_id_t key, fj_ptr_t value)
{
    struct map_node * node = fj_alloc_zeroed(sizeof(struct map_node));

    if (node == NULL) {
        return FJ_ERR(FJ_MALLOC_FAILED);
    }

    node->element.key = key;
    node->element.value = value;
    raw_insert(map, node);

    map->elements_count++;

    return validate_map(map);
}


struct fj_map * fj_map_new(void)
{
    struct fj_map * map = fj_alloc_zeroed(sizeof(struct fj_map));

    if (map == NULL) {
        return NULL;
    }

    map->buckets = fj_alloc_zeroed(sizeof(bucket_t));
    map->buckets_count = 1;

    if (map->buckets == NULL) {
        fj_free(map);
        return NULL;
    }

    return map;
}


void fj_map_del(struct fj_map * map)
{
    struct map_node * list_head = extract_nodes(map);

    if (list_head != NULL) {
        free_nodes(list_head);
    }

    if (map->buckets != NULL) {
        fj_free(map->buckets);
    }

    fj_free(map);
}


fj_err_t fj_map_set(struct fj_map * map, fj_id_t key, fj_ptr_t value)
{
    if (value == NULL) {
        return map_remove(map, key);
    }

    if (map_update(map, key, value)) {
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

        struct map_node * node = map->buckets[i];

        while (node != NULL) {
            fj_bool_t should_continue = callback(&node->element, data);
            
            if (!should_continue) {
                return;
            }

            node = node->next;
        }
    }
}
