Overview
======================
Memory fragmentation is when most of your memory is allocated in a large number of non-contiguous blocks, or chunks - leaving a good percentage of your total memory unallocated, but unusable for most typical scenarios. With this tool you can calculate fragmentation level in each memory zone. 

Compile
======================
gcc -o fraglevel fraglevel.c

Run
======================
./fraglevel

Output
======================
```
=== Zone Normal free_pages 3801
page_size:        1 frag_level: 0
page_size:        2 frag_level: 34
page_size:        4 frag_level: 54
page_size:        8 frag_level: 69
page_size:       16 frag_level: 73
page_size:       32 frag_level: 73
page_size:       64 frag_level: 74
page_size:      128 frag_level: 76
page_size:      256 frag_level: 79
page_size:      512 frag_level: 86
page_size:     1024 frag_level: 100
=== Zone HighMem free_pages 70
page_size:        1 frag_level: 0
page_size:        2 frag_level: 97
page_size:        4 frag_level: 100
page_size:        8 frag_level: 100
page_size:       16 frag_level: 100
page_size:       32 frag_level: 100
page_size:       64 frag_level: 100
page_size:      128 frag_level: 100
page_size:      256 frag_level: 100
page_size:      512 frag_level: 100
page_size:     1024 frag_level: 100
```

* `free_pages - it's just free memory pages in every memory zone`
* `page_size - page size (block)`
* `frag_level - fragmentation level measured in percents.`
