# GS, DMA, kernel, and vblank

Package work is on `agents/gs-dma-kernel`. Sources are under `experimental/sources/gs-dma-kernel-*`. Syscall wrappers and `sceGsResetPath` were assembled with the reviewed `authored_entry_asm` flags and compared with `experimental/sources/gs-dma-kernel-body-check/host_check.cpp` against the local executable image. The other functions were compiled with the `authored_cpp` flags and checked by the host contract beside each one. `host_contract.cpp` and the body checker are host tests only. They are not build-plan inputs.

`ChangeThreadPriority` (`0x002d4f70`, service 41) and `GetThreadId` (`0x002d4fd0`, service 47) are already defined in `experimental/sources/runtime-initsys-callees/`. This package does not define them again.

`origin/agents/cd-archive` does not define `vblankCount`, `vblankField`, or `vblankSemaphore2`. Its sync-vblank unit imports `norrath_archive_vblank_count_view`, `norrath_archive_vblank_field_view`, and `norrath_archive_vblank_semaphore_view`. Those three names stay unresolved. They are the same objects as `vblankCount` (`0x00484438`), `vblankField` (`0x0048443c`), and `vblankSemaphore2` (`0x00486a60`). An integrator alias can bind the views. This package defines the ELF names only.

The branch baseline partial image had 692 unresolved imports. `EnableIntc` and `PollSema` were the only names from this package already on that list. The GS, DMA, and vblank symbols below were absent because `engineInit__Ff` is not in this branch's plan. A private plan outside git, continuing from input 422, linked 25 more units: 502 functions, 697 unresolved imports, 0 common symbols. That image closed `EnableIntc` and `PollSema` and opened the seven names in the opened section.

`tools/check_source_snapshot.py` will reject these new source paths until `experimental/snapshot.json` records them. That snapshot was not edited. The shared plan on this branch ends at `input_order` 422. Renumber if the integrator's plan has grown.

## Functions

| Symbol | Original address | Original size | Form |
| --- | --- | --- | --- |
| `WaitSema` | `0x002d5120` | 16 | service 68, byte match |
| `AddIntcHandler` | `0x002d4dc0` | 16 | service 16, byte match |
| `PollSema` | `0x002d5130` | 16 | service 69, byte match |
| `iSignalSema` | `0x002d5110` | 16 | service -67, byte match |
| `RemoveIntcHandler` | `0x002d4de0` | 16 | service 17, byte match |
| `SetGsCrt` | `0x002d4ce0` | 16 | service 2, byte match |
| `GsPutIMR` | `0x002d5420` | 16 | service 113, byte match |
| `_GetGsDxDyOffset` | `0x002d1990` | 16 | service 128, byte match |
| `EnableIntc` | `0x002d5c28` | 104 | host contract |
| `DisableIntc` | `0x002d5bc0` | 104 | host contract |
| `sceGsGetGParam` | `0x002d1878` | 12 | host contract |
| `sceGsResetGraph` | `0x002d16e8` | 400 | host contract |
| `sceGsResetPath` | `0x002d1888` | 104 | byte match |
| `sceDmaGetChan` | `0x002d2ca8` | 40 | host contract |
| `CheckAddress` | `0x002d2c48` | 40 | host contract |
| `memclr` | `0x002d2c70` | 52 | host contract |
| `WaitDma` | `0x002d2b50` | 112 | host contract |
| `sceDmaSync` | `0x002d3278` | 56 | host contract |
| `sceDmaSendN` | `0x002d3040` | 116 | host contract |
| `sceDmaReset` | `0x002d2cd0` | 220 | host contract |
| `sceGsSetDefDispEnv` | `0x002d19a0` | 884 | host contract |
| `sceGsPutDispEnv` | `0x002d1d18` | 188 | host contract |
| `sceGsSyncPath` | `0x002d1f38` | 788 | host contract |
| `vblankHandler__Fi` | `0x0018e4a0` | 512 | host contract |

`AddIntcHandler` is service 16. The existing `AddIntcHandler2` is a different symbol at `0x002d4dd0` for the same service number.

`EnableIntc` samples COP0 Status bit `0x10000`. When that bit is set it calls `DIntr`, requests service 20 with the cause, executes `sync`, and calls `EIntr` on the same bit. `DisableIntc` is the same shape with service 21. The original issues those services through `_EnableIntc` (`0x002d4e20`) and `_DisableIntc` (`0x002d4e30`). This source issues the service directly. What the service writes into the interrupt mask is unknown.

