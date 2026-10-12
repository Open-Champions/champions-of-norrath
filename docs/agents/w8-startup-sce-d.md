# Startup SCE/IOP library and boot-support (4/4)

Package work is on `agents/w8-startup-sce-d`. Source is under `experimental/sources/w8-startup-sce-d-*`. Addresses were read from the pinned SLUS-20565 symbol table and instruction bytes of the private executable, used only as a local reference. No original bytes, disassembly or decompiler output is published here.

Two comparison tools exist. `w8-startup-sce-d-body-check/reloc_check.cpp` compares a compiled function body with the bytes at an address of the local executable, ignoring only the relocated field of any instruction that carries a relocation, and prints each relocated word's symbol so targets can be confirmed by hand:

```sh
c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/reloc_check experimental/sources/w8-startup-sce-d-body-check/reloc_check.cpp
<reviewed EE g++> <profile flags> -c <unit> -o /tmp/unit.o
/tmp/reloc_check <private executable> <address> /tmp/unit.o <symbol>
```

Every other unit has a `host_contract.cpp` beside it (`c++ -std=c++17 -Wall -Wextra -Werror -o /tmp/check <dir>/host_contract.cpp && /tmp/check`). The contracts include the unit, replace each import with a recording fake, and fail on a changed call order, argument, stored word, return value or error path. Neither kind of file is a build-plan input.

A private copy of the plan and snapshot, kept outside git with the 29 entries below, compiled all 1,436 inputs and linked with no duplicate definitions: 1,132 function definitions and 1,191 unresolved imports, zero COMMON. `tlbsrc` is the only name of this package that is still unresolved. `python3 -m unittest discover -s tools -p 'test_*.py'` passed, 214 tests. `tools/check_source_snapshot.py` was not run against these paths and will reject them until the lead records them in the snapshot.

## Functions

| Symbol | Address | Original bytes | Compiled bytes | Check |
| --- | --- | --- | --- | --- |
| `sceSifSetReg` | 0x002d54e0 | 16 | 16 | byte match (service 121 stub, assembler) |
| `sceSifGetReg` | 0x002d54f0 | 16 | 16 | byte match (service 122 stub, assembler) |
| `sceResetttyinit` | 0x002d5560 | 12 | 12 | byte match outside relocated fields |
| `sceSifGetSreg` | 0x002d7160 | 24 | 24 | byte match outside relocated fields |
| `sceSifFreeIopHeap` | 0x002dc2a0 | 28 | 8 | host contract |
| `sceSifExitRpc` | 0x002d7aa8 | 36 | 36 | host contract |
| `sceMcEnd` | 0x002e0d60 | 76 | 80 | host contract |
| `sceSifSendCmd` | 0x002d7690 | 60 | 32 | host contract |
| `serialPutchar` | 0x002d6868 | 52 | 64 | host contract |
| `sceMtapGetModVersion` | 0x002d4c40 | 100 | 120 | host contract |
| `scePadGetSlotMax` | 0x002d46e0 | 104 | 120 | host contract |
| `scePadGetModVersion` | 0x002d4748 | 100 | 124 | host contract |
| `scePadEnd` | 0x002d3690 | 128 | 156 | host contract |
| `scePadInit2` | 0x002d35f0 | 156 | 160 | host contract |
| `sceSifAllocIopHeap` | 0x002dc138 | 112 | 136 | host contract |
| `sceSifAddCmdHandler` | 0x002d7490 | 120 | 112 | host contract |
| `sceLibnetTerminate` | 0x002e5e28 | 128 | 176 | host contract |
| `sceSifExecNotifyHandler` | 0x002df7b0 | 168 | 156 | host contract |
| `sceSifResetIop` | 0x002dd360 | 340 | 296 | host contract |
| `sceSifInitCmd` | 0x002d71a8 | 640 | 600 | host contract |
| `sceMcGetInfo` | 0x002e16c0 | 380 | 360 | host contract |
| `sceOpen` | 0x002d92f8 | 644 | 900 | host contract |
| `sceRead` | 0x002d9938 | 608 | 836 | host contract |
| `sceSdRemote` | 0x002e0478 | 904 | 1504 | host contract |
| `setSRSInfo__Fv` | 0x001ca528 | 672 | 572 | host contract |

