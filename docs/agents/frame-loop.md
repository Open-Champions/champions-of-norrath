# Frame start, frame end, and the display list

Package work is on `agents/frame-loop`. New source is under `experimental/sources/frame-loop-*`. Host checks sit beside that source and include host headers, so they stay out of the EE plan. The reviewed EE profile is `authored_cpp`. A private plan was not committed.

The baseline partial link of this branch, before these units, has 789 unresolved imports and no COMMON. It does not name `engineFrameEnd__Fb`, `engineFrameStart__FUib`, `engineSetDisplayDisable__Fi`, `engineBrightnessCorrection`, `frameNm`, `glistp`, `gstart`, `GLISTSIZE`, or `coreClearColor`, because `simpleDrawLoop` is not on this branch. Those names are the engine-prefixed imports opened by `origin/agents/game-boot`, plus the five display-list words from that same list. `engineRemoveTask__FPFv_v` is not in this package.

The other unresolved `engine*` names on this branch stay with their owners: `engineAddTask__FPFv_viPci` and `enginePrintTasks__Fv` (scheduler), `engineClear__Fv` and `engineRestore__Fv` (engine-restore), `engineSetScreenColor__Fiiii` (render-lights), and the storage `engineInit` already imports.

Observed addresses use the pinned `SLUS_205.65` image. GP is `0x004879f0`. Fresh EE compilation is not an original-byte match.

## engineFrameEnd__Fb

Observed at `0x00190a48`, 732 bytes. The reviewed EE body is 1020 bytes. `simpleDrawLoop` calls it with 0.

A zero `coreLoadingScreenRunning` byte scans 5120 bytes at `0x01fc0000` for `0xA5`. A mismatch calls `scePrintf("stack error!  %s:%d\n", "engine.cc", 1245)` and stores zero at address 0, then continues. The EE object keeps that `sw`. A nonzero loading byte, including `0x80`, skips the scan.

The rest is `drawWaitFinished`, `sceGsSyncPath(0, 0)`, an increment of `drawFrameNm` before `texturePackDown`, then the half-frame counter. `fullFrameMode == 0` clears `switchToHalfCounter.797`. Otherwise the word at `0x10000000` is compared, as signed, with 9767. A larger value clears the counter. A smaller value stores `counter - timer + 9766` and, when that sum is not signed-less than 29299, calls `engineSetVideoMode(0)`.

`draw` at `0x00790ff0` supplies the words. ELF size 0; `dispEnv` is 64 bytes later. Offset 60 shifted left by 4 and added to 28 selects the gate word. Nonzero calls `WaitSema(vblankSemaphore)` before `PollSema`. `PollSema` repeats while its result equals `vblankSemaphore`. `DIntr` follows.

Word 16 nonzero, or else word 28 nonzero, waits again later and uses the even pair at offsets 24, 28, 32, and 36. Both zero compares word 52 with word 36 as signed. The smaller side selects the slot compared by xor with word 56, and selects the pair 16 bytes higher when word 52 is smaller. A copied nonzero word 16 clears offset 52 when the selector is 0 and offset 36 when it is 1. Offset 12 is cleared. `EIntr` runs after those stores. The saved second argument of `doSuperSample` is word 16 reloaded after the stores.

A later wait clears `switchTo30Counter.798`. The other arm increments it and, when the old signed value is at least 31, calls `engineSetVideoMode(1)` and clears it. `engineDisplayDisable` decreases only when positive. The timer word is cleared after `(timer + 4884) / 9766` is stored in `lastFrameVBlankCount`, and that quotient is stored before `doSuperSample`. Nonzero `powerOffMode` stores 1000 and does not return: it waits on `vblankSemaphore2` while `vblankCount` is unchanged, loads `vblankField`, and repeats.

```sh
c++ -std=c++17 -O2 -fwrapv -Wall -Wextra -Werror -fsanitize=address,undefined \
  -I experimental/sources/frame-loop-frame-end \
  experimental/sources/frame-loop-frame-end/host_check.cpp -o /tmp/frame-end-check
/tmp/frame-end-check
```

## engineFrameStart__FUib

Observed at `0x00191200`, 252 bytes. The reviewed EE body is 312 bytes. `simpleDrawLoop` calls it with `coreClearColor` and 0. The second argument is passed through unchanged.

