#define MUTIL_IMPLEMENTATION
#define MUTIL_STRIP_PREFIX

#define MUTILDEF static inline
#include "mutil.h"

#include <stdint.h>
#include <stdio.h>

typedef struct {
  int *items;
  size_t capacity;
  size_t count;
} IntegerDynamicArray;

int main(void) {

  printf("Basic Array Testing:\n");
  int a[10];
  a[0] = 0;
  a[1] = 1;
  printf("a len:%lu\n", ARRAY_LEN(a));
  printf("a[1]=%d\n", ARRAY_GET(a, 1));
  printf("---------------------\n");

  printf("Dynamic Array Testing:\n");
  IntegerDynamicArray ida = {0};
  da_append(&ida, 1);
  da_append(&ida, 2);
  da_append(&ida, 3);

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

  printf("Pop last element: %d\n", mutil_da_pop(&ida));
  printf("[");
  mutil_da_for_each(&ida, num) { printf("%d,", *num); }
  printf("]\n");

  printf("First element: %d\n", mutil_da_first(&ida));
  printf("Last element: %d\n", mutil_da_last(&ida));

  mutil_da_free(ida);

  uintptr_t ptr1 = 32;
  printf("Test: Is %lu power of 2? - %s\n", ptr1,
         is_power_of_two(ptr1) ? "true" : "false");

  return 0;
}