Data units: `ttyinit` (inside the `sceResetttyinit` unit), `libfs-state` (`_send_data`, `_rcv_data_rpc`, `_iob`, `_cd`, `_fs_fsq_semid`, `_fs_iob_semid`, `_sceFs_q`), `libmc-info-state` (four `norrath_libmc_*` views), `specialHandler`, and `sceneTransitionOK.1041`. Each has a host contract that checks size spans, initial values, and (for `libfs`) the spans to the next symbol.

None of the C++ bodies except the two byte-matched ones is an original-byte match. The compiled sizes differ because the reviewed compiler tail-calls where the original saved `$ra` and calls, orders epilogues differently, and compiles unrolled copies the original wrote out. Behavior, not bytes, is what the contracts pin.

## Observed

`sceSifSetReg` and `sceSifGetReg` are 16-byte system-call stubs: services 121 and 122, with the argument and result registers untouched. `sceSifGetSreg` returns word `index` of the array at 0x008debc0 with no bound check. `sceResetttyinit` stores 0 in the word `ttyinit` at 0x00444950. `sceSifExitRpc` runs `sceSifExitCmd` and then stores 0 in `_sceSifInitCheck` (0x00444968, defined by the sony-cd unit; the sibling `_cmd_init_check` at 0x00444964 is a different word).

`sceSifFreeIopHeap` returns the result of `sceSifFreeSysMemory`. `sceSifAllocIopHeap` returns 0 without a call while `_bind` (0x00444a0c) is negative, otherwise stores the size in word 0 of `sdata` (0x008e1d40), calls the RPC on the heap client (0x008e1cc0) with command 1, mode 0, 4 bytes sent from `sdata` and 4 received at `rdata` (0x008e1d00), and returns 0 on a negative result or word 0 of `rdata`. `sceSifSendCmd` takes six arguments and calls the local `_sceSifSendCmd` with the first, a zero, and the other five. `serialPutchar` calls the local `kputchar` with 13 first when the character is 10, and returns the last result.

`sceSifAddCmdHandler` indexes a 12-byte-entry table chosen by the sign of the command id: word 3 of the command state object (0x008dea18) for negative ids with the index `id & 0x7fffffff`, word 5 for nonnegative ids. The entry gets the handler, the second argument and the current `$gp`. `sceSifExecNotifyHandler` walks 16-byte entries (the 512-byte `g_SysRebootBuf` with count 32 for a nonzero first argument, otherwise `g_pUserRebootBuf` with the count `g_iUserRebootSize`), skips null handlers, switches `$gp` from entry word 2, calls the handler with the second argument and entry word 1, and restores `$gp`. `sceSifResetIop` is described in its source header: two notify passes, DMA stop, a 104-byte packet built in a 128-byte buffer, one `sceSifSetDma`, and four register writes on success. `sceSifInitCmd` is described in its header; it fills the command state, clears 32 handler records (words 0 and 1 only) and 32 soft-register words, registers two handlers, sets up the SIF0 DMA handler and ends with a `sceSifSendCmd`.

`scePadGetSlotMax`, `scePadGetModVersion`, `scePadEnd` and `scePadInit2` use the padsif client (0x008dd880) and the 128-byte padman buffer (0x008ddac0) with commands 13, 18, 15 and 16, each as one RPC with command 1, mode 0, buffer sent and received, 128 bytes each way. `scePadGetSlotMax` stores the port in word 1 and returns word 3, or 0 on a negative result. `scePadEnd` additionally clears `isInit` when word 3 is 1. `scePadInit2` ignores its argument, clears three words at offsets 16, 20 and 24 of each of eight `PadInfo` entries (28-byte step, second half 112 bytes on) and stores 0 in buffer word 4. `sceMtapGetModVersion` uses the `module_version` client (0x008ddbe0) and the buffer 0x008ddc40 and prints `sceMtapGetModVersion: rpc error\n` through `DPRINT` on a negative result.

`sceMcEnd` deletes the two memory-card semaphores (`semaidRegFunc`, `semaid_timerfunc`), clearing the first before the second delete, only when the first is nonnegative, and returns 1. `sceMcGetInfo` is described in its header: -100 for an unbound client, -200 when `PollSema` fails, otherwise an asynchronous RPC (command 1, mode 1) with the parameter block `sifParamOrd`, `retval`, and the completion function `mceGetInfoApdx`.

`sceOpen` and `sceRead` are the file-system client of the original library: a get/new slot from the 16-byte `_iob` table, one request in `_send_data`, a semaphore-guarded RPC on `_cd`, and error results -1, -9, -11, -19 and 0xfffefffc. Their headers list each word. `sceRead` queues the semaphore in `_sceFs_q` when the slot flags have bit 0x8000.

