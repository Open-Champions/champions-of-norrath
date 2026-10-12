# Startup SCE/IOP library and boot-support (2/4)

Package `w8-startup-sce-b`, branch `agents/w8-startup-sce-b`. New source is under `experimental/sources/w8-startup-sce-b-*`. Each function is its own commit with a host contract beside it (`host_contract.cpp`, not a plan input). Profile `authored_cpp`.

Private reference: `work/private/SLUS_205.65` in the main checkout (the executable named in `docs/BASELINE.md`). Addresses, sizes, strings and data words below were read from its symbol table and bytes; nothing from it is committed. None of these bodies is claimed as an original-byte match except where stated; the compiled sizes differ from the originals because the reviewed compiler schedules the same logic differently. Nothing here has run on an EE or as gameplay.

## Functions

| symbol | address | image bytes | compiled bytes | source |
|---|---|---|---|---|
| `_sceRpcFreePacket` | 0x002d7b78 | 28 | 24 | `w8-startup-sce-b-rpc-free-packet/rpc_free_packet.cpp` |
| `_sceRpcGetPacket` | 0x002d7ad0 | 168 | 156 | `w8-startup-sce-b-rpc-get-packet/rpc_get_packet.cpp` |
| `cdWait__Fv` | 0x00166d50 | 112 | 144 | `w8-startup-sce-b-cd-wait/cd_wait.cpp` |
| `cdClear__Fv` | 0x00166cd8 | 68 | 96 | `w8-startup-sce-b-cd-clear/cd_clear.cpp` |
| `deci2Putchar` | 0x002d67b8 | 176 | 216 | `w8-startup-sce-b-deci2-putchar/deci2_putchar.cpp` |
| `kprintf` | 0x002d7098 | 56 | 64 | `w8-startup-sce-b-kprintf/kprintf.cpp` |
| `_sceSifLoadModule` | 0x002dcdd8 | 548 | 336 | `w8-startup-sce-b-sif-load-module-call/load_module_call.cpp` |
| `lgCodecInit` | 0x002eaf50 | 8 | 8 | `w8-startup-sce-b-lg-codec-init/lg_codec_init.cpp` |
| `lgCodecEnumerate` (+ `g_codecs`) | 0x002eb280 | 52 | 44 | `w8-startup-sce-b-lg-codec-enumerate/lg_codec_enumerate.cpp` |
| `cdvd_exit` | 0x002e27a8 | 160 | 168 | `w8-startup-sce-b-cdvd-exit/cdvd_exit.cpp` |
| `initEzNetCtl` | 0x010bfb50 | 200 | 256 | `w8-startup-sce-b-init-ez-net-ctl/init_ez_net_ctl.cpp` |
| `_mallinfo_r` | 0x0030b028 | 240 | 224 | `w8-startup-sce-b-mallinfo-r/mallinfo_r.cpp` |
| `_request_end` | 0x002d7c08 | 204 | 172 | `w8-startup-sce-b-request-end/request_end.cpp` |
| `_request_call` | 0x002d81e0 | 140 | 144 | `w8-startup-sce-b-request-call/request_call.cpp` |
| `_request_bind` | 0x002d7fd0 | 204 | 208 | `w8-startup-sce-b-request-bind/request_bind.cpp` |
| `_request_rdata` | 0x002d7d18 | 208 | 184 | `w8-startup-sce-b-request-rdata/request_rdata.cpp` |
| `lgAudInit` | 0x002e9098 | 1204 | 984 (+28 for a local `halt`) | `w8-startup-sce-b-lg-aud-init/lg_aud_init.cpp` |
| `_kExitTLBHandler` | 0x002e0100 | 244 | 244 | `w8-startup-sce-b-k-exit-tlb-handler/k_exit_tlb_handler.cpp` |
| data words | see below | | | `w8-startup-sce-b-data-words/words.cpp` |

`_sceRpcFreePacket`, `_sceRpcGetPacket` and `_request_end` each have a second copy in the CD library (0x002e47a8, 0x002e4700, 0x002e47c8). The first two copies are the same body as the SIF RPC ones; the `_request_end` copy is longer (0x120 bytes) and is a different function. Only the SIF RPC copies are authored here, under the global names the other packages import.

