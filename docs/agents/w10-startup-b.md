# C runtime, libgcc, libm, C++ RTTI and startup data, part 2 of 4 (w10-startup-b)

Branch `agents/w10-startup-b`. Sources are under `experimental/sources/w10-startup-b-*`.
Each function is its own commit and compiles with the reviewed EE toolchain at the shared
`authored_cpp` flags (the three assembly units with the assembler profile). Every function
has a host contract that fails when a stored value, branch or call order is wrong. All but
the trivial ones were also run, on a private R5900 interpreter outside git, side by side
with the original routine from the private `SLUS_205.65`: identical arguments and memory,
identical stub callees, then return value, call log (with arguments) and every changed byte
outside the stack compared. No game binary, disassembly or extracted data is committed.
Nothing here claims a byte match with the original compiler output; most bodies are shorter
or longer than the original.

The private paired runs are evidence for the commit messages only. They do not replace a
hash-pinned baseline comparison, which this target does not have yet.

## What each unit does (observed unless marked)

- `_iWakeupThread` (kernel service -52) and `isceSifSetDma` (service -119): four-word syscall
  stubs, assembly. The assembled words equal the original's four words (private comparison);
  `check_words.py` in each directory checks them against the stated contract.
- `acos`, `acosf`, `atan2f`, `fmodf`: pass the argument(s) through to `__ieee754_<name>` and
  return the result. The original wraps a pointless frame around the tail jump; the compiled
  body here is shorter. No error-handling path exists in the original.
- `fabsf`: clears bit 31 of the pattern.
- `iAllocTimerCounter`, `iStartTimerCounter`, `iSetTimerHandler`: the interrupt-side halves of
  the EE kernel timer layer (the callers `AllocTimerCounter`, `StartTimerCounter`,
  `SetTimerHandler` are in w9-startup-crt-a). They work on `g_Timer` (authored in
  kernel-patches) and 64-byte counter records in `g_CounterBuf`. A handle is
  `(record address << 4) | (record word at +8)`; the other two routines recover the record as
  `(handle >> 10) << 6` and check the low ten bits against the word at +8.
  Finding: the `g_Timer` word at offset 12 is written (incremented) by `iAllocTimerCounter`,
  which the existing `g_Timer` notes call untouched. The word at offset 16 is also incremented
  there. Both are named by offset only.
- `counter_overflow_handler`, `init_ee_timer`, `get_time` and the two words `rollover_counter`
  and `timer_handle`: the keyboard library's millisecond clock. The handler adds one to a
  64-bit overflow count, clears the timer-0 mode flags by writing back what it read, `sync`,
  `ei`. `get_time` is `((rollover << 16) | (count & 0xffff)) / 576` through `__udivdi3`.
  Inferred from the public EE timer map: timer 0 at 0x10000000, mode 0x282 (1/256 bus clock,
  count enable, overflow interrupt), interrupt cause 9, so 576 ticks is one millisecond.
- `_sceIDC`: the data-cache index walk behind `InvalidDCache`, assembly. The assembled words
  equal the original's 41 (private comparison). Inferred: cache op 0x10 loads a tag, op 0x16
  stores a zero tag (invalidates) for lines whose address lies in the given range.
- `_sceFsIobSemaMK`, `_sceFs_Poff_Intr`, `_sceFs_Rcv_Intr`, `ee_retbuf_sel`, `sceCbPoff_Gp`:
  the file-system client's semaphores, power-off notification and reply handler. The reply
  handler reads the reply through the uncached alias, copies a 16-byte header, then handles
  commands 2, 11, 12, 23, 25 and 26 (the jump table at 0x0047b870 has 25 entries; all other
  values do nothing) and finally signals a semaphore (non-negative status) or releases the
  matching `_sceFs_q` entry (negative status). Command meanings are unknown.
