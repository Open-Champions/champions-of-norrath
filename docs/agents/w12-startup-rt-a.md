# w12-startup-rt-a: startup and runtime support (A)

Branch `agents/w12-startup-rt-a`. Sources are under `experimental/sources/w12-startup-rt-a-*`. Every address is from the
pinned local `SLUS_205.65`. Nothing here ran on an EE; "paired run" below means a private R5900 interpreter ran the
original function and the freshly compiled authored function from the same memory snapshot with identical scripted
callees and compared call logs, return values, saved registers where stated, and every changed byte outside the stack.
The interpreter and its scripts live outside git, so the integrator cannot rerun them; each unit therefore also carries a
`host_check.cpp`, `host_contract.cpp` or `object_check.cpp` that does run from the repository. None of these files is a
build-plan input. Compiled sizes below are not image sizes; no byte match is claimed except where stated.

## Method

* Baseline: `tools/build_partial.py` on the branch start (`c7243a6d`): 827 unresolved imports, all 28 package names present.
* Private build with the 25 new inputs appended to a copy of plan and snapshot kept outside git (`/tmp`): 5,138 inputs,
  3,926 function bodies, 0 COMMON, **822 unresolved imports** (22 package names closed, 17 names opened).
* Host checks (examples; every file names its own command in its header):

```sh
c++ -std=c++17 -o check experimental/sources/<unit>/host_contract.cpp && ./check
EE=work/toolchains/ee/bin/mips64r5900el-ps2-elf
$EE-g++ <authored_cpp flags> -c <unit>.cpp -o unit.o && c++ -std=c++17 -o chk experimental/sources/<unit>/host_check.cpp && ./chk unit.o
```

  Assembly units are assembled with the `authored_entry_asm` flags (`-x assembler-with-cpp`); `docs/PORTABLE_BUILD.md` has both profiles.
* Every host contract and paired run was repeated against deliberately broken copies (changed constant, comparison,
  argument order, mask); the number that failed is in the commit message of each unit.

## Units (all in their own commit)

| Symbol | Address | Original size | Form | Evidence |
|---|---|---:|---|---|
| `_Exit` | `0x002d4d00` | 16 | assembly, service 4 | object check on the four words; byte match with `gs-dma-kernel-body-check` |
| `_SetTLBEntry` | `0x002dfcc8` | 16 | assembly, service 86 | same |
| `_kExitTLBHandler` | `0x002e0100` | 244 | assembly | object check builds the words from field layouts and requires 14 relocations; a private link with the frame at `0x008e5780` matched the image bytes |
| `QueuePeekWriteDone` | `0x002d6258` | 60 | C++ | paired run (40 scenarios, 15/15 instructions); host check |
| `SyncDCache` | `0x002d59d8` | 124 | C++ | paired run with scripted Status words (35, 31/31); host contract |
| `IsEventModifier` + `ModifierCodes` | `0x002e8d88` / `0x00447f60` | 72 / 8 | C++ and data | paired run (32 values, 18/18); object and host checks |
| `AdjustTime` | `0x002d1608` | 140 | C++ | paired run with the image's own BCD helpers (304 scenarios incl. date rollover); host contract |
| `InitialiseIopDriver` + `LocalData` | `0x002e8898` / `0x008e8c80` | 268 / 7424 (gap extent) | C++ and bss | paired run with scripted RPC (11 scenarios incl. both never-ending paths, which both runs enter alike); host contract; object check |
| `cosf` | `0x002f1290` | 212 | C++ | paired run (400+ inputs, helpers real); host contract pins boundary, quadrants, signs |
| `atanf` | `0x002f0f28` | 652 | C++ | paired run (about 1,400 inputs); host contract against host `atan` |
| `dpdiv` | `0x002fb908` | 320 | C++ | paired run (about 2,300 operand pairs, real `__unpack_d`/`__pack_d`); host contract |
| `_ctype_` | `0x0047e1f0` | 257 | rodata | byte comparison with the image; object check against host C-locale classification |
| `__throw` | `0x002fb070` | 336 | C++ with compiler builtins | paired run (5 scenarios); object-shape check. **Known deviation, see below** |
| `eh_context_initialize` | `0x002fa350` | 100 | C++ | paired run (9, 25/25); host contract |
| `get_eh_context` | `0x004480dc` | 4 | data pointer, initial value is `eh_context_initialize` | object check (one relocation) |
| `dwarf_reg_size_table_initialized` | `0x004480e0` | 4 | .data word | object check |
| `__throw_type_match_rtti_2` | `0x002fe0d0` | 1292 | C++ | paired run over synthetic type graphs (1,500 random, 4,096 exhaustive qualifier chains, 60 targeted); object-shape check |
| `ee_retbuf_sel` | `0x004449f0` | 4 | .data word | object check |
| `_sceSifMInitCheck` | `0x00447c48` | 4 | .data word | object check |
| `__EH_FRAME_BEGIN__` | `0x00453970` | 0 | marker, **section differs** | object check; link checked with the reviewed `ld -r` against the full partial object |
| `_free_r` | `0x00300608` | 756 | name provider | paired run (6,000 heaps built by the original allocator) |
| `_malloc_r` | `0x00301840` | 1832 | name provider | paired run (40,000 heaps, sizes 8 to 0xffffffff) |
| `amx_Exec` | `0x00135de8` | 9176 | name provider for the authored `norrath_amx_exec` | paired run (2,580 one-opcode programs + 240 entry cases); one known defect in the pair |

