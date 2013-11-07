#include "darts.h"
#include "darts.hh"

using namespace Darts;

darts_t darts_new(void) {
  DoubleArray* inst = new DoubleArray();
  return inst;
}

void darts_delete(darts_t darts) {
  DoubleArray* inst = (DoubleArray*) darts;
  delete inst;
}

void darts_set_array(darts_t darts,
                     const void *ptr,
                     size_t size) {
  DoubleArray* inst = (DoubleArray*) darts;
  inst->set_array(ptr, size);
}

const void* darts_array(const darts_t darts) {
  const DoubleArray* inst = (const DoubleArray*) darts;
  return inst->array();
}

void darts_clear(darts_t darts) {
  DoubleArray* inst = (DoubleArray*) darts;
  inst->clear();
}

size_t darts_unit_size(const darts_t darts) {
  const DoubleArray* inst = (const DoubleArray*) darts;
  return inst->unit_size();
}

size_t darts_size(const darts_t darts) {
  const DoubleArray* inst = (const DoubleArray*) darts;
  return inst->size();
}

size_t darts_total_size(const darts_t darts) {
  const DoubleArray* inst = (const DoubleArray*) darts;
  return inst->total_size();
}

size_t darts_nonzero_size(const darts_t darts) {
  const DoubleArray* inst = (const DoubleArray*) darts;
  return inst->nonzero_size();
}

int darts_build(darts_t darts,
                size_t num_keys,
                const darts_key_type*const* keys,
                const size_t* lengths,
                const darts_value_type* values,
                int (*progress_func)(size_t, size_t)) {
  DoubleArray* inst = (DoubleArray*) darts;
  return inst->build(num_keys, keys, lengths, values, progress_func);
}

int darts_open(darts_t darts,
               const char* file_name,
               const char* mode,
               size_t offset,
               size_t size) {
  DoubleArray* inst = (DoubleArray*) darts;
  return inst->open(file_name, mode, offset, size);
}

int darts_save(const darts_t darts,
               const char* file_name,
               const char* mode,
               size_t offset) {
  const DoubleArray* inst = (const DoubleArray*) darts;
  return inst->save(file_name, mode, offset);
}

darts_value_type darts_exact_match_search(const darts_t darts,
                                          const darts_key_type* key,
                                          size_t length,
                                          size_t node_pos) {
  const DoubleArray* inst = (const DoubleArray*) darts;
  return inst->exactMatchSearch<darts_value_type>(key, length, node_pos);
}

size_t darts_common_prefix_search(const darts_t darts,
                                  const darts_key_type* key,
                                  darts_value_type* results,
                                  size_t max_num_results,
                                  size_t length,
                                  size_t node_pos) {
  const DoubleArray* inst = (const DoubleArray*) darts;
  return inst->commonPrefixSearch(key,
                                  results,
                                  max_num_results,
                                  length,
                                  node_pos);
}

darts_value_type darts_traverse(const darts_t darts,
                                const darts_key_type* key,
                                size_t* node_pos,
                                size_t* key_pos,
                                size_t length) {
  const DoubleArray* inst = (const DoubleArray*) darts;
  return inst->traverse(key, *node_pos, *key_pos, length);
}
