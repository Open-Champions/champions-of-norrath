# IOP module boot

Package: the fifteen callees of `initSif__Fv`, `sceSifSearchModuleByName`, and `keyboardFree__Fv` that were still unresolved. Branch `agents/iop-module-boot`. New source is under `experimental/sources/iop-module-boot-*`. Each function is its own commit with a host contract beside the source. Host contracts include host headers, so they stay out of the EE plan. The reviewed profile is `authored_cpp`.

Private reference: `work/private/SLUS_205.65` in the main checkout, matching the executable named in `docs/BASELINE.md`. The addresses, image extents, image data words, and string bytes below were read from its symbol table, its bytes, and its instruction stream. Nothing from it is committed. This work took over a worktree whose previous agent had left the fifteen units uncommitted; every unit was re-read against the original instructions before it was committed, and three were changed (see "Review changes").

None of these bodies is an original-byte match. The compiled bodies keep the original call order, branch structure, stored words, and constants, but the reviewed compiler schedules and frames them differently. No EE service has run, and none of this has run as gameplay.

## Functions

| symbol | address | image bytes | compiled bytes | source |
|---|---|---|---|---|
| `sceSifLoadModule` | 0x002dd000 | 32 | 32 | `sources/iop-module-boot-load-module/load_module.cpp` |
| `sceSifInitIopHeap` | 0x002dc0b0 | 136 | 160 | `sources/iop-module-boot-iop-heap/iop_heap.cpp` |
| `sceSifRebootIop` | 0x002dd530 | 272 | 272 | `sources/iop-module-boot-reboot-iop/reboot_iop.cpp` |
| `sceSifSyncIop` | 0x002dd4e0 | 80 | 80 | `sources/iop-module-boot-sync-iop/sync_iop.cpp` |
| `sceSifLoadFileReset` | 0x002dc790 | 56 | 56 | `sources/iop-module-boot-loadfile-reset/loadfile_reset.cpp` |
| `sceFsReset` | 0x002d92c0 | 52 | 52 | `sources/iop-module-boot-fs-reset/fs_reset.cpp` |
| `sceCdInit` | 0x002e3208 | 736 | 832 | `sources/iop-module-boot-cd-init/cd_init.cpp` |
| `sceCdMmode` | 0x002e3f18 | 208 | 240 | `sources/iop-module-boot-cd-mmode/cd_mmode.cpp` |
| `_lf_bind` | 0x002dc600 | 256 | 276 | `sources/iop-module-boot-lf-bind/lf_bind.cpp` |
| `_lf_version` | 0x002dc700 | 140 | 148 | `sources/iop-module-boot-lf-version/lf_version.cpp` |
| `PreparePowerOff__Fv` | 0x002cc230 | 140 | 148 | `sources/iop-module-boot-power-off/power_off.cpp` |
| `textSystemInit__Fv` | 0x002cb360 | 40 | 36 | `sources/iop-module-boot-text-system/text_system.cpp` |
| `Init__10VIKeyboard` | 0x002cbf30 | 84 | 88 | `sources/iop-module-boot-keyboard-init/keyboard_init.cpp` |
| `Free__10VIKeyboard` | 0x002cbf88 | 80 | 76 | `sources/iop-module-boot-keyboard-free/keyboard_free.cpp` |
| `commSystemInit__Fv` | 0x002ccec8 | 164 | 232 | `sources/iop-module-boot-comm/comm.cpp` |

`_lf_bind` and `_lf_version` are local in the original. They are published global because `sceSifSearchModuleByName`, `sceSifStopModule`, and `sceSifUnloadModule` import them.

## Observed