## Observed

- `_sceRpcFreePacket` clears the low bit of packet word 4 and zeroes word 6. `_sceRpcGetPacket` scans a pool (id word, packet array pointer, packet count) with interrupts off for a packet whose word 4 has bit 0 clear, marks it `(index << 16) | 5`, steps the pool id (an increment that lands on 1 steps again, so ids go 1, 3, 4, ...), stores the id in packet word 6 and the packet's own address in word 5.
- `cdWait__Fv` pushes the border colour 0xffffff, runs `cdProcess__Fv` and `engineSyncVBlank__Fv` while `cdBusy` is nonzero, pops the colour, and pauses a running stream (`sceCdStPause`, `streaming = 0`, `streamPaused = 1`). `cdClear__Fv` runs `cdWait__Fv`, calls `sceCdStStop` when `streaming` or `streamPaused` is set, and clears `streamPaused`, `cdBusy` and `streaming`. The three bytes are at 0x00483ffc, 0x00483ffd and 0x00484010.
- `deci2Putchar` keeps a 128-byte line buffer (`linebuf`, 0x008de8c0) and a position word (`count`, 0x0044495c) and flushes through `kputs`: when the position reaches 126, and on a newline. `kprintf` passes its format and varargs on to `_printf`.
- `_sceSifLoadModule`: `_lf_bind`, then `_lf_version`, then fills the shared `_senddata` request (path at offset 8 with a 252 count, argument bytes at offset 260, at most 252 of them) and calls load-file RPC `operation` with a 512-byte send and 8-byte receive. Reply word 1 goes through the result pointer and word 0 is returned. Failures return the load-file error codes the sibling functions in `sif-keyboard-module-calls` use.
- `lgCodecInit` returns 0. `lgCodecEnumerate` returns the address of record 0 or 1 of the 40-byte `g_codecs` table (0x00447f90) and null otherwise. The records are `{10, "PCM", 0x1f400, 0x140, 0x140}` and `{0x12c, "GSM", 0x32c8, 0x140, 0x21}`.
- `cdvd_exit`, `_mallinfo_r`, `initEzNetCtl` and `lgAudInit` follow their header comments, which carry the full observed sequences, strings and constants.
- `_request_end`, `_request_call`, `_request_bind`, `_request_rdata` are four local command handlers of the SIF RPC library. Which registered command id each one serves is not established here (`sceSifInitRpc` registers four ids, 0x80000008, 0x80000009, 0x8000000a and 0x8000000c, and `_request_end` itself branches on 0x80000009 and 0x8000000a). Their header comments list the packet offsets read and written. `_request_end` swaps the global pointer for the client's saved value around the callback (inline assembly on the EE, a hook on the host).
- `_kExitTLBHandler` is assembly that restores Status, EPC, HI/HI1/LO/LO1/SA and the 128-bit registers from an exception frame, then returns with ERET. A private build linked at 0x002e0100 with the frame symbol at 0x008e5780 produced the same 244 bytes as the image (`docs/agents/w8-startup-sce-b-tlb-compare.py`; it needs `NORRATH_EXE` and `NORRATH_EE`). That is a body comparison only; it is not an exact-match ledger entry.

### Data words (`words.cpp`)

| symbol | address | bytes | note |
|---|---|---|---|
| `_sceCd_scmdsdata` | 0x004476c0 | 1032 | all zero in the image |
| `eenull` | 0x00445940 | 40 | five nonzero 32-bit words, then 20 zero bytes; compared with the image bytes |
| `batchBuffer` | 0x007dc280 | 4096 | span to `streamAllocation` |
| `gCd` | 0x007dd7e0 | 96 | span to `sbuff`; used as a SIF client |
| `cancelStreamWait` | 0x00486b3c | 4 | |
| `decompressThreadid` | 0x00486968 | 4 | |
| `dmaGIF` | 0x0118e6a4 | 4 | pointer in the overlay range |

## Inferred