### Observed, inferred, unknown (per unit)

The header comment of every source carries its own observed / inferred / unknown split. Items that matter across units:

* **Observed.** `SyncDCache` samples COP0 Status bit `0x10000`, calls `DIntr` only when set, passes both addresses masked
  to a multiple of 64 to the local `_sceSDC`, and calls `EIntr` as its last act. `AdjustTime` converts an eight-byte BCD
  clock record to binary, adds minutes to byte 2 and carries into the hour one hour at a time with `addhour`/`subhour`.
  `InitialiseIopDriver` sends a 16-byte packet (two counts, `0x0106`, zero, status word) through `sceSifCallRpc` function 3,
  prints `driver version = %d.%02d`, and spins forever on a nonzero status word or when the expected and reported
  halfwords differ. `ModifierCodes` holds `e0..e7`. `dpdiv` divides 61 bits with no sticky bit and `__pack_d` flushes
  subnormals. `__throw_type_match_rtti_2` reads old-ABI `type_info` records (vptr at +4, pointee at +8, qualifier bits at +12).
* **Inferred.** `ModifierCodes` are keyboard modifier key codes (USB usage ids). `LocalData` extent is the gap to the next
  symbol and its 64-byte alignment is a guess from its use as an RPC buffer. The class values 0..4 of the soft-double
  records follow the existing `__unpack_d` notes. `get_eh_context` is replaced by `eh_context_static` on first use.
* **Unknown.** Meaning of RPC function 3 and of the status word at `LocalData+712`; the qualifier-bit names in the type
  matcher; which objects own the `.eh_frame` marker, `_ctype_`, and the 4-byte data words; whether any landing pad reads `$v0`.

### Deviations and weaker evidence (read before integrating)

