# SIF module calls and keyboard

Package work is on `agents/sif-keyboard`. New source is under `experimental/sources/sif-keyboard-*`. Host checks sit beside that source and stay out of the EE plan: they include host headers. The reviewed profile is `authored_cpp`. Addresses below were read from the pinned SLUS-20565 symbol table and the instruction bytes.

`engineInit__Ff` imports `initSif__Fv`, `sceSifQueryMaxFreeMemSize`, and `sceSifQueryTotalFreeMemSize`. The two queries are the functions already authored here, at the original addresses and extents, and they are already published under those names. `initSif__Fv` is a different function. It is reconstructed below under that name.

A private copy of this branch's merged plan, kept outside git, appended the four units. That plan by itself compiled 421 authored inputs, defined 477 functions, and left 692 unresolved imports. With these units it compiled 425 authored inputs, defined 485 functions, and left 704 unresolved imports, with zero COMMON. `python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests.

None of these bodies is an original-byte match. The host checks fail if the call order, sizes, failure results, or copied bytes change. No EE service has run.

## Observed

`sceSifQueryMaxFreeMemSize` at 0x002dc420 is 112 bytes. `sceSifQueryTotalFreeMemSize` at 0x002dc490 is 112 bytes. Both read the word `_bind` at 0x00444a0c. That word is file-backed `.data`, st_size 4, and the image value is `0xffffffff`. A negative word returns 0. Otherwise both call `sceSifCallRpc` with mode 0, no send buffer, receive size 4, and no completion function. The client object is the local `cd` at 0x008e1cc0. The receive object is the local `rdata` at 0x008e1d00. The maximum-free operation is 6 and the total-free operation is 7. A negative call returns -1. A nonnegative call returns the first receive word.

`keyboardFree__Fv` at 0x002cb228 and `keyboardRestart__Fv` at 0x002cb248 are 32 bytes each. Free passes the local object `vik` at 0x008d86a0 to `Free__10VIKeyboard`. Restart passes that same object to `Init__10VIKeyboard`.

`sceSifSearchModuleByName` at 0x002dcc68 is 160 bytes. `sceSifUnloadModule` at 0x002dcbd8 is 144 bytes. `sceSifStopModule` at 0x002dc9d0 is 520 bytes. A negative `_lf_bind` result returns `0xffff0000`. Any `_lf_version` result other than 0 returns `0xfffefffc`. Those checks run in that order, and the second call is skipped when the first result is negative.

Search then `strncpy`s the name to offset 8 of the local `_senddata` object at 0x008e1e80, with count 252, and sets offset 259 to 0. Its `sceSifCallRpc` uses the local client at 0x008e2080, operation 9, mode 0, that object as send and receive, send size 512, and receive size 4. A negative call returns `0xfffeffff`. A nonnegative call returns the first word.

Unload stores the identifier in the first word of `_senddata` only after both checks. Its call uses operation 8 and size 4 in both directions. The failure and success results match search.

Stop stores the identifier in the first word. A null argument pointer stores 0 in the second word and copies nothing. A length below 253 is copied to offset 260, and that length is stored in the second word. A length of 253 or more copies 252 bytes to offset 260 and stores 252. The original long copy is inline. This source copies those 252 bytes through `memcpy`. The call uses operation 7, send size 512, and receive size 8. A negative call returns `0xfffeffff` and does not write the result pointer. A nonnegative call reads both words, stores the second through the result pointer, and returns the first.

`launchSRS` calls the two queries four times each, then `keyboardFree__Fv` once, and later search, stop, and unload once each before `keyboardRestart__Fv`. Stop is passed a null argument pointer and a result pointer. `main` also calls both queries twice each.

`initSif__Fv` at 0x0018e6a0 is 2676 bytes. Its original binding is local. The definition is global so `engineInit__Ff` can resolve the name. It calls `sceSifInitRpc(0)` and `sceCdInit(0)`, then repeats `sceSifRebootIop` on `cdrom0:\BG\0MOD\DNAS280.IMG;1` while the result is 0, then repeats `sceSifSyncIop` while the result is 0. It calls `sceSifInitRpc(0)`, `sceSifLoadFileReset`, `sceFsReset`, `sceCdInit(0)`, and `sceCdMmode(2)`. It formats `maxstream=%d` with 1280 into a stack buffer. It then loads twenty modules. Each load formats `cdrom0:\bg\0mod\%s;1` into the local path buffer, folds that text from offset 7, prints `load iop module '%s'\n`, and calls `sceSifLoadModule`. A negative load prints `Can't load iop modules!\n` and calls `exit(0)`. The bytes after that call are the success tail, so a returning `exit` still reaches it. The tail is `PreparePowerOff__Fv`, `textSystemInit__Fv`, `commSystemInit__Fv`, and `sceSifInitIopHeap`.

