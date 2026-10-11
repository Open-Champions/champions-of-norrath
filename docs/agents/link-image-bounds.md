# Link image bounds

Package work is on `agents/link-image-bounds`. New source is under `experimental/sources/link-image-bounds-*`. Host checks sit beside that source and stay out of the EE plan. Function units use profile `authored_cpp`. Absolute image labels use profile `authored_entry_asm`. Addresses were read from the pinned SLUS-20565 symbol table. Fresh EE objects are not original-byte matches. No EE startup stage has run.

`__sce_sema_id` is not aliased to `norrath_player_malloc_semaphore_view`. Callers `_start`, `InitThread`, `__do_global_ctors`, `__do_global_dtors`, `supplement_crt0`, and `engineInit` stay imports.

## __register_frame_info

Observed at `0x002fc428`, 104 bytes. The caller passes the frame pointer and a six-word object. This body stores the frame pointer at word 2, clears words 0, 1, 3, and 4, then waits on `__sce_eh_sema_id`. After the wait it stores the previous `objects` head at word 5 and makes this object the head, then signals the semaphore. The id is loaded again after the wait. The compiled body is 92 bytes. The host contract fails if a word is left poisoned, if the link changes before the wait, or if the signal sees the old id or the old head.

`objects` is the local BSS word at `0x008eab18`. This unit defines it as a global four-byte pointer so the deregistrar can share it. The original binding is local. Its initial value is zero.

## Imports closed

`__register_frame_info`.

## Imports opened

None yet. `WaitSema` is already defined. `SignalSema` and `__sce_eh_sema_id` were already unresolved. `objects` is defined here.

## Build-plan entries needed

Append these authored inputs. Do not add `register_host.cpp`. Assign `input_order` at integration. Header closure is the frame object header.

## What is authored but never executed

`__register_frame_info` has not run on the EE. No constructor walk has called it.

## What is unknown

What `WaitSema` and `SignalSema` do with the semaphore id. The meanings of cleared words 0, 1, and 4 beyond the stores. Whether a final link should keep `objects` local. The other twelve package symbols are not defined yet.