`sceSdRemote` is described in its header, command by command. `setSRSInfo__Fv` fills `netGameLaunchInfo` (see its header for the offsets).

## Inferred

The names `norrath_libmc_info_*_pointer_word_view`, `norrath_libmc_param_next_view`, `norrath_sce_d_race_strings_view_4293e8` and `norrath_sce_d_unknown_sdata_word_view_4852d8` are mine and mark objects whose original names are file-local and ambiguous. The three `norrath_libmc_info_*` words are named for their use in `sceMcGetInfo`. The ten-pointer table at 0x004293e8 is named `raceStrings.1135` in the symbol table; that it indexes races is from the name alone.

The `ttyinit` word is published global from this unit because nothing else in the tree owns it yet. Other `ttyinit` users (the TTY code around `kputchar`) are not part of this package.

`_send_data`'s 3,136-byte span is only the distance to the next symbol, `_rcv_data_rpc`. `sceOpen` uses 1,048 bytes of it, `sceClose` 20, `sceRead` 32.

The reviewed `-G0` profile tail-calls `sceSifFreeSysMemory` and the last `kputchar`, which is why those bodies are smaller than the originals.

## Unknown

What any of the IOP command numbers, `sceSdRemote` command numbers (0x8130, 0x8140, 0x8160, 0x8170, 0x81a0 to 0x81d0, 0x9000 to 0x90f0) and fs request words mean on the IOP side. Whether the `_send_data` span is one object. What `sif_buffer` bytes 0x8ddc40 contain beyond the first word. Why the original returns a `$gp` value in `$v0` from `sceSifExecNotifyHandler` and what the word at 0x004852d8 (read as an int by `setSRSInfo__Fv` when flag bit 2 is set; it lies at the start of the string "Color") was meant to hold. The sizes of `playerSelectInfo`'s fields beyond the string at offset 0 and the signed byte at offset 32. The kernel's use of service 121/122 beyond the SDK names.

## Imports closed

`sceLibnetTerminate`, `sceMcEnd`, `sceMcGetInfo`, `sceMtapGetModVersion`, `sceOpen`, `scePadEnd`, `scePadGetModVersion`, `scePadGetSlotMax`, `scePadInit2`, `sceRead`, `sceResetttyinit`, `sceSdRemote`, `sceSifAddCmdHandler`, `sceSifAllocIopHeap`, `sceSifExecNotifyHandler`, `sceSifExitRpc`, `sceSifFreeIopHeap`, `sceSifGetReg`, `sceSifGetSreg`, `sceSifInitCmd`, `sceSifResetIop`, `sceSifSendCmd`, `sceSifSetReg`, `sceneTransitionOK.1041`, `serialPutchar`, `setSRSInfo__Fv`, `specialHandler`. All were confirmed absent from the unresolved list in the private full build.

Not closed: `tlbsrc` (0x004459d0, 816 bytes, `.data`). It is original machine code that `InitTLBFunctions` copies to 0x80075000. Publishing its bytes or a listing would republish original code, and a source reconstruction of a kernel TLB handler is a separate project; kernel-patches reached the same conclusion for `osdsrc`. It needs an owner decision (link from a private extraction at integration time, or a reconstruction written from understanding).

Also defined here and now closed for others: `_send_data`, `_rcv_data_rpc`, `_iob`, `_cd`, `_fs_fsq_semid`, `_fs_iob_semid`, `_sceFs_q` (the file-system state that `sceClose` in w8-startup-sce-c imports), `ttyinit`.

## Imports opened

Local routines of the original library, stay unresolved until someone owns them: `_sceSifSendCmd`, `kputchar`, `get_iob`, `new_iob`, `_sceFsWaitS`, `_sceFsSigSema`, `_fs_version`, `sceFsInit`, `mceGetInfoApdx`, `sceSifExitCmd`, `sceSifFreeSysMemory`, `sceSifMUnBindRpc`, `sceSifSetDChain`, `sceSifSetDma`, `sceSifStopDma`.

Memory-card and player code: `mcRunCmdNo`, `retval`, `MC_ByteCopy__FPvT0i`, `MC_GetNetGameInfo__FPiT0Pc`, `MC_GetRamPlayerPtr__Fi`, `MC_GetTempPlayerStorage__Fi`, `playerSelectInfo` (0x00438910, 164 bytes, global in the original).

