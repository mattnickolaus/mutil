#ifndef MUTIL_H_
#define MUTIL_H_

#ifndef MUTILDEF
/*
   Goes before declarations and definitions of the mutil functions. Useful to
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

#include <stdbool.h>
#include <stdint.h>
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

// Side note: The comma syntax here just takes the last item after the comma and
// passes that as the index of the array, but still allows me to use a assert
// chcek so that everything is still in one line
#define mutil_da_pop(da)                                                       \
  (da)->items[(MUTIL_ASSERT((da)->count > 0), --(da)->count)]

#define mutil_da_first(da) (da)->items[(MUTIL_ASSERT((da)->count > 0)), 0]
#define mutil_da_last(da)                                                      \
  (da)->items[(MUTIL_ASSERT((da)->count > 0)), (da)->count - 1]

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

#endif // MUTIL_H_

#ifdef MUTIL_IMPLEMENTATION
#undef MUTIL_IMPLEMENTATION // don't implement more than once

MUTILDEF bool mutil_is_power_of_two(uintptr_t x) { return (x & (x - 1)) == 0; }

MUTILDEF uintptr_t mutil_align_forward(uintptr_t ptr, size_t align) {
  uintptr_t p, a, modulo;

  assert(mutil_is_power_of_two(align));

  p = ptr;
  a = (uintptr_t)align;
  // Same as (p % a) but faster as 'a' is a power of two
  modulo = p & (a - 1);

  if (modulo != 0) {
    // If 'p' address is not aligned, push the address to the
    // next value which is aligned
    p += a - modulo;
  }
  return p;
}

#endif // MUTIL_IMPLEMENTATION

#ifndef MUTIL_STRIP_PREFIX_GUARD_
#define MUTIL_STRIP_PREFIX_GUARD_
// NOTE: The name stripping should be part of the header so it's not
// accidentally included several times. At the same time, it should be at the
// end of the file so to not create any potential conflicts in the
// MUTIL_IMPLEMENTATION. The header obviously cannot be at the end of the file
// because MUTIL_IMPLEMENTATION needs the forward declarations from there. So
// the solution is to split the header into two parts where the name stripping
// part is at the end of the file after the MUTIL_IMPLEMENTATION.
//
// Regardless simply `#define MUTIL_STRIP_PREFIX` below `#define
// MUTIL_IMPLEMENTATION` to to remove the prefixes from your function names
//
// Also it is important to note that if one of the binding names below conflict
// with another imported library I can simply `#undef da_append` for example to
// unbind and remove that namespace conflict
#ifndef MUTIL_UNSTRIP_PREFIX

#define ARRAY_GET MUTIL_ARRAY_GET
#define ARRAY_LEN MUTIL_ARRAY_LEN
#define da_reserve mutil_da_reserve
#define da_append mutil_da_append
#define da_free mutil_da_free
#define da_append_many mutil_da_append_many
#define da_for_each mutil_da_for_each
#define da_reverse mutil_da_reverse
#define da_pop mutil_da_pop
#define da_first mutil_da_first
#define da_last mutil_da_last
#define da_pop_unordered mutil_da_pop_unordered

#define is_power_of_two mutil_is_power_of_two
#define align_forward mutil_align_forward

#endif // MUTIL_STRIP_PREFIX

#endif // MUTIL_STRIP_PREFIX_GUARD_
