# Startup SCE/IOP library and boot-support (3/4)

Branch `agents/w8-startup-sce-c`. Every unit is its own commit under `experimental/sources/w8-startup-sce-c-*`. Each has a `host_contract.cpp` that compiles with the system C++ compiler, includes the source, replaces the imports with recording stubs, and returns a nonzero exit code when a result, argument, ordering or memory effect is wrong. Each source also compiles with the reviewed EE `authored_cpp` flags. The compiled sizes are not original-byte matches and the pinned ELF was not used as a verifier.

## Observed

All addresses are in the pinned `SLUS_205.65` (private, not committed).

- `sceCdSync` 0x002e2ed8, `sceCdSearchFile` 0x002e2cb0, `sceCdGetError`, `sceCdStInit`, `sceCdPOffCallback` 0x002e2848: described in the source headers (earlier commits of this branch).
- `sceCdLayerSearchFile` 0x002e2998 (788 bytes): N-command style prologue with RPC server 0x80000597, a 300-byte request block `_sf_data`, receive word `sfrdata`, 36-byte reply copied from the request block through the uncached alias, and debug prints of name, size and lbn.
- `sceCdReadDvdDualInfo` 0x002e3d90 (S-command 39, 12-byte reply), `sceCdReadClock` 0x002e3fe8 (S-command 15, RPC function 1, 16-byte reply, eight bytes copied out), `sceCdStRead` 0x002e41e0 (loop over `sceCdStream` with command 2).
- `sceGifPkInit` 0x0133efd0, `sceGifPkReset` 0x0133eff0, `sceGifPkSize` 0x0133f058, `sceGifPkTerminate` 0x0133f000, `sceGifPkAddGsData` 0x0133f2d0: a packet is three words (cursor, start, open-tag word).
- `sceGszbufaddr` 0x002d1dd8, `sceInetClose` 0x002e5400, `sceInetCtlDownInterface` 0x002e5c58, `sceInetCtlGetState` 0x002e5ce8, `sceInetGetInterfaceList` 0x002e58a8, `sceLibnetInitialize` 0x002e5d60. The Inet calls share a pattern: fill a request buffer, call an RPC helper (`CallRpc`) on the first argument, and return -542 on a negative helper result. `sceLibnetInitialize` returns -543 for a bad size and -540 for a failed bind.
- `lgKbdClose` 0x002e73f8 and `lgCodecOpen` 0x002eb0f8 (g_codecs is 40 bytes, two 20-byte records; only identifier 300 allocates).
- `sceClose` 0x002d9580. Its helpers `get_iob`, `_sceFsWaitS` and `_sceFsSigSema` are file-local (`t`) symbols in the original and are not defined here.
- Data: `loadThreadID` 0x004866c8 (4 bytes, image value 0), `sbuff` 0x007dd840 and `sbuffin` 0x007dd880 (.bss, 64 bytes by spacing), `preConfigure` 0x004289d0 (five 16-byte records, four name pointers).

## Inferred

- `sbuff`/`sbuffin` are 64 bytes each: the ELF gives no size; the neighbouring symbols are 0x40 away and callers use 64-byte receives. The other `sbuff` at 0x008e5a00 is a different object (published elsewhere as `norrath_sound_remote_packet_view`).
- The packet words of `sceGifPk*` are named cursor/start/tag from how they are used; the GS tag meaning of the word that `Terminate` increments is not established.
- The `sceInet*` first argument is an RPC client object and the second a request buffer: from how `netFree__Fb` and the launcher code pass them.

## Unknown

- Meaning of the reply words in `sceCdReadClock`, `sceCdReadDvdDualInfo` and `sceCdLayerSearchFile` beyond the printed fields.
- Meaning of `sceGsGetGParam`'s first 64-bit word in `sceGszbufaddr`, and of the `GLibData` slot states in `lgKbdClose`.
- Meaning of the three zero words after each `preConfigure` name, and what `MemcpyS` parameters mean beyond this call.
- Whether `loadThreadID` sits in small data (.sdata) in a final link; an authored zero-initialised int goes to .bss.

## Imports closed (names)

