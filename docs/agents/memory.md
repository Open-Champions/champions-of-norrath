# Heap and allocator state

Package work is on `agents/memory`. New source is under `experimental/sources/memory-*`. Host checks sit beside that source and must stay out of the EE plan: they include host headers. The reviewed EE profile is `authored_cpp` from `experimental/build-plan.json`. A private six-unit plan outside git compiled and linked these units: one function, 556 body bytes, and the three imports named below. `python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests.

## Observed

`memRealloc(void*, int)` is the 240-byte function at `0x00144278`. The request is raised to 64 by a signed compare. The block header is 28 bytes before the pointer. Its size word is compared signed with that clamped request.

- If the old size is smaller, the function calls `memAlloc` with the clamped request and the empty byte at `0x00483b10`, then `memcpy` for the size word reloaded after that call. It returns the allocation pointer and does not free the old block.
- If the unsigned difference, read as signed, is below 129, it returns the same pointer.
- Otherwise it rounds the request up to 8, links a tail at the old header plus that rounded size plus 28, marks the tail allocated (`0xABCD`, flag 1, name 0), and frees the tail payload.

`norrath_mem_realloc` is that function. Its EE body is 556 bytes. The host check covers the 64-byte clamp, slack 128 and 129, the rounded split, growth without a free, the reloaded copy length, and a negative request.

Player malloc storage, with the original symbol, address, size, and file bytes:

| Authored name | Original symbol | Address | Size | Initial bytes |
|---|---|---|---|---|
| `norrath_player_malloc_bins_view` | `__malloc_av_` | `0x00448488` | 1032 | two zero words, then 128 pairs |
| `norrath_player_malloc_trim_threshold_view` | `__malloc_trim_threshold` | `0x00448890` | 8 | `0x00020000` |
| `norrath_player_malloc_top_pad_view` | `__malloc_top_pad` | `0x00448898` | 8 | 0 |
| `norrath_player_malloc_break_base_view` | `__malloc_sbrk_base` | `0x004488a0` | 4 | `0xffffffff` |
| `norrath_player_malloc_max_break_view` | `__malloc_max_sbrked_mem` | `0x004488a8` | 8 | 0 |
| `norrath_player_malloc_max_total_view` | `__malloc_max_total_mem` | `0x004488b0` | 8 | 0 |
| `norrath_player_malloc_current_info_view` | `__malloc_current_mallinfo` | `0x004488b8` | 40 | 0 |
| `norrath_player_malloc_lock_owner_view` | `__malloc_lock_owner` | `0x004488e0` | 4 | `0xffffffff` |
| `norrath_player_malloc_lock_count_view` | `call_count` | `0x004488e4` | 4 | 0 |
| `norrath_player_malloc_semaphore_view` | `__sce_sema_id` | `0x00444a30` | 4 | 0 |

Each bin pair stores the address eight bytes before that pair. The bin object check reads the EE relocation image: 256 `R_MIPS_32` entries, addends `0,0,8,8,...`, and no generated function.

Game heap words, using GP `0x004879f0`:

| Authored name | Original symbol | Address | Size | Initial value |
|---|---|---|---|---|
| `norrath_heap_ring_word_view` | `startNode` | `0x004868b4` | 4 | 0, `.sbss` |
| `norrath_heap_next_fit_view` | `movingNode` | `0x004868b0` | 4 | 0, `.sbss` |
| `norrath_heap_fragment_threshold_view` | `rawMin` | `0x004868ac` | 4 | 0, `.sbss` |
| `norrath_heap_free_bytes_view` | `MEM_FreeHeap` | `0x00483b28` | 4 | 0 |
| `norrath_heap_allocation_tag_view` | `mark` | `0x00483b08` | 4 | 0 |
| `norrath_heap_total_bytes_view` | `MEM_HeapCapacity` | `0x00483b2c` | 4 | 0 |
| `norrath_heap_dump_count_view` | `MEM_DumpCount` | `0x00483b30` | 4 | 0 |
| `norrath_heap_check_file_view` | none | `0x00483b18` | 7 | `mem.cc` |
| `norrath_enable_malloc_wrapper_byte_view` | `enableMallocWrapper` | `0x00483b11` | 1 | 0 |

Dump and failure text is unsymbolized rodata. The host check compares these bytes, including the terminating zero:

- `0x00461eb8` `memAlloc failed!  (ra=%x, size=%d)\n`
- `0x00461ee0` thirty-nine dashes and a newline
- `0x00461f10` `     Start Heap Dump (%d)\n`
- `0x00461f30` `Free at %x, %d bytes.\n`
- `0x00461f48` `Used at %x '%s' (ra=%x), %d bytes.\n`
- `0x00461f70` `Total=%d, Used=%d, Free=%d\n`
- `0x00461f90` `      End Heap Dump (%d) \n`

Block storage:

- `norrath_block_data_pool_view` is `dataBlock` at `0x00519440`. `blockAllocInit` clears `0x000e7700` bytes, and `trailParticleFire` begins at `0x00600b40`, the same distance. The pool is 947968 zero BSS bytes.
- `norrath_block_region_view` is `region` at `0x00333d58`, 64 file bytes. Rows are capacity, count, then two words. The capacities are 384, 1280, and 3000, with counts 512, 512, and 32. The fourth capacity is `0x7fffffff` and its count is 0.

## Inferred

The empty growth name at `0x00483b10` has no symbol. It is the zero byte immediately before `enableMallocWrapper`. The function passes its own one-byte zero name. Callers that print the name still see an empty string.

`__sce_sema_id` and `norrath_player_malloc_semaphore_view` are one original word. Startup writes the original name. This package defines only the view, so the two names are not yet one object.

`call_count` is the word `__malloc_lock` and `__malloc_unlock` increment. The authored name is the lock-count view.

The reviewed `-G0` profile does not recreate `.sdata` or file-backed zero sections. Zero words may be BSS. The stored values match.

The official plan on this worktree, rebuilt outside git, still lists 911 unresolved names. The memory names in that list are the closed set below, plus `norrath_player_heap_pointer_word`, which stays unresolved. The sound IOP heap views belong to the audio package.

## Original names used by main and launchSRS

`enableMallocWrapper` is the one-byte object at `0x00483b11`. `memAlloc__FiPc` is the 492-byte function at `0x00144368`; its heap check uses line 502. `memFree__FPv` is the 232-byte function at `0x00144558`; its heap check uses line 583. Those three are the objects already authored as `norrath_enable_malloc_wrapper_byte_view`, `norrath_mem_alloc`, and `norrath_mem_free`. Each original name is a second global symbol on that same extent. The compiled bodies stay 1, 1248, and 1288 bytes. A host program reads the EE object and fails if the two names differ in section, value, or size. The flag's host byte check defines `NORRATH_WRAPPER_BYTE_HOST_CHECK` because this machine's host compiler rejects alias attributes.

`mallinfo` at `0x00302340` is 48 bytes and is not the 40-byte `__malloc_current_mallinfo` object. It loads `_impure_ptr` at `0x00448480` and calls `_mallinfo_r`, then returns the caller's struct pointer. `memSetMark__Fi` at `0x00144830` is 24 bytes and sets one bit of the mark word. `memClearMark__Fi` at `0x00144848` is 28 bytes and clears that bit. The shift amount is the argument's low five bits. `memDeleteMarked__Fi` at `0x00144868` is 124 bytes. It frees one matching allocated block, then starts again at the ring word. `memScrambleFree__Fv` at `0x00144ba8` is 80 bytes. It fills a free payload with `0xA5` for the block's size word, then reloads the ring word as the end of the walk.

`engineInit` also uses five objects from this package. `blockAllocInit__Fv` is the 220-byte function at `0x001440d8`. It is the existing `norrath_block_alloc_init` body, so the original name is a second symbol on that 320-byte compiled extent. `__wrap_malloc` at `0x001449a8` is 52 bytes and was not already authored. A zero `enableMallocWrapper` byte calls `malloc` with the same size. Any other byte calls `memAlloc__FiPc` with that size and the unsymbolized text `fake malloc` at `0x00461ea8`. The returned pointer is the callee's pointer. `memInit__FPvi` at `0x001448e8` is 108 bytes. A positive size fills that many bytes with `0xA5`. The block then becomes one free header, the ring and next-fit words point at it, the free and size words are the size minus 28, the capacity word is the size, the fragment threshold becomes 36, and the dump count becomes 0. `memAllocPtr` at `0x00486a54` and `memAllocSize` at `0x00486a58` are adjacent four-byte BSS words. Each authored object is four zero bytes. This relocatable build does not place them beside each other.

## Imports closed

`norrath_mem_realloc`, `norrath_player_malloc_bins_view`, `norrath_player_malloc_trim_threshold_view`, `norrath_player_malloc_top_pad_view`, `norrath_player_malloc_break_base_view`, `norrath_player_malloc_max_break_view`, `norrath_player_malloc_max_total_view`, `norrath_player_malloc_current_info_view`, `norrath_player_malloc_lock_owner_view`, `norrath_player_malloc_lock_count_view`, `norrath_player_malloc_semaphore_view`, `norrath_heap_ring_word_view`, `norrath_heap_next_fit_view`, `norrath_heap_fragment_threshold_view`, `norrath_heap_free_bytes_view`, `norrath_heap_allocation_tag_view`, `norrath_heap_total_bytes_view`, `norrath_heap_dump_count_view`, `norrath_heap_check_file_view`, `norrath_heap_failure_format_view`, `norrath_heap_dump_border_view`, `norrath_heap_dump_count_format_view`, `norrath_heap_dump_free_format_view`, `norrath_heap_dump_used_format_view`, `norrath_heap_dump_totals_format_view`, `norrath_heap_dump_serial_format_view`, `norrath_block_data_pool_view`, `norrath_block_region_view`, `norrath_enable_malloc_wrapper_byte_view`, `enableMallocWrapper`, `memAlloc__FiPc`, `memFree__FPv`, `memSetMark__Fi`, `memClearMark__Fi`, `memDeleteMarked__Fi`, `memScrambleFree__Fv`, `mallinfo`, `__wrap_malloc`, `memInit__FPvi`, `blockAllocInit__Fv`, `memAllocPtr`, `memAllocSize`.

## Imports opened

`memcpy`, `_impure_ptr`, `_mallinfo_r`, and `malloc`. `memcpy` comes from `norrath_mem_realloc`. `mallinfo` calls `_impure_ptr` and `_mallinfo_r`. `__wrap_malloc` calls `malloc` when the enable byte is zero, and calls the existing `memAlloc__FiPc` otherwise. `norrath_mem_alloc`, `norrath_mem_free`, `memset`, and `norrath_block_memory_set` were already defined. `norrath_factory_impure_pointer_word` is the same pinned address as `_impure_ptr` and stays a separate unresolved name.

## Build-plan entries needed

Append these authored inputs. Do not add the `host_check.cpp` or `check_bins_object.cpp` files. Assign `input_order` at integration. Profile `authored_cpp`. Header closure is empty. `emitted_functions` is empty except for realloc.

```json
[
  {
    "source": "sources/memory-realloc/mem_realloc.cpp",
    "object_name": "mem_realloc.o",
    "sha256": "3483ade3dc606a8eaadfd44ba4d22414914fdce8530e2776f83603d185023dfa",
    "emitted_functions": [{"name": "norrath_mem_realloc", "compiled_body_bytes": 556}]
  },
  {
    "source": "sources/memory-malloc-state/malloc_bins.cpp",
    "object_name": "malloc_bins.o",
    "sha256": "9f6197bb65523c6b35075848f80df3cd0957fa309470177fd2a9de5028a36925",
    "emitted_functions": []
  },
  {
    "source": "sources/memory-malloc-state/malloc_scalars.cpp",
    "object_name": "malloc_scalars.o",
    "sha256": "070b07958030ea0fea6672e99709b3c9df150642c8cdc2bd80d07647047b6cc5",
    "emitted_functions": []
  },
  {
    "source": "sources/memory-heap-state/heap_state.cpp",
    "object_name": "heap_state.o",
    "sha256": "1dfe7bf1dec13e6b6ba0b3b01804cc594430898889e450e69db34ec9625974ea",
    "emitted_functions": []
  },
  {
    "source": "sources/memory-block-storage/block_storage.cpp",
    "object_name": "block_storage.o",
    "sha256": "c2b0b8c97d88a62202e0795253b71ab843352abdeb8c05a8796e5290624eec5c",
    "emitted_functions": []
  },
  {
    "source": "sources/memory-wrapper-byte/wrapper_byte.cpp",
    "object_name": "wrapper_byte.o",
    "sha256": "b68e09b9f3b5d9234d3cd529e31fa711d86a6b6981e910a088b58078a1ce15e9",
    "emitted_functions": []
  },
  {
    "source": "sources/memory-set-mark/mem_set_mark.cpp",
    "object_name": "mem_set_mark.o",
    "sha256": "9a61ab6480fe75b82a6638db87d22aeeb7fc41995bf4ba3c937aedd2253b1ab8",
    "emitted_functions": [{"name": "memSetMark__Fi", "compiled_body_bytes": 60}]
  },
  {
    "source": "sources/memory-clear-mark/mem_clear_mark.cpp",
    "object_name": "mem_clear_mark.o",
    "sha256": "420d79a22da4a74a03626e5375c0295ad607efc307ff844f7aa8199ddfa48e6f",
    "emitted_functions": [{"name": "memClearMark__Fi", "compiled_body_bytes": 64}]
  },
  {
    "source": "sources/memory-delete-marked/mem_delete_marked.cpp",
    "object_name": "mem_delete_marked.o",
    "sha256": "0b55c13d0bfb9e2b59a6d3fd5923ecec75c2788106785e156a7ffdf724e50516",
    "emitted_functions": [{"name": "memDeleteMarked__Fi", "compiled_body_bytes": 160}]
  },
  {
    "source": "sources/memory-scramble-free/mem_scramble_free.cpp",
    "object_name": "mem_scramble_free.o",
    "sha256": "4ec81437a4173a9f26a7aa19a67e402efff1ee6239bfb37c7c056c8225522974",
    "emitted_functions": [{"name": "memScrambleFree__Fv", "compiled_body_bytes": 156}]
  },
  {
    "source": "sources/memory-mallinfo/mallinfo.cpp",
    "object_name": "mallinfo.o",
    "sha256": "7419c00356c3d742ac23769d2c7053194d4a3929570cda0273e14bcf7153718a",
    "emitted_functions": [{"name": "mallinfo", "compiled_body_bytes": 56}]
  }
]
```

Update these existing entries in place. Each gains one emitted function of the same size as the existing body. Do not add `check_same_extent.cpp` or any `host_check.cpp`.

```json
[
  {
    "source": "sources/game-heap-allocator-build/mem_alloc.cpp",
    "sha256": "3c74fa0480a0e22d85ab425ed2ee9125f415e9110a83847c2e0835a3174b2bfa",
    "add_emitted_function": {"name": "memAlloc__FiPc", "compiled_body_bytes": 1248}
  },
  {
    "source": "sources/game-sound-memory-lifecycle-build/mem_free.cpp",
    "sha256": "9e5771dc0780504f3ab59fe3e24700926eeb1ce12a46136bf6d85af1767d3309",
    "add_emitted_function": {"name": "memFree__FPv", "compiled_body_bytes": 1288}
  },
  {
    "source": "sources/block-alloc-source-v2/initialization.cpp",
    "sha256": "9686cb9f7a9b3030772d5a642f7d7ceed7bd6a21c82e1f6c8343ec632612d16b",
    "add_emitted_function": {"name": "blockAllocInit__Fv", "compiled_body_bytes": 320}
  }
]
```

Append these inputs. Header closure is empty. Do not add their host checks.

```json
[
  {
    "source": "sources/memory-wrap-malloc/wrap_malloc.cpp",
    "object_name": "wrap_malloc.o",
    "sha256": "03138602207ff4e1abc59d3ff9e3fbe68293c74b08ef46c783dc36de241c1af1",
    "emitted_functions": [{"name": "__wrap_malloc", "compiled_body_bytes": 40}]
  },
  {
    "source": "sources/memory-mem-init/mem_init.cpp",
    "object_name": "mem_init.o",
    "sha256": "934c88a2097bfd51cbe3807395d530b2b26e567234f01182bcd158ae4dcaeaa7",
    "emitted_functions": [{"name": "memInit__FPvi", "compiled_body_bytes": 392}]
  },
  {
    "source": "sources/memory-alloc-pointer/alloc_pointer.cpp",
    "object_name": "alloc_pointer.o",
    "sha256": "286e14926ec596ed5429d3cef1a658034e9f1651816c5ffd688eb885d46c93d6",
    "emitted_functions": []
  },
  {
    "source": "sources/memory-alloc-size/alloc_size.cpp",
    "object_name": "alloc_size.o",
    "sha256": "b7f86aeb6130ff51d194de3effaa5e961feb3bd87bee47609996b0a5e11b8646",
    "emitted_functions": []
  }
]
```

Host checks, from the worktree:

```sh
clang++ -std=c++17 -O2 -fsanitize=address,undefined -DNORRATH_REALLOC_HOST_CHECK -Dmemcpy=norrath_realloc_test_copy -c experimental/sources/memory-realloc/mem_realloc.cpp -o /tmp/mem_realloc_host.o
clang++ -std=c++17 -O2 -fsanitize=address,undefined -c experimental/sources/memory-realloc/host_check.cpp -o /tmp/mem_realloc_check.o
clang++ -fsanitize=address,undefined /tmp/mem_realloc_host.o /tmp/mem_realloc_check.o -o /tmp/mem_realloc_check && /tmp/mem_realloc_check
```

Compile `malloc_bins.cpp` with the EE profile, then run `check_bins_object` on that object. Compile `check_same_extent` on the host and pass it the EE objects for the two allocator names and the flag byte. Compile each other data file with host `clang++` and link it to its `host_check.cpp`. The flag byte's host compile defines `NORRATH_WRAPPER_BYTE_HOST_CHECK`. The deletion, scramble, and `mallinfo` host compiles define `NORRATH_DELETE_MARKED_HOST_CHECK`, `NORRATH_SCRAMBLE_FREE_HOST_CHECK`, and `NORRATH_MALLINFO_HOST_CHECK`.

## Authored but never executed

The EE objects were compiled and relocatable-linked. They were not executed on the EE, and no game path ran them. The host checks for realloc, the mark words, marked deletion, free scrambling, and `mallinfo` did run. The alias comparison ran on the EE objects. None of that is PS2 execution or an original-byte match. The compiled mark, deletion, scramble, and `mallinfo` bodies are 60, 64, 160, 156, and 56 bytes. `__wrap_malloc` compiled to 40 bytes and `memInit__FPvi` to 392. The two allocation words compiled to four BSS bytes each.

## Unknown

- Whether leaving the old block allocated on growth was deliberate. The 240-byte function does that, with no null check on the new pointer.
- Which of the absolute symbols `end` and `_end`, both at `0x008ec91c`, was the initializer of `heap_ptr.39` at `0x00444954`. That word is `norrath_player_heap_pointer_word`, and its file bytes are `0x008ec91c`. It stays unresolved rather than guessing the symbol.
- The four zero bytes at `0x004488a4`, between the break base and the max-break word. They have no symbol.
- The extra zero at `0x00483b19`, after `mem.cc`, and the `.lmp` text at `0x00483b20`. They are not part of the check-file symbol.
- Making `__sce_sema_id` and the semaphore view the same object. Startup still writes the original name.
- Original section class and GP placement under the reviewed `-G0` profile.
- Runtime of `memInit`, semaphore creation, and any later writer of the enable byte.
- The 240-byte body of `_mallinfo_r` at `0x0030b028`. `mallinfo` only calls it. That callee locks, copies the 40-byte current mallinfo object, and unlocks.
- Whether `norrath_factory_impure_pointer_word` and `_impure_ptr` should become one object. Both names refer to `0x00448480`, and both stay unresolved.
- The body of `malloc` at `0x00301548`, 84 bytes. `__wrap_malloc` calls it when the enable byte is zero.
- Placing `memAllocPtr` and `memAllocSize` in the adjacent BSS slots `0x00486a54` and `0x00486a58`. The authored objects have the observed sizes and zero initial bytes, without that placement.
