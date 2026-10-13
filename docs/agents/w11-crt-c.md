# w11-crt-c: startup, C runtime and IOP/pad/memory-card library (3 of 3)

Branch `agents/w11-crt-c`. Addresses and instruction behaviour come from the pinned `SLUS_205.65` image on this
machine, used only as local reference. No game bytes, disassembly or decompiler output is committed. Nothing here
ran on an EE; the paired runs below use a private R5900 interpreter with stub callees.

## What the package found first

Of the 34 names in the scope, 15 already had an authored source (written by earlier packages and left out of the
shared plan): the twelve libpad/libmtap views (`simloop-services-pad-mtap-state`), `norrath_loader_failure_word_view`
(`level-anim-load-loader-failure-word`), `sceCbPoff_Gp` (`w10-startup-b-cb-poff-gp`) and `timer_handle`
(`w10-startup-b-ee-timer-state`). They are closed once the integrator selects them; I did not write them a second time.
`norrath_sif_m_request_end` and `norrath_sif_m_rpc_packet_buffer_view` are only imported by
`w10-sce-a-sif-m-init-rpc`; the rest are new here. The measured effect is at the end.

## How each function was checked

* Each function is its own commit, compiled alone with the reviewed `authored_cpp` flags.
* Each unit has a `host_contract.cpp` (or `host_check.cpp` for data), not a plan input, with stand-in callees. It
  fails when call order, arguments, stored words or the result are wrong. I ran hand-made single-edit mutants of each
  source (not kept in git); the survivors found that way (`scePadInfoAct` selector bytes at non-zero indexes,
  `scePadInfoMode` negative indexes, `sceTtyHandler` negative length, `sceGsSetDefDBuffDc` halving of odd negative
  heights and the first parameter condition) were closed by stronger checks. Equivalent mutants were not found.
* Paired runs (private, `priv-w11-crt-c`, outside git): original and authored function run on identical memory
  snapshots with identical stub callees; call logs (callee, arguments, the memory the callee sees), the result where
  the original defines one, every changed byte outside the stack and (for `norrath_sif_m_request_end`) the final `$gp`
  are compared. Zero mismatches in every run, with every original instruction word reached except padding nops.
  Interpreter gaps fixed privately: `mult1`/`multu1`/`mfhi1`/`mflo1` and `por` (an unsupported opcode halts both
  sides identically and would have reported a false pass; I grepped every run for `HALT`). Arguments with the top bit
  set must be sign-extended to 64 bits as the real ABI does, otherwise branches on them never take the negative side
  (found in the reboot-handler and tty runs).

## Functions

| Symbol | Address | Orig. bytes | Compiled bytes | Unit | Paired scenarios |
|---|---|---|---|---|---|
| `scePadGetDmaStr` | 0x002d39b0 | 96 | 100 | `w11-crt-c-pad-get-dma-str` | 240 |
| `scePadGetState` | 0x002d3bc8 | 120 | 136 | `w11-crt-c-pad-get-state` | 480 |
| `scePadGetReqState` | 0x002d3ce0 | 80 | 96 | `w11-crt-c-pad-get-req-state` | 480 |
| `scePadPortClose` | 0x002d38f8 | 184 | 216 | `w11-crt-c-pad-port-close` | 480 |
| `scePadInfoAct` | 0x002d3d68 | 288 | 304 | `w11-crt-c-pad-info-act` | 5376 |
| `scePadInfoMode` | 0x002d3fa8 | 312 | 340 | `w11-crt-c-pad-info-mode` | 5760 |
| `sceCdStStart` | 0x002e4110 | 52 | 32 | `w11-crt-c-cd-st-start` | 100 |
| `sceCdStResume` | 0x002e43b8 | 84 | 112 | `w11-crt-c-cd-st-resume` | 80 |
| `sceScfGetLocalTimefromRTC` | 0x002d16a0 | 72 | 68 | `w11-crt-c-scf-local-time` | 60 |
| `sceDeci2Open` | 0x002df858 | 68 | 72 | `w11-crt-c-deci2-open` | 126 |
| `sceDeci2Poll` | 0x002df8f8 | 36 | 36 | `w11-crt-c-deci2-poll` | 42 |
| `sceDeci2ReqSend` | 0x002df8c8 | 48 | 48 | `w11-crt-c-deci2-req-send` | 126 |
| `sceSifAddRebootNotifyHandler` | 0x002df6a0 | 184 | 180 | `w11-crt-c-sif-add-reboot-notify-handler` | 336 |
| `sceTtyHandler` | 0x002d62d8 | 404 | 480 | `w11-crt-c-tty-handler` | 1280 |
| `norrath_sif_m_request_end` (local `_request_end`) | 0x002e47c8 | 288 | 216 | `w11-crt-c-sif-m-request-end` | 176 |
| `sceGsSetDefDBuffDc` | 0x002d2250 | 740 | 604 | `w11-crt-c-gs-set-def-dbuff-dc` | 147 |
| `sceGsSwapDBuffDc` | 0x002d2538 | 92 | 96 | `w11-crt-c-gs-swap-dbuff-dc` | 10 |

