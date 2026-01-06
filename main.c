#include "allocator.h"

int main() {
    printf("--- TEST 1: Allocation and Splitting ---\n");
    void *a = my_alloc(100);
    void *b = my_alloc(100);
    void *c = my_alloc(100);
    print_list();

    printf("--- TEST 2: Reusing and Coalescing Right ---\n");
    my_free(b);
    my_free(c);
    print_list();

    printf("--- TEST 3: Splitting a Reclaimed Block ---\n");
    void *d = my_alloc(50); //This should split the large free block we just made
    print_list();

    printf("--- TEST 4: Full Coalesce ---\n");
    my_free(a);
    my_free(d);
    //Everything is now free, coalesce should merge them into one big block
    print_list();

    return 0;
}