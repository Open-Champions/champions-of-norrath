# w14-startup-io-a: startup file, load thread, memory card, pad and keyboard-library symbols

Branch `agents/w14-startup-io-a`. One commit per function; the seven data symbols share one commit. Source is under
`experimental/sources/w14-startup-io-a-*`; each directory holds the authored source and a `host_contract.cpp` (host
check, not a plan input). Addresses and sizes are from the pinned private `SLUS_205.65`. Nothing here ran on the EE
hardware or as gameplay.

## How each function was checked

* Each unit compiles with the reviewed EE g++ and the `authored_cpp` flags (single-unit compile; I did not run a full
  partial build). A private copy of the plan with these 21 entries appended passes
  `python3 tools/build_partial.py --check-plan` (6,256 selected inputs).
* Each unit has a native host contract (`c++ -std=c++17`, exit 0) with scripted callees and an independently written
  expectation.
* Each function was also compared with the image in a private paired run (an R5900 interpreter outside git): the original
  and the freshly compiled authored function run from identical memory snapshots with identical stubbed callees; call
  logs, return values (float returns through `$f0`) and every changed byte outside the stack must be equal. Scenario
  counts are in the commit messages. For `ScaleStick`, `VIMemCardStatus`, `CDSync`, and `loadThreadMain` a deliberately
  wrong variant (a constant, a branch limit, a divisor) was run and fails.
* Compiled sizes are not byte matches. They differ from the image sizes for most functions (for example
  `loadThreadMain__FPv` 1040 bytes against 744 in the image and `CheckforModifierKeyEvents` 212 against 400).

## Functions

| Function | Image address | Image bytes | Authored bytes |
|---|---|---|---|
| `ScaleStick__5VIPadUc` | 0x010ff300 | 92 | 96 |
| `DequeueKeyEvent__10VIKeyboardP12VIConsoleKey` | 0x002cc0b0 | 120 | 120 |
| `TranslateEvent__10VIKeyboardP11_lgKbdEvent` | 0x002cbfd8 | 148 | 144 |
| `generate_key_release_events` | 0x002e80d8 | 276 | 280 |
| `generate_key_press_events` | 0x002e81f0 | 272 | 252 |
| `CheckforModifierKeyEvents` | 0x002e8300 | 400 | 212 |
| `VIMemCardStatus__FPi` | 0x010d2ce8 | 364 | 436 |
| `VIMemCardReadFile__FPCcPvi` | 0x010d3a40 | 276 | 288 |
| `Open__6VIFilePCci` | 0x010c9e60 | 356 | 348 |
| `MemorySegmOpen__6VIFileiPQ26VIFile7MemSegmil` | 0x01002350 | 308 | 312 |
| `CDSync__6VIFilei` | 0x010cac38 | 412 | 428 |
| `getPackChecksum__Fv` | 0x0025a7f8 | 612 | 516 |
| `loadThreadMain__FPv` | 0x002cd898 | 744 | 1040 |
| `PrepareQueueKeyEvent__10VIKeyboardP12VIConsoleKey` | 0x002cb9e8 | 504 | 436 |

Each source's header comment records what was observed, inferred and unknown for that function. In short:

* **Keyboard**: `TranslateEvent` scans 74 ten-byte records of `USEnglishKbdMapTable` and picks a column by flags B
  (0x22 shift-like, 0x40 other), upper-casing letters when flags A bit 1 is set. `DequeueKeyEvent` pops a two-byte
  record from a queue at +8 with count at +12. `PrepareQueueKeyEvent` is dead-key composition over `sFrDeadkey`
  (`sUsDeadkeyCount` words). The three keyboard-library routines build 16-byte events at state +40 with the count at
  +38 (limit 10, overflow status 0x8000000d); `CheckforModifierKeyEvents` applies the modifier byte differences.
* **Pad**: `ScaleStick` has a dead band 88 to 167 and scales by the constant 0x3c3a2e8b (one ulp under 1/88).
* **Memory card**: `VIMemCardStatus` polls `sceMcGetInfo`/`sceMcSync` and maps the type, result and format words to a
  small code; `VIMemCardReadFile` wraps the async read with two blocking syncs and a close.
