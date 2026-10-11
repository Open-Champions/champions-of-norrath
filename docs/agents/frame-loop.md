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

## Imports closed

`engineFrameEnd__Fb`.

## Imports opened

`DIntr`, `EIntr`, `PollSema`, `WaitSema`, `coreLoadingScreenRunning`, `doSuperSample__Fiib`, `draw`, `drawFrameNm`, `drawWaitFinished__Fv`, `engineDisplayDisable`, `engineSetVideoMode__Fb`, `fullFrameMode`, `lastFrameVBlankCount`, `powerOffMode`, `sceGsSyncPath`, `scePrintf`, `switchTo30Counter.798`, `switchToHalfCounter.797`, `texturePackDown__Fv`, `vblankCount`, `vblankField`, `vblankSemaphore`, `vblankSemaphore2`.

`scePrintf`, `DIntr`, `EIntr`, `WaitSema`, and `PollSema` already exist as names elsewhere. This unit references them and does not define them.

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

Do not add `host_check.cpp`. The integrator assigns `input_order`.

## Authored but never executed

`engineFrameEnd__Fb`. The host checks ran on this Mac, and the reviewed EE compiler produced a relocatable object. No GS, DMA, semaphore, timer, or interrupt call has run on the EE. No frame was presented.

## Unknown

What 9766, 9767, 29299, 4884, and the threshold 31 select. The hardware effect of reading and clearing `0x10000000`. Whether `PollSema` returning the semaphore id means the wait is unfinished; the compare is what the instructions do. The ELF size of `draw`, and the meaning of its words past the offsets used. The zero store at address 0 faults on the EE if the canary check is reached; the host store returns. `powerOffMode` has no return path in the image. Interrupts between the paired `vblankCount` loads were not sampled. No startup stage has run.