The module names, in order, are `sio2man.irx`, `mtapman.irx`, `mcman.irx`, `mcserv.irx`, `padman.irx`, `cdvdstm.irx`, `libsd.irx`, `sdrdrv.irx`, `hello.irx`, `usbd.irx`, `lgkbm.irx`, `lgaud.irx`, `dev9.irx`, `netcnf.irx`, `inet.irx`, `inetctl.irx`, `msifrpc.irx`, `libnet.irx`, `eznetctl.irx`, and `eznetcnf.irx`. Plain loads pass a zero length and a null argument. `usbd.irx` passes 43 bytes at 0x004640d0. `lgaud.irx` passes `strlen` of the maxstream text plus one, and that text. `netcnf.irx` passes 94 bytes at 0x00464140. `inetctl.irx` passes 20 bytes at 0x004641c0. Those blobs contain embedded NUL bytes. None of these strings has an ELF symbol.

## Inferred

`_bind` was a local symbol. It is published global because the two queries share that one word and live in one unit. Its image value is the initializer.

The heap `cd` and `rdata` are `.bss`, st_size 0, and local. The next symbols are 64 bytes later (`rdata`, then `sdata`). The loadfile `cd` at 0x008e2080 is a different local object; `_lfversion` is 40 bytes later. `_senddata` is 512 bytes, the span to that client. Those names occur more than once, so this package keeps each object static inside the unit that uses it and does not export `cd`, `rdata`, or `_senddata`.

`vik` is `.bss`, st_size 0, and local. `outMsg` is 536 bytes later. `Init__10VIKeyboard` stores a word at offset 528, so the used prefix is at least 532 bytes. The published object is 536 bytes. Bytes 532 through 535 are only that gap. The wrappers do not read `vik`.

The path buffer is the local `buffer.744` at 0x00790fa0. `draw` is 80 bytes later, and the published object is 80 bytes. It stays static. The fold reads `_ctype_` at 0x0047e1f0, size 257, at index `sign-extended byte + 1`, and subtracts 32 when bit 2 is set. The libc-format unit publishes those table bytes under `norrath_factory_ctype_minimum_view`, not under `_ctype_`, so this unit imports the original name.

The reviewed `-G0` profile produced these body sizes: each query 120 bytes, each keyboard wrapper 12 bytes, search 180, unload 172, stop 288, and `initSif__Fv` 480.

Host checks:

```sh
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/sif-keyboard-query-check \
  experimental/sources/sif-keyboard-iop-heap-query/host_contract.cpp
/tmp/sif-keyboard-query-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/sif-keyboard-key-check \
  experimental/sources/sif-keyboard-keyboard/host_contract.cpp
/tmp/sif-keyboard-key-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/sif-keyboard-module-check \
  experimental/sources/sif-keyboard-module-calls/host_contract.cpp
/tmp/sif-keyboard-module-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/sif-keyboard-init-check \
  experimental/sources/sif-keyboard-init-sif/host_contract.cpp
/tmp/sif-keyboard-init-check
```

## Imports closed

`sceSifQueryMaxFreeMemSize`, `sceSifQueryTotalFreeMemSize`, `keyboardFree__Fv`, `keyboardRestart__Fv`, `sceSifSearchModuleByName`, `sceSifStopModule`, `sceSifUnloadModule`, `initSif__Fv`.

The two query names are the original symbols at 0x002dc420 and 0x002dc490, each 112 bytes. They were not defined a second time. `initSif__Fv` was not in the merged plan's unresolved set, because `engineInit__Ff` is not an input there yet. This unit defines it.

## Imports opened

`_lf_bind`, `_lf_version`, `strncpy`, `Free__10VIKeyboard`, `Init__10VIKeyboard`, `_ctype_`, `PreparePowerOff__Fv`, `commSystemInit__Fv`, `sceCdInit`, `sceCdMmode`, `sceFsReset`, `sceSifInitIopHeap`, `sceSifLoadFileReset`, `sceSifLoadModule`, `sceSifRebootIop`, `sceSifSyncIop`, `sprintf`, `strlen`, `textSystemInit__Fv`.

