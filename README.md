# my_alloc

## Table of Contents
- [Overview](#overview)
- [Features](#allocation-features)
- [Performance characteristics](#performance-characteristics)
- [Project structure](#project-structure)
- [Installation and Usage](#installation-and-usage) 



## Overview
The Custom Memory Allocator is a low-level memory management system that implements malloc and free functionality using the sbrk system call. This solution provides insights into how operating systems manage dynamic memory allocation, featuring block splitting, coalescing, and efficient memory reuse strategies.

---

## Allocation features

| Feature                  | Implementation  | Description                                 |
|--------------------------|-----------------|---------------------------------------------|
| First-Fit Allocation     | DONE            | Searches free list for first suitable block |
| Block splitting          | DONE            | Splits larger blocks to refuce fragmentation|
| Coalescing               | DONE            | Merges adjacent free blocks automatically   |
| Boundary tags            | DONE            | Header metadata for block tracking          |
| Doubly-linked list       | DONE            | Efficient block traversal and management    |
| Heap inspection          | DONE            | Built-in debugging visualization            |

---

## Performance characteristics

| Operation             | Time Complexity | Space Overhead                 | Notes                       |
|-----------------------|-----------------|--------------------------------|-----------------------------|
| Allocation(best case) | O(1)            | sizeof(header) + aligment      |Immediate free block match   |
| Allocation(best case) | O(n)            | sizeof(header) + requested size|Requires heap expansion      |
| Free operation        | O(1)            | 0                              |Constant time with coalescing|
| Coalescing            | O(1)            | Negative(reduces overhead      |Merges adjacent free blocks  |
| Block splitting       | O(1)            | sizeof(header)                 |Creates new metadata block   |
---


## Project Structure
```custom-allocator/
│
├── CMakeLists.txt       # CMake build configuration
|
├── allocator.h          # Public interface and type definitions
│   ├── header_t struct
│   ├── header_wrapper union  
│   ├── my_alloc() declaration
│   ├── my_free() declaration
│   └── print_list() declaration
│
├── allocator.c          # Core implementation
│   ├── global_base pointer
│   ├── MIN_SPLIT_SIZE constant
│   ├── my_alloc() - Allocation logic
│   ├── my_free() - Deallocation with coalescing
│   └── print_list() - Debug visualization
│
├── main.c              # Test suite and demonstrations
│   ├── Test 1: Basic allocation
│   ├── Test 2: Free and coalesce right
│   ├── Test 3: Block splitting  
│   └── Test 4: Complete coalescing
│
└── README.md           # This documentation
```
---



## Installation and Usage

### Building from Source
- Clone the repo\
  `git clone https://github.com/yourusername/custom-allocator.git`\
  `cd custom-allocator`

- run CMake to build files\
  `mkdir build`\
  `cd build`\
  `cmake ..`\

  - then\
  `cmake --build .`\

  - once the build is done\
  `./working`
  


