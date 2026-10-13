# Startup, C runtime and IOP/pad/memory-card library (2 of 3)

Branch `agents/w11-crt-b`. Private reference: `work/private/SLUS_205.65`; addresses below are symbol
values of that file. No game bytes, disassembly or decompiler output are committed. Each unit was
compiled with the reviewed EE g++ `authored_cpp` flags. Host checks beside each source are not EE
inputs. A function counts as done only when it has its own commit, a host check that fails if the
arithmetic or stores are wrong, and a paired run: the original function and the authored object
run in a private R5900 interpreter (kept outside git) from identical memory with identical callee
stubs, and the call logs and every changed byte are compared. Each paired run was also repeated with
one deliberate mutation to confirm it reports a difference. Nothing ran on the EE, and nothing here
is a byte match.

## Checks performed

- Every unit compiles alone with the reviewed flags.
- A private copy of the plan (outside git) with the 16 new inputs completed a full partial build:
  4,470 sources compiled, one link, 3,414 function bodies, 947 unresolved imports (the same tree
  without them: 957), zero COMMON symbols.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, all passed.
- The interpreter copy needed `mult1` (the pad library uses it); it was added privately.

## Imports closed (by units authored here)

`litodp`, `dptoli`, `dptoul`, `lgCodecGSMEncode`, `lgCodecGSMDecode`, `lgKbdEnumHint`,
`lgKbdSetRepeatRate`, `lgKbdEnumerate`, `lgKbdOpen`, `lgKbdRead`, `_iopnotify`, `_send_to_iop`,
`mceIntrReadFixAlign`, `dst`, `norrath_lgkbm_reader_thread_record_view`,
`norrath_lgkbm_reader_thread_stack_view`, `norrath_lgaud_rpc_client_view`,
`norrath_lgaud_async_record_view`. (Confirmed by diffing the two `unresolved-imports.json` files.)

## Names in scope that I did not close, and why

Already defined by an authored source that the build plan does not select (not authored a second
time; the integrator needs to add these existing inputs):
- `_sceSifMInitCheck`: `w10-startup-b-sif-m-init-check`.
- `ee_retbuf_sel`: `w10-startup-b-fs-reply-selector`.
- `_kExitTLBHandler`: `w8-startup-sce-b-k-exit-tlb-handler` (assembly; in the snapshot, not the plan).
- `norrath_archive_vblank_count_view`, `..._field_view`, `..._semaphore_view`: `sony-cd-vblank-views`.
- `norrath_inventory_exit_sound_resource_view`: `w9-hud-inventory-items-b-view-exit-sound-resource`.
- `norrath_libmtap_change_priority_client_view`, `norrath_libmtap_get_connection_client_view`:
  `simloop-services-pad-mtap-state`.

Defined in the plan under another name (a symbol alias is needed; the build forbids them):
- `_malloc_r` is `norrath_player_malloc_reentrant` (`game-player-malloc-build-v2`), and `_free_r` is
  `norrath_player_free_reentrant` (`game-player-free-build`). Same code at 0x00301840 and 0x00300608.
  I did not add a forwarding wrapper, because that would be an extra function the original lacks.
- `norrath_inventory_message_handler_word_view` is the word `invMsgHandle` (0x00486a88) already defined
  by `items-player-inventory-init-net`; see `docs/agents/w9-hud-inventory-items-b.md` for the alias line.

Not authorable as source:
- `_fbss` (0x00486800, start of small bss), `_gp` (absolute 0x004879f0), `_srsctorsstart` (0x008eab1c)
  and `_srsctorsend` (0x008eab58) are linker-script boundary symbols. `items-player-pic-init` also
  assigns to `_fbss` as if it were a variable, because the first small-bss object sits at that address;
  this needs a link-script decision, not a definition.

Held / blocked:
- `amx_Exec` (0x00135de8, 9,176 bytes): the interpreter draft keeps its three `#error` holds
  (frame/register/lifetime transport unresolved). I did not touch it; the script-amx owner decides.