`sceGsGetGParam` returns `gp.15` (`0x00444880`, 16 bytes). The file image is shorts 1, 2, 1, 3 and then two zero words. Those offsets are interlace, frame mode, the half at offset 4, and the half at offset 6. The last two words are a handler id and a handler argument. The C names are offset labels.

`sceGsResetGraph` sign-extends all four arguments to 16 bits. Mode 1 stores `0x100` at `0x12001000` and returns. Mode 0 stores `0x200` there, records interlace and frame mode, stores bits 16..23 of a later read of that address at offset 6, calls `GsPutIMR(0xff00)`, and stores 1 at offset 4 when the version is nonzero. A nonzero word at offset 8 calls `DisableIntc(2)` and `RemoveIntcHandler(2, word at offset 12)`, then clears offset 12 and offset 8. It tail-calls `SetGsCrt` with interlace bit 0, the low eight frame-mode bits, and version bit 0. Mode 5 updates the same halves from the current word at `0x12001000` without the `0x200` store, without `GsPutIMR`, and without removing a handler. Every other mode, including negatives, returns immediately. The host model keeps the written word separate from the value a later read returns.

`sceGsResetPath` writes 1 to `0x10003c10` and 2 to `0x10003c20`, sets bit 9 of VU0 vi28, and copies two quadwords from the absolute address `0x00444890` into `0x10005000`. The second store uses the same destination as the first. It then writes 1 to `0x10003000`. The 32 bytes at `0x00444890` are `init_vif_regs.12`: `04 04 00 01 00 00 00 20` followed by a zero word and the words 5, 6, 3, 2, 4. The byte-matched body embeds that address. This package does not define `init_vif_regs.12`. A relocated link still has to have those bytes at `0x00444890`.

`sceDmaGetChan` returns 0 when the unsigned channel is 10 or more. Otherwise it returns the pointer word in `dch` (`0x004448c0`, 40 bytes): `0x10008000`, `0x10009000`, `0x1000a000`, `0x1000b000`, `0x1000b400`, `0x1000c000`, `0x1000c400`, `0x1000c800`, `0x1000d000`, `0x1000d400`.

`CheckAddress` rewrites an address whose top nibble is 7 to `(address & 0x0fffffff) | 0x80000000`.

`memclr` returns when the count is 0. Otherwise it writes that many zero bytes. The original symbol is local. `sceDmaReset` calls it, so this definition is global.

`WaitDma` returns when bit 8 of the channel word is clear. Otherwise the counter starts at `0x01000000` and drops by two on each busy poll: one step before the timeout test and one step when the poll continues. A negative counter prints `libdma: sync timeout\n` and calls `sceDmaPause`, and that pair can repeat. The host contract shortens the counter. The EE constant stays `0x01000000`.

`sceDmaSync` takes the channel, then the mode. Mode 1 returns bit 8 of the channel word and does not call `WaitDma`. Every other mode calls `WaitDma` and returns 0. A third argument is unread.

`sceDmaSendN` passes the packet through `CheckAddress`, waits, and stores that address at channel offset 16 unless the word already there is `0xffffffff`. Offset 32 receives the quadword count. The first word clears bits 2 and 3, then sets bits 0 and 8.

`sceDmaReset` keeps bit 0 of `0x1000e000`. For each of the ten `dch` entries, a nonzero `isclr` word clears channel offsets 128, 0, 48, 16, 80, and 64, in that order. `isclr` at `0x0047b388` is the ten words 1, 1, 1, 1, 1, 0, 0, 0, 1, 1. Channels 5, 6, and 7 are the zero slots. Naming those three SIF is inferred from the public DMA map. The zeros themselves are the file image. The function writes `0xff1f` to `0x1000e010`, reads that word, keeps bits in `0xff1f0000`, and stores the result. Twenty stack bytes are cleared and passed to `sceDmaPutEnv`. Mode 1 sets bit 0 of `0x1000e000`. The saved bit is returned.

