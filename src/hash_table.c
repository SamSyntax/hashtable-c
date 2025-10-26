#include "../includes/hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_table(hash_table *hash_table) {
  token_t *table = (token_t *)hash_table->table;
  for (size_t i = 0; i < hash_table->capacity; ++i) {
    table[i].key = NULL;
    table[i].value = 0;
  }
}

size_t hash(char *str, size_t capacity) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }
  if (capacity > 0) {
    return hash % capacity;
  }
  return 0;
}

void *tokenize(char *input, hash_table *hash_table) {
  if (input == NULL)
    return NULL;
  char *str = strdup(input);
  if (!str)
    return NULL;

  const char *delimiters = " \n\r\t";
  char *token;
  char *rest = str;
  size_t collisions_count = 0;
  size_t token_count = 0;
  size_t unique_tokens = 0;

  printf("\nTokens:\n");

  while ((token = strtok_r(rest, delimiters, &rest)) != NULL) {
    token_count++;
    if (hash_table->size >= hash_table->capacity) {
      size_t new_capacity = hash_table->capacity * 2;
      if (new_capacity == 0) {
        new_capacity = 8;
      }
      token_t *new_table = malloc(new_capacity * sizeof(token_t));
      if (!new_table) {
        free(str);
        void *analyzer_ptr = malloc(sizeof(token_analysis_t));
        token_analysis_t res = {.global_token_count = token_count,
                                .collisions_count = collisions_count,
                                .unique_tokens = unique_tokens};
        memcpy(analyzer_ptr, &res, sizeof(res));
        return analyzer_ptr;
      }
      for (size_t i = 0; i < new_capacity; i++) {
        new_table[i].key = NULL;
        new_table[i].value = 0;
      }

      for (size_t i = 0; i < hash_table->capacity; i++) {
        token_t *old_entry = &((token_t *)hash_table->table)[i];
        if (old_entry->key != NULL) {
          size_t new_key = hash(old_entry->key, new_capacity);
          size_t original_new_key = new_key;
          while (new_table[new_key].key != NULL) {
            new_key = (new_key + 1) % new_capacity;
            if (new_key == original_new_key) {
              break;
            }
          }
          new_table[new_key].key = old_entry->key;
          new_table[new_key].value = old_entry->value;
          new_table[new_key].hash_key = new_key;
        }
      }
      free(hash_table->table);
      hash_table->table = new_table;
      hash_table->capacity = new_capacity;
    }
    token_t *table = (token_t *)hash_table->table;
    size_t key = hash(token, hash_table->capacity);
    size_t original_key = key;
    while (table[key].key != NULL && strcmp(table[key].key, token) != 0) {
      key = (key + 1) % hash_table->capacity;
      if (key == original_key) {
        break;
      }
    }
    if (table[key].key == NULL) {
      table[key].key = strdup(token);
      table[key].value = 1;
      table[key].hash_key = key;
      hash_table->size += 1;
      unique_tokens++;
    }
    if (key != original_key) {
      collisions_count++;
    } else {
      table[key].value += 1;
    }
  }
  free(str);
  void *analyzer_ptr = malloc(sizeof(token_analysis_t));
  token_analysis_t res = {.global_token_count = token_count,
                          .collisions_count = collisions_count,
                          .unique_tokens = unique_tokens};
  memcpy(analyzer_ptr, &res, sizeof(res));
  return analyzer_ptr;
}

void free_table(token_t *table, size_t capacity) {
  for (size_t i = 0; i < capacity; i++) {
    if (table[i].key != NULL) {
      free(table[i].key);
      table[i].key = NULL;
    }
  }
}

void naive(char *input, hash_table *hash_table) {
  if (input == NULL)
    return;
  char *str = strdup(input);
  if (!str)
    return;

  const char *delimiters = " \n\r\t";
  char *token;
  char *rest = str;
  token_t *naive_table = (token_t *)hash_table->table;

  while ((token = strtok_r(rest, delimiters, &rest)) != NULL) {
    size_t i;
    for (i = 0; i < hash_table->capacity; ++i) {
      if (naive_table[i].key == NULL) {
        naive_table[i].key = strdup(token);
        if (naive_table[i].key == NULL) {
          continue;
        }
        naive_table[i].value = 1;
        hash_table->size++;
        break;
      } else if (strcmp(naive_table[i].key, token) == 0) {
        naive_table[i].value += 1;
        break;
      }
    }
  }
  free(str);
}