- `_raise_r`, `_getpid_r`, `_kill_r`, `_init_signal_r`, `getpid`, `kill`: the signal layer (observed
  structure matches the usual one: range check, lazy 32-word handler table, default / ignore /
  error markers 0 / 1 / -1, one-shot handler). Record offsets used: 0 error number, 468 handler
  table. `getpid` returns 1 and `kill(1, n)` calls `Exit(n)`; any other process identifier is
  ignored and the result is 0.
- `_memalign_r`: aligned allocation on a dlmalloc-style heap, calling `_malloc_r`, `_free_r`,
  `__malloc_lock`, `__malloc_unlock`. Compiled with a 32-bit divide where the original calls
  `__umoddi3`. Only powers of two align (the original masks too).
- `ftoi`: reads a 64-bit register as double bits (sign ignored) and rounds with an unusual
  three-quarter rule; below one half it returns 0, from exponent 13 up it returns 9999.
  Its only caller passes it the result of `__fixunsdfdi`, which is an integer, not double bits;
  whether that is a mis-reading in the caller's notes or in the original is unknown.
- `dpmul`, `_fpadd_parts`: forwarders from the original names to the existing authored
  `norrath_player_double_multiply` and `norrath_player_double_add_parts`
  (game-player-softdouble-build), the same approach as w9-startup-crt-e-softdouble-names.
  They add no arithmetic. Evidence that the targets are the right routines: privately, the
  original `dpmul` and the authored multiply gave identical results on 1500 operand pairs, and
  the original `_fpadd_parts` and the authored add-parts gave identical returned pointers and
  memory on 1500 record pairs, with the helper symbols bound to the original `__unpack_d`,
  `__pack_d`, `__muldi3` and `__fpcmp_parts_d`.
- `norrath_legacy_pointer_type_info_vtable_view`: the three-pair table at 0x004538f8, read
  from the image (adjustment 0 plus `__tf19__pointer_type_info`, adjustment 0 plus
  `_$_19__pointer_type_info`, a zero pair).
- `_sceSifMInitCheck`: zero word at 0x00447c48.

## Policy question for the owner

`__ieee754_acos`, `__ieee754_acosf`, `__ieee754_atan2f` and `__ieee754_fmodf` are newly
opened imports. They are Sun/Cygnus fdlibm code in the original. The repository handles
`floorf` and `ceilf` as optional external Newlib inputs instead of authored sources. I did
not author these four from memory of that code. Decide whether they are external Newlib inputs
too or are to be authored from evidence by another package (they are not in any wave-10
list).

## Not closable from source in this package

- `_end`, `_fbss`, `_gp`: section-boundary and global-pointer symbols that a link script
  provides (`_fbss` start of the zero-initialised data, `_end` after it, `_gp` the global
  pointer value). The docs say the partial link imports no absolute game addresses, so they
  stay unresolved until a link layout exists.
- `_srsctorsstart`, `_srsctorsend`: bound a table of 15 constructor addresses in the `.srsctors`
  section of the SRS overlay (all values are in the 0x01000000 range). The constructors
  themselves are overlay code that is not authored, so the table would be a list of
  unresolved addresses; I did not write it.
- `_kExitTLBHandler`, `norrath_archive_vblank_count_view`, `norrath_archive_vblank_field_view`,
  `norrath_archive_vblank_semaphore_view` already have authored sources
  (`w8-startup-sce-b-k-exit-tlb-handler`, `sony-cd-vblank-views`) that are not in the shared
  build plan. They are closed by adding plan entries, which this package prints below.
  (`gs-dma-kernel-vblank-state` defines the unprefixed vblank words and none of the views.)

## Build and checks performed

- Baseline: `build_partial.py` on the shared plan at the start of this branch: 3474 units,
  1332 unresolved imports.
- With a private plan copy (outside git, made by `docs/agents/w10-startup-b-plan-entries.py
  --private-root`) that appends the 34 units below: 3508 compilations and one relocatable link,
  no failures, 1314 unresolved imports. 27 of the 32 assigned names left the list, 9 names
  entered it (listed under "Imports opened"). This is a source-only partial link, not an
  executable.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.
