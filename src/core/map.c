#include <fejix/core/map.h>

#include <fejix/core/alloc.h>
#include <fejix/core/utils.h>

#include <string.h>


#define MAX_LOAD_FACTOR (0.618f)
#define MIN_LOAD_FACTOR (MAX_LOAD_FACTOR/4.0f)


fj_bool32_t fj_map_is_empty(struct fj_map const * map)
{
    return map->element_count == 0;
}

fj_bool32_t fj_map_has_allocated(struct fj_map const * map)
{
    return map->buckets != NULL;
}


static
union fj_any get_key(struct fj_map_node const * node)
{
    return node->element.key;
}


static
fj_bool32_t key_eq(struct fj_map const * map, struct fj_map_node const * node, union fj_any key)
{
    return fj_any_eq(get_key(node), key, map->key_type);
}


static
uint32_t get_bucket_index(struct fj_map const * map, union fj_any key)
{
    return fj_any_hash32(key, map->key_type) % map->bucket_count;
}


static
struct fj_map_node * * get_bucket(struct fj_map const * map, union fj_any key)
{
    uint32_t index = get_bucket_index(map, key);
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
void insert_node_to_bucket(struct fj_map_node * * bucket, struct fj_map_node * node)
{
    struct fj_map_node * next = *bucket;
    *bucket = node;
    node->next = next;
}


static
void remove_node_from_bucket(
    struct fj_map_node * * bucket,
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
    struct fj_map_node * * bucket = get_bucket(map, get_key(node));
    insert_node_to_bucket(bucket, node);
}


static
void find_node_in_bucket(
    struct fj_map const * map,
    struct fj_map_node * * bucket,
    union fj_any key,
    struct fj_map_node */*? out*/ * found_node,
    struct fj_map_node */*? out*/ * found_previous_node
)
{
    *found_node = NULL;
    *found_previous_node = NULL;

    if (bucket == NULL) {
        return;
    }

    struct fj_map_node * previous_node = NULL;
    struct fj_map_node * current_node = *bucket;

    while (current_node != NULL && !key_eq(map, current_node, key)) {
        previous_node = current_node;
        current_node = current_node->next;
    }

    if (current_node != NULL && key_eq(map, current_node, key)) {
        *found_node = current_node;
        *found_previous_node = previous_node;
    }
}


static
struct fj_map_node * raw_remove(struct fj_map * map, union fj_any key)
{
    struct fj_map_node * * bucket = get_bucket(map, key);

    struct fj_map_node * node;
    struct fj_map_node * prev_node;
    find_node_in_bucket(map, bucket, key, &node, &prev_node);

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
        FJ_FREE(&node);
        node = next_node;
    }
}


static
float get_load_factor(struct fj_map * map)
{
    return (float) map->element_count / (float) map->bucket_count;
}


static
fj_bool32_t map_needs_to_grow(float load_factor)
{
    return load_factor > MAX_LOAD_FACTOR;
}


static
fj_bool32_t map_needs_to_shrink(float load_factor)
{
    return load_factor < MIN_LOAD_FACTOR;
}


static
fj_bool32_t map_is_validated(float load_factor)
{
    return !map_needs_to_grow(load_factor) && !map_needs_to_shrink(load_factor);
}


static
fj_err_t resize_buckets(struct fj_map * map, uint32_t bucket_count)
{
    FJ_INIT_TRY

    FJ_TRY(FJ_REALLOC_ZEROED(&map->buckets, map->bucket_count, bucket_count)) {
        return FJ_RESULT;
    }

    map->bucket_count = bucket_count;

    return FJ_OK;
}


static
fj_err_t resize_map(struct fj_map * map, fj_bool32_t grow)
{
    uint32_t bucket_count = 0;

    if (grow) {
        bucket_count = map->bucket_count * 2;
    } else {
        bucket_count = FJ_MAX(1, map->bucket_count / 2);
    }

    return resize_buckets(map, bucket_count);
}


