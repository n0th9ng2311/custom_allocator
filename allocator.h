#ifndef WORKING_ALLOCATOR_H
#define WORKING_ALLOCATOR_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>


typedef uint32_t u32;

//header block to store info about the allocated/deallocated memory block
typedef struct __header_t {
    size_t size;
    int in_use;
    struct __header_t *next;
    struct __header_t *prev;
}header_t;

//union to help align the header_t struct
typedef union __header_wrapper{
    header_t header_t;
    long double padding_t;
}header_wrapper;


void *my_alloc(size_t size);

void my_free(void *ptr);

void print_list();

#endif