Views owned by w8-pad-memcard-b (not on this branch yet): `norrath_libpad_rpc_clients_view`, `norrath_libpad_rpc_buffer_view`, `norrath_libpad_port_info_view`, `norrath_libpad_is_init_word`, `norrath_libmtap_module_version_client_view`, `norrath_libmtap_rpc_buffer_view`. They are declared in my units as `int` or `unsigned char` arrays; the owner defines them with its own element type, which is fine for the linker but each needs word alignment (the padman buffer and the PadInfo block are 16 and 64 aligned in the original).

Names mine, to be defined by whoever owns the objects: `norrath_sce_d_race_strings_view_4293e8` (the ten-pointer table at 0x004293e8) and `norrath_sce_d_unknown_sdata_word_view_4852d8`. libc: `strcpy`, `strcat`, `strlen` (w8-runtime-libc-b), `DPRINT` (w8-startup-sce-a).

## Overlaps worth the integrator's attention

The `sif-keyboard` heap-query unit (`sceSifQueryMaxFreeMemSize`, `sceSifQueryTotalFreeMemSize`) keeps its own `static cd` and `rdata` objects, so its RPC calls use a client that `sceSifInitIopHeap` never binds. `sceSifAllocIopHeap` here uses the shared `norrath_iop_heap_client`, `norrath_sound_iop_heap_receive_view` and `norrath_sound_iop_heap_send_view` instead, which is what the original does. The `sif-keyboard` queries should be moved to the same objects.

The audio package defines the command-init storage and the three handlers (`norrath_sound_command_*`) that `sceSifInitCmd` installs. This package's `sceSifInitCmd` uses those names. The sound package's `norrath_sound_initialize_command` is an independent authoring of the same function at the same address, so only one of them should be in the plan; a symbol alias from `sceSifInitCmd` to it would also close the import.

`_data_table` is two different locals in the original: the command state at 0x008dea18 (`norrath_sound_command_state_view`) and the RPC table at 0x008e0440 (defined by sony-cd). `sceSifAddCmdHandler` uses the first.

## Authored but never executed

All of it. No EE or IOP service has run. The two syscall stubs are byte-identical to the originals but no kernel has been entered. Everything else was run only on the host against recording fakes, with host pointers truncated to 32-bit words where the original stores EE addresses. The `$gp` switching in `sceSifAddCmdHandler`, `sceSifExecNotifyHandler` and `sceSdRemote` is compiled to real `move` instructions but only exercised against a stand-in variable on the host. Hardware register accesses in `sceSifInitCmd` (0x1000e010, 0x1000c000) were exercised only through test variables.

## Build-plan entries needed

Append these to `experimental/build-plan.json` and record the same paths in `experimental/snapshot.json` (a private snapshot also needs `emitted_functions` per input). Do not add the host contracts or `w8-startup-sce-d-body-check/reloc_check.cpp`. `header_closure` is empty. `input_order` continues from 1,408 here and must be renumbered. The `special_handler.o` name collides with the pre-logo-dialog setter object, so it is renamed.