- `sceSifLoadModule` forwards its path, argument length, and argument to `_sceSifLoadModule` with a stack word and a fifth argument of 0. The stack word is never read.
- `sceSifInitIopHeap`, `_lf_bind`, and `sceCdInit` bind a 40-byte client, test the word at client offset 36, and run a countdown from 0x00100000 to -1 before rebinding when that word is zero. `sceSifInitIopHeap` and `_lf_bind` return -1 on a negative bind. `sceCdInit` rebinds instead, and prints `Libcdvd bind err %d CD_Init %d` first when `SCE_CD_debug` is positive. Servers: 0x80000003 for the heap, 0x80000006 for load-file, 0x80000592 for CD.
- `sceSifRebootIop` rejects a path when its length plus 11 is 81 or more, printing `too long parameter '%s'`. Otherwise it calls `sceSifInitRpc(0)`, `sceSifExitRpc`, builds `rom0:UDNL ` plus the path in an 80-byte stack buffer, and returns `sceSifResetIop` of it with 0.
- `sceSifSyncIop` returns 0 when bit 0x40000 of `sceSifGetReg(4)` is clear. Otherwise it calls `sceResetttyinit`, `sceSifExecNotifyHandler(1, 1)`, `sceSifExecNotifyHandler(0, 1)`, and returns 1.
- `sceSifLoadFileReset` stores -1 in `_bind_check` and zeroes four bytes of `_lfversion`. `sceFsReset` stores 0 in `_fs_init` and zeroes four bytes of `_fsversion`. Both return 0 through `memset`.
- `_lf_bind` returns 0 when `_bind_check` is nonnegative. After a good bind it stores 0 in `_bind_check`, calls operation 255 with no send buffer and a four-byte receive into `_senddata`, and returns 0xfffeffff on a negative call. Otherwise it copies those four bytes to `_lfversion`.
- `_lf_version` compares four bytes of `_lfversion` with offset 12 of `__ps2_klibinfo__` (`PsIIlibkernl2800`, so `2800`) and with the four bytes `_lfwildcard` points at (`....` then a NUL). Either match returns 0. Otherwise it returns 1 when those two reference words differ and 0 when they match.
- `sceCdInit` takes the observed path described in the comment at the top of its source: `sceCdSyncS(1)` must return 0; the command count word is incremented; five bind words, `_it_bind`, and `_sceCd_ee_read_mode` are reset; the mode is sent in a four-byte RPC and 16 bytes come back. Word 12 of the reply equal to 255 or 254 yields 1, and 254 also sets `SCE_CD_debug` to 1. Otherwise word 4 then word 8 are probed (the word itself when it is at least 0, else plus 255, then shifted right 8) and a probe below 2 yields 2. Mode 5 prints `Libcdvd Exit`, calls `cdvd_exit`, and stores -1 in the two command semaphores and `cb_semid`. Every other mode calls `cmd_sem_init` and `PowerOffCB`.
- `sceCdMmode` needs `_sceCd_scmd_prechk(34)` nonzero, sends the mode through `_sceCd_scmdsdata` with `rpc_mmode`, `outcnt_mmode`, and `incnt_mmode` (image values 0x22, 4, 4), reads the reply word through the uncached alias before signalling the semaphore, and returns 0 on a negative RPC.
- `PreparePowerOff__Fv` creates a semaphore record (1000, 0, 0 at words 1, 2, 5) and a thread record (entry `PowerOffThread__FPv`, the 8192-byte `stack`, `_gp`, and 1 in word 5, whose meaning is not established), starts the thread with a null argument, and registers `PowerOffHandler__FPv` with `sceCdPOffCallback`. `sid` and `tid` are stored before the next call.
- `textSystemInit__Fv` calls `VIStartKeyboardDriver__Fv` and then `Init__10VIKeyboard(vik)`.
- `Init__10VIKeyboard` clears `gKeyboardAttached`, prints `======= INIT KEYBOARD =======`, stores -1, the address of `sQueue`, 0, 0, 0 at words 1, 2, 132, 3, 0 and returns 0. `Free__10VIKeyboard` prints `======= FREE KEYBOARD =======`, and when word 1 is not -1 calls `lgKbdClose` with it and stores -1.
- `commSystemInit__Fv` sets `enableMallocWrapper`, calls `lgAudInit(0, 0)` once (guarded by `audinitted`), `lgCodecInit`, enumerates codecs keeping the last whose name is `GSM`, passes it (or -1) to `lgCodecOpen` with `codec`, and clears `enableMallocWrapper`.

