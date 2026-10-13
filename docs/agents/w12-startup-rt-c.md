# w12-startup-rt-c: startup and runtime support (C)

Package work is on `agents/w12-startup-rt-c`. Sources are under `experimental/sources/w12-startup-rt-c-*`; the
worktree held no earlier commits or notes for this package. The private reference is the hash-pinned
`SLUS_205.65` executable; all addresses and observations below were read from it locally. Nothing from it
(bytes, disassembly, decompiler output) is committed. No code ran on the PlayStation 2; "paired run" means the
private R5900 interpreter ran the original function and the authored function (compiled with the reviewed
`authored_cpp` flags) from identical memory snapshots with identical stubbed callees, and compared the callee
call log (arguments and order), the return value and every changed byte outside the stack.

## Functions authored (one commit each)

| Symbol | Address (size) | Check |
| --- | --- | --- |
| `sinf` | `0x2f1468` (232) | host contract with stub kernels; three mutants fail |
| `sceVu0CopyMatrix` | `0x1348c80` (100) | paired run (8 scenarios), host contract |
| `sceScfGetTimeZone` | `0x2d11a8` (76) | paired run (232 scenarios), host contract |
| `sceScfGetSummerTime` | `0x2d1260` (100) | paired run (232), host contract |
| `sceSifSetSreg` | `0x2d7178` (24) | paired run (32), host contract |
| `sceDeci2ExRecv` | `0x2df978` (52) | paired run (6), mutant caught |
| `sceDeci2ExSend` | `0x2df9b0` (52) | paired run (6), mutant caught |
| `sceGsSetDefDrawEnv` | `0x2d2598` (488) | paired run (747), host contract, two mutants caught |
| `sceGsSetDefDrawEnv2` | `0x2d2970` (480) | paired run (747), host contract |
| `sceGsSetDefClear` | `0x2d2780` (252) | paired run (1500, incl. stack-passed arguments), host contract, mutants caught |
| `sceGsPutDrawEnv` | `0x2d2888` (224) | paired run (300); timeout path by host contract only (see Unknown) |
| `setup_kbd_device` | `0x2e7ce8` (336) | paired run (400, 84/84 words), host contract, mutants caught |
| `setup_mouse_device` | `0x2e7e38` (288) | paired run (400, 71/72 words), host contract, mutants caught |
| `process_mouse_data` | `0x2e86d8` (448) | paired run (900, 109/112 words), host contract (ASan/UBSan), mutants caught |
| `process_kbd_data` | `0x2e8490` (584) | paired run (700, 145/146 words), host contract (ASan/UBSan), mutants caught |
| `sin` | `0x2f0df0` (308) | paired run (352), host contract, mutants caught |
| `qsort` | `0x3025d0` (2540) | paired run (2500 random arrays, 615/635 words, every compare call in order), host contract with original-derived trace digests, six mutants caught |

None of these is claimed as a byte match: the authored compilations differ in size from the originals (for
example `sinf` 224 against 232 bytes, `qsort` 2656 against 2540).