`sceGsSetDefDispEnv` sign-extends its six arguments to 16 bits. Offset 0 of the environment is 102. Offset 8 is 2 when the interlace half is 0, 3 when the half at offset 4 is nonzero, and 1 otherwise. Offset 16 packs the pixel mode at bit 15 and `((width + 63) >> 6)` masked to six bits at bit 9. When the frame-mode half is 2 or 3 and the width is 0, the function executes `break 0,7` and this source returns without writing offsets 24 or 32. Resumption after that breakpoint is unknown. Frame modes 2 and 3 divide `(width + 2559) / width`. The display bases are 636 and 50, 636 and 25, 656 and 72, or 656 and 36, selected by frame mode and whether interlace is 1. The height field is `(height << 1) - 1` only when interlace is 1 and the half at offset 4 is nonzero. Frame mode 80 uses `720 - width`, a doubled horizontal step, base 232 and 35, and magnitude 1. Any other frame mode prints `sceGsDefDispEnv:Not support displaymode for 0x%x!!\n` and leaves offset 24 unwritten. Offset 32 is cleared on every path that gets past the breakpoint. When the frame mode is outside 2 and 3, `checkModelVersion` runs. A nonzero result calls `_GetGsDxDyOffset` with that half and four stack slots. The reviewed compiler passes the fifth pointer in `$t0`. The first two slots are added into the horizontal and vertical display fields.

`sceGsPutDispEnv` reads the half at `gp.15` offset 6. When that half is 1, environment offsets 0, 16, 24, and 32 go to `0x12000000`, `0x12000070`, `0x12000080`, and `0x120000c0`. Otherwise offsets 0, 8, 16, 24, and 32 go to `0x12000000`, `0x12000020`, `0x12000090`, `0x120000a0`, and `0x120000e0`. The initial half at offset 6 is 3, so the second path is the one taken before a reset changes it. Register names for those addresses are inferred from the public GS map.

`sceGsSyncPath` clears its second argument before reading it. A nonzero mode returns a bit mask and does not wait: bit 0 from bit 8 of `0x10009000`, bit 1 from bit 8 of `0x1000a000`, bit 2 from `0x10003c00` masked with `0x1f000003`, bit 3 from bit 8 of VU0 vi29, and bit 4 from `0x10003020` masked with `0xc00`. Mode 0 waits for those conditions in that order. The spin count starts at 0, is shared, and gives up when it is already above `0x01000000`. A timeout prints one termination line and then the ten register lines, and returns -1. A finished wait returns 0. The timeout dump is a local function in the same translation unit. The strings are:

- `sceGsSyncPath: DMA Ch.1 does not terminate\r\n`
- `sceGsSyncPath: DMA Ch.2 does not terminate\r\n`
- `sceGsSyncPath: VIF1 does not terminate\r\n`
- `sceGsSyncPath: VU1 does not terminate\r\n`
- `sceGsSyncPath: GIF does not terminate\r\n`
- `\t<D1_CHCR=%08x:` `D1_TADR=%08x:` `D1_MADR=%08x:` `D1_QWC=%08x>\r\n`
- `\t<D2_CHCR=%08x:` `D2_TADR=%08x:` `D2_MADR=%08x:` `D2_QWC=%08x>\r\n`
- `\t<VIF1_STAT=%08x:` `GIF_STAT=%08x>\r\n`

## Vblank state

`vblankCount` at `0x00484438` is 4 zero sdata bytes. `vblankField` at `0x0048443c` is the next sdata byte and is zero. `vblankHID` at `0x004843ec` is 4 zero sdata bytes. `vblankSemaphore` at `0x00486a5c` and `vblankSemaphore2` at `0x00486a60` are sbss, size 0 in the symbol table, with the next symbol 4 bytes later. Callers load each as a word. The file image of all five is zero. `-G0` places the initialized words in `.data` and the uninitialized words in `.bss`. This profile does not place them at the original addresses.

`vblankHandler__Fi` ignores its cause and returns 0. A nonzero `engineDisplayDisable` stores 0 at `0x12000000`; otherwise it stores 2. A nonzero `specialMpegMode` skips the draw walk. Otherwise the signed words at `draw+36` and `draw+52` are compared with the signed word at `draw+56`. A value that is greater, and strictly below the current limit, replaces that word and stores its index at `draw+60`. The limit starts at `0x7fffffff` and becomes the chosen value, so a later slot has to be smaller than an earlier match. The word at `draw+28` plus `index * 16` selects the rest of the stores. A nonzero word stores 1 at `0x12000020` and builds the display words from `displayBuffer1StartAddress`, including bit 43 on the store to `0x12000070` and `0x1be9ff0183227c` at `0x12000080` and `0x120000a0`. A zero word stores 3 at `0x12000020` and uses `displayBuffer2StartAddress` when the index is nonzero. Both paths then take bit 13 of the 64-bit word at `0x12001000`, store that bit in `vblankField`, add 1 to `vblankCount`, signal `vblankSemaphore` and then `vblankSemaphore2`, execute `sync` and `ei`, and return 0. `draw` is the object at `0x00790ff0`. This package does not define `draw` or `dispEnv`.