- Host contracts: each unit directory has a `host_contract.cpp` (or `check_words.py` /
  `check_structure.py` for assembly) that compiles and runs on the host with
  `c++ -std=c++17 -I<dir> host_contract.cpp`; they are not build inputs.

## Imports closed (names)

`_fpadd_parts`, `_iWakeupThread`, `_kExitTLBHandler` (existing source, plan entry only),
`_memalign_r`, `_raise_r`, `_sceFsIobSemaMK`, `_sceFs_Poff_Intr`, `_sceFs_Rcv_Intr`,
`_sceIDC`, `_sceSifMInitCheck`, `acos`, `acosf`, `atan2f`, `dpmul`, `fabsf`, `fmodf`, `ftoi`,
`get_time`, `iAllocTimerCounter`, `iSetTimerHandler`, `iStartTimerCounter`, `init_ee_timer`,
`isceSifSetDma`, `norrath_archive_vblank_count_view`, `norrath_archive_vblank_field_view`,
`norrath_archive_vblank_semaphore_view` (existing source, plan entry only),
`norrath_legacy_pointer_type_info_vtable_view`.

Left open from the assignment: `_end`, `_fbss`, `_gp`, `_srsctorsend`, `_srsctorsstart` (above).

## Imports opened (left unresolved)

`__ieee754_acos`, `__ieee754_acosf`, `__ieee754_atan2f`, `__ieee754_fmodf` (policy question
above); `_malloc_r`, `_free_r` (Newlib malloc, used by `_memalign_r` and `_init_signal_r`); `Exit`
(already open and assigned to w10-startup-a; `kill` calls it);
`__tf19__pointer_type_info`, `_$_19__pointer_type_info` (the two functions in the pointer
type_info table); `norrath_ee_exception_frame` (imported by the existing
`_kExitTLBHandler` unit). That is nine names. New data defined here that nothing else reads yet: `ee_retbuf_sel`,
`rollover_counter`, `timer_handle`, `sceCbPoff_Gp`, `_sceSifMInitCheck`. These use zero
initialisers, so they land in uninitialised data, not initialised data as in the original.

## Build-plan entries needed

Not added (shared file). Run

    python3 docs/agents/w10-startup-b-plan-entries.py --toolchain <reviewed ee root>

to print the 34 entries (hash, profile, object name, emitted functions) and, with
`--private-root DIR`, to write a private plan and snapshot copy to build with. All are
`kind: independently_authored_source`, empty `header_closure`. Sources (relative to
`experimental/`), profile `authored_cpp` unless noted:

- sources/w10-startup-b-acos/acos.cpp
- sources/w10-startup-b-acosf/acosf.cpp
- sources/w10-startup-b-atan2f/atan2f.cpp
- sources/w10-startup-b-cb-poff-gp/cb_poff_gp.cpp
- sources/w10-startup-b-counter-overflow-handler/counter_overflow_handler.cpp
- sources/w10-startup-b-dpmul/dpmul.cpp
- sources/w10-startup-b-ee-timer-state/ee_timer_state.cpp
- sources/w10-startup-b-fabsf/fabsf.cpp
- sources/w10-startup-b-fmodf/fmodf.cpp
- sources/w10-startup-b-fpadd-parts/fpadd_parts.cpp
- sources/w10-startup-b-fs-reply-selector/fs_reply_selector.cpp
- sources/w10-startup-b-ftoi/ftoi.cpp
- sources/w10-startup-b-get-time/get_time.cpp
- sources/w10-startup-b-getpid/getpid.cpp
- sources/w10-startup-b-getpid-r/getpid_r.cpp
- sources/w10-startup-b-i-alloc-timer-counter/i_alloc_timer_counter.cpp
- sources/w10-startup-b-i-set-timer-handler/i_set_timer_handler.cpp
- sources/w10-startup-b-i-start-timer-counter/i_start_timer_counter.cpp
- sources/w10-startup-b-i-wakeup-thread-syscall/i_wakeup_thread_syscall.S (`authored_entry_asm`)
- sources/w10-startup-b-init-ee-timer/init_ee_timer.cpp
- sources/w10-startup-b-init-signal-r/init_signal_r.cpp
- sources/w10-startup-b-isce-sif-set-dma/isce_sif_set_dma.S (`authored_entry_asm`)
- sources/w10-startup-b-kill/kill.cpp
- sources/w10-startup-b-kill-r/kill_r.cpp
- sources/w10-startup-b-memalign-r/memalign_r.cpp
- sources/w10-startup-b-pointer-type-info-vtable/pointer_type_info_vtable.cpp
- sources/w10-startup-b-raise-r/raise_r.cpp
- sources/w10-startup-b-sce-fs-iob-sema-mk/sce_fs_iob_sema_mk.cpp
- sources/w10-startup-b-sce-fs-poff-intr/sce_fs_poff_intr.cpp
- sources/w10-startup-b-sce-fs-rcv-intr/sce_fs_rcv_intr.cpp
- sources/w10-startup-b-sce-idc/sce_idc.S (`authored_entry_asm`)
- sources/w10-startup-b-sif-m-init-check/sif_m_init_check.cpp
- sources/w8-startup-sce-b-k-exit-tlb-handler/k_exit_tlb_handler.cpp (existing, not in the plan)
- sources/sony-cd-vblank-views/views.cpp (existing, not in the plan)

The `host_contract.cpp`, `check_words.py` and `check_structure.py` files are tests, not plan
inputs.

## Authored but never executed

Nothing here has run on an EE, an emulator or the game. The host contracts run the authored
code on the build machine against stub callees. The private paired runs executed the
authored code on a small interpreter next to the original code with stubbed callees; that
interpreter is not a hardware model (no cache, no interrupts, no timer registers, no
uncached-alias semantics beyond address folding, `ei` treated as a no-op).
Per unit:

- Only compiled, never run: the five data words, the pointer type_info table (checked against
  the image by eye and by a host test of the source), the two forwarders' targets are covered
  by the privately run equivalence of the authored routines, not by running the forwarders.
- `_sceIDC`, `_iWakeupThread`, `isceSifSetDma`: assembled and compared as words; the cache and
  syscall instructions never ran.
- `counter_overflow_handler`, `init_ee_timer`, `get_time`: ran against plain memory, not real
  timer hardware or the interrupt dispatcher.
- `_sceFs_Rcv_Intr`: ran with the reply buffer in plain memory; 236 of the original's 237
  words executed in the paired runs (the one other word is a delay-slot nop).
- `_memalign_r`, `_raise_r`, `_init_signal_r`, `_kill_r`, `_getpid_r`: ran against faked
  allocator and signal callees, not the real ones.

## Unknown

- Meaning of the `g_Timer` offset-12 generation word and offset-16 count beyond their being
  incremented by `iAllocTimerCounter`; units of the timer value passed to `iSetTimerHandler`;
  the meaning of the counter record words at +16, +24 and +32 beyond how they are written.
- Meaning of the file-system reply commands 2, 11, 12, 23, 25, 26, of each copied block, and
  of the reply buffer beyond the offsets used; who stores `sceCbPoff_Gp` and the power-off
  callback block; whether `ee_retbuf_sel` is read anywhere.
- Whether `ftoi` is ever given double bits (its only caller passes an integer from
  `__fixunsdfdi`), and why its rounding point is three quarters.
- The reentrancy record layout beyond offsets 0 and 468.
- The two functions in the pointer type_info table; the SRS constructors; the link-script
  symbols; original storage placement of the zero-initialised words.
- Whether `dpmul` and `_fpadd_parts` should stay forwarders or be merged by the integrator
  into the authored routines' symbol names.