`sceCdSync`, `sceCdSearchFile`, `sceCdGetError`, `sceCdStInit`, `sceCdPOffCallback`, `sceCdLayerSearchFile`, `sceCdReadClock`, `sceCdReadDvdDualInfo`, `sceCdStRead`, `sceGifPkInit`, `sceGifPkReset`, `sceGifPkSize`, `sceGifPkTerminate`, `sceGifPkAddGsData`, `sceGszbufaddr`, `sceInetClose`, `sceInetCtlDownInterface`, `sceInetCtlGetState`, `sceInetGetInterfaceList`, `sceLibnetInitialize`, `lgKbdClose`, `lgCodecOpen`, `sceClose`, `loadThreadID`, `sbuff`, `sbuffin`, `preConfigure`. Also defined for `sceCdPOffCallback`: `sceCdPoffCbfunc`, `sceCdCbPoff_Gp`, `sceCdPoffCbdata`.

Already defined by another package, so nothing to add here (they only become resolved once that package's source is in the plan): `pad_dma_buf` (`simloop-services-pad-dma-buffer`) and `sceDmaCurrentEnv` (`gs-dma-present-sce-dma-put-env`).

## Imports not closed

- `osdsrc` (0x00444a38, 1,960 bytes): this is a binary payload that `_InitSys` copies to 0x80074000 and runs in kernel mode. Its content is original machine code and data, not a function or table I can describe independently; reproducing the bytes would be an assembly dump. It needs a decision from the owner on whether to author the kernel patch as independent source (see `kernel-patches`).

## Imports opened

Newly opened by these units (not defined in this package):
`CallRpc`, `MemcpyS`, `sceSifMBindRpcParam`, `GLibData`, `g_codecs`, `gsm_create`, `gsm_option`, `_cd`, `_fs_init`, `_iob`, `_send_data`, `_rcv_data_rpc`, `get_iob`, `_sceFsWaitS`, `_sceFsSigSema`.

Used and expected from other packages: `__wrap_malloc`, `memset`, `PollSema`, `ReferThreadStatus`, `DelayThread`, `CreateSema`, `DeleteSema`, `DIntr`, `EIntr`, `PowerOffCB`, `SignalSema`/`norrath_player_signal_semaphore`, `norrath_player_wait_semaphore`, `scePrintf`, `sceSifInitRpc`, `sceSifBindRpc`, `sceSifCallRpc`, `sceSifCheckStatRpc`, `sceSifWriteBackDCache`, `sceCdStream`, `_sceCd_scmd_prechk`, `cmd_sem_init`, `sceGsGetGParam`, and the sony-cd data objects (`_sceCd_*`, `_sf_data`, `sfrdata`, `cdrdsf`, `_sf_bind`, `_icmd_bind`, `dum_mode`, `stm_status`, `my_thid`, `my_th_info`, `ncmd_sema_keep_cmd`, `SCE_CD_debug`).

`_sf_data`, `sfrdata`, `cdrdsf`, `_sf_bind`, `dum_mode` and `stm_status` are listed as closed by `sony-cd`; if that branch has not defined all of them yet, they stay unresolved. The `get_iob` family and the `_iob`/`_send_data`/`_rcv_data_rpc`/`_cd` objects are the libfs layer (`sceFsInit` at 0x002d9028 and neighbours); whoever takes `sceOpen`/`sceRead` should define them once, with global names.

## Build-plan entries needed

Append to `experimental/build-plan.json` inputs with `input_order` taken from the next free orders on the integration tip. `snapshot.json` must record each source and host contract first. The sha256 values are for the files at the last commit of this branch. Host contracts are not listed.

```json
[
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-cd-get-error.o",
    "profile": "authored_cpp",
    "sha256": "56c83ddf30fa5a5e37e37257bc304ccbb95ba356e75ef3a060dbeef3a1f19d18",
    "source": "sources/w8-startup-sce-c-cd-get-error/get_error.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-cd-layer-search-file.o",
    "profile": "authored_cpp",
    "sha256": "bae0f5601563704b1de7cbefba21925c37e407f78a13c681f7b2a7ff9c2fabb2",
    "source": "sources/w8-startup-sce-c-cd-layer-search-file/layer_search_file.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-cd-poff-callback.o",
    "profile": "authored_cpp",
    "sha256": "5f47ab719dfecf29f4282b99b54374a5c83e0df5f3a3998c7249f8203ab9b897",
    "source": "sources/w8-startup-sce-c-cd-poff-callback/poff_callback.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-cd-read-clock.o",
    "profile": "authored_cpp",
    "sha256": "a794dd9778d41c80b51b3c60af44cdf6465013d763f567b6f5575dfd24fd8b8c",
    "source": "sources/w8-startup-sce-c-cd-read-clock/read_clock.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-cd-read-dvd-dual-info.o",
    "profile": "authored_cpp",
    "sha256": "ec88f0e022c04177b6d33d506b0a70ac02cd989bf0bcfbbc1db556d1d144751e",
    "source": "sources/w8-startup-sce-c-cd-read-dvd-dual-info/read_dvd_dual_info.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-cd-search-file.o",
    "profile": "authored_cpp",
    "sha256": "d639a519074345bc4a7d99fcf5036a073c58aed40ce4e20c598ce8af0a0c5280",
    "source": "sources/w8-startup-sce-c-cd-search-file/search_file.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-cd-st-init.o",
    "profile": "authored_cpp",
    "sha256": "bcf90a938b98c712b377db6ad4f5c05da4079d04c7cee6712d882ec7deddec1f",
    "source": "sources/w8-startup-sce-c-cd-st-init/st_init.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-cd-st-read.o",
    "profile": "authored_cpp",
    "sha256": "87be43a78633ae3b75066292c4afa374407fb21fc9d1fdba03259a0e4b89cc22",
    "source": "sources/w8-startup-sce-c-cd-st-read/st_read.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-cd-sync.o",
    "profile": "authored_cpp",
    "sha256": "3ddcc7290365b516c02579ab82d64c2157413b76de014f2a937444a6ba28fde5",
    "source": "sources/w8-startup-sce-c-cd-sync/cd_sync.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-data-load-thread-id.o",
    "profile": "authored_cpp",
    "sha256": "ba9903048a55226f3449b4f16910d0b1ee9d3d64df1e925f6cd4a637601ab737",
    "source": "sources/w8-startup-sce-c-data-load-thread-id/load_thread_id.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-data-pre-configure.o",
    "profile": "authored_cpp",
    "sha256": "a39782a87fa1a931c5cf999da8783a2c47a4598a2c083dd70620ea787bab9a61",
    "source": "sources/w8-startup-sce-c-data-pre-configure/pre_configure.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-data-sbuff.o",
    "profile": "authored_cpp",
    "sha256": "446c3d13e50cba09756b4285886165e4aac4ce2d184c04e0dc28e1aa971257d9",
    "source": "sources/w8-startup-sce-c-data-sbuff/sbuff.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-data-sbuffin.o",
    "profile": "authored_cpp",
    "sha256": "661fdd810d222a58407971ea9e866997c9e2e31c5c7b97eb6409761063fce6c4",
    "source": "sources/w8-startup-sce-c-data-sbuffin/sbuffin.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-fs-close.o",
    "profile": "authored_cpp",
    "sha256": "ff8b3b139a5d4df921aeb778d14681baba9966a81a550e3e1d9642591d4ada4c",
    "source": "sources/w8-startup-sce-c-fs-close/fs_close.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-gif-pk-add-gs-data.o",
    "profile": "authored_cpp",
    "sha256": "fa2920f21c636f8be599a1cbadbf8f5e17d8e2f032fc75fe76911f53873074c7",
    "source": "sources/w8-startup-sce-c-gif-pk-add-gs-data/pk_add_gs_data.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-gif-pk-init.o",
    "profile": "authored_cpp",
    "sha256": "09cc9e75d1096e0e1159bbe6fd9212d957e8b4b2964639e77f1fca78b441f652",
    "source": "sources/w8-startup-sce-c-gif-pk-init/pk_init.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-gif-pk-reset.o",
    "profile": "authored_cpp",
    "sha256": "b2c79fd748aa4f1f6014326169d8ccd1eeb9db00451f734fc919ca2c3ac19553",
    "source": "sources/w8-startup-sce-c-gif-pk-reset/pk_reset.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-gif-pk-size.o",
    "profile": "authored_cpp",
    "sha256": "e56b88f2e2fd61864e87c1ed434d26b9a19532c75474bd0bc0fd95bba7a34958",
    "source": "sources/w8-startup-sce-c-gif-pk-size/pk_size.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-gif-pk-terminate.o",
    "profile": "authored_cpp",
    "sha256": "3b0a8888f97d89ab38f64f83519cb5d8eab973c8619570f1409e0efaa9196333",
    "source": "sources/w8-startup-sce-c-gif-pk-terminate/pk_terminate.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-gs-zbuf-addr.o",
    "profile": "authored_cpp",
    "sha256": "0a4bf472e5b502576346a6aa92bc1562e08bd734f804b115464bcbcbb08639eb",
    "source": "sources/w8-startup-sce-c-gs-zbuf-addr/zbuf_addr.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-inet-close.o",
    "profile": "authored_cpp",
    "sha256": "e51bb6711869a39947eb507adf703811c7ee9a6c73ce387c736a3621d4ab83c1",
    "source": "sources/w8-startup-sce-c-inet-close/inet_close.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-inet-ctl-down-interface.o",
    "profile": "authored_cpp",
    "sha256": "c57ffb73a50ae63559c71d4a90eb9ce34a3a8bdc8878c54789d5d94acd52e119",
    "source": "sources/w8-startup-sce-c-inet-ctl-down-interface/down_interface.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-inet-ctl-get-state.o",
    "profile": "authored_cpp",
    "sha256": "d44ef363141b39e41f5327cca26792101c1196c9f5ee3d856ca03e238c051bb9",
    "source": "sources/w8-startup-sce-c-inet-ctl-get-state/get_state.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-inet-get-interface-list.o",
    "profile": "authored_cpp",
    "sha256": "ce86366218a02df34a54f9f7fce6ac88f6ee029d5ac4b02e28bcefe9120e4966",
    "source": "sources/w8-startup-sce-c-inet-get-interface-list/get_interface_list.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-lg-codec-open.o",
    "profile": "authored_cpp",
    "sha256": "07e3486b445d6d087a26ae95b2c75b847a5d151c2e60c2263ad8d96a74df0851",
    "source": "sources/w8-startup-sce-c-lg-codec-open/codec_open.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-lg-kbd-close.o",
    "profile": "authored_cpp",
    "sha256": "6c7ceb12f8650159ed8ea1aec37123ebcb50eca022cf4459eb385544253482cc",
    "source": "sources/w8-startup-sce-c-lg-kbd-close/kbd_close.cpp"
  },
  {
    "header_closure": [],
    "kind": "independently_authored_source",
    "object_name": "w8-startup-sce-c-libnet-initialize.o",
    "profile": "authored_cpp",
    "sha256": "5e5943549765cc1cb6f87046bb032c58103853737d80fa262e7f9dc30f076a0c",
    "source": "sources/w8-startup-sce-c-libnet-initialize/libnet_initialize.cpp"
  }
]
```

## Authored but never executed

All twenty-seven units compile under the reviewed flags and pass their host contracts. None has run in the game or on the target: every external call is a recording stub. Specific gaps:

- Uncached-alias reads (`0x20000000` ORed into an address) are replaced by plain reads on the host.
- `sceCdPOffCallback` reads the global-pointer register; the host contract substitutes a variable.
- The 0x10000 and 0x100000 delay loops (`sceLibnetInitialize`, `sceCdLayerSearchFile`) use an empty `asm volatile` or a volatile counter; the instruction counts differ from the original.
- `sceClose` leaves the three file-local helpers and all state objects unresolved; its host contract uses a hook (`norrath_host_note_result`) to let the stub write the local result word.
- `preConfigure` holds pointers into string literals; the pause-menu code that copies player names into the entries is not checked against this layout.
- Unaligned 64-bit copies in the original (`ldl/ldr`) are written as byte or word copies.

## Verification run

`python3 -m unittest discover -s tools -p 'test_*.py'` passed (214 tests) after the last source commit. No partial build was run: the sources are not in the shared plan, and single units were compiled with the plan's `authored_cpp` flags instead.
