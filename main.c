#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

#define TABLE_SIZE 100000

token_t table[TABLE_SIZE];
token_t naive_table[TABLE_SIZE];

void init_table(token_t *table) {
  for (size_t i = 0; i < TABLE_SIZE; ++i) {
    table[i].key = NULL;
    table[i].value = 0;
  }
}

size_t hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }

  return hash % TABLE_SIZE;
}

char *read_file(char *path, char *mode) {
  FILE *fptr;
  fptr = fopen(path, mode);
  if (!fptr)
    return NULL;

  fseek(fptr, 0, SEEK_END);
  long file_size = ftell(fptr);
  fseek(fptr, 0, SEEK_SET);

  char *content = malloc(file_size + 1);
  if (!content) {
    fclose(fptr);
    return NULL;
  }
  size_t bytes_read = fread(content, 1, file_size, fptr);
  if (bytes_read != (size_t)file_size) {
    fclose(fptr);
    free(content);
    return NULL;
  }
  content[file_size] = '\0';
  fclose(fptr);
  return content;
}

void naive(char *input) {
  if (input == NULL)
    return;
  char *str = strdup(input);
  const char *delimiters = " \n\r\t";
  char *token;
  char *rest = str;
  if (!str)
    return;
  while ((token = strtok_r(rest, delimiters, &rest)) != NULL) {
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
      if (naive_table[i].key == NULL ||
          strcmp(naive_table[i].key, token) == 0) {
        naive_table[i].key = strdup(token);
        naive_table[i].value += 1;
        break;
      }
    }
  }
  free(str);
}

void *tokenize(char *input) {
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
    size_t key = hash(token);

    if (table[key].key == NULL) {
      table[key].key = strdup(token);
      table[key].value = 1;
      table[key].hash_key = key;
      unique_tokens++;
    } else if (table[key].key != NULL && strcmp(table[key].key, token) == 0) {
      table[key].value += 1;
    } else {
      key = (key + 1) % TABLE_SIZE;
      table[key].key = strdup(token);
      table[key].value = 1;
      table[key].hash_key = key;
      collisions_count++;
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

int comp(const void *elem1, const void *elem2) {
  int f = ((token_t *)elem1)->value;
  int s = ((token_t *)elem2)->value;
  return s - f;
}

void print_table(size_t top, token_t *table) {
  qsort(table, TABLE_SIZE, sizeof(token_t), comp);
  printf("Top %zu entries:\n", top);
  for (size_t i = 0; i < top; ++i) {
    if (table[i].key != NULL) {
      printf("Idx: %zu, Key: %4s, hash_key: %8zu => %10zu\n", i + 1,
             table[i].key, table[i].hash_key, table[i].value);
    }
  }
}

void free_table() {
  for (size_t i = 0; i < TABLE_SIZE; i++) {
    if (table[i].key != NULL) {
      free(table[i].key);
      table[i].key = NULL;
    }
  }
}

void test_hash(char *content) {
  clock_t start = clock();
  token_analysis_t *collisions = (token_analysis_t *)tokenize(content);

  print_table(10, table);
  clock_t end = clock();
  printf("\nCollisions: %zu\nTotal tokens parsed: %zu, Unique tokens: %zu",
         collisions->collisions_count, collisions->global_token_count,
         collisions->unique_tokens);
  free(collisions);
  printf("\nTime elapsed: %f", (float)(end - start) / CLOCKS_PER_SEC);
}

void test_naive(char *content) {
  clock_t start = clock();
  naive(content);
  print_table(10, naive_table);
  clock_t end = clock();
  printf("\nTime elapsed: %f", (float)(end - start) / CLOCKS_PER_SEC);
}

int main(int argc, char **argv) {
  char *content = read_file(argv[1], "r");
  if (!content)
    return 1;
  if (argc < 2)
    return 1;
  init_table(table);
  init_table(naive_table);

  test_naive(content);
  test_hash(content);
  free(content);
  free_table();
}