Compiled sizes are from the reviewed profile and are not original-byte matches. Data units:
`norrath_deci2_user_area_view` (0x008e4748, symbol `userarea`, 4152 bytes = gap to the next symbol),
`norrath_sif_m_rpc_packet_buffer_view` (0x008e8400, symbol `_packet_buffer`, 2048 bytes = gap to `_data_table`, which is
32 packets of 64 bytes) and `src` (0x00486d00, a word).

## Observed

* PadInfo (0x008dd8d0) is 28 bytes per slot and 112 per port. Word +16 is the opened flag, word +0 points at a
  256-byte area. `scePadGetDmaStr` writes that area back with `SyncDCache(base, base + 255)` and returns the half
  selected by comparing the signed words at +88 and +216. The pad state/request/info routines read bytes 100..114 of
  that 128-byte half: `scePadGetState` returns 99 for a closed port, otherwise byte 112 except that bytes 112 = 6 and
  113 = 2 give 5; `scePadGetReqState` returns byte 113; `scePadInfoAct` and `scePadInfoMode` (selectors and bounds in
  their unit comments) return bytes from offsets 48.. and 80.. under checks on bytes 100, 101, 104, 105, 106, 113, 114.
* `scePadPortClose` sends command 14 with port, slot and a 1 through `sceSifCallRpc` on the first padsif client and
  clears the opened flag only when the call does not return a negative value.
* `sceDeci2Open/Poll/ReqSend` are thin wrappers around `Deci2Call` (request numbers 1, 4, 3). Open passes the 16-bit
  protocol, two pointers and the address of `userarea` with bit 29 set.
* `sceSifAddRebootNotifyHandler` mirrors the already authored `sceSifRemoveRebootNotifyHandler`: the same two tables
  (`g_pUserRebootBuf`/`g_iUserRebootSize`, or the 32-entry `g_SysRebootBuf` for identifiers with the top bit set),
  16-byte entries, stores handler, data and `$gp` under `DIntr`/`EIntr`, error 0x80000069 for an out-of-range index.
* `sceTtyHandler` events 1/2 (receive, with a count or with a complete packet to push into the queue), 3 (send
  progress), 4 (send complete) and the four diagnostic strings, as listed in its unit comment.
* `norrath_sif_m_request_end` is the longer of two same-named local routines (the other, at 0x002d7c08, is
  `w8-startup-sce-b-request-end`). It uses a second semaphore at client offset 40 for commands 0x80000019, 0x8000001a
  and 0x8000001d and a word at client offset 44 for 0x8000001d. It calls the CD-module copy of `_sceRpcFreePacket`
  (0x002e47a8); the authored `_sceRpcFreePacket` stands for both copies (same body), so the paired run binds the hook
  to both addresses.