## Inferred

- The original names `cd` (three locals), `rdata`, and `stack` are not unique, so two client objects are published under authored names: `norrath_iop_heap_client` (0x008e1cc0, 64-byte span) and `norrath_loadfile_client` (0x008e2080, 40 bytes). `_senddata` (0x008e1e80, 512 bytes) and `vik` (0x008d86a0, 536-byte span) are unique and keep their original names. The 64-byte alignment on the three .bss objects is the observed address alignment, not a recovered attribute.
- `Init_seq`, `_icmd_bind`, `_scmd_dr_bind`, `_it_bind`, `_i_mode`, `cdit`, `rpc_mmode`, `outcnt_mmode`, `incnt_mmode`, `sid`, `tid`, `audinitted`, `codec`, `gKeyboardAttached`, `sQueue`, `_fs_init`, `_fsversion`, `_bind_check`, `_lfversion`, `_lfwildcard`, and `__ps2_klibinfo__` are defined here with image values read from the executable. The words `SCE_CD_debug`, `cb_semid`, `_ncmd_bind`, `_sf_bind`, `_scmd_bind`, and `my_thid` are defined by `sony-cd-words`, so `sceCdInit` imports them. `_bind` is defined by `sif-keyboard-iop-heap-query`, and `norrath_archive_cd_command_count_view` (the word `c_cnt.66` at 0x004460d4) by the cd-archive package.
- Countdowns are `volatile` so `-O2` keeps them. The original holds the count in a register with four nops in the body. The host contracts count 0x100001 steps, matching the original's loop from 0x00100000 down to -1.
- `GSM` is the four bytes at 0x00486620 with no symbol. It is a string literal here.

## Review changes

- `sceCdInit` previously redefined six `sony-cd-words` objects, which would collide at link. They are now imports.
- The clients and `_senddata` were private statics in each unit, which would leave the module calls using an unbound copy of the client. They are exported so one object is shared.
- `Init__10VIKeyboard` and `Free__10VIKeyboard` moved from byte-wise to word accesses; compiled sizes fell from 160 and 92 to 88 and 76 bytes.

## Integration edits required in other packages

These belong to `sif-keyboard`, which this package does not edit. Without them the load-file client, heap client, `_senddata`, and `vik` would each exist twice, and the second copy would never be bound or initialised:

- `sif-keyboard-iop-heap-query/query.cpp`: replace `static unsigned char cd[64]` with `extern "C" unsigned char norrath_iop_heap_client[64]` and use it for both queries.
- `sif-keyboard-module-calls/module.cpp`: replace `static unsigned char cd[40]` with `extern "C" unsigned char norrath_loadfile_client[40]`, and the static `_senddata` union with `extern "C" unsigned char _senddata[512]` plus word accesses.
- `sif-keyboard-keyboard/keyboard.cpp`: replace `static unsigned char vik[536]` with `extern "C" unsigned char vik[536]`.

## Host checks

Run from the repository root. Each prints a pass line and exits 0.