- `lgAudInit` statics are anonymous in the original. They are published under authored names (`norrath_lgaud_*`) so later audio calls can share them; the roles are read from this function only. `norrath_lgaud_build_string` points at the library's own build string ("liblgaud version 1.09.004, built on Sep 15 2003 at 18:00:05").
- `initEzNetCtl` keeps `m_cd` and `m_rpcRunning` as file-local statics, as the original does (`m_rpcRunning` is volatile only so the write-only store survives).
- The server-block, queue and client field names in the `_request_*` sources are positional or role names from one function each; they are not recovered names.
- `deci2Putchar`'s `linebuf` and `count` keep their original local names as file statics.

## Unknown

- What the last three words of each `g_codecs` record mean.
- Whether `_request_end` and the other handlers' second argument is ever used by a caller (none of the four reads it).
- The caller-side meaning of the queue words in `_request_call` beyond "wake the named thread when word 1 is zero".
- The exact size of the `norrath_lgaud_*` objects beyond the observed spans.

## Host checks

Run from the repository root. Each prints a pass line and exits 0. `lgAudInit`, `initEzNetCtl` and `_request_end` mock their callees; the others do too, and every mock records the call order or arguments the contract then checks.

```sh
for d in rpc-free-packet rpc-get-packet cd-wait cd-clear deci2-putchar kprintf sif-load-module-call lg-codec-init lg-codec-enumerate cdvd-exit init-ez-net-ctl mallinfo-r request-end request-call request-bind request-rdata lg-aud-init data-words; do
  c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/w8-startup-sce-b-$d-check experimental/sources/w8-startup-sce-b-$d/host_contract.cpp && /tmp/w8-startup-sce-b-$d-check
done
NORRATH_EXE=/path/to/SLUS_205.65 NORRATH_EE=/path/to/ee python3 docs/agents/w8-startup-sce-b-tlb-compare.py
```

## Imports closed

`_kExitTLBHandler`, `_mallinfo_r`, `_request_bind`, `_request_call`, `_request_end`, `_request_rdata`, `_sceCd_scmdsdata`, `_sceRpcFreePacket`, `_sceRpcGetPacket`, `_sceSifLoadModule`, `batchBuffer`, `cancelStreamWait`, `cdClear__Fv`, `cdWait__Fv`, `cdvd_exit`, `deci2Putchar`, `decompressThreadid`, `dmaGIF`, `eenull`, `gCd`, `initEzNetCtl`, `kprintf`, `lgAudInit`, `lgCodecEnumerate`, `lgCodecInit`.

Not closed, of the 30 named in the assignment:

- `_fbss`, `_gp`, `_srsctorsstart`, `_srsctorsend`: absolute linker-script symbols (`_fbss` 0x00486800, `_gp` 0x004879f0, the two `_srsctors*` at 0x008eab1c and 0x008eab58, all type `A`/`B` markers of the original link). The recipe forbids absolute address imports and symbol aliases, and a C++ unit cannot define them without that. They need the integrator's link layout (`docs/LINK_LAYOUT.md`), not source.
- `_impure_ptr`: the object is the libc-format package's `norrath_factory_impure_pointer_word` (0x00448480, initially 0x00448190). Defining it again here would duplicate the owner. An asm alias in a separate unit does not work with the reviewed toolchain: the alias to an undefined target emits no symbol (tried with the reviewed assembler and linker). Either libc-format publishes that object under the name `_impure_ptr` or the integrator adds the alias in the link.

## Imports opened

Checked by comparing `unresolved-imports.json` before and after the private build (1195 before, 1189 after; the 25 closed names above left, 19 appeared):

`InvalidDCache`, `__malloc_current_mallinfo`, `__malloc_lock`, `__malloc_unlock`, `__malloc_update_mallinfo` (the allocator package owns these; `memory-malloc-state` defines the current-mallinfo word on its branch), `_alarm_bind`, `_alarm_rdata`, `_sceRpcGetFPacket`, `_sceRpcGetFPacket2`, `_search_svdata`, `iSetTimerAlarm`, `iWakeupThread`, `isceSifSendCmd` (all SIF RPC library routines), `kputs`, `sceCdStPause`, `sceCdStStop`, `sceSifRemoveCmdHandler`, `streamPaused` (the byte at 0x00484010, which the archive views do not publish yet), and `norrath_ee_exception_frame` (the 0x230-byte exception save block at 0x008e5780, written by the unauthored exception entry code).