- `mcErrOn` (0x01190c30, 4 bytes): observed only as a symbol-table entry whose address lies outside every
  loaded section of the reference executable, so its initial value cannot be read. `w10-vi-c-wnd-setfocus`
  reads it as a flag. Defining it as zero would be a guess, so it stays open.

## Imports opened

`IsEventModifier` (0x002e8d88), `initialise_event_list` (0x002e8dd0), `lgFrameEncode` (0x002eb2b8),
`lgFrameDecode` (0x002eb4a0), `gsm_frame_encode` (0x002eb740), `gsm_frame_decode` (0x002eb768),
`sceSifSetSreg` (0x002d7178), `SyncDCache` (0x002d59d8). All are original-image symbols. Other
imports used by these units were already unresolved or defined (`GLibData`, `WaitSema`, `SignalSema`,
`iSignalSema`, `semaid_timerfunc`, `__pack_d`, `__unpack_d`, `scePrintf`, `sceSifDmaStat`,
`sceSifSetDma`, `_sceSifMInitCheck`, `norrath_libpad_port_info_view`, `norrath_libpad_is_warning_word`).

## Build-plan entries needed

Sixteen inputs, profile `authored_cpp`, kind `independently_authored_source`, empty header closure,
`input_order` left to the integrator. The complete ready-made entries (with source SHA-256 and emitted
function extents from the private build) are in `docs/agents/w11-crt-b-plan-entries.json`. Host checks
(`host_check.cpp`) are not inputs. Bytes are compiled body sizes.

| function | source | bytes |
|---|---|---|
| `dptoli` | `sources/w11-crt-b-dptoli/dptoli.cpp` | 164 |
| `dptoul` | `sources/w11-crt-b-dptoul/dptoul.cpp` | 152 |
| `litodp` | `sources/w11-crt-b-litodp/litodp.cpp` | 164 |
| `lgCodecGSMEncode` | `sources/w11-crt-b-lg-codec-gsm-encode/lg_codec_gsm_encode.cpp` | 84 |
| `lgCodecGSMDecode` | `sources/w11-crt-b-lg-codec-gsm-decode/lg_codec_gsm_decode.cpp` | 84 |
| `lgKbdEnumHint` | `sources/w11-crt-b-lg-kbd-enum-hint/lg_kbd_enum_hint.cpp` | 56 |
| `lgKbdSetRepeatRate` | `sources/w11-crt-b-lg-kbd-set-repeat-rate/lg_kbd_set_repeat_rate.cpp` | 148 |
| `lgKbdEnumerate` | `sources/w11-crt-b-lg-kbd-enumerate/lg_kbd_enumerate.cpp` | 356 |
| `lgKbdOpen` | `sources/w11-crt-b-lg-kbd-open/lg_kbd_open.cpp` | 328 |
| `lgKbdRead` | `sources/w11-crt-b-lg-kbd-read/lg_kbd_read.cpp` | 608 |
| `_iopnotify` | `sources/w11-crt-b-iopnotify/iopnotify.cpp` | 40 |
| `_send_to_iop` | `sources/w11-crt-b-send-to-iop/send_to_iop.cpp` | 340 |
| `mceIntrReadFixAlign` | `sources/w11-crt-b-mce-intr-read-fix-align/mce_intr_read_fix_align.cpp` | 160 |
| `dst` (data) | `sources/w11-crt-b-dst-word/dst.cpp` | 4 (bss) |
| keyboard thread record and stack (data) | `sources/w11-crt-b-lgkbm-reader-thread-data/reader_thread_data.cpp` | 64 + 6144 (bss) |
| audio RPC client and async record (data) | `sources/w11-crt-b-lgaud-data/lgaud_data.cpp` | 40 + 16 (bss) |

Observed by comparison with the originals' extents: `lgKbdRead` compiled to 608 bytes, the same as the
original; the others differ (for example `litodp` 164 against 188, `mceIntrReadFixAlign` 160 against 152).
Those are not byte matches and none is claimed.

## Evidence notes (observed / inferred)