## Imports closed

`WaitSema`, `AddIntcHandler`, `PollSema`, `iSignalSema`, `RemoveIntcHandler`, `SetGsCrt`, `GsPutIMR`, `_GetGsDxDyOffset`, `EnableIntc`, `DisableIntc`, `sceGsGetGParam`, `sceGsResetGraph`, `sceGsResetPath`, `sceDmaGetChan`, `CheckAddress`, `memclr`, `WaitDma`, `sceDmaSync`, `sceDmaSendN`, `sceDmaReset`, `sceGsSetDefDispEnv`, `sceGsPutDispEnv`, `sceGsSyncPath`, `vblankHandler__Fi`, `gp.15`, `dch`, `isclr`, `vblankCount`, `vblankField`, `vblankHID`, `vblankSemaphore`, `vblankSemaphore2`.

`EnableIntc` and `PollSema` were unresolved on the branch baseline. The others close once `engineInit__Ff` and the vblank registration are linked. `ChangeThreadPriority` and `GetThreadId` were already closed by `runtime-initsys-callees`.

## Imports opened

`checkModelVersion`, `sceDmaPutEnv`, `sceDmaPause`, `displayBuffer1StartAddress`, `displayBuffer2StartAddress`, `draw`, `specialMpegMode`.

`engineDisplayDisable` was already unresolved on the branch baseline. `vblankHandler__Fi` reads it and does not define it. `norrath_archive_vblank_count_view`, `norrath_archive_vblank_field_view`, and `norrath_archive_vblank_semaphore_view` stay unresolved for the cd-archive overlap above. `DIntr`, `EIntr`, and `scePrintf` already have definitions and resolved in the private link. `_EnableIntc` and `_DisableIntc` are not referenced.

## Build-plan entries needed

Append these to `experimental/build-plan.json`. Do not add the host contracts or `gs-dma-kernel-body-check/host_check.cpp`. `header_closure` is empty. Suggested `input_order` continues from 422 on this branch.