`lightVU0StoppedProcessing` runs first. `frameStartPacket` receives the color, that second argument, and `draw` word 8 from before `FlushCache(0)` and `drawStart`. A nonzero word 0 after those calls copies the reloaded words 8 and 4 to words 20 and 16. A zero word 0 stores zero in both. Word 12 becomes 1. Word 0 ends as 1; a zero store immediately before that 1 has no call between them.

`gstart` and `glistp` both receive `0x0038f9e0 + (drawFrameNm & 1) * 0x445c0`, with `0x30000000` set. Even is `0x3038f9e0`. Odd is `0x303d3fa0`. `drawFrameNm` is loaded after `drawStart`. `fullFrameMode` zero stores the sign-extended `vblankField` byte in word 8. Any other value stores 0. Word 4 then receives `fullFrameMode`.

```sh
c++ -std=c++17 -O2 -fwrapv -Wall -Wextra -Werror -fsanitize=address,undefined \
  -I experimental/sources/frame-loop-frame-start \
  experimental/sources/frame-loop-frame-start/host_check.cpp -o /tmp/frame-start-check
/tmp/frame-start-check
```

## engineSetDisplayDisable__Fi

Observed at `0x001911f8`, 8 bytes. The reviewed EE body is 12 bytes. It stores the argument in `engineDisplayDisable` and returns. `simpleDrawLoop` calls it with 2 and later with 1.

```sh
c++ -std=c++17 -O2 -fwrapv -Wall -Wextra -Werror -fsanitize=address,undefined \
  -I experimental/sources/frame-loop-display-disable \
  experimental/sources/frame-loop-display-disable/host_check.cpp -o /tmp/display-disable-check
/tmp/display-disable-check
```

## Display-list words

These six objects are 4 bytes in `.sdata`. Under `-G0` the three zero words land in BSS and the three nonzero words land in `.data`. The EE bytes match the file bytes. None of these units emits a function. `engineFrameStart` writes `gstart` and `glistp`. `simpleDrawLoop` writes `frameNm` and reads `coreClearColor`. The pause-menu check reads `GLISTSIZE`, `glistp`, and `gstart`. `gameInit` later replaces `engineBrightnessCorrection` from `MC_GameConfig+4`; this unit is the initial file word.

| Symbol | Address | File bytes | Value |
| --- | --- | --- | --- |
| `frameNm` | `0x004844a4` | `00 00 00 00` | 0 |
| `gstart` | `0x004844c4` | `00 00 00 00` | 0 |
| `glistp` | `0x004844c8` | `00 00 00 00` | 0 |
| `GLISTSIZE` | `0x00484468` | `b8 88 00 00` | 35000 |
| `coreClearColor` | `0x0048667c` | `00 00 00 80` | `0x80000000` |
| `engineBrightnessCorrection` | `0x00484420` | `00 00 00 3f` | `0x3f000000` |

Host checks compare those bytes and fail if a one-byte change still compares equal. Example:

```sh
c++ -std=c++17 -O2 -Wall -Wextra -Werror -fsanitize=address,undefined \
  -I experimental/sources/frame-loop-glist-size \
  experimental/sources/frame-loop-glist-size/host_check.cpp -o /tmp/glist-size-check
/tmp/glist-size-check
```

The same command shape was used for `frameNm`, `glistp`, `gstart`, `coreClearColor`, and `engineBrightnessCorrection`.

## Imports closed

`engineFrameEnd__Fb`, `engineFrameStart__FUib`, `engineSetDisplayDisable__Fi`, `engineBrightnessCorrection`, `frameNm`, `glistp`, `gstart`, `GLISTSIZE`, `coreClearColor`.

`engineRemoveTask__FPFv_v` is not here. The scheduler package defines it.

## Imports opened

`DIntr`, `EIntr`, `FlushCache`, `PollSema`, `WaitSema`, `coreLoadingScreenRunning`, `doSuperSample__Fiib`, `draw`, `drawFrameNm`, `drawStart__Fv`, `drawWaitFinished__Fv`, `engineDisplayDisable`, `engineSetVideoMode__Fb`, `frameStartPacket__FUibi`, `fullFrameMode`, `lastFrameVBlankCount`, `lightVU0StoppedProcessing__Fv`, `powerOffMode`, `sceGsSyncPath`, `scePrintf`, `switchTo30Counter.798`, `switchToHalfCounter.797`, `texturePackDown__Fv`, `vblankCount`, `vblankField`, `vblankSemaphore`, `vblankSemaphore2`.