- Soft-float conversions (`litodp`, `dptoli`, `dptoul`): observed that each works on the 32-byte record
  `__unpack_d`/`__pack_d` use (class +0, sign +4, power +8, fraction +16). `litodp` normalises the
  fraction until bit 60 is set and returns `0xc1e0000000000000` directly for `INT_MIN`. `dptoli`
  saturates at class 4 or power >= 31 and returns 0 for power < 0; `dptoul` returns 0 for any set sign
  (negative infinity included) and `0xffffffff` for positive infinity or power >= 32. Host checks compare
  with the compiler's own conversions (truncation, saturation, NaN as zero).
- GSM adapters: observed pass-through of four words and fixed offsets (+28/+348 and 320 for encode,
  +352/+388 and 33 for decode, then 160, 33, the frame routine and the object). Inferred only from names:
  GSM 06.10 framing.
- Keyboard library: the state object `GLibData` (0x008ebdd8) holds a semaphore id at +44, a record count at
  +64, a running minimum index at +72, a counter at +88, an enable flag at +96, a 64-bit word at +112, a
  word at +120 and a time-function address at +128. Records are 272 bytes from +136; per record +16
  pending result, +20 handle, +24 owner index, +28 state, +38 event count, +40 events (16 bytes each),
  +200 saved event, +212 its kind, +216 due time. States seen: 1 (free, claimed by Enumerate), 2 (claimed),
  4 (open), 6; the meaning of the states is inferred only. In `lgKbdRead`, the delivered-saved-event path
  (due-time repeat, then the word at +112 as the next interval, halfword 3 stored) behaves like a key
  repeat; that reading is inferred. `lgKbdOpen` stores the counter before releasing the semaphore (the
  store is in the call's delay slot), which the paired run's hook checks.
- `lgKbdSetRepeatRate`: the earlier unit comment calls the two clamped values only "time values". From
  `lgKbdRead`: the word at +120 (clamped 100..3000) is added after a batch of events, and the 64-bit word at
  +112 (clamped 20..2000) after a repeat. Which of them is a delay and which a period is inferred, not shown.
- `_send_to_iop`: PadInfo is 28 bytes per slot, 112 per port; entry +4 request object, +8 receiving buffer,
  +12 last queued transfer id. The packet is source, destination, 32, 0. Observed string `libpad: sceSifSetDma
  faild`. The original uses `mult1`; the interpreter now models it.
- `mceIntrReadFixAlign`: reads counts A at +0 and B at +4 through the uncached alias and copies A bytes from
  +16 to the address at +8 and B bytes from +80 to the address at +12, re-reading the counts after each byte.

## Authored but never executed

All sixteen units. They ran only in the private R5900 interpreter against the original function, with the
callees stubbed (semaphores, SIF DMA, the time function, the frame codecs, printf). Not covered: the real
callees, timing, interrupts, a second thread inside the semaphore, the EE data cache, and every
function running in the actual boot. The three data units were only checked for extents and zero value.
The host checks for `lgKbdRead`, `_send_to_iop` and `mceIntrReadFixAlign` run on a 64-bit host, so they use
a host-contract mapping for 32-bit addresses (`lgKbdRead` host check covers status paths only; its delivery
paths are covered by the paired run).

## Unknown

- Purpose and range of the state values 3, 5 and 7+ in the keyboard records; fields +0..+15, +32..+37 of a
  record; what writes the hint word at `GLibData`+100.
- Size of the audio RPC client and the asynchronous record (40 and 16 bytes are the smallest consistent
  with use; the next symbol is 80 bytes after the record), and any extent of the keyboard stack beyond the
  6144 bytes passed to `CreateThread`.
- Value of `mcErrOn`; owner of the words `src` and `dst` (written by `initPosition__Fii`, no reader seen).
- The meaning of the `lgFrameEncode`/`lgFrameDecode` forwarded words and of the codec object layout.
- The exact result of `_iopnotify` on its do-nothing path (a leftover register) and the meaning of Sreg slot 1.
