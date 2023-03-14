# malloc_21sch
#### This is a custom memory allocator written in C. Project of 21 School
##### It has 3 types of allocations:
- Tiny for areas with size <= 32 bytes
- Small for areas with 32B < size <= 4096B
- Large for areas with size more than 4096B
