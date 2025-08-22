#include "includes/hash_table.c"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

int comp(const void *elem1, const void *elem2) {
  int f = ((token_t *)elem1)->value;
  int s = ((token_t *)elem2)->value;
  return s - f;
}

void print_table(size_t top, token_t *table, size_t table_size) {
  qsort(table, table_size, sizeof(token_t), comp);
  printf("Top %zu entries:\n", top);
  for (size_t i = 0; i < top; ++i) {
    if (table[i].key != NULL) {
      printf("Idx: %zu, Key: %4s, hash_key: %8zu => %10zu\n", i + 1,
             table[i].key, table[i].hash_key, table[i].value);
    }
  }
}

void test_hash(char *content, hash_table *hash_table) {
  clock_t start = clock();
  token_analysis_t *collisions =
      (token_analysis_t *)tokenize(content, hash_table);

  print_table(10, (token_t *)hash_table->table, hash_table->capacity);
  clock_t end = clock();
  printf("\nCollisions: %zu\nTotal tokens parsed: %zu, Unique tokens: %zu",
         collisions->collisions_count, collisions->global_token_count,
         collisions->unique_tokens);
  printf("\nTime elapsed: %f", (float)(end - start) / CLOCKS_PER_SEC);
  free(collisions);
}

void test_naive(char *content, hash_table *naive_table) {
  clock_t start = clock();
  naive(content, naive_table);
  print_table(10, (token_t *)naive_table->table, naive_table->capacity);
  clock_t end = clock();
  printf("\nTime elapsed: %f", (float)(end - start) / CLOCKS_PER_SEC);
}

int main(int argc, char **argv) {
  if (argc < 2)
    return 1;
  char *content = read_file(argv[1], "r");
  if (!content)
    return 1;
  void *table = malloc(TABLE_SIZE * sizeof(token_t));
  void *naive_table = malloc(TABLE_SIZE * sizeof(token_t));
  hash_table hash_table_impl = {
      .table = table,
      .capacity = TABLE_SIZE,
      .size = 0,
  };

  // hash_table naive_hash_table = {
  //     .table = naive_table,
  //     .capacity = TABLE_SIZE,
  //     .size = 0,
  // };

  init_table(&hash_table_impl);
  // init_table(&naive_hash_table);

  // test_naive(content, &naive_hash_table);
  test_hash(content, &hash_table_impl);
  free(content);
  free_table((token_t *)hash_table_impl.table, hash_table_impl.capacity);
  // free_table((token_t *)naive_hash_table.table, naive_hash_table.capacity);
  free(hash_table_impl.table);
  // free(naive_hash_table.table);
}