* `sceGsSetDefDBuffDc` fills the 816-byte double-buffer record (two display environments, two draw/draw2 pairs, two
  optional clear packets and two 16-byte packet headers) and then conditionally patches the low nine bits of three
  doublewords from `sceGszbufaddr`; offsets and values are in its unit comment.

## Inferred

* The meaning of the PadInfo bytes and fields (state value, request state, actuator and mode tables), the DECI2 request
  numbers, the multi-client RPC reply commands, the roles of the gparam halfwords at 0 and 4 (interlace and field/frame
  mode, from how `sceGsSetDefDispEnv` uses them), the three adjusted doublewords as frame-buffer base fields. Each is
  marked "Inferred" in the unit and none is established beyond the observed arithmetic.
* `sceScfGetLocalTimefromRTC` passes `zone + 60 * summer - 540` minutes to `AdjustTime`; the unit of the zone value is
  inferred from the constant 540.

## Unknown

* `osdsrc` (0x00444a38, 1960 bytes) and `tlbsrc` (0x004459d0, 816 bytes) are EE machine code copied to 0x80074000 and
  0x80075000 by `InitExecPS2`/`InitTLBFunctions`. Publishing their bytes as data or listings would republish original
  code, as `docs/agents/kernel-patches.md` and `w10-world-d.md` already concluded. Closing them needs an owner
  decision (private extraction at integration, or independently written handlers). I did not attempt them.
* `dst` (0x00486d04, next to `src`) is an unresolved import used by `initPosition__Fii` and was not in my scope; no
  package I could see defines it. It would be one more `int` in `w11-crt-c-src-word`.
* Whether anything reads `userarea`, how much of its span the kernel uses, `_packet_buffer`'s packet layout, whether
  `sceTtyHandler`'s receive path is ever reached with a nonzero count in the game, what the 16-bit arguments of
  `sceGsSetDefDBuffDc` mean beyond their use, and the return values of the routines that leave a stale result register
  (`sceTtyHandler`, `sceGsSetDefDBuffDc`, `sceGsSwapDBuffDc`; authored as `void`).
* In `sceTtyHandler` the first diagnostic print passes whatever the compiler leaves in the second argument register;
  the paired run saw the same value as the original, but that is incidental, not a property of the source.

## Imports closed (by my units)

`norrath_sif_m_request_end`, `norrath_sif_m_rpc_packet_buffer_view`, `sceCdStResume`, `sceCdStStart`, `sceDeci2Open`,
`sceDeci2Poll`, `sceDeci2ReqSend`, `sceGsSetDefDBuffDc`, `sceGsSwapDBuffDc`, `scePadGetDmaStr`, `scePadGetReqState`,
`scePadGetState`, `scePadInfoAct`, `scePadInfoMode`, `scePadPortClose`, `sceScfGetLocalTimefromRTC`,
`sceSifAddRebootNotifyHandler`, `sceTtyHandler`, `src`.

Closed once the owners' units are in the plan (authored elsewhere, not repeated here):
`norrath_libmtap_module_version_client_view`, `norrath_libmtap_port_close_client_view`,
`norrath_libmtap_port_open_client_view`, `norrath_libmtap_rpc_buffer_view`, `norrath_libpad_dma_info_view`,
`norrath_libpad_is_init_word`, `norrath_libpad_is_warning_word`, `norrath_libpad_port_info_view`,
`norrath_libpad_rpc_buffer_view`, `norrath_libpad_rpc_clients_view` (`simloop-services-pad-mtap-state`),
`norrath_loader_failure_word_view` (`level-anim-load-loader-failure-word`), `sceCbPoff_Gp`
(`w10-startup-b-cb-poff-gp`), `timer_handle` (`w10-startup-b-ee-timer-state`). Also closed by my `norrath_deci2_user_area_view`
unit, which satisfies the import that `sceDeci2Open` itself opens.

Still open from the scope: `osdsrc`, `tlbsrc` (see Unknown).

## Imports opened

