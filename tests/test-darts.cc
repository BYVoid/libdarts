#include "../src/darts.h"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <set>
#include <string>
#include <vector>

void generate_valid_keys(std::size_t num_keys,
    std::set<std::string> *valid_keys) {
  std::vector<char> key;
  while (valid_keys->size() < num_keys) {
    key.resize(1 + (std::rand() % 8));
    for (std::size_t i = 0; i < key.size(); ++i) {
      key[i] = 'A' + (std::rand() % 26);
    }
    std::string key_str = std::string(&key[0], key.size());
    key_str += "漢字";
    valid_keys->insert(key_str);
  }
}

void generate_invalid_keys(std::size_t num_keys,
    const std::set<std::string> &valid_keys,
    std::set<std::string> *invalid_keys) {
  std::vector<char> key;
  while (invalid_keys->size() < num_keys) {
    key.resize(1 + (std::rand() % 8));
    for (std::size_t i = 0; i < key.size(); ++i) {
      key[i] = 'A' + (std::rand() % 26);
    }
    std::string generated_key(&key[0], key.size());
    if (valid_keys.find(generated_key) == valid_keys.end())
      invalid_keys->insert(std::string(&key[0], key.size()));
  }
}

void test_dic(darts_t dic, const std::vector<const char *> &keys,
    const std::vector<std::size_t> &lengths,
    const std::vector<darts_value_type> &values,
    const std::set<std::string> &invalid_keys) {
  darts_value_type value;
  darts_result_pair_type result;

  for (std::size_t i = 0; i < keys.size(); ++i) {
    value = darts_exact_match_search(dic, keys[i], 0, 0);
    assert(value == values[i]);
    
    result = darts_exact_match_search_pair(dic, keys[i], 0, 0);
    assert(result.value == values[i]);
    assert(result.length == lengths[i]);
    
    value = darts_exact_match_search(dic, keys[i], lengths[i], 0);
    assert(value == values[i]);

    result = darts_exact_match_search_pair(dic, keys[i], lengths[i], 0);
    assert(result.value == values[i]);
    assert(result.length == lengths[i]);
  }

  for (std::set<std::string>::const_iterator it = invalid_keys.begin();
      it != invalid_keys.end(); ++it) {
    value = darts_exact_match_search(dic, it->c_str(), 0, 0);
    assert(value == -1);

    result = darts_exact_match_search_pair(dic, it->c_str(), 0, 0);
    assert(result.value == -1);

    value = darts_exact_match_search(dic, it->c_str(), it->length(), 0);
    assert(value == -1);

    result = darts_exact_match_search_pair(dic, it->c_str(), it->length(), 0);
    assert(result.value == -1);
  }

  std::cerr << "ok" << std::endl;
}

void test_common_prefix_search(darts_t dic,
    const std::vector<const char *> &keys,
    const std::vector<std::size_t> &lengths,
    const std::vector<darts_value_type> &values,
    const std::set<std::string> &invalid_keys) {
  static const std::size_t MAX_NUM_RESULTS = 16;
  darts_result_pair_type results[MAX_NUM_RESULTS];
  darts_result_pair_type results_with_length[MAX_NUM_RESULTS];

  for (std::size_t i = 0; i < keys.size(); ++i) {
    std::size_t num_results = darts_common_prefix_search(
        dic, keys[i], results, MAX_NUM_RESULTS, 0, 0);

    assert(num_results >= 1);
    assert(num_results < 10);

    assert(results[num_results - 1].value == values[i]);
    assert(results[num_results - 1].length == lengths[i]);

    std::size_t num_results_with_length = darts_common_prefix_search(
        dic, keys[i], results_with_length, MAX_NUM_RESULTS, lengths[i], 0);

    assert(num_results == num_results_with_length);
    for (std::size_t j = 0; j < num_results; ++j) {
      assert(results[j].value == results_with_length[j].value);
      assert(results[j].length == results_with_length[j].length);
    }
  }

  for (std::set<std::string>::const_iterator it = invalid_keys.begin();
      it != invalid_keys.end(); ++it) {
    std::size_t num_results = darts_common_prefix_search(dic,
        it->c_str(), results, MAX_NUM_RESULTS, 0, 0);

    assert(num_results < 10);

    if (num_results > 0) {
      assert(results[num_results - 1].value != -1);
      assert(results[num_results - 1].length < it->length());
    }

    std::size_t num_results_with_length = darts_common_prefix_search(
      dic, it->c_str(), results_with_length, MAX_NUM_RESULTS, it->length(), 0);

    assert(num_results == num_results_with_length);
    for (std::size_t j = 0; j < num_results; ++j) {
      assert(results[j].value == results_with_length[j].value);
      assert(results[j].length == results_with_length[j].length);
    }
  }

  std::cerr << "ok" << std::endl;
}

