#ifndef MUTIL_H_
#define MUTIL_H_

#ifndef MUTILDEF
/*
   Goes before declarations and definitions of the nob functions. Useful to
   `#define MUTILDEF static inline` if your source code is a single file and you
   want the compiler to remove unused functions.
*/
#define MUTILDEF
#endif /* MUTILDEF */

#ifndef MUTIL_ASSERT
#include <assert.h>
#define MUTIL_ASSERT assert
#endif /* MUTIL_ASSERT */

#ifndef MUTIL_REALLOC
#include <stdlib.h>
#define MUTIL_REALLOC realloc
#endif /* MUTIL_REALLOC */

#ifndef MUTIL_FREE
#include <stdlib.h>
#define MUTIL_FREE free
#endif /* MUTIL_FREE */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MUTIL_ARRAY_LEN(array) (sizeof(array) / sizeof(array[0]))
#define MUTIL_ARRAY_GET(array, index)                                          \
  (MUTIL_ASSERT((size_t)index < MUTIL_ARRAY_LEN(array)), array[(size_t)index])

#ifndef MUTIL_DA_INIT_CAP
#define MUTIL_DA_INIT_CAP 32
#endif // MUTIL_DA_INIT_CAP

#define mutil_da_reserve(da, expected_capacity)                                \
  do {                                                                         \
    if ((expected_capacity) > (da)->capacity) {                                \
      if ((da)->capacity == 0) {                                               \
        (da)->capacity = MUTIL_DA_INIT_CAP;                                    \
      }                                                                        \
      while ((expected_capacity) > (da)->capacity) {                           \
        (da)->capacity *= 2;                                                   \
      }                                                                        \
      (da)->items =                                                            \
          MUTIL_REALLOC((da)->items, (da)->capacity * sizeof(*(da)->items));   \
      assert((da)->items != NULL && "Out of memory lol");                      \
    }                                                                          \
  } while (0)

#define mutil_da_append(da, item)                                              \
  do {                                                                         \
    mutil_da_reserve((da), (da)->count + 1);                                   \
    (da)->items[(da)->count] = (item);                                         \
    (da)->count++;                                                             \
  } while (0)

#define mutil_da_free(da) MUTIL_FREE((da).items)

#define mutil_da_append_many(da, new_items, new_items_count)                   \
  do {                                                                         \
    mutil_da_reserve((da), (da)->count + (new_items_count));                   \
    memcpy((da)->items + (da)->count, (new_items),                             \
           (new_items_count) * sizeof(*(da)->items));                          \
    (da)->count += (new_items_count);                                          \
  } while (0)

#define mutil_da_reverse(da)                                                   \
  do {                                                                         \
    for (int i = 0; i < (da)->count / 2; i++) {                                \
      typeof((da)->items[i]) temp = (da)->items[i];                            \
      (da)->items[i] = (da)->items[(da)->count - 1 - i];                       \
      (da)->items[(da)->count - 1 - i] = temp;                                 \
    }                                                                          \
  } while (0)

// Side note: The comma syntax here just takes the last item and passes that as
// the index of the array, but still allows me to use a assert chcek in one line
#define mutil_da_pop(da)                                                       \
  (da)->items[(MUTIL_ASSERT((da)->count > 0), --(da)->count)]

#define mutil_da_first(da) (da)->items[(MUTIL_ASSERT((da)->count > 0), 0]
#define mutil_da_last(da) (da)->items[(MUTIL_ASSERT((da)->count > 0, (da)->count-1]

#define mutil_da_pop_unordered(da, i)                                          \
  do {                                                                         \
    MUTIL_ASSERT(i < (da)->count);                                             \
    for (; i < (da)->count - 1; i++) {                                         \
      (da)->items[i] = (da)->items[i + 1];                                     \
    }                                                                          \
    (da)->items[i] = (typeof((da)->items[0])){0};                              \
    (da)->count--;                                                             \
  } while (0)

#define mutil_da_for_each(da, it)                                              \
  for (typeof((da)->items[0]) *it = (da)->items;                               \
       it < (da)->items + (da)->count; it++)

#endif // MUTIL_H