Already defined in the base and now also used here: `CreateSema`, `DIntr`, `DeleteSema`, `EIntr`, `SignalSema`, `__wrap_malloc`, `_lf_bind`, `_lf_version`, `_senddata` and `norrath_loadfile_client` (from `iop-module-boot-lf-bind`), the three `_sceCd_*_semid` words, `cb_semid`, `cb_thid`, `sceCdCbfunc_num`, `cdBusy`, `cdProcess__Fv`, `engineSyncVBlank__Fv`, `enginePushBorderColor__FUi`, `enginePopBorderColor__Fv`, `iSignalSema`, `memcpy`, `memset`, `sceSifBindRpc`, `sceSifCallRpc`, `sceSifInitRpc`, `sceSifSearchModuleByName`. Already unresolved before this package and still used here: `__wrap_free`, `_printf`, `printf`, `streaming`, `strncpy`.

## Integration edits required in other packages

- `sif-keyboard-module-calls/module.cpp` still defines its own static load-file client and `_senddata`. `_sceSifLoadModule` uses the shared `norrath_loadfile_client` and `_senddata` that `iop-module-boot-lf-bind` exports, so until the edits listed in `docs/agents/iop-module-boot.md` are applied the module-search and unload calls use a different client copy.
- `simloop-services-sound-stream-records` defines `stream` at 0x007dd290 and says audio defines the same address under another name. `gCd` here follows it at 0x007dd7e0 and the 96-byte span ends at `sbuff`; if audio's views also define `gCd` or `batchBuffer`, the integrator must keep one.
- `kernel-patches-tlb-sysentry` takes the address of `_kExitTLBHandler` and now links to it.
- `libc-format`: see `_impure_ptr` above.

## Build-plan entries needed

Append these after the current last input (order 1408 at base `d4fa081`) and renumber from there if the shared plan has grown: replace `NEXT+n` with the next free `input_order` plus n. Profile `authored_cpp`, empty header closures. Put each `sha256` under `files` in both the plan and the snapshot, add the matching snapshot input (component `w8-startup-sce-b-<directory suffix>`, same `emitted_functions`), and raise `default_authored_inputs` by 19. Do not add any `host_contract.cpp` or the compare script. The `data-words` entry has no emitted function (it defines only data).

