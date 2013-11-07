#ifndef __LIBDARTS_H__
#define __LIBDARTS_H__

#ifdef __cplusplus
extern "C" {
#endif
  
#include <stddef.h>

typedef void* darts_t;
typedef char darts_key_type;
typedef int darts_value_type;

darts_t darts_new(void);

void darts_delete(darts_t darts);

void darts_set_array(darts_t darts,
                     const void *ptr,
                     size_t size);

const void* darts_array(const darts_t darts);

void darts_clear(darts_t darts);

size_t darts_unit_size(const darts_t darts);

size_t darts_size(const darts_t darts);

size_t darts_total_size(const darts_t darts);

size_t darts_nonzero_size(const darts_t darts);

int darts_build(darts_t darts,
                size_t num_keys,
                const darts_key_type*const* keys,
                const size_t* lengths,
                const darts_value_type* values,
                int (*progress_func)(size_t, size_t));

int darts_open(darts_t darts,
               const char* file_name,
               const char* mode,
               size_t offset,
               size_t size);

int darts_save(const darts_t darts,
               const char* file_name,
               const char* mode,
               size_t offset);

darts_value_type darts_exact_match_search(const darts_t darts,
                                          const darts_key_type* key,
                                          size_t length,
                                          size_t node_pos);

size_t darts_common_prefix_search(const darts_t darts,
                                  const darts_key_type* key,
                                  darts_value_type* results,
                                  size_t max_num_results,
                                  size_t length,
                                  size_t node_pos);

darts_value_type darts_traverse(const darts_t darts,
                                const darts_key_type* key,
                                size_t* node_pos,
                                size_t* key_pos,
                                size_t length);
  
#ifdef __cplusplus
}
#endif

#endif
