# Link image bounds

Package work is on `agents/link-image-bounds`. New source is under `experimental/sources/link-image-bounds-*`. Host checks sit beside that source and stay out of the EE plan. Function units use profile `authored_cpp`. Absolute image labels use profile `authored_entry_asm`. Addresses were read from the pinned SLUS-20565 symbol table. Fresh EE objects are not original-byte matches. No EE startup stage has run.

`__sce_sema_id` is not aliased to `norrath_player_malloc_semaphore_view`. Callers `_start`, `InitThread`, `__do_global_ctors`, `__do_global_dtors`, `supplement_crt0`, and `engineInit` stay imports.

## __register_frame_info

Observed at `0x002fc428`, 104 bytes. The caller passes the frame pointer and a six-word object. This body stores the frame pointer at word 2, clears words 0, 1, 3, and 4, then waits on `__sce_eh_sema_id`. After the wait it stores the previous `objects` head at word 5 and makes this object the head, then signals the semaphore. The id is loaded again after the wait. The compiled body is 92 bytes. The host contract fails if a word is left poisoned, if the link changes before the wait, or if the signal sees the old id or the old head.

`objects` is the local BSS word at `0x008eab18`. This unit defines it as a global four-byte pointer so the deregistrar can share it. The original binding is local. Its initial value is zero.

## atexit

Observed at `0x002ff8d8`, 160 bytes. It loads the object addressed by `_impure_ptr`. The word at offset 328 selects a 136-byte block; a zero word selects the block at offset 332 and stores that address. The block is a next address, a signed count, and 32 function addresses. Count 32 or more calls `__wrap_malloc(136)`. A null allocation returns -1 and leaves the previous block unchanged. Otherwise the new block's next word is the previous block, the function is stored at the current count, and the count increases by one. Success returns 0. The compiled body is 372 bytes. The host contract fails if the 32nd registration allocates, if the 33rd does not allocate 136 bytes, if the new block is not linked in front, or if allocation failure still returns 0.

## __deregister_frame_info

Observed at `0x002fc558`, 188 bytes. It waits on `__sce_eh_sema_id` and walks `objects`. A match is the node whose word 2 is the caller's frame pointer. That node is unlinked. Word 3 is passed to `__wrap_free` only when it is nonzero and is not the same pointer. The semaphore is signaled and the node is returned. No match signals the semaphore and calls `abort`. The compiled body is 160 bytes. The host contract fails if the middle node stays linked, if a null or identical word 3 is freed, if the signal uses the id from before the wait, or if a missing node skips `abort`.

## Imports closed

`__register_frame_info`, `atexit`, `__deregister_frame_info`.

## Imports opened

`__wrap_free`, `abort`. `WaitSema` and `__wrap_malloc` are already defined. `SignalSema`, `__sce_eh_sema_id`, and `_impure_ptr` were already unresolved. `objects` is defined with the registrar.

## Build-plan entries needed

Append these authored inputs. Do not add the host contracts. Assign `input_order` at integration. The registrar's header closure is `sources/link-image-bounds-frame/frame_object.hpp`. `atexit.cpp` has an empty header closure.

## What is authored but never executed

`__register_frame_info`, `atexit`, and `__deregister_frame_info` have not run on the EE. No constructor walk has called them, and nothing has invoked a registered function or freed a frame node.

## What is unknown

What `WaitSema` and `SignalSema` do with the semaphore id. The meanings of cleared words 0, 1, and 4 beyond the stores. Whether a final link should keep `objects` local. The bytes of the reentrancy object outside offsets 328 and 332. Negative block counts are outside this contract. What `abort` does after the missing-node signal. The other ten package symbols are not defined yet.
