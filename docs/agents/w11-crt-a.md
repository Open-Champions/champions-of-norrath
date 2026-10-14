# w11-crt-a: startup, C runtime and IOP/pad/memory-card library (1 of 3)

Branch `agents/w11-crt-a`. Sources are under `experimental/sources/w11-crt-a-*`. Each has its own
notes in its header comment (observed address, size, inferred and unknown). Reference executable:
the private `SLUS_205.65` named in the docs; nothing derived from it is committed.

## Observed

A previous run of this package already committed one reconstruction per symbol (see `git log`).
This session re-checked them and added the plan helper below; no source was changed.

Host checks re-run this session with `clang++ -std=c++17 -w <unit>/host_check.cpp`:
all pass except the cases below, which need a flag.

- `queue-init`, `queue-peek-read-done`, `tty-queue-storage`: add `-DNORRATH_HOST_CONTRACT`
  (the tty header otherwise casts a 64-bit host pointer to 32 bits). They pass with it.
- `memcpy-d` and `ieee754-atan2f`: pass at `-O0` and fail at `-O1`/`-O2` on the host (Apple clang).
  Inferred, not investigated: the host optimiser treats the stub named `memcpy` and the counted
  `atanf` stub as builtins and elides or reorders the recorded calls. The EE-compiled code is not involved.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.

## Where each import is closed

| Import | Source |
|---|---|
| DLG_CheckMutex__Fv | w11-crt-a-dlg-check-mutex |
| FlushIopCommand__Fb | w11-crt-a-flush-iop-command |
| KbmReaderThread | w11-crt-a-kbm-reader-thread |
| LoadSRSServer__Fv | w11-crt-a-load-srs-server |
| MemcpyD | w11-crt-a-memcpy-d |
| PadData | w11-crt-a-pad-data |
| QueueInit | w11-crt-a-queue-init (storage view in w11-crt-a-tty-queue-storage) |
| QueuePeekReadDone | w11-crt-a-queue-peek-read-done |
| SurfCache | w11-crt-a-surf-cache |
| TLBInfo (and kernelTLB, defaultTLB, extendTLB tables) | w11-crt-a-tlb-info, -kernel-tlb, -default-tlb, -extend-tlb |
| VIEEDMASync__Fi | w11-crt-a-vieedmasync |
| _Exit, _SetTLBEntry | w11-crt-a-exit-stub, w11-crt-a-set-tlb-entry (assembly kernel-service stubs) |
| __EH_FRAME_BEGIN__ | w11-crt-a-eh-frame-begin |
| __builtin_vec_delete, __builtin_vec_new | w11-crt-a-builtin-vec-delete, -vec-new |
| __cplus_type_matcher | w11-crt-a-cplus-type-matcher |
| __extendsfdf2 | w11-crt-a-extendsfdf2 (bridge: the executable has no such function; its helper is fptodp) |
| __floatdidf, __floatdisf, __pack_f | w11-crt-a-floatdidf, -floatdisf, -pack-f |
| __get_eh_context, __get_eh_info | w11-crt-a-get-eh-context, -get-eh-info |
| __ieee754_acos, _acosf, _atan2f, _fmodf | w11-crt-a-ieee754-* |
| __malloc_current_mallinfo | w11-crt-a-malloc-current-mallinfo |
| _$_19__pointer_type_info, __tf19__pointer_type_info | w11-crt-a-dtor-19-pointer-type-info, -tf-19-pointer-type-info |
| __ti5Spell | w11-crt-a-ti-spell |
| _args | w11-crt-a-args |
| __throw | existing draft `w10-startup-a-throw/throw.cpp`; this package adds only the host check w11-crt-a-throw-check |
| _ctype_ | existing `w8-startup-sce-a-ctype-table`; this package adds the host check w11-crt-a-ctype-check |
| _end, _bssend | not authored: link-layout symbols (see Unknown) |

`w11-crt-a-data-check/data_check.cpp` is a host tool that compares a compiled data object with the
private executable's bytes; it is not an EE input.

## Imports closed

DLG_CheckMutex__Fv, FlushIopCommand__Fb, KbmReaderThread, LoadSRSServer__Fv, MemcpyD, PadData,
QueueInit, QueuePeekReadDone, SurfCache, TLBInfo, VIEEDMASync__Fi, _Exit, _SetTLBEntry,
__EH_FRAME_BEGIN__, __builtin_vec_delete, __builtin_vec_new, __cplus_type_matcher, __extendsfdf2,
__floatdidf, __floatdisf, __get_eh_context, __get_eh_info, __ieee754_acos, __ieee754_acosf,
__ieee754_atan2f, __ieee754_fmodf, __malloc_current_mallinfo, __pack_f,
_$_19__pointer_type_info, __tf19__pointer_type_info, __ti5Spell, _args, and (through existing
w10/w8 drafts, now with committed host checks) __throw and _ctype_.
These closures were not re-verified by a full partial build in this session (one-at-a-time rule);
the list of what is still unresolved comes from the integrator's build.

## Imports opened

None known from the sources themselves. Callees that remain unresolved imports of individual units
(local helpers of the IOP/pad code, the soft-double entry points, `__builtin_delete`/`__builtin_new`)
belong to other packages or to the shared plan; check `unresolved-imports.json` after merging.

## Build-plan entries needed

None of these sources is in `experimental/build-plan.json`. Print the exact entries (JSON lines,
compiled with the reviewed toolchain, host-only directories skipped, `.S` stubs included):

    python3 docs/agents/w11-crt-a-plan-entries.py --toolchain /Users/case/norrath-agents/repo/work/toolchains/ee

`--private-root DIR` writes a private plan and snapshot copy outside git for a check build.
Skipped on purpose (host-only): ctype-check, throw-check, data-check. The tty storage header is
included through its users. Data-only units (args, tlb tables, pad-data, surf-cache, ti-spell,
tlb-info, malloc-current-mallinfo, eh-frame-begin, tty-queue-storage) have empty function lists.

## Authored but never executed

Everything. Objects were compiled for the EE and host checks ran on the host with stubs for
callees; nothing ran on the EE or in the private paired-run harness this session. Host checks
are stub-based contracts for control flow, arguments and numeric results, not original-byte matches.
The numeric routines (`__ieee754_*`, `__pack_f`, `__floatdi*`) are compared with host libm or
an independent restatement, not with the original's output. The `__throw` host check replaces the
stack-walking builtins with recording stubs.

## Unknown

- Which of `end` and `_end` initialised the heap-pointer word, and whether `_end`/`_bssend` can be
  expressed outside a linker script. The entry code uses them as link-time addresses; `_bssend`
  differs from `_end` so that the SRS tables are not cleared. They stay unresolved by design.
- Exact EE code size matches: authored bodies differ from the original sizes in several units
  (for example `__builtin_vec_new` 8 vs 116 bytes in the original) because the original inlined
  `__builtin_new` pieces or used different codegen. No match is claimed.
- The register hand-over of `__throw` into the exception handler (v0 context), which the compiler's
  expansion cannot place.
- Names of fields in the IOP/RPC objects, the meaning of `-543` in `MemcpyD`, and sizes of
  data objects without recorded symbol sizes (`_args`, `PadData`, `SurfCache`).
- Whether the host-check failures at `-O1`/`-O2` for `memcpy-d` and `ieee754-atan2f` are only
  host-optimiser artefacts (not investigated).