void test_traverse(const darts_t dic,
    const std::vector<const char *> &keys,
    const std::vector<std::size_t> &lengths,
    const std::vector<darts_value_type> &values,
    const std::set<std::string> &invalid_keys) {
  for (std::size_t i = 0; i < keys.size(); ++i) {
    const char *key = keys[i];
    std::size_t id = 0;
    std::size_t key_pos = 0;
    darts_value_type result = 0;
    for (std::size_t j = 0; j < lengths[i]; ++j) {
      result = darts_traverse(dic, key, &id, &key_pos, j + 1);
      assert(result != -2);
    }
    assert(result == values[i]);
  }

  for (std::set<std::string>::const_iterator it = invalid_keys.begin();
      it != invalid_keys.end(); ++it) {
    const char *key = it->c_str();
    std::size_t id = 0;
    std::size_t key_pos = 0;
    darts_value_type result = 0;
    for (std::size_t i = 0; i < it->length(); ++i) {
      result = darts_traverse(dic, key, &id, &key_pos, i + 1);
      if (result == -2) {
        break;
      }
    }
    assert(result < 0);
  }

  std::cerr << "ok" << std::endl;
}

void test_darts(const std::set<std::string> &valid_keys,
    const std::set<std::string> &invalid_keys) {
  std::vector<const char *> keys(valid_keys.size());
  std::vector<std::size_t> lengths(valid_keys.size());
  std::vector<darts_value_type> values(valid_keys.size());

  std::size_t key_id = 0;
  for (std::set<std::string>::const_iterator it = valid_keys.begin();
      it != valid_keys.end(); ++it, ++key_id) {
    keys[key_id] = it->c_str();
    lengths[key_id] = it->length();
    values[key_id] = static_cast<darts_value_type>(key_id);
  }

  darts_t dic = darts_new();

  std::cerr << "build() with keys: ";
  darts_build(dic, keys.size(), &keys[0], NULL, NULL, NULL);
  test_dic(dic, keys, lengths, values, invalid_keys);

  std::cerr << "build() with keys and lengths: ";
  darts_build(dic, keys.size(), &keys[0], &lengths[0], NULL, NULL);
  test_dic(dic, keys, lengths, values, invalid_keys);

  std::cerr << "build() with keys, lengths and values: ";
  darts_build(dic, keys.size(), &keys[0], &lengths[0], &values[0], NULL);
  test_dic(dic, keys, lengths, values, invalid_keys);

  for (std::size_t i = 0; i < values.size(); ++i) {
    values[i] = std::rand() % 10;
  }

  std::cerr << "build() with keys, lengths and random values: ";
  darts_build(dic, keys.size(), &keys[0], &lengths[0], &values[0], NULL);
  test_dic(dic, keys, lengths, values, invalid_keys);

  darts_t dic_copy = darts_new();

  std::cerr << "save() and open(): ";
  assert(darts_save(dic, "test-darts.dic", "wb", 0) == 0);
  assert(darts_open(dic_copy, "test-darts.dic", "rb", 0, 0) == 0);
  assert(darts_size(dic) == darts_size(dic_copy));
  test_dic(dic_copy, keys, lengths, values, invalid_keys);

  std::cerr << "set_array() with array(): ";
  darts_set_array(dic_copy, darts_array(dic), 0);
  assert(darts_size(dic_copy) == 0);
  test_dic(dic_copy, keys, lengths, values, invalid_keys);

  std::cerr << "set_array() with array() and size(): ";
  darts_set_array(dic_copy, darts_array(dic), darts_size(dic));
  assert(darts_size(dic) == darts_size(dic_copy));
  test_dic(dic_copy, keys, lengths, values, invalid_keys);

  std::cerr << "commonPrefixSearch(): ";
  test_common_prefix_search(dic, keys, lengths, values, invalid_keys);

  std::cerr << "traverse(): ";
  test_traverse(dic, keys, lengths, values, invalid_keys);
  
  darts_delete(dic);
}

int main() {
  try {
    std::srand((unsigned int)time(NULL));

    static const std::size_t NUM_VALID_KEYS = 1 << 16;
    static const std::size_t NUM_INVALID_KEYS = 1 << 17;

    std::set<std::string> valid_keys;
    generate_valid_keys(NUM_VALID_KEYS, &valid_keys);

    std::set<std::string> invalid_keys;
    generate_invalid_keys(NUM_INVALID_KEYS, valid_keys, &invalid_keys);

    test_darts(valid_keys, invalid_keys);
  } catch (const std::exception &ex) {
    std::cerr << "exception: " << ex.what() << std::endl;
    throw ex;
  }

  return 0;
}
