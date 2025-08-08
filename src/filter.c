#include "include/filter.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/iso8601.h"

#define HASH_SIZE 256

typedef struct hash_node {
  iso8601_time_t *time;
  struct hash_node *next;
} node_t;

// TODO:  There are only about 15 different characters in this string, could
// that effect the hash distribution?  Add some cache monitoring
node_t *hash_table[HASH_SIZE];

void init_filter() { memset(hash_table, 0, sizeof(hash_table)); }

static int64_t compute_adjusted_time(const iso8601_time_t *time) {
  return time->seconds + time->adjust_direction * time->adjust_value;
}

static uint8_t compute_hash(const iso8601_time_t *time) {
  return compute_adjusted_time(time) % HASH_SIZE;
}

static uint8_t compute_hash_str(const iso8601_time_t *time) {
  const char *hash_string = (const char *)time->str;

  uint8_t val = 0;
  int len = strlen(hash_string);
  for (int i = 0; i < len; i++) {
    val ^= hash_string[i];
  }
  return val;
}

static node_t *new_node(iso8601_time_t *time) {
  node_t *node = malloc(sizeof(node_t));
  node->next = NULL;
  node->time = time;
  return node;
}

static bool find_entry(node_t *top, iso8601_time_t *time) {
  node_t *node = top;

  int64_t adjusted_time = compute_adjusted_time(time);
  while (node != NULL) {
    int64_t adjusted_node = compute_adjusted_time(node->time);
    if (adjusted_time == adjusted_node) {
      return true;
    }
    node = node->next;
  }
  return false;
}

// Checking for presence shouldn't change the entry
static bool check_presence(iso8601_time_t *time) {
  uint8_t hash_val = compute_hash(time);

  node_t *table_entry = hash_table[hash_val];
  // No collision, so add to the hash and don't filter
  if (table_entry == NULL) {
    node_t *node = new_node(time);
    hash_table[hash_val] = node;
    return false;
  } else {
    // Check to see if it is in the list, if not, add
    if (find_entry(table_entry, time) == false) {
      // TODO: You may be able to optimize this better for searching
      // Possibly roll into find
      node_t *node = new_node(time);
      node->next = table_entry;
      hash_table[hash_val] = node;
      return false;
    }
  }
  return true;
}

// Apply filter(s) to time
bool filter_time(iso8601_time_t *time) {
  bool remove = false;
  remove = check_presence(time);
  return remove;
}