`SyncDCache` (0x002d59d8), `Deci2Call` (0x002d5510; already provided by an earlier package, no new import was
measured), `sceDeci2ExRecv`, `sceDeci2ExSend`, `QueuePeekWriteDone`, `sceScfGetTimeZone`, `sceScfGetSummerTime`,
`AdjustTime`, `sceGsSetDefDrawEnv`, `sceGsSetDefDrawEnv2`, `sceGsSetDefClear`, `sceGsPutDrawEnv`. Existing names also
used: `kprintf`, `iSignalSema`, `_sceRpcFreePacket`, `sceSifCallRpc`, `DIntr`, `EIntr`, `sceGsGetGParam`,
`sceGsSetDefDispEnv`, `sceGszbufaddr`, `sceGsPutDispEnv`, `norrath_sce_printf`, `stm_status`, `SCE_CD_debug`,
`dum_mode`, `sceCdStream`, `g_pUserRebootBuf`, `g_iUserRebootSize`, `g_SysRebootBuf`, `tinfo` and the other tty objects.

Measured with a private plan (shared plan + my 20 units + the four earlier units above, one partial build): 957 → 933
unresolved imports; 32 of the 34 scope names left the list, `osdsrc` and `tlbsrc` remained, and eleven names appeared
(`AdjustTime`, `QueuePeekWriteDone`, `SyncDCache`, `sceDeci2ExRecv`, `sceDeci2ExSend`, `sceGsPutDrawEnv`,
`sceGsSetDefClear`, `sceGsSetDefDrawEnv`, `sceGsSetDefDrawEnv2`, `sceScfGetSummerTime`, `sceScfGetTimeZone`).

## Build-plan entries needed

Profile `authored_cpp`; do not add `host_contract.cpp`/`host_check.cpp`. The exact JSON entries are printed by
`python3 docs/agents/w11-crt-c-plan-entries.py --toolchain /path/to/ee [--private-root DIR]` (add `--with-others` to also
include the four earlier units above, for a private measurement; the lead takes those from their owners). Object names
are the unit directory names with underscores. The snapshot needs the emitted-function records in the third column
(empty for data).