```json
{"header_closure":[],"input_order":1409,"kind":"independently_authored_source","object_name":"fs_open.o","profile":"authored_cpp","sha256":"6148c93564c37e040e782fa71e5943c625d0cbf511f45f0f7ee1e7bdcb9a6ffe","source":"sources/w8-startup-sce-d-fs-open/fs_open.cpp"}
{"header_closure":[],"input_order":1410,"kind":"independently_authored_source","object_name":"fs_read.o","profile":"authored_cpp","sha256":"521a65204766f897387f74c350c841c30b782eb5ee9caeec2f8a7f7685d19dc8","source":"sources/w8-startup-sce-d-fs-read/fs_read.cpp"}
{"header_closure":[],"input_order":1411,"kind":"independently_authored_source","object_name":"libfs_state.o","profile":"authored_cpp","sha256":"da3b42577fe63dc9f729b4162d3aff8065f58ee9c5567f4835932f24e47b5f3c","source":"sources/w8-startup-sce-d-libfs-state/libfs_state.cpp"}
{"header_closure":[],"input_order":1412,"kind":"independently_authored_source","object_name":"libmc_info_state.o","profile":"authored_cpp","sha256":"e2c8d3eb7fec16f5366a38991605ac41b229f9a282f8301596da4b7935f7f8eb","source":"sources/w8-startup-sce-d-libmc-info-state/libmc_info_state.cpp"}
{"header_closure":[],"input_order":1413,"kind":"independently_authored_source","object_name":"libnet_terminate.o","profile":"authored_cpp","sha256":"3c3cfc8c45bb664eb77bf047d9fc7cb354fa5ef828593e39022ce5c4bd6e6121","source":"sources/w8-startup-sce-d-libnet-terminate/libnet_terminate.cpp"}
{"header_closure":[],"input_order":1414,"kind":"independently_authored_source","object_name":"mc_end.o","profile":"authored_cpp","sha256":"e3335305286c3fc861913376bb458be5b12ad41a851fd7def1692f3e390063ab","source":"sources/w8-startup-sce-d-mc-end/mc_end.cpp"}
{"header_closure":[],"input_order":1415,"kind":"independently_authored_source","object_name":"mc_get_info.o","profile":"authored_cpp","sha256":"cbfc4b7c87a067821202e816a8debc6c8b2e9b44066d5d307785496484be3d8a","source":"sources/w8-startup-sce-d-mc-get-info/mc_get_info.cpp"}
{"header_closure":[],"input_order":1416,"kind":"independently_authored_source","object_name":"mtap_get_mod_version.o","profile":"authored_cpp","sha256":"3a8069d2e20238f367b98d9b2e3f34ee3dc88e5209b378738cf9eb0c8d4ec5d4","source":"sources/w8-startup-sce-d-mtap-get-mod-version/mtap_get_mod_version.cpp"}
{"header_closure":[],"input_order":1417,"kind":"independently_authored_source","object_name":"pad_end.o","profile":"authored_cpp","sha256":"a532742268bc76691e1dc33f520f87e25e6d06f3e4f57f4df74bcff40ac05e34","source":"sources/w8-startup-sce-d-pad-end/pad_end.cpp"}
{"header_closure":[],"input_order":1418,"kind":"independently_authored_source","object_name":"pad_get_mod_version.o","profile":"authored_cpp","sha256":"422eb2e0f1833b8ce8a2fcbd13666e001854cc78c0aca46dc88f0acb0f42a763","source":"sources/w8-startup-sce-d-pad-get-mod-version/pad_get_mod_version.cpp"}
{"header_closure":[],"input_order":1419,"kind":"independently_authored_source","object_name":"pad_get_slot_max.o","profile":"authored_cpp","sha256":"c63e09ecf322ce286e63d3e8db5777ab4086ab07fcee68356435e369cf869d9b","source":"sources/w8-startup-sce-d-pad-get-slot-max/pad_get_slot_max.cpp"}
{"header_closure":[],"input_order":1420,"kind":"independently_authored_source","object_name":"pad_init2.o","profile":"authored_cpp","sha256":"87565b0a23f263cfb39ffda5f1bf90e4fed3b70d82f0bc90dd38dbb6b5820e02","source":"sources/w8-startup-sce-d-pad-init2/pad_init2.cpp"}
{"header_closure":[],"input_order":1421,"kind":"independently_authored_source","object_name":"reset_tty_init.o","profile":"authored_cpp","sha256":"2a047687aa199b6287742caab4f133bf05f4f4a3c1469a74da68ba6209bc2d6f","source":"sources/w8-startup-sce-d-reset-tty-init/reset_tty_init.cpp"}
{"header_closure":[],"input_order":1422,"kind":"independently_authored_source","object_name":"scene_transition_ok.o","profile":"authored_cpp","sha256":"a3f254bfa898475498b52910426e03a3ebe011365414ee99b34bbc9e939a8de1","source":"sources/w8-startup-sce-d-scene-transition-ok/scene_transition_ok.cpp"}
{"header_closure":[],"input_order":1423,"kind":"independently_authored_source","object_name":"sd_remote.o","profile":"authored_cpp","sha256":"04d317cf5eb936e535d55fca7a2c5d967a732fc82a36a1d47f9394d58ae72acd","source":"sources/w8-startup-sce-d-sd-remote/sd_remote.cpp"}
{"header_closure":[],"input_order":1424,"kind":"independently_authored_source","object_name":"serial_putchar.o","profile":"authored_cpp","sha256":"554877d621dd5942c9e38e2c5e41cf75b017a78243a6c6304dedc792ff643bc1","source":"sources/w8-startup-sce-d-serial-putchar/serial_putchar.cpp"}
{"header_closure":[],"input_order":1425,"kind":"independently_authored_source","object_name":"set_srs_info.o","profile":"authored_cpp","sha256":"f82f12d67cd1318e558de7ef4b6a6d1b1039e9533896a48c734dde5e547accba","source":"sources/w8-startup-sce-d-set-srs-info/set_srs_info.cpp"}
{"header_closure":[],"input_order":1426,"kind":"independently_authored_source","object_name":"sif_add_cmd_handler.o","profile":"authored_cpp","sha256":"9554a7eb59a452a765fcb2a9d2795075353fdd179640fece77ac85ca19754341","source":"sources/w8-startup-sce-d-sif-add-cmd-handler/sif_add_cmd_handler.cpp"}
{"header_closure":[],"input_order":1427,"kind":"independently_authored_source","object_name":"sif_alloc_iop_heap.o","profile":"authored_cpp","sha256":"a4e7545c6d6dbed7613a54caf9b686af8e2baddc7a6268b6c4d0c22142377ff2","source":"sources/w8-startup-sce-d-sif-alloc-iop-heap/sif_alloc_iop_heap.cpp"}
{"header_closure":[],"input_order":1428,"kind":"independently_authored_source","object_name":"sif_exec_notify_handler.o","profile":"authored_cpp","sha256":"1ccdf85b5545d986db63f2232fbbc576b262ac5fdb87b1c03ec89994763afa5c","source":"sources/w8-startup-sce-d-sif-exec-notify-handler/sif_exec_notify_handler.cpp"}
{"header_closure":[],"input_order":1429,"kind":"independently_authored_source","object_name":"sif_exit_rpc.o","profile":"authored_cpp","sha256":"c320f2f8db60e67fcf1c4b115f0c1530187090401ad98c7d540783fcd714ab76","source":"sources/w8-startup-sce-d-sif-exit-rpc/sif_exit_rpc.cpp"}
{"header_closure":[],"input_order":1430,"kind":"independently_authored_source","object_name":"sif_free_iop_heap.o","profile":"authored_cpp","sha256":"3ffc0a2be16026cdcee163cbda83d1a8594de411cf16b4d3c2ce1251f91bd66c","source":"sources/w8-startup-sce-d-sif-free-iop-heap/sif_free_iop_heap.cpp"}
{"header_closure":[],"input_order":1431,"kind":"independently_authored_source","object_name":"sif_get_reg.o","profile":"authored_entry_asm","sha256":"24ceae83ab577f3c24029d8258a6811115c04a6348cf0c4f60ca86df8c777d6c","source":"sources/w8-startup-sce-d-sif-get-reg/sif_get_reg.S"}
{"header_closure":[],"input_order":1432,"kind":"independently_authored_source","object_name":"sif_get_sreg.o","profile":"authored_cpp","sha256":"3d5a2dd076540dcd27ed979937aa0222cc09edd82a85a16c31126314c397a201","source":"sources/w8-startup-sce-d-sif-get-sreg/sif_get_sreg.cpp"}
{"header_closure":[],"input_order":1433,"kind":"independently_authored_source","object_name":"sif_init_cmd.o","profile":"authored_cpp","sha256":"e29386b0ea76fb12c1dc684a2d36f93dd9c6b594de71923d44ddde03708ec356","source":"sources/w8-startup-sce-d-sif-init-cmd/sif_init_cmd.cpp"}
{"header_closure":[],"input_order":1434,"kind":"independently_authored_source","object_name":"sif_reset_iop.o","profile":"authored_cpp","sha256":"a6f9fe35b80cd967e2e8266cd8a48ebf73d0f804553953562f976d36b59cf162","source":"sources/w8-startup-sce-d-sif-reset-iop/sif_reset_iop.cpp"}
{"header_closure":[],"input_order":1435,"kind":"independently_authored_source","object_name":"sif_send_cmd.o","profile":"authored_cpp","sha256":"b9b62e7d96a1781fcc82e188cc75377b99a11b0e0898b75c964e6a41b0b4c77b","source":"sources/w8-startup-sce-d-sif-send-cmd/sif_send_cmd.cpp"}
{"header_closure":[],"input_order":1436,"kind":"independently_authored_source","object_name":"sif_set_reg.o","profile":"authored_entry_asm","sha256":"2993a69c2420dcde9222ab528302445440ea5bbf481925d87bb8059eb2abb472","source":"sources/w8-startup-sce-d-sif-set-reg/sif_set_reg.S"}
{"header_closure":[],"input_order":1437,"kind":"independently_authored_source","object_name":"w8d_special_handler.o","profile":"authored_cpp","sha256":"da24db9a1f9549b1aecd1a242c7067c4f625404d678c29db12c3a015cd6b29a9","source":"sources/w8-startup-sce-d-special-handler/special_handler.cpp"}
```