```json
{"header_closure":[],"input_order":423,"kind":"independently_authored_source","object_name":"wait_sema.o","profile":"authored_entry_asm","sha256":"7a90dd1fdbd7a4221ed9575f204c7d37e2a3af0087c984c0f710f60dc06f4960","source":"sources/gs-dma-kernel-wait-sema/wait_sema.S"}
{"header_closure":[],"input_order":424,"kind":"independently_authored_source","object_name":"add_intc_handler.o","profile":"authored_entry_asm","sha256":"854cc9b92ca70ac4aaa4ec9c560b8241488d1b83997bb157f109238783f71c3f","source":"sources/gs-dma-kernel-add-intc-handler/add_intc_handler.S"}
{"header_closure":[],"input_order":425,"kind":"independently_authored_source","object_name":"poll_sema.o","profile":"authored_entry_asm","sha256":"60b7918e81e7e677b79616aa2fbccc902677ce64872b0d5c8742bf9ee06becd8","source":"sources/gs-dma-kernel-poll-sema/poll_sema.S"}
{"header_closure":[],"input_order":426,"kind":"independently_authored_source","object_name":"isignal_sema.o","profile":"authored_entry_asm","sha256":"15b93ae93d4e93efdf05c242f334a70e91dea8a47c573360d1ebb8388dfeda11","source":"sources/gs-dma-kernel-isignal-sema/isignal_sema.S"}
{"header_closure":[],"input_order":427,"kind":"independently_authored_source","object_name":"remove_intc_handler.o","profile":"authored_entry_asm","sha256":"c7b034773a46d45f8f40e87a292c3a0086d8112a7da7e31b52b674e7f46163ad","source":"sources/gs-dma-kernel-remove-intc-handler/remove_intc_handler.S"}
{"header_closure":[],"input_order":428,"kind":"independently_authored_source","object_name":"set_gs_crt.o","profile":"authored_entry_asm","sha256":"4597c7318592fa9e8af8ea17de467fba14d49f488f51dbc98001dbd0c5e26d1f","source":"sources/gs-dma-kernel-set-gs-crt/set_gs_crt.S"}
{"header_closure":[],"input_order":429,"kind":"independently_authored_source","object_name":"gs_put_imr.o","profile":"authored_entry_asm","sha256":"eebe155d360aca68e84646f07bf83a7f7a94c370d2973b4d99151e58f314f6cc","source":"sources/gs-dma-kernel-gs-put-imr/gs_put_imr.S"}
{"header_closure":[],"input_order":430,"kind":"independently_authored_source","object_name":"get_gs_dxdy_offset.o","profile":"authored_entry_asm","sha256":"75ad40f9d07ff59332416804e7f8119c9e3b4a4d79794ec10f3e6a35663e2c5e","source":"sources/gs-dma-kernel-get-gs-dxdy-offset/get_gs_dxdy_offset.S"}
{"header_closure":[],"input_order":431,"kind":"independently_authored_source","object_name":"enable_intc.o","profile":"authored_cpp","sha256":"c1d3ba1e833cad9c5e4860995cb8865bab8859cc26dfbb4c758fa0f66e9ceab5","source":"sources/gs-dma-kernel-enable-intc/enable_intc.cpp"}
{"header_closure":[],"input_order":432,"kind":"independently_authored_source","object_name":"disable_intc.o","profile":"authored_cpp","sha256":"514ab62ceda3e37bfeddecccb48ea52e864823621f9adc8d7337be02f6888e15","source":"sources/gs-dma-kernel-disable-intc/disable_intc.cpp"}
{"header_closure":[],"input_order":433,"kind":"independently_authored_source","object_name":"get_gparam.o","profile":"authored_cpp","sha256":"ef40467bc99f4c4a4228ca3e1ce88d4877794971de4532f5cad4ca4c7dd8289a","source":"sources/gs-dma-kernel-get-gparam/get_gparam.cpp"}
{"header_closure":[],"input_order":434,"kind":"independently_authored_source","object_name":"reset_graph.o","profile":"authored_cpp","sha256":"217a9656287fa093643c0d836e9f37e45f00d8f0790cf61a1127521eb1d6e8e9","source":"sources/gs-dma-kernel-reset-graph/reset_graph.cpp"}
{"header_closure":[],"input_order":435,"kind":"independently_authored_source","object_name":"reset_path.o","profile":"authored_entry_asm","sha256":"9291d7005cd39f81d0d5ef931b0274a090fbc936d2920d6594eb97ffe760c90b","source":"sources/gs-dma-kernel-reset-path/reset_path.S"}
{"header_closure":[],"input_order":436,"kind":"independently_authored_source","object_name":"dma_get_chan.o","profile":"authored_cpp","sha256":"2ff4e9f6eceae942cc28a63169844c84d36063e56adb0f4abd12115dce3ef28c","source":"sources/gs-dma-kernel-dma-get-chan/dma_get_chan.cpp"}
{"header_closure":[],"input_order":437,"kind":"independently_authored_source","object_name":"check_address.o","profile":"authored_cpp","sha256":"7378c646cb2a9191d4004e19052f83b16b9e3210d023d8b51b738424c7deccc6","source":"sources/gs-dma-kernel-check-address/check_address.cpp"}
{"header_closure":[],"input_order":438,"kind":"independently_authored_source","object_name":"memclr.o","profile":"authored_cpp","sha256":"a31cd8ad9b0c09a5e6446bcab13380205e98984f3b2b20756e771f47b76da101","source":"sources/gs-dma-kernel-memclr/memclr.cpp"}
{"header_closure":[],"input_order":439,"kind":"independently_authored_source","object_name":"wait_dma.o","profile":"authored_cpp","sha256":"b192e1852a53238ab1e7e0f8ad3e0d940d9c062343f6920e5de9d2ea01fcc9ff","source":"sources/gs-dma-kernel-wait-dma/wait_dma.cpp"}
{"header_closure":[],"input_order":440,"kind":"independently_authored_source","object_name":"dma_sync.o","profile":"authored_cpp","sha256":"c78aa2bed44a1d1b889c34f9c26c82b97d9319779a6ed0165c981593a245ade5","source":"sources/gs-dma-kernel-dma-sync/dma_sync.cpp"}
{"header_closure":[],"input_order":441,"kind":"independently_authored_source","object_name":"dma_send_n.o","profile":"authored_cpp","sha256":"b87e3fc45964a76f2eea530dbf1c989c7bbb0e9d48f489d373bb70cf83ff37b1","source":"sources/gs-dma-kernel-dma-send-n/dma_send_n.cpp"}
{"header_closure":[],"input_order":442,"kind":"independently_authored_source","object_name":"dma_reset.o","profile":"authored_cpp","sha256":"0bd01401796a3a52090e78e43efdad7da77d662c16e96517ea4d9f7998f0f070","source":"sources/gs-dma-kernel-dma-reset/dma_reset.cpp"}
{"header_closure":[],"input_order":443,"kind":"independently_authored_source","object_name":"set_def_disp_env.o","profile":"authored_cpp","sha256":"61a1b4d3fa98b063d3860a72919cf6e3b3313721060a25d8b2cf999b89bfc22b","source":"sources/gs-dma-kernel-set-def-disp-env/set_def_disp_env.cpp"}
{"header_closure":[],"input_order":444,"kind":"independently_authored_source","object_name":"put_disp_env.o","profile":"authored_cpp","sha256":"91b51b7103907efc2e9065129d6fa0ec54591736e928c8718f323724f68e123f","source":"sources/gs-dma-kernel-put-disp-env/put_disp_env.cpp"}
{"header_closure":[],"input_order":445,"kind":"independently_authored_source","object_name":"sync_path.o","profile":"authored_cpp","sha256":"fc59efc4f5f2b2caba2b6050ca87efe81f89a77ad8a1b86c2380f15fcb581290","source":"sources/gs-dma-kernel-sync-path/sync_path.cpp"}
{"header_closure":[],"input_order":446,"kind":"independently_authored_source","object_name":"vblank_state.o","profile":"authored_cpp","sha256":"84a689b6b29fd940935160e7423d7162e6101da48adc9b5831fe994c691fcec0","source":"sources/gs-dma-kernel-vblank-state/vblank_state.cpp"}
{"header_closure":[],"input_order":447,"kind":"independently_authored_source","object_name":"vblank_handler.o","profile":"authored_cpp","sha256":"69c30b1d85fba4187c76cdaacbcbac02cacb585f05559dd635b093526d4af0ee","source":"sources/gs-dma-kernel-vblank-handler/vblank_handler.cpp"}
```

