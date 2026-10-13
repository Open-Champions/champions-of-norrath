# Startup and runtime support (f) — `w9-startup-crt-f`

Branch `agents/w9-startup-crt-f`; sources under `experimental/sources/w9-startup-crt-f-*`. Addresses are from the symbol table and instruction bytes of the private SLUS_205.65 on this machine (not committed).

## How each function was checked

Each function is its own commit. The libc string functions (`strcat`, `strchr`, `strrchr`, `strticmp__FPCcT0`, `ustrcat__FPUsPc`) have host tests beside them. The `sceSif*` service stubs `sceSifSetDma`, `sceSifSetDChain` and `sceSifStopDma` are assembler and have a host check of the call shape. The remaining functions were compiled with the reviewed `authored_cpp` flags and run against the original in a private paired R5900 run (outside git): original and authored code start from identical memory snapshots with identical stub callees, and the call logs, return values and every changed byte outside the stack must be equal. Several have a `host_contract.cpp` too (compile with `c++ -std=c++17`; it supplies stand-in callees). Paired runs are not part of the repository.

## Imports closed (by authored sources in this package)

sceMcOpen, sceMcRead, sceMcSeek, sceMcWrite, scePadEnterPressMode, scePadInfoPressMode, scePadRead, scePadSetActAlign, scePadSetActDirect, scePadSetMainMode, sceSdTransToIOP, sceSifExitCmd, sceSifFreeSysMemory, sceSifMBindRpcParam, sceSifMExitRpc, sceSifMUnBindRpc, sceSifRemoveCmdHandler, sceSifSetDChain, sceSifSetDma, sceSifStopDma, sceTtyInit, sceTtyRead, sceTtyWrite, strcat, strchr, strrchr, strticmp__FPCcT0, unet_shutdown__Fv, ustrcat__FPUsPc.

## Closed by units that already exist but are not in the plan (no work here)

- `sceResetttyinit` and `ttyinit`: `experimental/sources/w8-startup-sce-d-*/reset_tty_init.cpp` (`w8-startup-sce-d-reset-tty-init`) defines both. It only needs a plan entry.
- `sceSifGetSreg`: `experimental/sources/w8-startup-sce-d-sif-get-sreg/sif_get_sreg.cpp`, same situation.

## Not closed

- `srcfile` (0x00445200, 1856 bytes) and `tlbsrc` (0x004459d0, 816 bytes): observed to be R5900 machine code that `InitAlarm` and `InitTLBFunctions` copy to 0x80076000 and 0x80075000. Authoring them as bytes or a listing would republish original code, and nothing here establishes a source form. The same conclusion is in `kernel-patches.md`, `w8-core-loop-e.md` and `w8-startup-sce-d.md`. Owner decision needed.

## Imports opened

