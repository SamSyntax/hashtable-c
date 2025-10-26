#pragma once
#include <stdlib.h>

#define TABLE_SIZE 100000

typedef struct {
  char *key;
  size_t value;
  size_t hash_key;
} token_t;

typedef struct {
  size_t collisions_count;
  size_t global_token_count;
  size_t unique_tokens;
} token_analysis_t;

typedef struct {
  void *table;
  size_t size;
  size_t capacity;
} hash_table;

void init_table(hash_table *hash_table);
size_t hash(char *str, size_t capacity);
void *tokenize(char *input, hash_table *table);
void free_table(token_t *table, size_t capacity);
void naive(char *input, hash_table *hash_table);