The private link also needs these `emitted_functions` records in the snapshot, because `tools/build_partial.py` compares them:

| Object | Emitted functions |
| --- | --- |
| `wait_sema.o` through `get_gs_dxdy_offset.o` | the one syscall symbol, 16 bytes, except `sceGsResetPath` at 104 |
| `enable_intc.o` | `EnableIntc` 112 |
| `disable_intc.o` | `DisableIntc` 112 |
| `get_gparam.o` | `sceGsGetGParam` 12 |
| `reset_graph.o` | `sceGsResetGraph` 432 |
| `dma_get_chan.o` | `sceDmaGetChan` 40 |
| `check_address.o` | `CheckAddress` 56 |
| `memclr.o` | `memclr` 40 |
| `wait_dma.o` | `WaitDma` 156 |
| `dma_sync.o` | `sceDmaSync` 64 |
| `dma_send_n.o` | `sceDmaSendN` 104 |
| `dma_reset.o` | `sceDmaReset` 204 |
| `set_def_disp_env.o` | `sceGsSetDefDispEnv` 1104 |
| `put_disp_env.o` | `sceGsPutDispEnv` 568 |
| `sync_path.o` | `_ZL12norrath_dumpv` 200, `sceGsSyncPath` 812 |
| `vblank_state.o` | none |
| `vblank_handler.o` | `vblankHandler__Fi` 564 |

## Authored but never executed

Every function above was compiled with the reviewed EE toolchain and, except the data-only vblank words, checked by a host contract or a body comparison. The private relocatable link completed. None of these functions ran on the EE. The GS and DMA stores were not issued to hardware. The `0x01000000` spin limits were not run at full length. The `break 0,7` path returns from the authored source, so the instructions after that breakpoint in the original were not executed. `sceGsResetPath` was compared as bytes and was not run. `checkModelVersion`, `sceDmaPutEnv`, and `sceDmaPause` have no bodies here, so reset-graph's ROM version path, DMA reset's environment write, and a DMA timeout stop at those imports.

## Unknown

`checkModelVersion` at `0x002d18f0` (160 bytes) reads `rom0:ROMVER` through `sceOpen`, `sceRead`, `sceClose`, and `atoi`, and compares the result with `0x01315670`. Those callees and the ROM text are unresolved, so the offset values `_GetGsDxDyOffset` would write are unknown. Service behavior for the syscall wrappers is unknown beyond the service number and the arguments the callers pass. The GS register names on the put-display and vblank stores are inferred. Whether a read of `0x12001000` after the mode-0 store of `0x200` still returns the previous status is a property of the GS, not of the instruction sequence. What `draw` contains at runtime, and which buffer addresses are live, is outside this package. `init_vif_regs.12` is not a defined symbol. Hardware effects of the reset-path stores are unknown.