* **`__throw`** is not faithful at the handler entry. The paired run matches the hook-visible calls (including the
  frame-address word written into the 1,144-byte record and the `pc - 1` argument), the final stack pointer, all callee-saved
  integer and float registers and the handler entry address. It does not match `$v0` (the original passes the context
  pointer; the compiler's exception return leaves the handler address), `$ra` (stub address vs handler address) and the
  argument registers (restored to entry values). The earlier `w10-startup-a-throw` was rejected for the same `$v0` gap; an
  inline `move $2` before `__builtin_eh_return` does not survive the expansion. The code after the second `__terminate` call
  in the original falls into the return stub; both runs were only compared on the paths where `__terminate` returns
  into normal flow (it is expected not to return).
* **`__EH_FRAME_BEGIN__`** is a zero-length object in the input section `.norrath_eh_frame_begin`, not in `.eh_frame`.
  Placed in `.eh_frame` (the older unit) it crashes `ld -r` with a BFD assertion at `elflink.c:9839` as soon as another
  object relocates against it; reproduced here against the full partial object (`ld -m elf32lr5900 -r --warn-common`).
  A final link needs `.eh_frame : { KEEP(*(.norrath_eh_frame_begin)) KEEP(*(.eh_frame)) }` (or an equivalent ordering) for
  the symbol to equal the section start as in the original.
* **`_free_r` and `_malloc_r`** are name providers that forward to the existing authored
  `norrath_player_free_reentrant` / `norrath_player_malloc_reentrant` (game-player-*-build). They add no new allocator
  logic. The value of the commits is the paired run, which is the first comparison of those bodies against the original
  allocator on real heaps (the `malloc_extend_top` body and `_malloc_trim_r` were the original's during the run, and `_sbrk_r`
  was scripted). 24 of 458 malloc instructions were not reached.
* **`amx_Exec`** is a name provider that forwards to the existing authored `norrath_amx_exec` (script-amx-exec, not this
  package). The paired run is the first comparison of that executor with the original on real inputs. Method: the original
  is direct-threaded, so each run gets the same program with its own opcode cells (the original's handler addresses from its
  131-entry table at `0x00333710`, ids `0..130` for the authored executor); operands, data, machine record and parameters
  are identical. 2,580 programs of the form `PROC; set PRI/ALT; one opcode with random operands; store observables; restore the
  stack; RETN` over opcodes 1..129 (jumps, calls, switch and the symbol/line records included), the SYSREQ callback and the memory
  copy/compare calls scripted identically, plus 240 entry-condition cases. Status, result word, machine record words and every
  data byte match for 2,554 of 2,580 programs. Of the 26 others, **20 are a real defect and 6 are undefined test inputs**:
  * **PUSH.R (opcode 38) differs.** The original pushes PRI `count` times (count 5 pushes five values); the authored executor
    loops `while (count != -1)` and pushes `count + 1` values (count 0 pushes once). Fix for the script-amx owner: loop `while (count != 0)`.
    All 20 PUSH.R programs of the run differ in the stack pointer and the pushed words. This wrapper is committed anyway so the import
    resolves; do not treat script execution as verified until the executor is corrected.
  * The six undefined cases store into the saved return-address slot of the frame (`STOR.S`, `STRB.I`, `INC.S` at frame+4 and the
    like) and then `RETN`; the original jumps into garbage while the authored executor reports status 6. They say nothing about either.
  * Not comparable by design: a null machine record returns the handler table of the respective executor (different addresses),
    and public index -2 in this build indexes the publics table at -48 bytes (garbage) in both.
  * Division of the minimum negative value by -1 was exercised only through the interpreter's own divide; the authored executor
    documents that case as unknown.
* **`dpdiv`**, **`atanf`**, **`cosf`**, **`__throw_type_match_rtti_2`** compile to different sizes than the originals
  (364/320, 648/652, 200/212, 1636/1292).

## Imports closed

`AdjustTime`, `InitialiseIopDriver`, `IsEventModifier`, `LocalData`, `QueuePeekWriteDone`, `SyncDCache`, `_Exit`,
`_SetTLBEntry`, `__EH_FRAME_BEGIN__`, `__throw`, `__throw_type_match_rtti_2`, `_ctype_`, `_free_r`, `_kExitTLBHandler`,
`_malloc_r`, `_sceSifMInitCheck`, `amx_Exec`, `atanf`, `cosf`, `dpdiv`, `ee_retbuf_sel`, `get_eh_context` (22 of the 28; `amx_Exec`
by name only, with the PUSH.R defect above).
Other definitions added without being on the list: `ModifierCodes`, `eh_context_initialize`, `dwarf_reg_size_table_initialized`.

## Imports opened

`__dynamic_cast_2`, `__frame_state_for`, `__kernel_cosf`, `__kernel_sinf`, `__tf16__attr_type_info`, `__tf16__func_type_info`,
`__tfv`, `_sceSDC`, `addhour`, `convertfrombcd`, `converttobcd`, `eh_context_static`, `init_reg_size_table`,
`norrath_ee_exception_frame` (the 0x230-byte block at `0x008e5780`, the same name the unselected w8 unit used),
`subhour`, `throw_helper`, `upcast__C16__user_type_infoRC9type_infoPvPPv`. Original addresses (from the symbol table): `__tfv`
`0x002fe838`, `__tf16__attr_type_info` `0x0031af50`, `__tf16__func_type_info` `0x0031b0c0`, `eh_context_static` `0x002fa3b8`,
`init_reg_size_table` `0x002fa130`, `throw_helper` `0x002face0`, `_sceSDC` `0x002d5930`, `convertfrombcd`/`converttobcd`
`0x002d1380`/`0x002d13e8`, `addhour`/`subhour` `0x002d15b0`/`0x002d15e0`. `__tf16__attr_type_info` and `__tf16__func_type_info`
have the same shape as the authored `__tf16__user_type_info` (initialise-once, `__tf9type_info` then `__rtti_si`) and need
their data objects `__ti16__attr_type_info` / `__ti16__func_type_info` (12 bytes bss each) and two name strings; nothing authored
defines them yet. Net change of the unresolved count in the private build: 827 to 822.

## Build-plan entries needed

Twenty-five inputs, kind `independently_authored_source`, `input_order` left to the integrator. Ready-made entries with
SHA-256, header closure and emitted function extents are in `docs/agents/w12-startup-rt-a-plan-entries.json` (force-added;
`docs/agents/` ignores `.json`). Profiles: `authored_entry_asm` for `exit_stub.S`, `set_tlb_entry.S`,
`k_exit_tlb_handler.S`; `authored_cpp` for the rest. `queue_peek_write_done.cpp` needs
`sources/w9-startup-crt-f-tty-state/tty_state.hpp` in its header closure. Host checks and `w12-startup-rt-a-check/*.hpp` are not inputs
(they are reached only by `#include "..."` from host files). Quoted includes are all inside the repository; the sources
contain no `#include <...>`.

Overlaps to resolve when merging: the older units `w11-crt-a-exit-stub`, `w11-crt-a-set-tlb-entry`, `w11-crt-a-eh-frame-begin`,
`w8-startup-sce-b-k-exit-tlb-handler`, `w8-startup-sce-a-ctype-table`, `w10-startup-b-fs-reply-selector`,
`w10-startup-b-sif-m-init-check` and `w10-startup-a-throw` define the same symbols and are unselected; select only one
definition of each. This package's versions are the ones that carry a failing-capable check.

## Not closed

* **Linker-defined symbols** `_gp`, `_fbss`, `_end`, `_bssend`, `_srsctorsstart`, `_srsctorsend`. These are layout values, not
  source; no translation unit can define them without fixing a layout, and the recipe forbids absolute address imports.
  Observed in the pinned image (section extents from the ELF headers): `_fbss = 0x00486800` (start of `.sbss`);
  `_bssend = _srsctorsstart = 0x008eab1c` (end of `.bss` 0x00486e00 + 0x00463d1c, and start of `.srsctors`);
  `_srsctorsend = 0x008eab58` (end of the 0x3c-byte `.srsctors`); `_gp = 0x004879f0` (inside the small-data window, 0x47f0
  above the start of `.sdata` at 0x00483200); `_end = 0x008ec91c` (0x1e00 above `_bssend`, not the end of any section: unknown).
  Suggested linker-script lines for the lead integrator, to be reviewed (not applied here):
  `_fbss = ADDR(.sbss); _bssend = ADDR(.bss) + SIZEOF(.bss); _srsctorsstart = ADDR(.srsctors); _srsctorsend = ADDR(.srsctors) + SIZEOF(.srsctors);`
  The rule that placed `_gp` and the origin of `_end` are unknown.

## Authored but never executed

All of it, on an EE. Every function was run only in the host contract and in the private interpreter. The assembly stubs and
`_kExitTLBHandler` were not executed even there: `_Exit` and `_SetTLBEntry` are kernel service requests, and
`_kExitTLBHandler` restores privileged state and returns from an exception. `__throw` and `__throw_type_match_rtti_2` have never
run with real exception tables or real `type_info` objects, only with scripted helpers over synthetic records. The soft-float
and `cosf`/`atanf` comparisons use the interpreter's float model, which rounds to nearest; the EE FPU does not. The interpreter
needed `mfc0`/`mtc0` for `SyncDCache`; a rerun with an unmodelled instruction halts and was treated as a failure.

## Unknown

Everything listed under "Unknown" above, plus: which original object files own the library routines, the exact layout of
the 1,144-byte frame-state record beyond word 0, the semantics of `_sceSDC`, `throw_helper` and the type-info accessors, and
how a final link should order `.eh_frame`, `.srsctors` and the small-data window.