`sceSifCallRpc`, `memcpy`, `sceSifInitRpc`, and `exit` were already unresolved. `scePrintf` resolved against the merged plan.

## Build-plan entries needed

Append these authored inputs after the current last input. On this branch the merged plan ends at order 422 and `default_authored_inputs` is 421. The private trial used orders 423 through 426, and `default_authored_inputs` becomes 425. Leave the two optional external inputs in place. Do not add any `host_contract.cpp` file. `snapshot.json` must record the same sources, object names, and emitted sizes before `tools/check_source_snapshot.py` will accept the tree. Header closures are empty. Profile `authored_cpp`. If the shared plan gains inputs before integration, renumber these after that new tail.

```json
{
  "header_closure": [],
  "input_order": 423,
  "kind": "independently_authored_source",
  "object_name": "query.o",
  "profile": "authored_cpp",
  "sha256": "0dbc63a16aaaadfa13c16b818500cf92d052eef652e66aa009fb1cd8852039d6",
  "source": "sources/sif-keyboard-iop-heap-query/query.cpp"
}
```

Emitted functions: `sceSifQueryMaxFreeMemSize` 120, `sceSifQueryTotalFreeMemSize` 120. Fresh object SHA-256 `8d98ff5da8fd7ae910492150f89af4dc99df2ff8fadff93e05ea3d2e6364e9cb`.

```json
{
  "header_closure": [],
  "input_order": 424,
  "kind": "independently_authored_source",
  "object_name": "keyboard.o",
  "profile": "authored_cpp",
  "sha256": "baf3788f3af26f3e5615dc334afb4f51785cb813dc220550413bb4da5f7c4c68",
  "source": "sources/sif-keyboard-keyboard/keyboard.cpp"
}
```

Emitted functions: `keyboardFree__Fv` 12, `keyboardRestart__Fv` 12. Fresh object SHA-256 `acabeb04e8eb79fc677b093e29aa12462ef0e1fad78e17515a84fc5af5e66671`.

```json
{
  "header_closure": [],
  "input_order": 425,
  "kind": "independently_authored_source",
  "object_name": "module_calls.o",
  "profile": "authored_cpp",
  "sha256": "e3c35e6d2df618563cc219f14397a3aee372f0bcad06c1aa511197ebb6ca75bd",
  "source": "sources/sif-keyboard-module-calls/module.cpp"
}
```

Emitted functions: `sceSifSearchModuleByName` 180, `sceSifStopModule` 288, `sceSifUnloadModule` 172. Fresh object SHA-256 `dd3b34c5df98b549a18965a0a06cdd4f590c0e7c74263816d16ac06fad064dac`.

```json
{
  "header_closure": [],
  "input_order": 426,
  "kind": "independently_authored_source",
  "object_name": "init_sif.o",
  "profile": "authored_cpp",
  "sha256": "1ddc53e86e167c01dfc1838c4b76a7c85f34c4f071c6894e120c7cb26b198d95",
  "source": "sources/sif-keyboard-init-sif/init_sif.cpp"
}
```

Emitted function: `initSif__Fv` 480. Fresh object SHA-256 `2bdfaf1275fef8642204a38205d44a5e8524834465431d1cddf3c6d50c045e53`.

## Authored but never executed

All eight functions, the `_bind` word, and the static client, receive, send, `vik`, and path-buffer objects. Host checks passed on this Mac, and the reviewed EE compiler produced the relocatable objects above. The private 425-input link checked symbol closure only. No routine ran on the EE, under the original process, or against gameplay.

## Unknown

What operations 6, 7, 8, and 9 do on the IOP, and what a completion function would observe. `_lf_bind` and `_lf_version` are still unresolved; only their result tests are in these functions. `Free__10VIKeyboard` and `Init__10VIKeyboard` are unresolved. Init's stores at offsets 0, 4, 8, 12, and 528 were read, and the rest of `vik` was not. Whether bytes 532 through 535 are padding is not established. The heap receive call uses four bytes; the other 60 bytes in the 64-byte span are not read here. The original stop path copies 252 bytes inline when the length is at least 253. This source uses `memcpy` for those bytes. A negative length is passed through to `memcpy` because the signed compare treats it as below 253. What the reboot image and the twenty modules do on the IOP is unknown. Whether `exit` returns is unknown; the bytes continue into the tail if it does. The path fold's negative-character index reaches before `_ctype_` and is not used by these module names. Interrupts, timing, and the original register spills were not reproduced.
