#include "allocator.h"

#define MIN_SPLIT_SIZE 16

void *global_base = NULL; //this will act as the start of our malloc block list

void *my_alloc(size_t size) {
    if (size <= 0) return NULL;

    header_wrapper* header;

    if (!global_base){ //checks if we are allocating the block for the first time
        void *p = sbrk(size + sizeof(header_wrapper));
        if (p == (void *)-1) return NULL;

        header = (header_wrapper *)p;
        header->header_t.size = size;
        header->header_t.in_use = 1;
        header->header_t.next = NULL;
        header->header_t.prev = NULL;

        global_base = header;//we set the header as the starting block
    }else {

        header_wrapper *current = (header_wrapper *)global_base; //if this is not the first time allocating then, find the last block to attach to
        header_wrapper *last = NULL;


        //Search the list for free space
        while (current != NULL) {
            if (current->header_t.in_use == 0 && current->header_t.size >= size){

                //first we check if the size of the block that is not in use, is it big enough to accommodate size + header(new header) + extra 16 bytes
                if (current->header_t.size > size + sizeof(header_wrapper) + MIN_SPLIT_SIZE) {
                    //We first calculate the address of the NEW header
                    //We move forward by, header + requested user size
                    header_wrapper *new_block = (header_wrapper *)((char* ) current + sizeof(header_wrapper) + size);

                    //now we initialize the new header
                    new_block->header_t.size = current->header_t.size - size - sizeof(header_wrapper);
                    new_block->header_t.in_use = 0; //this half is free
                    new_block->header_t.next = current->header_t.next;
                    new_block->header_t.prev = (header_t*)current;

                    //update the current block to smaller size
                    current->header_t.size = size;
                    current->header_t.next = (header_t *)new_block;

                    //update the 'prev' of the block  that used to be after 'current'
                    if (new_block->header_t.next) {
                        header_wrapper *after_new = (header_wrapper *)new_block->header_t.next;
                        after_new->header_t.prev = (header_t*)new_block;
                    }
                }
                current->header_t.in_use = 1;
                return (void *)(current + 1);
            }
            last = current; //keep track of the last block in case the search fails
            current = (header_wrapper *)current->header_t.next;
        }

        //FALLBACK CASE: Where we don't have enough free space to accommodate we create new space by call to sbrk
        void *p = sbrk(size + sizeof(header_wrapper));
        if (p == (void *)-1) return NULL;

        header = (header_wrapper *)p;
        header->header_t.size = size;
        header->header_t.in_use = 1;
        header->header_t.next = NULL;

        header->header_t.prev = (header_t *)last; //link back
        last->header_t.next = (header_t *)header; //link forward

    }
    // printf("Size of union: %lu\n", sizeof(header_wrapper));
    // printf("Value of size allocated: %lu\n\n", header->header_t.size + sizeof(header_wrapper));

    return (void *)(header + 1);

}


void my_free(void *ptr) {
    if (!ptr) return;

    //move the pointer back to reach the metadata
    header_wrapper *header = (header_wrapper *)ptr - 1;

    //mark the block as available for the search loop in my_alloc
    header->header_t.in_use = 0;

    // MERGING RIGHT --->
    header_wrapper *next_node = (header_wrapper *)header->header_t.next;
    if (next_node != NULL && next_node->header_t.in_use == 0) {
        //adding the neighbor's footprint to our size
        header->header_t.size += sizeof(header_wrapper) + next_node->header_t.size;

        //skip over the neighbor
        header->header_t.next = next_node->header_t.next;

        //fixing the back-link ONLY if there's a block after the one we deleted
        if (header->header_t.next != NULL) {
            header_wrapper *new_next = (header_wrapper *)header->header_t.next;
            new_next->header_t.prev = (header_t *)header;
        }
    }

    // MERGING LEFT <---
    header_wrapper *prev_node = (header_wrapper *)header->header_t.prev;

    // Check if there is a block behind us AND if it is free
    if (prev_node != NULL && prev_node->header_t.in_use == 0) {

        prev_node->header_t.size += sizeof(header_wrapper) + header->header_t.size;

        prev_node->header_t.next = header->header_t.next;

        if (prev_node->header_t.next != NULL) {
            header_wrapper *next_after_us = (header_wrapper *)prev_node->header_t.next;
            next_after_us->header_t.prev = (header_t *)prev_node;
        }

        //IMPORTANT: Since we merged 'header' into 'prev_node',
        //the 'header' address is now technically inside the middle of 'prev_node'.
        //We update our 'header' pointer so the code knows who the "survivor" is.
        header = prev_node;
    }
}


void print_list() {
    header_wrapper *current = (header_wrapper *)global_base;
    int block_count = 0;

    printf("\n--- HEAP INSPECTOR ---\n");
    while (current != NULL) {
        printf("Block [%d] at %p | Size: %zu | In Use: %d\n",
                block_count++, (void*)current, current->header_t.size, current->header_t.in_use);

        current = (header_wrapper *)current->header_t.next;
    }
    printf("----------------------\n\n");
}

