# w14-startup-sys: startup, linker and system-library symbols

Branch `agents/w14-startup-sys`. Sources are under `experimental/sources/w14-startup-sys-*`. Every function
below is its own commit. Each was compared against the pinned `SLUS_205.65` with a private R5900 paired run:
original and authored function start from identical memory, callees are scripted identically on both sides
(call logs and arguments are compared), and every changed byte outside the stack is compared. Each also has a
committed host contract or check that builds and runs on the host. Hand mutations of the source were run to
show that the comparisons can fail (counts are in the commit messages). The paired-run tooling stays outside
git. Compiled sizes are not image sizes and no byte match is claimed. Addresses are those of the pinned image.

## Imports closed (23 of the 32 assigned)

Confirmed by one private full partial build (the shared branch plus the 25 entries below, plan kept outside
git, 6260 inputs, 4942 function bodies, zero COMMON, one link):

`FindIopFunction__FUi`, `GetRomName`, `VIGetFeatureFlags__Fv`, `__frame_state_for`, `__kernel_rem_pio2`,
`a.1122`, `adddate`, `ezNetCtlGetStatus`, `fabs`, `frombcd`, `gRomName`, `gsm_decode`, `gsm_encode`, `inflate`,
`inflateEnd`, `inflateInit_`, `sceLseek`, `sceVu0ApplyMatrix`, `sceWrite`, `subdate`, `throw_helper`, `tobcd`,
`upcast__C16__user_type_infoRC9type_infoPvPPv`.

`gsm_decode`, `gsm_encode` and `upcast__C16__user_type_infoRC9type_infoPvPPv` were authored by
`w13-startup-fn-a` but left out of the shared plan for lack of a committed check. This package adds the checks
(`w14-startup-sys-gsm-decode`, `-gsm-encode`, `-upcast`, host checks that include the existing sources) and
repeats their paired runs on the merged tree (zero mismatches); their plan entries are in the JSON below.

Total unresolved imports: 957 before and 957 after, because the new bodies' callees are now visible (below).

## Imports opened (23)

`Gsm_LPC_Analysis`, `Gsm_Long_Term_Predictor`, `Gsm_Long_Term_Synthesis_Filtering`, `Gsm_RPE_Decoding`,
`Gsm_RPE_Encoding`, `Gsm_Short_Term_Analysis_Filter`, `Gsm_Short_Term_Synthesis_Filter` (the two GSM units);
`__unwinding_cleanup`, `copy_reg`, `decode_uleb128`, `execute_cfa_insn`, `extract_cie_info`,
`find_exception_handler`, `find_fde`, `get_reg_addr`, `next_stack_level`, `old_find_exception_handler`
(`__frame_state_for`, `throw_helper`; all local routines of the original's exception support, kept under their
symbol-table names); `inflateInit2_`, `inflate_blocks`, `inflate_blocks_free`, `inflate_blocks_reset` (zlib);
`scalbn`, `floor` (`__kernel_rem_pio2`, soft-double signatures: they take and return the 64-bit pattern, so a
provider must match that, not the host `double` signature).

## Not closed (9) and why

* `_gp`, `_fbss`, `_bssend`, `_end`, `_srsctorsstart`, `_srsctorsend`: values defined by the linker script, not by
  any translation unit (see `w12-startup-rt-a.md`). Nothing to author without an approved link layout.
* `__EH_FRAME_BEGIN__`: the authored units (`w11-crt-a-eh-frame-begin`, `w12-startup-rt-a-eh-frame-begin`) stay
  unplanned because the relocatable link asserts or needs an unreviewed section ordering. Not retried here.
* `osdsrc` (1960 bytes at 0x00444a38) and `tlbsrc` (816 bytes at 0x004459d0): blobs whose bytes would have to be
  published to define them; no independently authored equivalent is known. Not attempted.

## Functions (observed address, what was checked)