```sh
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/iop-module-boot-load-module-check experimental/sources/iop-module-boot-load-module/host_contract.cpp && /tmp/iop-module-boot-load-module-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/iop-module-boot-iop-heap-check experimental/sources/iop-module-boot-iop-heap/host_contract.cpp && /tmp/iop-module-boot-iop-heap-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/iop-module-boot-reboot-iop-check experimental/sources/iop-module-boot-reboot-iop/host_contract.cpp && /tmp/iop-module-boot-reboot-iop-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/iop-module-boot-sync-iop-check experimental/sources/iop-module-boot-sync-iop/host_contract.cpp && /tmp/iop-module-boot-sync-iop-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/iop-module-boot-loadfile-reset-check experimental/sources/iop-module-boot-loadfile-reset/host_contract.cpp && /tmp/iop-module-boot-loadfile-reset-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/iop-module-boot-fs-reset-check experimental/sources/iop-module-boot-fs-reset/host_contract.cpp && /tmp/iop-module-boot-fs-reset-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/iop-module-boot-cd-init-check experimental/sources/iop-module-boot-cd-init/host_contract.cpp && /tmp/iop-module-boot-cd-init-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/iop-module-boot-cd-mmode-check experimental/sources/iop-module-boot-cd-mmode/host_contract.cpp && /tmp/iop-module-boot-cd-mmode-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/iop-module-boot-lf-bind-check experimental/sources/iop-module-boot-lf-bind/host_contract.cpp && /tmp/iop-module-boot-lf-bind-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/iop-module-boot-lf-version-check experimental/sources/iop-module-boot-lf-version/host_contract.cpp && /tmp/iop-module-boot-lf-version-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/iop-module-boot-power-off-check experimental/sources/iop-module-boot-power-off/host_contract.cpp && /tmp/iop-module-boot-power-off-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/iop-module-boot-text-system-check experimental/sources/iop-module-boot-text-system/host_contract.cpp && /tmp/iop-module-boot-text-system-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/iop-module-boot-keyboard-init-check experimental/sources/iop-module-boot-keyboard-init/host_contract.cpp && /tmp/iop-module-boot-keyboard-init-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/iop-module-boot-keyboard-free-check experimental/sources/iop-module-boot-keyboard-free/host_contract.cpp && /tmp/iop-module-boot-keyboard-free-check
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/iop-module-boot-comm-check experimental/sources/iop-module-boot-comm/host_contract.cpp && /tmp/iop-module-boot-comm-check
```

## Imports closed

`Free__10VIKeyboard`, `Init__10VIKeyboard`, `PreparePowerOff__Fv`, `_lf_bind`, `_lf_version`, `commSystemInit__Fv`, `sceCdInit`, `sceCdMmode`, `sceFsReset`, `sceSifInitIopHeap`, `sceSifLoadFileReset`, `sceSifLoadModule`, `sceSifRebootIop`, `sceSifSyncIop`, `textSystemInit__Fv`.