New unresolved imports of the units added here: `QueueInit` (0x002d6230, one argument, returns the queue pointer), `QueuePeekReadDone` (0x002d6298), `sceTtyHandler` (0x002d62d8, the deci2 handler), `FlushCache`, `DIntr`, `EIntr`, `sceDeci2Open`, `sceDeci2ReqSend`, `sceDeci2Poll`, plus whatever the Mc, Pad and Sif units import (listed in each unit's header comment). `QueueInit`, `QueuePeekReadDone`, `QueuePeekWriteDone` and `sceTtyHandler` are local functions of the same original library file and are owned by nobody I could find. Note that `w8-runtime-libc-b-write/write.cpp` declares `sceTtyInit(const void*)`; the original ignores any argument (`$a0` is zeroed), the authored definition takes none. Both link the same under C linkage.

The tty data objects `tinfo`, `wbuf` and `rbuf` are defined in `w9-startup-crt-f-tty-state/tty_state.cpp` (new, not in the original's closure list); `rbuf` is declared with 256 bytes as a lower bound, its true size is unknown.

## Build-plan entries needed

Not added to the shared plan. Each needs an `inputs` entry with `kind: independently_authored_source`, `profile: authored_cpp`, an `object_name`, the source path, the sha256 below, `header_closure` as shown, and an integrator-assigned `input_order`; the same sha goes in `files`. The three `.S` stubs (`sif-set-dma`, `sif-set-d-chain`, `sif-stop-dma`) need the assembler profile used by `sources/runtime-entry/start.S` (`authored_entry_asm`); I did not verify that profile for them. Host check and host contract files stay out of the plan. Also needed: the two w8 units named above.

| Source | sha256 | Header closure |
|---|---|---|
| `sources/w9-startup-crt-f-mc-open/mc_open.cpp` | `798f83464a3dc9a377483adf80aa1858a94201bd3481ee15c14d4bb9c0ac41d9` | - |
| `sources/w9-startup-crt-f-mc-read/mc_read.cpp` | `ca8da0b1e9e2594887d6356ade7bb33daf267e5c2367633803add6b6c757e05a` | - |
| `sources/w9-startup-crt-f-mc-seek/mc_seek.cpp` | `3ad1105759d4a643f0903ea43b14e5936730fd935d3d0454bb27d48ed6c466bb` | - |
| `sources/w9-startup-crt-f-mc-write/mc_write.cpp` | `6630fbf3a10aa09030708f2da5825c81d9e848f4a8e576e9036a9dbf55292aac` | - |
| `sources/w9-startup-crt-f-pad-enter-press-mode/pad_enter_press_mode.cpp` | `e33b80a177d4c96a91210eeafee71a342822775c35b99c7407ad7c1ffe755bef` | - |
| `sources/w9-startup-crt-f-pad-info-press-mode/pad_info_press_mode.cpp` | `8b8ee119848bcce47e7e286c51fe31a3187f8264e0d256c9522d2d3677b070d4` | - |
| `sources/w9-startup-crt-f-pad-read/pad_read.cpp` | `70542f095450df1beb2ffa48f0cf169eec83c40d95f23417cf6ab6cd4d945266` | - |
| `sources/w9-startup-crt-f-pad-set-act-align/pad_set_act_align.cpp` | `a3296e5d9e7c85c809eaa809665342b85c37deee8de6fe3360f6e4c0af8cba1d` | - |
| `sources/w9-startup-crt-f-pad-set-act-direct/pad_set_act_direct.cpp` | `f2a4fb941a0df20c8ee7336bef060de1a32a939a8ff9e7c55bff8652708ede6b` | - |
| `sources/w9-startup-crt-f-pad-set-main-mode/pad_set_main_mode.cpp` | `22b5a7477401e9f6bc30eb2abc9bea29e298f84defe4e0fa64b93e8c747e3cf8` | - |
| `sources/w9-startup-crt-f-sd-trans-to-iop/sd_trans_to_iop.cpp` | `eb52f2ac0074f771e7cc888a1558b73db9583c8f0f8fd28d3a2bdc9bd536a0c2` | - |
| `sources/w9-startup-crt-f-sif-exit-cmd/sif_exit_cmd.cpp` | `0c476c4e85a0a4766281df3dbad7529f39fded0d4eaa1761f08abc344fd0246f` | - |
| `sources/w9-startup-crt-f-sif-free-sys-memory/sif_free_sys_memory.cpp` | `0b7e4ed496c24aa4f89d7475bc979a15a8ce61cd9cbf8568682f55aa436e0b1a` | - |
| `sources/w9-startup-crt-f-sif-m-bind-rpc-param/sif_m_bind_rpc_param.cpp` | `7f2bb190d322fe08272246320fc5dfb853a6416f4d272999379928facbdbc0d3` | - |
| `sources/w9-startup-crt-f-sif-m-exit-rpc/sif_m_exit_rpc.cpp` | `0e0fb752e14e2ca73afa75cb9d3b8db20304a54d53511ef702c78b53b98435af` | - |
| `sources/w9-startup-crt-f-sif-m-un-bind-rpc/sif_m_un_bind_rpc.cpp` | `b244f1a69cb607c0fa856c4fe37124558d410fe7fefefa1393dbe67b2b2e836c` | - |
| `sources/w9-startup-crt-f-sif-remove-cmd-handler/sif_remove_cmd_handler.cpp` | `0140dff38fb23e662494af78bb92c27582a0a30243c5b825ffacee1ecf5afe7c` | - |
| `sources/w9-startup-crt-f-sif-set-d-chain/sif_set_d_chain.S` | `2a542403f1a9fc8d65ab6b4f11d17903cb0a5c54911974c2bf1cc436056062d8` | - |
| `sources/w9-startup-crt-f-sif-set-dma/sif_set_dma.S` | `f2416bf89d9fcb3adc5fe06160b4011ed9eaf3841a4795ef1ade6ff0337d1489` | - |
| `sources/w9-startup-crt-f-sif-stop-dma/sif_stop_dma.S` | `021f4d6fc58d84da8de88a96be185829b22c8de35ff8d4cac4c60b0f03976de6` | - |
| `sources/w9-startup-crt-f-strcat/strcat.cpp` | `ee646e1903fb520787f4fbbee6a6b7de804cd79aeef55b630f7b7baaae055dec` | - |
| `sources/w9-startup-crt-f-strchr/strchr.cpp` | `a51fef37ef168c9d27234f7e939252dbac72fed831991aa57be0f4f1ca9e2528` | - |
| `sources/w9-startup-crt-f-strrchr/strrchr.cpp` | `e053978ddae101c971eb1e29b21b05f6ad19f1d2e2ab083af323df07a001cc28` | - |
| `sources/w9-startup-crt-f-strticmp/strticmp.cpp` | `1d4ac81937ac64caa8ed8802269857df412917c04ebba6a4c64c337628970273` | - |
| `sources/w9-startup-crt-f-tty-init/tty_init.cpp` | `010a504470d95b6ad12ed8d532ee554ec39faab7d0dc0da92ef39a7da4c58463` | `sources/w9-startup-crt-f-tty-state/tty_state.hpp` |
| `sources/w9-startup-crt-f-tty-read/tty_read.cpp` | `12d0ca3da4b4b6fc2555b7cc33c621ab03bf55ff59c227591f2bbe9e288f0bb6` | `sources/w9-startup-crt-f-tty-state/tty_state.hpp` |
| `sources/w9-startup-crt-f-tty-state/tty_state.cpp` | `22b10554112bda4eab039abfcf07a7bc4c82c491617eff55221e389b0543711b` | `sources/w9-startup-crt-f-tty-state/tty_state.hpp` |
| `sources/w9-startup-crt-f-tty-write/tty_write.cpp` | `5d07d63b2f168a09e28c30285f2bb04806c24b6dd3b9d582cc5749c2a5e9c0b5` | `sources/w9-startup-crt-f-tty-state/tty_state.hpp` |
| `sources/w9-startup-crt-f-unet-shutdown/unet_shutdown.cpp` | `833aa3b8f60b432ed54f46c12c8e679a7771c5936c2aaddaddbf81eb8355ead0` | - |
| `sources/w9-startup-crt-f-ustrcat/ustrcat.cpp` | `5819f7283950de60941bb75da514da52549789009975b1c1bbde244cd885bd2f` | - |

## Authored but never executed

Everything above. The paired runs execute the authored code in a private interpreter against the original image, with stubbed callees; nothing has run on a real EE, in the linked game or in gameplay. The string functions ran on the host only. The three `.S` stubs (`sceSifSetDma`, `sceSifSetDChain`, `sceSifStopDma`) were assembled with the EE toolchain (`-mno-abicalls`) and their four instruction words compared with the original's 16 bytes at each address: identical, no relocations.

## Unknown

- Meaning of kernel services 119 (SetDma), and the other services behind the Sif stubs; the IOP-side meaning of the DMA attribute word 0 in `sceSdTransToIOP`.
- The unused 20 bytes of `tinfo` (+28..+47), the true size of `rbuf`, and the original names of the `tinfo` fields.
- What `srcfile` and `tlbsrc` should be as source.
- Whether other callers pass a meaningful value to `sceTtyInit`.