static
fj_err_t rehash(struct fj_map * map, fj_bool32_t grow)
{
    FJ_INIT_TRY

    struct fj_map_node * list_head = extract_nodes(map);

    FJ_TRY(resize_map(map, grow)) {
        free_nodes(list_head);
        FJ_FREE(&map->buckets);
        FJ_FREE(&map);

        return FJ_RESULT;
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
fj_err_t map_remove(struct fj_map * map, union fj_any key)
{
    struct fj_map_node * node = raw_remove(map, key);

    if (node == NULL) {
        return FJ_OK;
    }

    FJ_FREE(&node);

    map->element_count--;

    return validate_map(map);
}


static
struct fj_map_element * map_find(struct fj_map const * map, union fj_any key)
{
    struct fj_map_node * * bucket = get_bucket(map, key);

    struct fj_map_node * prev_node;
    struct fj_map_node * node;
    find_node_in_bucket(map, bucket, key, &node, &prev_node);

    if (node == NULL) {
        return NULL;
    }

    return &node->element;
}


/** Returns true if the value was updated, false if the record does not exist in the map. */
static
fj_bool32_t map_update(struct fj_map * map, union fj_any key, union fj_any value)
{
    struct fj_map_element * element = map_find(map, key);

    if (element == NULL) {
        return false;
    }

    element->value = value;

    return true;
}


static
fj_err_t map_insert(struct fj_map * map, union fj_any key, union fj_any value)
{
    FJ_INIT_TRY

    struct fj_map_node * node = NULL;

    FJ_TRY(FJ_ALLOC_ZEROED(&node)) {
        return FJ_RESULT;
    }

    node->element.key = key;
    node->element.value = value;
    raw_insert(map, node);

    map->element_count++;

    return validate_map(map);
}


static
fj_err_t map_allocate(struct fj_map * map)
{
    FJ_INIT_TRY

    FJ_TRY(FJ_REALLOC_ZEROED(&map->buckets, 0, 1)) {
        return FJ_RESULT;
    }

    map->bucket_count = 1;

    return FJ_OK;
}


static
fj_err_t map_set(struct fj_map * map, union fj_any key, union fj_any value)
{
    if (map_update(map, key, value)) {
        return FJ_OK;
    }

    return map_insert(map, key, value);
}


static
void */*?*/ map_get(struct fj_map const * map, union fj_any key)
{
    struct fj_map_element * element = map_find(map, key);

    if (element == NULL) {
        return NULL;
    }

    return &element->value;
}


void fj_map_init(struct fj_map /*out*/ * map, fj_any_type_t key_type, fj_any_type_t value_type)
{
    *map = (struct fj_map) {
        .key_type = key_type,
        .value_type = value_type
    };
}


void fj_map_deinit(struct fj_map * map)
{
    if (!fj_map_has_allocated(map)) {
        return;
    }

    struct fj_map_node * list_head = extract_nodes(map);

    if (list_head != NULL) {
        free_nodes(list_head);
    }

    if (map->buckets != NULL) {
        FJ_FREE(&map->buckets);
    }
}


fj_err_t fj_map_set(struct fj_map * map, union fj_any key, union fj_any value)
{
    FJ_INIT_TRY

    if (!fj_map_has_allocated(map)) {
        FJ_TRY(map_allocate(map)) {
            return FJ_RESULT;
        }
    }

    return map_set(map, key, value);
}


void */*?*/ fj_map_get(struct fj_map const * map, union fj_any key)
{
    if (!fj_map_has_allocated(map)) {
        return NULL;
    }

    return map_get(map, key);
}


fj_err_t fj_map_remove(struct fj_map * map, union fj_any key)
{
    if (!fj_map_has_allocated(map)) {
        return FJ_OK;
    }

    return map_remove(map, key);
}


void fj_map_iter_init(struct fj_map_iter /*out*/ * iter, struct fj_map const * map)
{
    iter->map = map;
    iter->bucket_index = 0;
    iter->current_node = NULL;
}


fj_bool32_t fj_map_iter_finished(struct fj_map_iter const * iter)
{
    return iter->bucket_index >= iter->map->bucket_count;
}


static
fj_bool32_t iter_process_result(
    struct fj_map_iter * iter,
    struct fj_map_element * /*out*/ * element
)
{
    if (iter->current_node == NULL) {
        *element = NULL;
        return false;
    }

    *element = &iter->current_node->element;
    return true;
}

static
fj_bool32_t iter_can_walk_nodes(struct fj_map_iter const * iter)
{
    return iter->current_node != NULL && iter->current_node->next != NULL;
}

static
void iter_walk_nodes(struct fj_map_iter * iter)
{
    iter->current_node = iter->current_node->next;
}

static
fj_bool32_t iter_started_bucket(struct fj_map_iter const * iter)
{
    return iter->current_node != NULL;
}

static
fj_bool32_t iter_can_start_bucket(struct fj_map_iter const * iter)
{
    return iter->map->buckets[iter->bucket_index] != NULL;
}

static
void iter_start_bucket(struct fj_map_iter * iter)
{
    iter->current_node = iter->map->buckets[iter->bucket_index];
}

static
void iter_next_bucket(struct fj_map_iter * iter)
{
    iter->bucket_index++;
    iter->current_node = NULL;
}

static
void iter_walk_buckets(struct fj_map_iter * iter)
{
    while (!fj_map_iter_finished(iter)) {
        if (!iter_started_bucket(iter) && iter_can_start_bucket(iter)) {
            iter_start_bucket(iter);
            break;
        }

        iter_next_bucket(iter);
    }
}


fj_bool32_t fj_map_iter_next(
    struct fj_map_iter * iter,
    struct fj_map_element */*? out*/ * element
)
{
    if (iter_can_walk_nodes(iter)) {
        iter_walk_nodes(iter);
    } else {
        iter_walk_buckets(iter);
    }

    return iter_process_result(iter, element);
}