`scePrintf`, `DIntr`, `EIntr`, `WaitSema`, `PollSema`, and `FlushCache` already exist as names elsewhere. These units reference them and do not define them. `engineDisplayDisable` stays unresolved; its file word is 3, and this package does not own that object.

## Build-plan entries needed

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "frame_end.o",
  "profile": "authored_cpp",
  "sha256": "488cd2dd60133947a888e5a2d300e5ca8a1cda3d1f01210babaa402fe9445363",
  "source": "sources/frame-loop-frame-end/frame_end.cpp",
  "emitted_functions": [
    {"name": "engineFrameEnd__Fb", "compiled_body_bytes": 1020}
  ]
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "frame_start.o",
  "profile": "authored_cpp",
  "sha256": "6902c57b116f1a66aeae62408fea11c95b9e2c787685335c25e450f04e5a7177",
  "source": "sources/frame-loop-frame-start/frame_start.cpp",
  "emitted_functions": [
    {"name": "engineFrameStart__FUib", "compiled_body_bytes": 312}
  ]
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "set_display_disable.o",
  "profile": "authored_cpp",
  "sha256": "d883c1b36b6b80895c06ed1b79f930a0c2e23a93af436b536c9965b647ff1073",
  "source": "sources/frame-loop-display-disable/set_display_disable.cpp",
  "emitted_functions": [
    {"name": "engineSetDisplayDisable__Fi", "compiled_body_bytes": 12}
  ]
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "frame_nm.o",
  "profile": "authored_cpp",
  "sha256": "85f69867540303854ead89778bc07a36c069f551c19ec6544954cd0dd441f89c",
  "source": "sources/frame-loop-frame-nm/frame_nm.cpp",
  "emitted_functions": []
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "glistp.o",
  "profile": "authored_cpp",
  "sha256": "684051e9f824df564a6a0d2be8fbc00add99d3aaa63ef400d969b5ea2da9db6f",
  "source": "sources/frame-loop-glistp/glistp.cpp",
  "emitted_functions": []
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "gstart.o",
  "profile": "authored_cpp",
  "sha256": "73545183c1fb7589a9428f9673bca9f967073420df222a429ce99220a8be340a",
  "source": "sources/frame-loop-gstart/gstart.cpp",
  "emitted_functions": []
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "glist_size.o",
  "profile": "authored_cpp",
  "sha256": "97109965d0378b58114473ef0805b2b2b407e0d264fe5b64f39353620cc6b147",
  "source": "sources/frame-loop-glist-size/glist_size.cpp",
  "emitted_functions": []
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "clear_color.o",
  "profile": "authored_cpp",
  "sha256": "fd623bb6d60f4d0e4c750565abb38796646c4f4845a5ce8d3ff0b006f8ebbc52",
  "source": "sources/frame-loop-clear-color/clear_color.cpp",
  "emitted_functions": []
}
```

```json
{
  "header_closure": [],
  "kind": "independently_authored_source",
  "object_name": "brightness.o",
  "profile": "authored_cpp",
  "sha256": "ecf041ce054154010a0162112fad9672002ebe750f93876ed36165573af70478",
  "source": "sources/frame-loop-brightness/brightness.cpp",
  "emitted_functions": []
}
```

Do not add any `host_check.cpp`. The integrator assigns `input_order`. A private link of these nine objects, kept outside git, resolves `gstart` and `glistp` inside the package and leaves the opened names above undefined. It adds no COMMON symbols.

## Authored but never executed

`engineFrameEnd__Fb`, `engineFrameStart__FUib`, and `engineSetDisplayDisable__Fi`, plus the six words. The host checks ran on this Mac, and the reviewed EE compiler produced relocatable objects. No GS, DMA, semaphore, timer, or interrupt call has run on the EE. No frame was presented. The words were not read by a running game.

## Unknown

What 9766, 9767, 29299, 4884, and the threshold 31 select. The hardware effect of reading and clearing `0x10000000`. Whether `PollSema` returning the semaphore id means the wait is unfinished; the compare is what the instructions do. The ELF size of `draw`, and the meaning of its words past the offsets used. The zero store at address 0 faults on the EE if the canary check is reached; the host store returns. `powerOffMode` has no return path in the image. Interrupts between the paired `vblankCount` loads were not sampled. Why `0x0038f9e0` sits 0x10000 bytes into `packetBuffer` is not established beyond that arithmetic; the instruction uses the immediate. `GLISTSIZE` is 35000 and is not that `0x445c0` stride. No startup stage has run.