* **File**: `Open__6VIFilePCci` builds `\NAME;1` upper-cased with `/` turned into `\`, searches the disc and allocates
  a 0x8010-byte buffer; `MemorySegmOpen` builds 24-byte records from sixteen-byte descriptors; `CDSync` waits and
  restarts the last read on error for the direct and the IOP-stream path.
* **Load thread**: `loadThreadMain` is the loading-screen drawing loop; `getPackChecksum` folds three `ramSlot`
  regions into a 16-bit sum.
* **Data**: `loadBuffer` (60032), `loadStack` (8192), `loadThreadParam` (48), `loadFrames`, `mcDisableStartupScript`,
  `mcerring`, `_6VIFile$sIopStream` with real symbol names. Sizes of the first three are the gaps to the next symbol.

## Overlap with other work

* `CheckforModifierKeyEvents`, `generate_key_press_events` and `generate_key_release_events` are also authored in
  `experimental/sources/w13-startup-fn-a-*` (not yet in `experimental/build-plan.json` at the time of writing).
  Plan only one copy of each. These three units were checked against the image with 3,000 paired runs each.
* `save-net-anim-views` defines `norrath_mc_load_buffer_bytes` and `norrath_mc_disable_startup_script_bytes`, which
  do not carry the real symbol names; the `loadBuffer` and `mcDisableStartupScript` units here do. Plan only one
  pair, or the storage is duplicated.
* `vi-launcher-init` defines `sProductTitle` as a file-local symbol, so `VIMemCardStatus` and `VIMemCardReadFile`
  cannot bind to it; it stays an import until that definition is made global.

## Imports closed

`CDSync__6VIFilei`, `CheckforModifierKeyEvents`, `DequeueKeyEvent__10VIKeyboardP12VIConsoleKey`,
`MemorySegmOpen__6VIFileiPQ26VIFile7MemSegmil`, `Open__6VIFilePCci`,
`PrepareQueueKeyEvent__10VIKeyboardP12VIConsoleKey`, `ScaleStick__5VIPadUc`,
`TranslateEvent__10VIKeyboardP11_lgKbdEvent`, `VIMemCardReadFile__FPCcPvi`, `VIMemCardStatus__FPi`,
`_6VIFile$sIopStream`, `generate_key_press_events`, `generate_key_release_events`, `getPackChecksum__Fv`, `loadBuffer`,
`loadFrames`, `loadStack`, `loadThreadMain__FPv`, `loadThreadParam`, `mcDisableStartupScript`, `mcerring`.
That is all 21 names in the package.

## Imports opened

Each is referenced by these units and defined elsewhere (some may already be closed by other packages):
`BufferClear__6VIFile`, `CDDiskReady__6VIFilei`, `CDSearchFile__6VIFileP10sceCdlFILEPCc`,
`CombineDeadkey__10VIKeyboardUc`, `GetSaveFootPrint__6Camera`, `MemorySegmCalcSegm__6VIFile`, `ModifierCodes`,
`QueueKeyEvent__10VIKeyboardP12VIConsoleKey`, `SetIcon__16VIIconDefinitionPvi`, `SetTitle__16VIIconDefinitionPCc`,
`USEnglishKbdMapTable`, `VICdStreamDiskReady__Fv`, `VICdStreamRead__FUiUiPvi`, `VICdStreamStatus__Fv`,
`VICdStreamSync__Fi`, `VIEEFileDelay__Fi`, `VIMemCardReadFileAsync__FPCcPvi`, `__builtin_vec_new`, `blackoutTexCopy`,
`drawFrameNm`, `engineDisplayDisable`, `engineFrameEnd__Fb`, `engineFrameStart__FUib`, `engineSetDisplayDisable__Fi`,
`foregrndTexCopy`, `gameCam`, `jumpGateSaveFootPrint__Fv`, `loadingScreenRequestExit`, `printf`, `ramSlot`,
`sAsyncFd`, `sFrDeadkey`, `sIconDefinition`, `sListIcon`, `sMemCardInitialized`, `sProductTitle`,
`sUsDeadkeyCount`, `sceCdDiskReady`, `sceCdGetError`, `sceCdRead`, `sceCdSync`, `sceGsSyncV`, `sceMcClose`,
`sceMcGetInfo`, `sceMcSync`, `scrollmapTexCopy`, `snprintf__8VIStringPciPCce`, `strlen`, `strupr__8VIStringPc`,
`texNormalizedHeight__FP8_texture`, `texNormalizedWidth__FP8_texture`, `textureDrawColor__FP8_textureiiiUi`,
`textureDrawFloat__FP8_textureffiUiff`, `textureDrawSuperColor__FP8_textureiiiUiiiiicc`, `titleloadTexCopy`.

## Build-plan entries needed

Append after the current last input, in this order. Profile `authored_cpp`, empty header closure. Do not plan
`host_contract.cpp`; `snapshot.json` must record every file before `tools/check_source_snapshot.py` accepts the tree.

```json
{"source":"sources/w14-startup-io-a-data-file-iop-stream/iop_stream.cpp","object_name":"iop_stream.o","sha256":"594de9940da8919c181296474877ff0860306c1bc9a58668ba2b4da9ae42fc8d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w14-startup-io-a-data-load-buffer/load_buffer.cpp","object_name":"load_buffer.o","sha256":"6eae881cf70eabbbb6e2114920c891db6ab32c8e10e3753c9dc510cb79aa928e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w14-startup-io-a-data-load-frames/load_frames.cpp","object_name":"load_frames.o","sha256":"67d2e65c6d3b802474809a7199b260f2f8e1626cbfb4f888dcbb9444a52be9d4","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w14-startup-io-a-data-load-stack/load_stack.cpp","object_name":"load_stack.o","sha256":"da7b40b2d25cce9b7337ebcb23d53d7c160ca7b5bfd9e33bd7d9c8ed9f963737","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w14-startup-io-a-data-load-thread-param/load_thread_param.cpp","object_name":"load_thread_param.o","sha256":"7a18c9d247eaa59164d80efb509fcc724106dc2315b1504d9b79fc023dee8cf1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w14-startup-io-a-data-mc-disable-startup-script/mc_disable_startup_script.cpp","object_name":"mc_disable_startup_script.o","sha256":"9546a74308cf5244c5d6ba27b89644b14d3cd49452eae6cccd16f4e0c2cdf003","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w14-startup-io-a-data-mcerring/mcerring.cpp","object_name":"mcerring.o","sha256":"2d8a6b1c7394c541fcd6a1d1568f56049cdb3c5852282bf1cdf869b801920b98","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w14-startup-io-a-file-cdsync/cdsync.cpp","object_name":"cdsync.o","sha256":"30455169e34f03069dcdeef23d4370a0078ccae61203cec6dcfe0e7a093cffd4","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"CDSync__6VIFilei","compiled_body_bytes":428}]}
{"source":"sources/w14-startup-io-a-file-memseg-open/memseg_open.cpp","object_name":"memseg_open.o","sha256":"398494f02eca53a06a32b6b05ba1abd401c929a76274cfd25a6534a0cd060e34","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"MemorySegmOpen__6VIFileiPQ26VIFile7MemSegmil","compiled_body_bytes":312}]}
{"source":"sources/w14-startup-io-a-file-open-path/open_path.cpp","object_name":"open_path.o","sha256":"860d595954f894bd6e13e679f03247be72bc2382749773b78d2072920769f1e5","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Open__6VIFilePCci","compiled_body_bytes":348}]}
{"source":"sources/w14-startup-io-a-kbd-lib-modifier/modifier.cpp","object_name":"modifier.o","sha256":"9f35b9df39634c987739ba4db1cf550a94cd7a1855319495b9687594835f935f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"CheckforModifierKeyEvents","compiled_body_bytes":212}]}
{"source":"sources/w14-startup-io-a-kbd-lib-press/press.cpp","object_name":"press.o","sha256":"aad81aceec3439c449197bf024b7b037490c879f03e8f8c23bc207d3ea39c292","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"generate_key_press_events","compiled_body_bytes":252}]}
{"source":"sources/w14-startup-io-a-kbd-lib-release/release.cpp","object_name":"release.o","sha256":"a53ddec97ecd0edada52bc26664df9531a9cb1d47b451e6aa0cc080bb9918222","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"generate_key_release_events","compiled_body_bytes":280}]}
{"source":"sources/w14-startup-io-a-keyboard-dequeue/dequeue.cpp","object_name":"dequeue.o","sha256":"ed6d7c37336ddeb14e021422eefe338217250abddb0e84cc724b7b71592e04be","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DequeueKeyEvent__10VIKeyboardP12VIConsoleKey","compiled_body_bytes":120}]}
{"source":"sources/w14-startup-io-a-keyboard-prepare-queue/prepare_queue.cpp","object_name":"prepare_queue.o","sha256":"d87fe66ca335aeec6d33dad77ff46d1b67486425957c6b87d349abc87def484c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"PrepareQueueKeyEvent__10VIKeyboardP12VIConsoleKey","compiled_body_bytes":436}]}
{"source":"sources/w14-startup-io-a-keyboard-translate/translate.cpp","object_name":"translate.o","sha256":"124ec4e904550dddf128451edc7156972ded34901e4fff63885a8c289ec73da2","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"TranslateEvent__10VIKeyboardP11_lgKbdEvent","compiled_body_bytes":144}]}
{"source":"sources/w14-startup-io-a-load-thread-main/load_thread_main.cpp","object_name":"load_thread_main.o","sha256":"4922ac378dc71806a1945fa4fb440f748a4d040d586f2ac08f053757c0f7aa72","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"loadThreadMain__FPv","compiled_body_bytes":1040}]}
{"source":"sources/w14-startup-io-a-memcard-read-file/read_file.cpp","object_name":"read_file.o","sha256":"6f6d72f9d93162b8b2b47d8e911421677d003f72323a82676c2ced067981e8f1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"VIMemCardReadFile__FPCcPvi","compiled_body_bytes":288}]}
{"source":"sources/w14-startup-io-a-memcard-status/status.cpp","object_name":"status.o","sha256":"9497c83f1ea746c57f2893c3c3762f291abe196bab39f9f5944bb1a260da639b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"VIMemCardStatus__FPi","compiled_body_bytes":436}]}
{"source":"sources/w14-startup-io-a-pack-checksum/pack_checksum.cpp","object_name":"pack_checksum.o","sha256":"a981078c7c1ce5751665c30a95d01d95ef10ca3aec04e4273f6089c9015761ec","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"getPackChecksum__Fv","compiled_body_bytes":516}]}
{"source":"sources/w14-startup-io-a-pad-scale-stick/scale_stick.cpp","object_name":"scale_stick.o","sha256":"f8cf48f5053d116f9906b659dd04837c54cefabdda9cf2f35679135580ae71c1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"ScaleStick__5VIPadUc","compiled_body_bytes":96}]}
```

## Authored but never executed

All 14 functions and 7 data symbols. The functions were exercised only on the host against scripted callees and in the
private interpreter against the image with stubbed callees. Nothing ran on the EE, with the real SDK, a real memory
card, a disc, a keyboard or the game. Data symbols were only compiled and size-checked; where the image keeps a symbol
in initialized data (`loadFrames`, `mcDisableStartupScript`) the authored definition lands in zero-filled storage
because the zero initializer does not select the data section.

## Unknown

* Whether any compiled body matches the original instruction for instruction; sizes differ for most.
* Member and field names: the keyboard state and event fields, the VIFile fields at +4, +8, +16, +32 to +56, +72,
  +108 to +128 and +184 to +188, the texture record fields at offsets 0 and 2, and the 36-byte record stride in
  `getPackChecksum`.
* The meaning of the return codes of `VIMemCardStatus`, the mode bits 8 and 0x10 of `MemorySegmOpen`, the
  `textureDrawSuperColor` numeric arguments, and the added constant 71994 in `getPackChecksum`.
* The real extents of `loadBuffer` and `loadThreadParam` (they are upper bounds from the symbol gaps) and the
  readers of `loadFrames` and `mcerring`.
* Whether `USEnglishKbdMapTable` really has 74 records and what the unused record bytes hold.