A private copy of the plan and snapshot kept outside git added these 19 units after the 1407 inputs at base `d4fa081` (1426 authored inputs). `--check-plan` passed. The full partial build then compiled all 1426 units and linked once: 1126 function bodies (1107 before), 1189 unresolved imports (1195 before), zero COMMON, no retries. `python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests.

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 24,
      "name": "_sceRpcFreePacket"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+0",
  "kind": "independently_authored_source",
  "object_name": "rpc_free_packet.o",
  "profile": "authored_cpp",
  "sha256": "34775d3b33fece606d640f90b9abd749b8f65e187fdd24e792e2068f01852002",
  "source": "sources/w8-startup-sce-b-rpc-free-packet/rpc_free_packet.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 156,
      "name": "_sceRpcGetPacket"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+1",
  "kind": "independently_authored_source",
  "object_name": "rpc_get_packet.o",
  "profile": "authored_cpp",
  "sha256": "3b41e7ef93a4b6347b697a706486619134c3c420b8898746f5995ae19f0d3f42",
  "source": "sources/w8-startup-sce-b-rpc-get-packet/rpc_get_packet.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 144,
      "name": "cdWait__Fv"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+2",
  "kind": "independently_authored_source",
  "object_name": "cd_wait.o",
  "profile": "authored_cpp",
  "sha256": "7c1dd86fdfceb4e3e6a9ef857647e6263e1ddb329e600ce31732035977114393",
  "source": "sources/w8-startup-sce-b-cd-wait/cd_wait.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 96,
      "name": "cdClear__Fv"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+3",
  "kind": "independently_authored_source",
  "object_name": "cd_clear.o",
  "profile": "authored_cpp",
  "sha256": "0cec4d8d190ed2c49c1896001e9ae984ad2da724a5a13fb8679979550b7d129d",
  "source": "sources/w8-startup-sce-b-cd-clear/cd_clear.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 216,
      "name": "deci2Putchar"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+4",
  "kind": "independently_authored_source",
  "object_name": "deci2_putchar.o",
  "profile": "authored_cpp",
  "sha256": "27a28a3a86151bc10361a647e4991b83814433affe8a20e478cd10dca1cab9e0",
  "source": "sources/w8-startup-sce-b-deci2-putchar/deci2_putchar.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 64,
      "name": "kprintf"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+5",
  "kind": "independently_authored_source",
  "object_name": "kprintf.o",
  "profile": "authored_cpp",
  "sha256": "48577bb253637b5c3c92ba42ca8b2fd2a1261dbe6f17a888f706a86a5f8ef633",
  "source": "sources/w8-startup-sce-b-kprintf/kprintf.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 336,
      "name": "_sceSifLoadModule"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+6",
  "kind": "independently_authored_source",
  "object_name": "load_module_call.o",
  "profile": "authored_cpp",
  "sha256": "c050b568867fbf6691adcab1df153ea27fc1f0ac7340948ef8d3ceb6a60091c3",
  "source": "sources/w8-startup-sce-b-sif-load-module-call/load_module_call.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 8,
      "name": "lgCodecInit"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+7",
  "kind": "independently_authored_source",
  "object_name": "lg_codec_init.o",
  "profile": "authored_cpp",
  "sha256": "285f11389c6b0b2bcb8536df0a799aaa346d5df802e461f5861d5e442107d6dc",
  "source": "sources/w8-startup-sce-b-lg-codec-init/lg_codec_init.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 44,
      "name": "lgCodecEnumerate"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+8",
  "kind": "independently_authored_source",
  "object_name": "lg_codec_enumerate.o",
  "profile": "authored_cpp",
  "sha256": "1666295827a1b7ef1b600a26b7b7a89e2f78ea53fa4a5dd6fc02d9166847bce4",
  "source": "sources/w8-startup-sce-b-lg-codec-enumerate/lg_codec_enumerate.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 168,
      "name": "cdvd_exit"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+9",
  "kind": "independently_authored_source",
  "object_name": "cdvd_exit.o",
  "profile": "authored_cpp",
  "sha256": "9bdb0902a2cd4508f220c25bb4550497c724e60583836a238ee3bf3a16d8beea",
  "source": "sources/w8-startup-sce-b-cdvd-exit/cdvd_exit.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 256,
      "name": "initEzNetCtl"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+10",
  "kind": "independently_authored_source",
  "object_name": "init_ez_net_ctl.o",
  "profile": "authored_cpp",
  "sha256": "49e30ca9f9acb8c394cbfd951264651e1a2b8f4249f33e015ba2c8d6bd11681a",
  "source": "sources/w8-startup-sce-b-init-ez-net-ctl/init_ez_net_ctl.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 224,
      "name": "_mallinfo_r"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+11",
  "kind": "independently_authored_source",
  "object_name": "mallinfo_r.o",
  "profile": "authored_cpp",
  "sha256": "e6cdb226edb69a2f421cc135ff01a60107fc8aea296f5bc79849bcbb3009d8ca",
  "source": "sources/w8-startup-sce-b-mallinfo-r/mallinfo_r.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 172,
      "name": "_request_end"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+12",
  "kind": "independently_authored_source",
  "object_name": "request_end.o",
  "profile": "authored_cpp",
  "sha256": "bd280ab76975cc6cafe731a057f9cc484a2aff3de67d62b9fb6a168a80a4643a",
  "source": "sources/w8-startup-sce-b-request-end/request_end.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 144,
      "name": "_request_call"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+13",
  "kind": "independently_authored_source",
  "object_name": "request_call.o",
  "profile": "authored_cpp",
  "sha256": "9c2351139d73c272311cade9ec15b43eba6560e8c49f8436f27cc467592a8150",
  "source": "sources/w8-startup-sce-b-request-call/request_call.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 208,
      "name": "_request_bind"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+14",
  "kind": "independently_authored_source",
  "object_name": "request_bind.o",
  "profile": "authored_cpp",
  "sha256": "94bceb66cfd7f7a16a65fea68d763fb8be0b684723bb28281c4665d82b1a62ca",
  "source": "sources/w8-startup-sce-b-request-bind/request_bind.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 184,
      "name": "_request_rdata"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+15",
  "kind": "independently_authored_source",
  "object_name": "request_rdata.o",
  "profile": "authored_cpp",
  "sha256": "f4fb0971e59dc13a08e0a1c514557cea073abf03b9a27859ba53ba4b787b6049",
  "source": "sources/w8-startup-sce-b-request-rdata/request_rdata.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 28,
      "name": "halt"
    },
    {
      "compiled_body_bytes": 984,
      "name": "lgAudInit"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+16",
  "kind": "independently_authored_source",
  "object_name": "lg_aud_init.o",
  "profile": "authored_cpp",
  "sha256": "7d13b8b1e087c6c8f293734afb227c09ff7b2369bb58cdb10b534bcb01bc0425",
  "source": "sources/w8-startup-sce-b-lg-aud-init/lg_aud_init.cpp"
}
```