Data: `experimental/sources/w12-startup-rt-c-overlay-iop-buffers/iop_buffers.cpp` defines `sSend`, `sRecv`,
`sFunctionIndex`, `sIopFunctionData` and `sSifClient` (uninitialised; sizes are the distances to the next symbol in
the image's table, which read as zero there) and makes the three placeholder `norrath_overlay_*` names aliases of the
same storage. Checked by `docs/agents/w12-startup-rt-c-data-check.py <ee-toolchain>` (nm sizes, section, aliasing).

## Evidence notes (observed / inferred / unknown, short form)

Observed (per function in the source headers): the argument handling, branch conditions, constants, stored fields
and callee argument lists. Highlights worth knowing across units:

* `sceGsSetDefDrawEnv` and `sceGsSetDefDrawEnv2` differ in the buffer-width rounding of the frame word
  (`(width + 63) >> 6` against `width >> 6`) and in the register numbers (76/78/24/64/71 against 77/79/25/65/72);
  both return 8 and end with a `sync`. The `sync` is emitted with inline assembly guarded for the EE target
  because the compiler drops `__sync_synchronize()`.
* `sceGsSetDefClear` takes eleven arguments; the last three are on the stack (blue and alpha as bytes, then a word).
* `process_kbd_data` builds a text buffer of held-key numbers with `sprintf`/`strcat` and never uses it.
* `qsort` is a Bentley and McIlroy style quicksort with three swap widths; the host contract replays digests of the
  original's compare-call sequence rather than relying on the output order alone.

Inferred: meanings of the GS register numbers, of the DMA channel 2 values, of the keyboard/mouse slot fields and the
five-entry ring in `process_mouse_data`, and the roles of `sSend`/`sRecv`. Each source header marks these.

Unknown: see the end of this file.

## Private checks and how to repeat them

Paired-run scripts live outside git in `priv-w12-startup-rt-c/` (a copy of the earlier private harness; two
interpreter fixes were made there: `lq`/`sq` now carry the upper 64 bits, and arguments beyond the eighth are written
to the stack). They are not published. Host contracts are beside each source and are not build-plan inputs:

```sh
c++ -std=c++17 -Wall -Wextra -D<symbol>=<alias> <unit>.cpp host_contract.cpp -o /tmp/t && /tmp/t
```

(each `host_contract.cpp` header gives its exact `-D` flags; `qsort` compiles the source as its own translation unit,
and `sceGsPutDrawEnv` needs `-include host_regs.hpp`.) The EE compile of every unit uses
`cc.sh`-style flags identical to the `authored_cpp` profile.

## Build

One private partial build (plan kept outside git: the shared plan plus the 18 units of this package plus the two
earlier-wave sources below, 5,133 selected inputs, driver unchanged) compiled every input with no
retries and linked once: 3,926 function bodies and 811 unresolved imports, against 827 for the shared plan at the
start (`work/plan12-build`). Of the 27 names in the scope list, 25 left the unresolved list; `osdsrc` and `tlbsrc`
remain (see Not closed). Eleven names appeared that were not unresolved before (see Imports opened); the net change is
-16. This is a relocatable partial link, not an executable or a runtime verification.

## Build-plan entries needed

`python3 docs/agents/w12-startup-rt-c-plan-entries.py --toolchain <ee> [--with-others] [--private-root DIR]` prints
the entries for the 18 directories `experimental/sources/w12-startup-rt-c-*` (object names are the directory names with
underscores) and, with `--with-others`, for two earlier-wave sources that already define three of the scope names but
are not in the shared plan:

* `sources/w10-startup-b-ee-timer-state/ee_timer_state.cpp` defines `rollover_counter` and `timer_handle`;
* `sources/w10-startup-b-cb-poff-gp/cb_poff_gp.cpp` defines `sceCbPoff_Gp`.

These two are authored by an earlier package, compile with the reviewed flags, and were only missing from the plan.
All entries use profile `authored_cpp`, kind `independently_authored_source`, and no header closure.

## Imports closed

From the scope list: `process_kbd_data`, `process_mouse_data`, `qsort`, `sceDeci2ExRecv`, `sceDeci2ExSend`,
`sceGsPutDrawEnv`, `sceGsSetDefClear`, `sceGsSetDefDrawEnv`, `sceGsSetDefDrawEnv2`, `sceScfGetSummerTime`,
`sceScfGetTimeZone`, `sceSifSetSreg`, `sceVu0CopyMatrix`, `setup_kbd_device`, `setup_mouse_device`, `sin`, `sinf`,
`sFunctionIndex`, `sRecv`, `sSend`, `sSifClient`, `norrath_overlay_sIopFunctionData_11edf08`,
`norrath_overlay_sSend_11edb00` (the last two by alias in the overlay buffer unit).

Closed by adding existing earlier-wave sources to the plan (no new code here): `rollover_counter`, `timer_handle`,
`sceCbPoff_Gp`.

## Imports opened

Names that appeared in `unresolved-imports.json` of the private build and were absent from the shared list
(checked by diffing the two files):

* `IsT10K` (`0x2d1068`) and `gConfig` (`0x00444850`, 8 bytes, initialised data), called/read by `sceScfGetTimeZone`
  and `sceScfGetSummerTime`;
* `GetOsdConfigParam2` (`0x2d53f0`), called by `sceScfGetSummerTime`; an assembly unit exists in
  `w8-startup-sce-a-get-osd-config-param` but it only defines `GetOsdConfigParam`, so this one has no definition;
* `CheckforModifierKeyEvents` (`0x2e8300`), `generate_key_release_events` (`0x2e80d8`) and
  `generate_key_press_events` (`0x2e81f0`), local functions called by `process_kbd_data`;
* `__kernel_sin` (`0x2f6a08`), `__kernel_cos` (`0x2f5be0`), `__ieee754_rem_pio2` (`0x2f3c78`), called by `sin`;
  `__kernel_sinf` (`0x2f75f8`) and `__kernel_cosf` (`0x2f6c40`), called by `sinf` (`__kernel_tanf`,
  `__ieee754_rem_pio2f`, `dpsub` and the other callees were already resolved).

Not new names but shared by design: `norrath_sound_system_register_words_view` (the view name `sceSifGetSreg` already
imported, now also used by `sceSifSetSreg`; it was unresolved before and still is).

## Not closed and why

* `osdsrc` (`0x00444a38`, 1960 bytes) and `tlbsrc` (`0x004459d0`, 816 bytes): original EE machine code that
  `_InitSys` copies to `0x80074000` and `0x80075000`. Defining them would republish original instructions as data or as
  a listing, the same conclusion the kernel-patches, w10-world-c/d and w11-crt-c notes reached. They need an owner
  decision: link a private extraction at integration time, or write independent replacement handlers.

## Authored but never executed

Everything above. No function ran on the EE or in an emulator; the paired runs used the private interpreter with
stubbed callees, so memory-mapped behaviour is stand-in memory. In particular: the DMA and GS packet functions
(`sceGsPutDrawEnv`, the draw-environment builders) never touched hardware; `sceScf*` used stubbed IsT10K/OSD
queries; `setup_*_device` and `process_*_data` were never fed real IOP keyboard or mouse reports; `qsort` ran only
against interpreter memory and host memory; `sin`/`sinf` were checked against stub kernels, not numerically.

## Unknown

* Whether the 16.7-million-iteration wait in `sceGsPutDrawEnv` behaves identically on the original: the private
  interpreter's step limit stops both sides, so only the authored version's timeout path is checked (host contract).
* The meaning of the GS register values, of `gConfig`, of the OSD word fields, of GLibData's slot fields and of the
  record words beyond the offsets used.
* Exact sizes of `sSifClient` (at most 56 bytes) and of the placement of the five overlay objects, which the linker
  chooses here.
* Whether `process_kbd_data`'s unused text buffer (208 and 112 bytes in the original frame) can overflow on real data;
  the authored version has the same unguarded behaviour.
* Contents and a legal route for `osdsrc`/`tlbsrc` (above).
