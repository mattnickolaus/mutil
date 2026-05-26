#include "mutil.h"
#include <stdio.h>

typedef struct {
  int *items;
  size_t capacity;
  size_t count;
} IntDA;

int main(void) {

  printf("Basic Array Testing:\n");
  int a[10];
  a[0] = 0;
  a[1] = 1;
  printf("a len:%lu\n", MUTIL_ARRAY_LEN(a));
  printf("a[1]=%d\n", MUTIL_ARRAY_GET(a, 1));
  printf("---------------------\n");

  printf("Dynamic Array Testing:\n");
  IntDA ida = {0};
  mutil_da_append(&ida, 1);
  mutil_da_append(&ida, 2);
  mutil_da_append(&ida, 3);

  size_t new_items_count = 3;
  int new_items[3] = {4, 5, 6};
  mutil_da_append_many(&ida, new_items, new_items_count);

  printf("Integer dyamic array with append and append_many\n");
  printf("[");
  mutil_da_for_each(&ida, num) { printf("%d,", *num); }
  printf("]\n");

  printf("Dynamic Array Reversed\n");
  mutil_da_reverse(&ida);
  printf("[");
  mutil_da_for_each(&ida, num) { printf("%d,", *num); }
  printf("]\n");

  mutil_da_free(ida);

  return 0;
}