```json
{
  "emitted_functions": [],
  "header_closure": [],
  "input_order": "NEXT+17",
  "kind": "independently_authored_source",
  "object_name": "words.o",
  "profile": "authored_cpp",
  "sha256": "aa984d1f5f861f7a3dc50c7232cd0290a0d75e00fdce7d03cc80575f173e9508",
  "source": "sources/w8-startup-sce-b-data-words/words.cpp"
}
```

```json
{
  "emitted_functions": [
    {
      "compiled_body_bytes": 244,
      "name": "_kExitTLBHandler"
    }
  ],
  "header_closure": [],
  "input_order": "NEXT+18",
  "kind": "independently_authored_source",
  "object_name": "k_exit_tlb_handler.o",
  "profile": "authored_cpp",
  "sha256": "1b4ce43d1bd5c846d17ce7327892b99a2500c54f3665f03aba581e9319acd675",
  "source": "sources/w8-startup-sce-b-k-exit-tlb-handler/k_exit_tlb_handler.cpp"
}
```

## Authored but never executed

Everything. No EE service, IOP module or game code ran. Specifically:

- All host contracts exercise the C++ with mocked callees on the build host; none ran the compiled EE objects. The mocks decide the call order the contract checks (for `lgAudInit` the printed text, the version cases, the allocation sizes and the semaphore path; for `_sceRpcGetPacket` the id stepping and the interrupt bracket; for `_request_*` the packet fields read and written).
- The infinite stop loops (`halt` in `lgAudInit`) are exited in the contract by a long jump from the mocked `printf`; the real loop is never run.
- `_request_end`'s global-pointer swap is inline assembly on the EE; the host contract uses a hook in its place, so the assembly path is compiled but not exercised. `_kExitTLBHandler` is not executed anywhere; its only check is the byte comparison above.
- Data words are compared with the image only for `eenull` (privately, once) and checked for size and zero fill by their contract.

## Not attempted

- `_alarm_bind` (0x002d7f90), `_alarm_rdata` (0x002d7cd8), `_sceRpcGetFPacket` (0x002d7b98), `_sceRpcGetFPacket2` (0x002d7bc8), `_search_svdata` (0x002d7f40), `isceSifSendCmd` (0x002d76d0), `iSetTimerAlarm` (0x002df3e8) and `iWakeupThread` (0x002d5f90) were opened as imports by the `_request_*` handlers and sit beside the functions authored here. They were outside the assigned list and are the next candidates for the sibling startup-SCE packages.
- `_gp`, `_fbss`, `_srsctorsstart`, `_srsctorsend`, `_impure_ptr`: see "Imports closed".