A private copy of the shared plan and snapshot, kept outside git, appended the fifteen units (1271 authored inputs). `--check-plan` passed. The link produced 999 function bodies, 1152 unresolved imports, and zero COMMON, against 1149 unresolved for the same base without these units: the fifteen names above left the list and eighteen appeared. `python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests.

## Imports opened

`PowerOffCB`, `PowerOffHandler__FPv`, `PowerOffThread__FPv`, `VIStartKeyboardDriver__Fv`, `_sceCd_scmdsdata`, `_sceSifLoadModule`, `cdvd_exit`, `lgAudInit`, `lgCodecEnumerate`, `lgCodecInit`, `lgCodecOpen`, `lgKbdClose`, `memcmp`, `sceCdPOffCallback`, `sceResetttyinit`, `sceSifExecNotifyHandler`, `sceSifExitRpc`, `sceSifResetIop`.

Already unresolved and now also used: `_gp`, `printf`, `sceSifGetReg`, `SignalSema`. Already defined and used: `scePrintf`, `sceSifBindRpc`, `sceSifCallRpc`, `sceSifInitRpc`, `sceSifWriteBackDCache`, `sceCdSyncS`, `memset`, `strcmp`, `GetThreadId`, `CreateSema`, `CreateThread`, `StartThread`, `cmd_sem_init`, `_sceCd_scmd_prechk`, the `_sceCd_*` words, and the `sony-cd-words` objects.

## Build-plan entries needed

Append these after the current last input (order 1257 at base `5744679`) and renumber if the shared plan has grown. Profile `authored_cpp`, empty header closures. Put each `sha256` under `files` in both the plan and the snapshot, add the matching snapshot input (component `iop-module-boot-<suffix>`, same `emitted_functions`), and raise `default_authored_inputs` by 15. Do not add any `host_contract.cpp`.

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 32,
      "name": "sceSifLoadModule"
    }
  ],
  "header_closure": [],
  "input_order": 1258,
  "kind": "independently_authored_source",
  "object_name": "load_module.o",
  "profile": "authored_cpp",
  "sha256": "fd44523aff489f79c14d00efa78adb61cc8c65d6e606bea408ee656a6c1d183f",
  "source": "sources/iop-module-boot-load-module/load_module.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 160,
      "name": "sceSifInitIopHeap"
    }
  ],
  "header_closure": [],
  "input_order": 1259,
  "kind": "independently_authored_source",
  "object_name": "iop_heap.o",
  "profile": "authored_cpp",
  "sha256": "0a8ffb1513d56eb5da194073525290a66fce4031e46ff90143a6f375a95cd0c8",
  "source": "sources/iop-module-boot-iop-heap/iop_heap.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 272,
      "name": "sceSifRebootIop"
    }
  ],
  "header_closure": [],
  "input_order": 1260,
  "kind": "independently_authored_source",
  "object_name": "reboot_iop.o",
  "profile": "authored_cpp",
  "sha256": "9105dfa09368f8d01296d0dfd9c6cea2a4c847def8e35352ae60b4461cafd5f7",
  "source": "sources/iop-module-boot-reboot-iop/reboot_iop.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 80,
      "name": "sceSifSyncIop"
    }
  ],
  "header_closure": [],
  "input_order": 1261,
  "kind": "independently_authored_source",
  "object_name": "sync_iop.o",
  "profile": "authored_cpp",
  "sha256": "1dda9d32d3c5f387e82b1ee15cdad6e1e57fd932fc9a645224bdd802a54719a4",
  "source": "sources/iop-module-boot-sync-iop/sync_iop.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 56,
      "name": "sceSifLoadFileReset"
    }
  ],
  "header_closure": [],
  "input_order": 1262,
  "kind": "independently_authored_source",
  "object_name": "loadfile_reset.o",
  "profile": "authored_cpp",
  "sha256": "c97e65beb01fe4ec845d51ba9b438f8e2a732e3f5726f631905aee782c505d7a",
  "source": "sources/iop-module-boot-loadfile-reset/loadfile_reset.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 52,
      "name": "sceFsReset"
    }
  ],
  "header_closure": [],
  "input_order": 1263,
  "kind": "independently_authored_source",
  "object_name": "fs_reset.o",
  "profile": "authored_cpp",
  "sha256": "38c617397bbaa40ab764f931f7a8685e7a1d39fc61600c11b4b84316b21cc644",
  "source": "sources/iop-module-boot-fs-reset/fs_reset.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 832,
      "name": "sceCdInit"
    }
  ],
  "header_closure": [],
  "input_order": 1264,
  "kind": "independently_authored_source",
  "object_name": "cd_init.o",
  "profile": "authored_cpp",
  "sha256": "d23471a49595b999bab1a0ac5a11bf663b83f89cb78b45948ce380fefa864169",
  "source": "sources/iop-module-boot-cd-init/cd_init.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 240,
      "name": "sceCdMmode"
    }
  ],
  "header_closure": [],
  "input_order": 1265,
  "kind": "independently_authored_source",
  "object_name": "cd_mmode.o",
  "profile": "authored_cpp",
  "sha256": "cdd3bfbd31e246da6b5b7144ff48720c96e6a2ad445f73b1bf75cb42f482b52f",
  "source": "sources/iop-module-boot-cd-mmode/cd_mmode.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 276,
      "name": "_lf_bind"
    }
  ],
  "header_closure": [],
  "input_order": 1266,
  "kind": "independently_authored_source",
  "object_name": "lf_bind.o",
  "profile": "authored_cpp",
  "sha256": "ad3773fddc7cd5f7e753a5b087771bf0f28014427295d2ce46cf054a996a5955",
  "source": "sources/iop-module-boot-lf-bind/lf_bind.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 148,
      "name": "_lf_version"
    }
  ],
  "header_closure": [],
  "input_order": 1267,
  "kind": "independently_authored_source",
  "object_name": "lf_version.o",
  "profile": "authored_cpp",
  "sha256": "0dab255aa979f8d6515c4ba2fff465062f02fc48f2c4eba20bf197264b1fe5da",
  "source": "sources/iop-module-boot-lf-version/lf_version.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 148,
      "name": "PreparePowerOff__Fv"
    }
  ],
  "header_closure": [],
  "input_order": 1268,
  "kind": "independently_authored_source",
  "object_name": "power_off.o",
  "profile": "authored_cpp",
  "sha256": "a05fded8d589abad05d2e7165a3b9f05a625ca0d1d81cf29c1fc710b33e6ae34",
  "source": "sources/iop-module-boot-power-off/power_off.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 36,
      "name": "textSystemInit__Fv"
    }
  ],
  "header_closure": [],
  "input_order": 1269,
  "kind": "independently_authored_source",
  "object_name": "text_system.o",
  "profile": "authored_cpp",
  "sha256": "3f535ec6a36350296af5f4706f17ead65dbbe5cfc4f6bfc5026ead87c10f01c0",
  "source": "sources/iop-module-boot-text-system/text_system.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 88,
      "name": "Init__10VIKeyboard"
    }
  ],
  "header_closure": [],
  "input_order": 1270,
  "kind": "independently_authored_source",
  "object_name": "keyboard_init.o",
  "profile": "authored_cpp",
  "sha256": "ead6981c4b04b0eb61dfaaa237cec94c74d657dd176a846d3d73682cc5f4ce63",
  "source": "sources/iop-module-boot-keyboard-init/keyboard_init.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 76,
      "name": "Free__10VIKeyboard"
    }
  ],
  "header_closure": [],
  "input_order": 1271,
  "kind": "independently_authored_source",
  "object_name": "keyboard_free.o",
  "profile": "authored_cpp",
  "sha256": "d39470e3feeaae226f5d1ded27fa31fc46a9b6579f1a8635d07990a24508c501",
  "source": "sources/iop-module-boot-keyboard-free/keyboard_free.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 232,
      "name": "commSystemInit__Fv"
    }
  ],
  "header_closure": [],
  "input_order": 1272,
  "kind": "independently_authored_source",
  "object_name": "comm.o",
  "profile": "authored_cpp",
  "sha256": "fe731db9f14a190b24d35fc1bb50893a2d4c8f70a22895b38769dcbd93386bac",
  "source": "sources/iop-module-boot-comm/comm.cpp"
}
```

## Authored but never executed

All fifteen functions and every object they define. Host contracts ran on this Mac with stubs for each callee and passed. The reviewed EE compiler built all fifteen, and the private link above checked symbol closure only. No routine ran on the EE, under the original process, or in gameplay.

## Unknown

- What the IOP does for server 0x80000003, 0x80000006 (operation 255), and 0x80000592, and what the reply words of `sceCdInit` mean beyond the tests the code makes on them.
- Whether `sceCdInit`'s `SCE_CD_debug` value of 254 in word 12 has a documented meaning; only the stored effect is known.
- Whether the bytes of `vik` between offset 16 and 528, and bytes 532 through 535, are used. `Init__10VIKeyboard` does not store them.
- What the 48 bytes after the 8192-byte `stack` are; the thread record names only 8192.
- Which other readers use `_senddata` beyond the four words the module calls and `_lf_bind` read.
- Interrupts, timing, and the original register spills were not reproduced.