| source | sha256 | emitted functions (compiled bytes) | header closure |
|---|---|---|---|
| `sources/w11-crt-c-cd-st-resume/cd_st_resume.cpp` | `46c0e9ebd76f6cc111d70d3337699387201050313ae608fcdfa1f3f130968b22` | `sceCdStResume` (112) | - |
| `sources/w11-crt-c-cd-st-start/cd_st_start.cpp` | `2ac431fc4b20a7fad04208f067c210f497513f7f2959f47463aa9ac57172b21a` | `sceCdStStart` (32) | - |
| `sources/w11-crt-c-deci2-open/deci2_open.cpp` | `74ee97d86ece56e8aa9481b3635992dcdd815ff6ecb18f5a8b860ba3b884a373` | `sceDeci2Open` (72) | - |
| `sources/w11-crt-c-deci2-poll/deci2_poll.cpp` | `98396596d69d7aa009b0f025df4b69d4d3a90427719c7242dc6b074481c5d0e1` | `sceDeci2Poll` (36) | - |
| `sources/w11-crt-c-deci2-req-send/deci2_req_send.cpp` | `c168629739c52223ad1b6bb3d9c3a4abff9adae99ee65496c570ca48eb6d8c0d` | `sceDeci2ReqSend` (48) | - |
| `sources/w11-crt-c-deci2-user-area/deci2_user_area.cpp` | `2d8bd98967f25e96e176265eae2b79ab453f1f0394be1bee5fdba19f69d8fa11` | data only | - |
| `sources/w11-crt-c-gs-set-def-dbuff-dc/gs_set_def_dbuff_dc.cpp` | `e0cc2877fc2249f64e1ff84890d01312d0c81087d2748884395db346d255776f` | `sceGsSetDefDBuffDc` (604) | - |
| `sources/w11-crt-c-gs-swap-dbuff-dc/gs_swap_dbuff_dc.cpp` | `0fbe9b98131007a7d1b8a1363563ed06d275771f8e7e4334987af49b9399c508` | `sceGsSwapDBuffDc` (96) | - |
| `sources/w11-crt-c-pad-get-dma-str/pad_get_dma_str.cpp` | `4a7a2299709dea759a9029b382fac5c4847943a042b74fec0cfc3d9875b76b0f` | `scePadGetDmaStr` (100) | - |
| `sources/w11-crt-c-pad-get-req-state/pad_get_req_state.cpp` | `bbd0c6d9bace35b1300b67d3d9264bb6105f5229739d067b19b933f0ccecce96` | `scePadGetReqState` (96) | - |
| `sources/w11-crt-c-pad-get-state/pad_get_state.cpp` | `1c458df3223d1dfdd66dd15fa0c130d09e4828a5ecd8a64fe9fc4bbf742d99cb` | `scePadGetState` (136) | - |
| `sources/w11-crt-c-pad-info-act/pad_info_act.cpp` | `ecf5eb3487b78caa60bf95eb4193c2e0386c01a2e0c3b7f43254d03b9ae81bca` | `scePadInfoAct` (304) | - |
| `sources/w11-crt-c-pad-info-mode/pad_info_mode.cpp` | `e851b256e291705a085dfbb080bae7b026a77767ed058e954a59eaefb8031f7b` | `scePadInfoMode` (340) | - |
| `sources/w11-crt-c-pad-port-close/pad_port_close.cpp` | `360a51a5594a39a85e36d7d7d4e488b648d945cb7d6bc4b0d044052c027eae68` | `scePadPortClose` (216) | - |
| `sources/w11-crt-c-scf-local-time/scf_local_time.cpp` | `9aa309187526efbd48559285e4cda2d9a24dca5bea2134dc468c54e3d07744d7` | `sceScfGetLocalTimefromRTC` (68) | - |
| `sources/w11-crt-c-sif-add-reboot-notify-handler/sif_add_reboot_notify_handler.cpp` | `ab34e96c5769d2e6da85ae0d9efb680d425fcf5bf73281f947e7b2cf5cab2bf2` | `sceSifAddRebootNotifyHandler` (180) | - |
| `sources/w11-crt-c-sif-m-packet-buffer/packet_buffer.cpp` | `45c432adbab2f770052e9ed74c2020f446531391f841a2cea4b966bcfd92f60b` | data only | - |
| `sources/w11-crt-c-sif-m-request-end/sif_m_request_end.cpp` | `10878ecbcd9fac9484fd5e17f906ae856cb7d7efad2a63f43fd06228dfa91299` | `norrath_sif_m_request_end` (216) | - |
| `sources/w11-crt-c-src-word/src_word.cpp` | `0b92999d3a00ec34980f94a2ff485c96f361332661506f3298d0c568dc0e0924` | data only | - |
| `sources/w11-crt-c-tty-handler/tty_handler.cpp` | `7fd0ece38620d4d6efcfdb20d2775d088667c54ade635364009fb9a358b790ad` | `sceTtyHandler` (480) | `sources/w9-startup-crt-f-tty-state/tty_state.hpp` |

## What is authored but never executed

All of it, on an EE. The paired runs execute original and authored code on a private interpreter with stub callees, so
interrupts (`DIntr`/`EIntr`, `iSignalSema` from interrupt context), cache write-back (`SyncDCache`), the IOP and DECI2
side, the real RTC and timing were never exercised. `sceGsSetDefDBuffDc` and `sceTtyHandler`'s receive path were run
against stubs that fill the touched regions with random bytes, not against real GS environment writers. The host
contracts use a 64-bit host for pointer-width-sensitive units (`scePadGetDmaStr`, `sceSifAddRebootNotifyHandler`,
`sceDeci2Open`) through a `NORRATH_HOST_CONTRACT` hook or low-32-bit comparisons. The `sif_m_request_end` host build
replaces the gp-switching asm with a stub; the asm path was only covered by the paired run (which compares the final
`$gp`). Data units are only compiled and size-checked.