| Symbol | Address | Notes |
|---|---|---|
| `frombcd`, `tobcd` | 0x002d1360, 0x002d1330 | Clock-library BCD byte converters. The original masks its argument register itself, so the parameter is declared `unsigned` (a `unsigned char` parameter lets the compiler drop the mask; the first attempt mismatched for registers with bits above 8). 256 values plus wide registers. |
| `adddate`, `subdate` | 0x002d1450, 0x002d1508 | Day increment/decrement over clock-record bytes 5 (day), 6 (month), 7 (year 0..99). Month table 31 28 31 30 ... read from 0x0047b158; leap rule year % 4 == 0 (year 0 counts). Paired over 1200 dates; host contracts walk all 36525 days of a century. Returned register is garbage in the original (void routine) and is not compared. |
| `fabs` | 0x002f7e50 | Soft-double absolute value: clears bit 63 of the 64-bit pattern. 313 patterns. |
| `GetRomName`, `gRomName` | 0x002d0ff8, 0x00444858 | One-shot read of 14 bytes of the file `rom0:ROMVER` into the 15-byte object. `gRomName` is zero in the image's `.data`; the compiler puts the authored definition in zero-initialised storage (placement unknown). |
| `sFeatureFlags`, `VIGetFeatureFlags__Fv` | 0x0118e540, 0x010001a0 | Word read. `VISetFeatureFlags__FUi` (0x010001b0) is not authored (no caller). |
| `a.1122` | 0x007d0568 | Per-player fade alpha of the quick-spell menu; extent 16 bytes from label spacing only. |
| `sceLseek`, `sceWrite` | 0x002d9700, 0x002d9b98 | File-system client calls in the style of the authored `sceRead` (message to the file server through `sceSifCallRpc`, functions 4 and 3). `sceWrite` also copies the unaligned head of the buffer (up to the next 16-byte boundary, capped at the size) into the message through the uncached alias. 600 and 700 scenarios each. |
| `sceVu0ApplyMatrix` | 0x01348500 | VU0 vector times matrix, interrupts bracketed by DIntr/EIntr. Inline assembly as in the authored matrix routines. 430 scenarios with a VU0 macro-mode model. |
| `FindIopFunction__FUi` | 0x011270d8 | Walks the pending-command buffer at 0x011edb00 (count, then code/length/words). 900 states. |
| `m_cd`, `ezNetCtlGetStatus` | 0x011e8500, 0x010bfcf0 | eznetctl status query (function 3, 272-byte record sent and received). `m_cd` is published as one global (64 bytes by label spacing). |
| `inflateInit_`, `inflateEnd`, `inflate` | 0x010a3720, 0x010a3588, 0x010a2f30 | zlib 1.1.x wrapper level. `inflate` is the header/dictionary/trailer state machine over the state word (14 states) with `inflate_blocks` and `inflate_blocks_reset` scripted: 3000 states, 315 of 321 original words covered, 11 mutations caught. |
| `__frame_state_for` | 0x002fd0f0 | Unwind-table driver (find the descriptor, interpret the common entry and the descriptor up to the address, copy 1144 bytes out). 110 of 110 words covered. |
| `throw_helper` | 0x002face0 | Handler search and unwind replay behind `__throw`. 224 of 227 words covered. |
| `__kernel_rem_pio2` | 0x002f5eb8 | Double-precision large-argument reduction, every operation one soft-double helper call in the original's order. 703 of 723 words covered over five input styles. |

## Build-plan entries needed

`docs/agents/w14-startup-sys-plan-entries.json` holds 25 entries, without `input_order`, in the plan's format,
hashes taken from this branch's last commit. 22 are this package's units (`authored_cpp`, unchanged profile, no
header closure; the data units have an empty `emitted_functions`). The last three point at the existing
`w13-startup-fn-a` sources for `gsm_decode`, `gsm_encode` and `upcast__C16__user_type_infoRC9type_infoPvPPv`.
Select only one provider per symbol: the older `w12-startup-rt-b-gsm-frame-*` and `w11-crt-b-lg-codec-*`
units define `gsm_*` too. Host checks and contracts (`host_check.cpp`, `host_contract.cpp`) are not plan inputs.

## For other owners (not changed here)

* `w13-startup-fn-a-throw` declares the stack adjustment as `long` (4 bytes under `-mlong32`), but the original
  `throw_helper` stores 8 bytes through that pointer (`sd`) and the original `__throw` keeps a 64-bit slot there.
  Linked with the authored `throw_helper` the 4-byte local would be overrun. The adjustment slot needs to be
  64-bit in `__throw`.
* `m_cd` (0x011e8500) is now one global. `w8-startup-sce-b-init-ez-net-ctl` and `vi-launcher-eznet-down` keep
  file-local copies, so they bind and call a different object at link time. They should refer to the global.
* `inflateInit_` and `inflate` assume the 56-byte stream record of `uncompress` (state pointer at offset 28,
  release function at 36, opaque at 40, message at 24, adler at 48); `inflateInit2_` is not authored.

## Authored but never executed

Nothing here has run on a console or in an emulator. The only execution is the private interpreter's paired run
of single functions against the image with scripted callees, and host-side contracts. In particular nothing
exercises: the real `get_iob`/`_sceFsWaitS`/`_sceFsSigSema` and the IOP file server behind `sceLseek` and
`sceWrite` (their callees were scripted); the VU0 instruction sequence on hardware (a macro-mode model was
used, round to nearest); the real soft-double helpers behind `__kernel_rem_pio2` (exact IEEE operations were
scripted on both sides); the real block decoder behind `inflate`; the unwinder helpers behind `__frame_state_for`
and `throw_helper` (and `__terminate`, which is expected not to return: with a returning stub the two versions
agree, but that path is not a real behaviour); the real clock records and ROM file.
`__kernel_rem_pio2`: nine of about 1000 random inputs ran away in both versions (out-of-range stack reads in
the digit loop, step limit hit by both); they are not counted as agreement.

## Unknown

* The meaning of most words of the 1144-byte frame record (only the offsets read here: 0, 4, 24, 1018, 1020+),
  and of the 32-byte `info` record of `extract_cie_info` beyond offsets 0, 4 and 16.
* Placement of `gRomName` (`.data` in the image, zero) and whether `a.1122` and `m_cd` have the extents given
  (both are label spacing only).
* Whether the original `fabs`, `frombcd`, `tobcd` were built with the compiler that keeps argument masks;
  the authored versions only reproduce the observable behaviour.
* The `z >= 2^24` branch of `__kernel_rem_pio2` (about 40 words) was reached only by forcing a scripted
  `scalbn` result; it may be unreachable from real inputs.
* What the 9 remaining assigned symbols need (linker layout decisions, the two blobs).
