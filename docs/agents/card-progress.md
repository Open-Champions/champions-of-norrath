# card progress words

Package work is on `agents/card-progress`. New source is under `experimental/sources/card-progress-*`. Host checks are in `host-tests/card-progress/` and stay out of the EE plan. `.gitignore` allowlists that directory the same way it allowlists `host-tests/save-net-anim/`. The reviewed profile is `authored_cpp`. Addresses below were read from the pinned SLUS-20565 symbol table and the instruction bytes. Fresh EE compilation is not an original-byte match. Nothing here has run on the EE.

`MC_Configure` calls `stopCardQuery__Fv` and then `newMode__Fii(0, 0)`. `sceMcInit` calls `sceMcSync(0, 0, 0)`. Every `MC_` and `MCA_` function stays an import. `frontEndWasRunning` stays an import for the front-end shell.

## Functions

`newMode__Fii` at `0x0025bbd0` is 28 original bytes. A zero second argument copies `asyncMode` into `oldAsyncMode` before storing the first argument, and the return register is that previous mode. A nonzero second argument stores the first argument and leaves `oldAsyncMode` unchanged. The original does not write the return register on that path. `forceGoodCard__Fi` is the only direct caller that forwards its own argument; the other fourteen direct callers pass zero. Both paths store zero in `syncState`. The authored function returns the previous mode on both paths so the zero-argument callers, including `MC_Configure`, get the observed value. The compiled body is 36 bytes.

`stopCardQuery__Fv` at `0x0025b908` is 96 original bytes. It calls `MCA_Reset__Fv`, then reads `MCA_FileOpen`. A nonzero word calls `MCA_Cmd__Fiiii(1, 0, 0, 0)` and `MCA_Sync__FPi` on `errorCondition`. It then reloads `asyncMode` and `syncState`. An `asyncMode` of -1 replaces `syncState` with zero. `errorCondition` is stored as zero afterward. The return register is the bitwise complement of the reloaded mode. The compiled body is 164 bytes.

`sceMcSync` at `0x002e15a0` is 196 original bytes. A zero `mcRunCmdNo` returns -1 and does not call a semaphore or write either pointer. Otherwise it polls `semaid_timerfunc`. Mode zero waits on that semaphore when the poll is negative and ignores the wait result. Any other mode does not wait. A non-null command pointer receives the current `mcRunCmdNo`. A negative poll on a nonzero mode returns 0 and leaves `mcRunCmdNo` set. A completed call stores zero into `mcRunCmdNo`, stores `retval` through a non-null result pointer, signals `semaidRegFunc`, and returns 1. The compiled body is 228 bytes.

## Data

Initialized words: `actCount` is 5. `actLevelCount` is the five words 22, 10, 18, 9, 5. `allActs` is five pointers to `act1`, `act2`, `act3`, `act4`, and `act5`. `_3map$autoMapBitsSize` is `0x16c8`. The four zero bytes between `actLevelCount` and `allActs` are not part of either symbol. Bytes after the automap word belong to other pooled objects.

Zero words, sized from the word or byte access and the gap to the next symbol: `asyncMode`, `oldAsyncMode`, `slotFootPrint`, `bareSlotFootPrint`, `errorCondition`, `disableMC`, `totalFootPrint`, `curAct`, `browserState`, `configState`, `slotClusters`, `totalClusters`, `lastCardPort`, `lastCardSlot`, and `saveCheckPointCount` are 4 bytes. `pauseStackPos`, `keepPlayerPositions`, and `killPlayer` are 1 byte. `iconHeader` is 964 bytes, the length `MC_Init` clears and the later card command sends. The next symbol starts four bytes after that. `sortedTimes` is 160 bytes, ten 16-byte records. The browser indexes that stride, and the list generator keeps at most ten records.

`flareAlpha` and `flareDelta` in this package are the card words at `0x00486cb0` and `0x00486cb4`. `MC_Init` stores the integer 128 and zero there. Other local symbols of the same names, at `0x00486d3c`, `0x00486d44`, and `0x00486d90`, are different objects and are not defined here.

`slotClusters`, `totalClusters`, and the card `flareAlpha` also have floating-point loads of the same address. The definitions are the 4-byte words `MC_Init` stores as integers.

The private relocatable link of these 29 objects, using the plan's `authored_cpp` flags and `ld -m elf32lr5900 -r --warn-common`, produced no COMMON symbols. `tools/build_partial.py` was not run. It rejects sources that are absent from `experimental/snapshot.json`, and that shared file was left unchanged.

## Imports closed

`_3map$autoMapBitsSize`, `actCount`, `actLevelCount`, `allActs`, `asyncMode`, `bareSlotFootPrint`, `browserState`, `configState`, `curAct`, `disableMC`, `errorCondition`, `flareAlpha`, `flareDelta`, `iconHeader`, `keepPlayerPositions`, `killPlayer`, `lastCardPort`, `lastCardSlot`, `newMode__Fii`, `oldAsyncMode`, `pauseStackPos`, `saveCheckPointCount`, `sceMcSync`, `slotClusters`, `slotFootPrint`, `sortedTimes`, `stopCardQuery__Fv`, `totalClusters`, `totalFootPrint`.

## Imports opened

`newMode__Fii` references `syncState`. `asyncMode` and `oldAsyncMode` are defined here.

`stopCardQuery__Fv` references `MCA_Reset__Fv`, `MCA_Cmd__Fiiii`, `MCA_Sync__FPi`, `MCA_FileOpen`, and `syncState`. `asyncMode` and `errorCondition` are defined here.

`sceMcSync` references `PollSema`, `WaitSema`, `SignalSema`, `mcRunCmdNo`, `semaidRegFunc`, `semaid_timerfunc`, and `retval`.

`allActs` references `act1`, `act2`, `act3`, `act4`, and `act5`.

`frontEndWasRunning` is not defined. Every `MC_` and `MCA_` function stays unresolved.

## Build-plan entries needed

Append these to `experimental/build-plan.json`. Orders 847 through 875 follow this worktree's current maximum of 846. Renumber them if another package takes those orders first. `default_authored_inputs` is 845 and becomes 874 when these 29 authored inputs are the next ones. Leave the two optional external inputs in place. `snapshot.json` needs the same sources, empty `emitted_functions` for the data objects, and a contiguous input list, or `check_source_snapshot.py` fails. Do not add the host tests.

```json
[
  {
    "header_closure": [],
    "input_order": 847,
    "kind": "independently_authored_source",
    "object_name": "new_mode.o",
    "profile": "authored_cpp",
    "sha256": "d93adbf5856726be1d2de825259ac187ffa26e89f8ba182fbc48f0a1f56f1575",
    "source": "sources/card-progress-new-mode/new_mode.cpp"
  },
  {
    "header_closure": [],
    "input_order": 848,
    "kind": "independently_authored_source",
    "object_name": "stop_card_query.o",
    "profile": "authored_cpp",
    "sha256": "d0bd6c54817eadf085353a11d5c3756a4a500a6b46d4f5bff40107f2ff1b2fd7",
    "source": "sources/card-progress-stop-card-query/stop_card_query.cpp"
  },
  {
    "header_closure": [],
    "input_order": 849,
    "kind": "independently_authored_source",
    "object_name": "sce_mc_sync.o",
    "profile": "authored_cpp",
    "sha256": "2c733ef99908c981740d78af642fa67ed25e4113392b997b9e495154b8fe7c0d",
    "source": "sources/card-progress-sce-mc-sync/sce_mc_sync.cpp"
  },
  {
    "header_closure": [],
    "input_order": 850,
    "kind": "independently_authored_source",
    "object_name": "flare_alpha.o",
    "profile": "authored_cpp",
    "sha256": "d655d7c3a4a24c490a095a716752e809d830cb71c77b04a3c149a117cf714df5",
    "source": "sources/card-progress-flare-alpha/flare_alpha.cpp"
  },
  {
    "header_closure": [],
    "input_order": 851,
    "kind": "independently_authored_source",
    "object_name": "config_state.o",
    "profile": "authored_cpp",
    "sha256": "f316cbfcc5533576af41c1de8c0b6a29c17300a8b09c495b0a697bf6ece5e8e9",
    "source": "sources/card-progress-config-state/config_state.cpp"
  },
  {
    "header_closure": [],
    "input_order": 852,
    "kind": "independently_authored_source",
    "object_name": "browser_state.o",
    "profile": "authored_cpp",
    "sha256": "93be8e735ca323fc9a3e0444941975ecd365c1b6aca345d7d41a6ee85face196",
    "source": "sources/card-progress-browser-state/browser_state.cpp"
  },
  {
    "header_closure": [],
    "input_order": 853,
    "kind": "independently_authored_source",
    "object_name": "flare_delta.o",
    "profile": "authored_cpp",
    "sha256": "334deaf40e5b93edc30a3b73d1b37da50e0f1232ed092296184df7a5cb7da860",
    "source": "sources/card-progress-flare-delta/flare_delta.cpp"
  },
  {
    "header_closure": [],
    "input_order": 854,
    "kind": "independently_authored_source",
    "object_name": "async_mode.o",
    "profile": "authored_cpp",
    "sha256": "911ec7c730fe98bd9f282991e4ebaa1085061ff72b32c040b1a63f75dce518fa",
    "source": "sources/card-progress-async-mode/async_mode.cpp"
  },
  {
    "header_closure": [],
    "input_order": 855,
    "kind": "independently_authored_source",
    "object_name": "disable_mc.o",
    "profile": "authored_cpp",
    "sha256": "59d71fc7f98bb0dec8fb7fb9ec9e81fabd100636b4e74385800c557832286a0b",
    "source": "sources/card-progress-disable-mc/disable_mc.cpp"
  },
  {
    "header_closure": [],
    "input_order": 856,
    "kind": "independently_authored_source",
    "object_name": "cur_act.o",
    "profile": "authored_cpp",
    "sha256": "5191a18acad77e271d28f66a88eb679751b8eba8fadc78441da6bcd2d3417009",
    "source": "sources/card-progress-cur-act/cur_act.cpp"
  },
  {
    "header_closure": [],
    "input_order": 857,
    "kind": "independently_authored_source",
    "object_name": "error_condition.o",
    "profile": "authored_cpp",
    "sha256": "0ced0977770e911808e50d3642049df63f7b77fea661ab0e749cecdee6afac3e",
    "source": "sources/card-progress-error-condition/error_condition.cpp"
  },
  {
    "header_closure": [],
    "input_order": 858,
    "kind": "independently_authored_source",
    "object_name": "save_checkpoint_count.o",
    "profile": "authored_cpp",
    "sha256": "a1f4447d23cd9ecd7af2612b8f2c411a78dea79c1c08a3d82adb639126ed3ed4",
    "source": "sources/card-progress-save-checkpoint-count/save_checkpoint_count.cpp"
  },
  {
    "header_closure": [],
    "input_order": 859,
    "kind": "independently_authored_source",
    "object_name": "pause_stack_pos.o",
    "profile": "authored_cpp",
    "sha256": "551a1c3493c60371fecff60baf17909a1bef9d46a6346a6128e5067a0c8de2ca",
    "source": "sources/card-progress-pause-stack-pos/pause_stack_pos.cpp"
  },
  {
    "header_closure": [],
    "input_order": 860,
    "kind": "independently_authored_source",
    "object_name": "last_card_port.o",
    "profile": "authored_cpp",
    "sha256": "2b867d82f02ae8a33af716134073247d675b9d13d13fd2f10010cc208e156334",
    "source": "sources/card-progress-last-card-port/last_card_port.cpp"
  },
  {
    "header_closure": [],
    "input_order": 861,
    "kind": "independently_authored_source",
    "object_name": "last_card_slot.o",
    "profile": "authored_cpp",
    "sha256": "4752971c03a0a23794a1c8d1f3402abea9106edaf9ecf035d22658b4f08774c0",
    "source": "sources/card-progress-last-card-slot/last_card_slot.cpp"
  },
  {
    "header_closure": [],
    "input_order": 862,
    "kind": "independently_authored_source",
    "object_name": "keep_player_positions.o",
    "profile": "authored_cpp",
    "sha256": "014ecdc5ad5109bf6019a09d40da2842862bd95085bf1b363c71b84910a122c5",
    "source": "sources/card-progress-keep-player-positions/keep_player_positions.cpp"
  },
  {
    "header_closure": [],
    "input_order": 863,
    "kind": "independently_authored_source",
    "object_name": "kill_player.o",
    "profile": "authored_cpp",
    "sha256": "ef065b52742861fa37f30e32b393a6880ecbc077805d6af85ad73abb84f978ee",
    "source": "sources/card-progress-kill-player/kill_player.cpp"
  },
  {
    "header_closure": [],
    "input_order": 864,
    "kind": "independently_authored_source",
    "object_name": "old_async_mode.o",
    "profile": "authored_cpp",
    "sha256": "0b856df7c5246baa51f370ebfcec0d16ca9f656f26822a72db59957eaa39e547",
    "source": "sources/card-progress-old-async-mode/old_async_mode.cpp"
  },
  {
    "header_closure": [],
    "input_order": 865,
    "kind": "independently_authored_source",
    "object_name": "slot_footprint.o",
    "profile": "authored_cpp",
    "sha256": "3387c78f2f4c1c4f562a07518f43b449e4d961d41ea570771c1aef110eca55a4",
    "source": "sources/card-progress-slot-footprint/slot_footprint.cpp"
  },
  {
    "header_closure": [],
    "input_order": 866,
    "kind": "independently_authored_source",
    "object_name": "act_count.o",
    "profile": "authored_cpp",
    "sha256": "03c11c36f0e84df665efd08cd1fed7f378b8cd3834615211c1c7d2d025fb2a44",
    "source": "sources/card-progress-act-count/act_count.cpp"
  },
  {
    "header_closure": [],
    "input_order": 867,
    "kind": "independently_authored_source",
    "object_name": "act_level_count.o",
    "profile": "authored_cpp",
    "sha256": "e787d28f5f3047ce2e6e9ee37048b0decf764e337e13fb6556d19ad6e5c86e66",
    "source": "sources/card-progress-act-level-count/act_level_count.cpp"
  },
  {
    "header_closure": [],
    "input_order": 868,
    "kind": "independently_authored_source",
    "object_name": "all_acts.o",
    "profile": "authored_cpp",
    "sha256": "c321df22922e24d5a783a3a5f05110c925ae9b6da87d9ebb4ec3eb9152e70189",
    "source": "sources/card-progress-all-acts/all_acts.cpp"
  },
  {
    "header_closure": [],
    "input_order": 869,
    "kind": "independently_authored_source",
    "object_name": "auto_map_bits_size.o",
    "profile": "authored_cpp",
    "sha256": "66f80d14d1d4c2c016701ae930e6dc900f4668323d916bd002eaf4f4c74e1678",
    "source": "sources/card-progress-automap-bits/auto_map_bits_size.cpp"
  },
  {
    "header_closure": [],
    "input_order": 870,
    "kind": "independently_authored_source",
    "object_name": "slot_clusters.o",
    "profile": "authored_cpp",
    "sha256": "a811013bbce14e189ccc9e2857a2612f2e220620704a3429a0dd45972e02862b",
    "source": "sources/card-progress-slot-clusters/slot_clusters.cpp"
  },
  {
    "header_closure": [],
    "input_order": 871,
    "kind": "independently_authored_source",
    "object_name": "total_clusters.o",
    "profile": "authored_cpp",
    "sha256": "2eda4a6f003b1c0e7fb00748e67ed7c8b09f5b1d615572b43ca9ebc34660881b",
    "source": "sources/card-progress-total-clusters/total_clusters.cpp"
  },
  {
    "header_closure": [],
    "input_order": 872,
    "kind": "independently_authored_source",
    "object_name": "bare_slot_footprint.o",
    "profile": "authored_cpp",
    "sha256": "c2f0c6447f93526ec4ee6a0162b6ac8ab2112ba37ffdf33074a795ebc37e4560",
    "source": "sources/card-progress-bare-slot-footprint/bare_slot_footprint.cpp"
  },
  {
    "header_closure": [],
    "input_order": 873,
    "kind": "independently_authored_source",
    "object_name": "total_footprint.o",
    "profile": "authored_cpp",
    "sha256": "b89e09e37a7ddccc58257617a9b6b37124f5f4a1ae310d5d6414c7ed8e3c112f",
    "source": "sources/card-progress-total-footprint/total_footprint.cpp"
  },
  {
    "header_closure": [],
    "input_order": 874,
    "kind": "independently_authored_source",
    "object_name": "icon_header.o",
    "profile": "authored_cpp",
    "sha256": "811069f9e3f9436f4fd643555bf85d13fa607ccb7f06e114321cad80308c3e55",
    "source": "sources/card-progress-icon-header/icon_header.cpp"
  },
  {
    "header_closure": [],
    "input_order": 875,
    "kind": "independently_authored_source",
    "object_name": "sorted_times.o",
    "profile": "authored_cpp",
    "sha256": "7e93fbf2d139e1819a455391d26f6fafe8193bf5412cde271e8149fe4843762d",
    "source": "sources/card-progress-sorted-times/sorted_times.cpp"
  }
]
```

Snapshot metadata from the private EE objects, including compiled body sizes. Data objects emit no functions.

```json
[
  {
    "input_order": 847,
    "component": "card-progress-new-mode",
    "object_name": "new_mode.o",
    "private_object_sha256": "561295883a3e9e77712fe0704823424bac63c9efe9f533c2f3bc7ad96e7e5443",
    "emitted_functions": [
      {
        "name": "newMode__Fii",
        "compiled_body_bytes": 36
      }
    ],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-new-mode/new_mode.cpp"
  },
  {
    "input_order": 848,
    "component": "card-progress-stop-card-query",
    "object_name": "stop_card_query.o",
    "private_object_sha256": "002f92187e9887d6fba7f361cc73cfafc3bcb3fc7c646c57bb1c478d31d3e5d9",
    "emitted_functions": [
      {
        "name": "stopCardQuery__Fv",
        "compiled_body_bytes": 164
      }
    ],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-stop-card-query/stop_card_query.cpp"
  },
  {
    "input_order": 849,
    "component": "card-progress-sce-mc-sync",
    "object_name": "sce_mc_sync.o",
    "private_object_sha256": "15a093df9c5b5e8d6923a70c29b82914ccf4b94f141ab197df4ddac914c78808",
    "emitted_functions": [
      {
        "name": "sceMcSync",
        "compiled_body_bytes": 228
      }
    ],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-sce-mc-sync/sce_mc_sync.cpp"
  },
  {
    "input_order": 850,
    "component": "card-progress-flare-alpha",
    "object_name": "flare_alpha.o",
    "private_object_sha256": "515aac56e93ee71cef7e78a9ad1200d8241137938c016fa76bb4dc2985723235",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-flare-alpha/flare_alpha.cpp"
  },
  {
    "input_order": 851,
    "component": "card-progress-config-state",
    "object_name": "config_state.o",
    "private_object_sha256": "8d94560eccf521f7ae472cf7bf29a03a903f52e6fbbd2b049a735c1146c346f4",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-config-state/config_state.cpp"
  },
  {
    "input_order": 852,
    "component": "card-progress-browser-state",
    "object_name": "browser_state.o",
    "private_object_sha256": "fcc75515c67dd7897fa1d31e09d68908cbbb9eecf909bbc4b69467019a6121d4",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-browser-state/browser_state.cpp"
  },
  {
    "input_order": 853,
    "component": "card-progress-flare-delta",
    "object_name": "flare_delta.o",
    "private_object_sha256": "513f63c16b148a10cb6685574bfef5098f2389749d4e09e175c76d205b6661a7",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-flare-delta/flare_delta.cpp"
  },
  {
    "input_order": 854,
    "component": "card-progress-async-mode",
    "object_name": "async_mode.o",
    "private_object_sha256": "4d7e14e0e1c800e3d65f4244c526ac6f5b893a4ad0860330497faef328435696",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-async-mode/async_mode.cpp"
  },
  {
    "input_order": 855,
    "component": "card-progress-disable-mc",
    "object_name": "disable_mc.o",
    "private_object_sha256": "a6d9c44b04125300c0a8ab03d23d93b921786063519cccbfe9fe868bcb1c6094",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-disable-mc/disable_mc.cpp"
  },
  {
    "input_order": 856,
    "component": "card-progress-cur-act",
    "object_name": "cur_act.o",
    "private_object_sha256": "501159ff033949e90dd8e61df6d11cbe5611fb181a434a9462d0e6b7fee5e79d",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-cur-act/cur_act.cpp"
  },
  {
    "input_order": 857,
    "component": "card-progress-error-condition",
    "object_name": "error_condition.o",
    "private_object_sha256": "d0982e5257b6224987f1d2e70ac4fe9b425cfe520effe48bc02379967b5c50e5",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-error-condition/error_condition.cpp"
  },
  {
    "input_order": 858,
    "component": "card-progress-save-checkpoint-count",
    "object_name": "save_checkpoint_count.o",
    "private_object_sha256": "8565d57246e577ba37f6911d493627bdd95143f20ae3558442e87a9201c3056d",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-save-checkpoint-count/save_checkpoint_count.cpp"
  },
  {
    "input_order": 859,
    "component": "card-progress-pause-stack-pos",
    "object_name": "pause_stack_pos.o",
    "private_object_sha256": "dc4a96aa30be3adbf51c4e8b4da1352cc1ebdcc876ad68ba4abaa76208d85da1",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-pause-stack-pos/pause_stack_pos.cpp"
  },
  {
    "input_order": 860,
    "component": "card-progress-last-card-port",
    "object_name": "last_card_port.o",
    "private_object_sha256": "d2903b2b0eba68ed0c324973cc8121686d023f256deaf45082de9af68b4e4835",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-last-card-port/last_card_port.cpp"
  },
  {
    "input_order": 861,
    "component": "card-progress-last-card-slot",
    "object_name": "last_card_slot.o",
    "private_object_sha256": "e41198b08e649b1cc361ec78f5f76a432dc8bcd9aa280ef19ffad4ce0f536bc4",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-last-card-slot/last_card_slot.cpp"
  },
  {
    "input_order": 862,
    "component": "card-progress-keep-player-positions",
    "object_name": "keep_player_positions.o",
    "private_object_sha256": "a9e1c91ccb90807e80af6ae3dd42ee08aab147dd2d9f2c51d7b9b9d8af312c44",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-keep-player-positions/keep_player_positions.cpp"
  },
  {
    "input_order": 863,
    "component": "card-progress-kill-player",
    "object_name": "kill_player.o",
    "private_object_sha256": "7b67aa1f93af77f2f244db7fd0e4803abd025365eb6ed53541bc4bc174cdef64",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-kill-player/kill_player.cpp"
  },
  {
    "input_order": 864,
    "component": "card-progress-old-async-mode",
    "object_name": "old_async_mode.o",
    "private_object_sha256": "4757ccb57b857ea8281106711a3ca7aef33aa97375eeafae7dafc2133461b9cd",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-old-async-mode/old_async_mode.cpp"
  },
  {
    "input_order": 865,
    "component": "card-progress-slot-footprint",
    "object_name": "slot_footprint.o",
    "private_object_sha256": "155a82da96422a39a571b493d9068ccc660abeaddb2ec52e39307d30bcbfe931",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-slot-footprint/slot_footprint.cpp"
  },
  {
    "input_order": 866,
    "component": "card-progress-act-count",
    "object_name": "act_count.o",
    "private_object_sha256": "457fe954b011bd8182f6cbcf995fc59119e20722be2d762e9486d3d9e5677f9f",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-act-count/act_count.cpp"
  },
  {
    "input_order": 867,
    "component": "card-progress-act-level-count",
    "object_name": "act_level_count.o",
    "private_object_sha256": "c59695c8a968bf96c5a551d6808004586a32f61c4d02ca3e7b28119f8e3a723f",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-act-level-count/act_level_count.cpp"
  },
  {
    "input_order": 868,
    "component": "card-progress-all-acts",
    "object_name": "all_acts.o",
    "private_object_sha256": "9d2275f9704f05c3068b00d30bed3279a8c8793fdb03790585ae87f5b496f0ac",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-all-acts/all_acts.cpp"
  },
  {
    "input_order": 869,
    "component": "card-progress-automap-bits",
    "object_name": "auto_map_bits_size.o",
    "private_object_sha256": "31ba0786589df0f24554b4d5690042edcbff7eb7a387836334c19eeb3a11afa3",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-automap-bits/auto_map_bits_size.cpp"
  },
  {
    "input_order": 870,
    "component": "card-progress-slot-clusters",
    "object_name": "slot_clusters.o",
    "private_object_sha256": "c5d0d24675cbca6114a46c869e80dc41a12f7ef11b5244102de350546aba4568",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-slot-clusters/slot_clusters.cpp"
  },
  {
    "input_order": 871,
    "component": "card-progress-total-clusters",
    "object_name": "total_clusters.o",
    "private_object_sha256": "9d3b378175e1295d1ca6e99d1f2d58cdeacc94e9cc503642e241ef6753e12400",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-total-clusters/total_clusters.cpp"
  },
  {
    "input_order": 872,
    "component": "card-progress-bare-slot-footprint",
    "object_name": "bare_slot_footprint.o",
    "private_object_sha256": "bd27ea24217c05e5b05bd5f71a7dcb55455a330c46ae0541e44a225c91d2b3eb",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-bare-slot-footprint/bare_slot_footprint.cpp"
  },
  {
    "input_order": 873,
    "component": "card-progress-total-footprint",
    "object_name": "total_footprint.o",
    "private_object_sha256": "2b8489a9c72d3c1a1f4794edb9b9bfcb534fe278cbb7a329a6e1f40e7c09e627",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-total-footprint/total_footprint.cpp"
  },
  {
    "input_order": 874,
    "component": "card-progress-icon-header",
    "object_name": "icon_header.o",
    "private_object_sha256": "b420af47c9aa2a8ca05cabd973781baf822d0228462da138595cab77dc74d60f",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-icon-header/icon_header.cpp"
  },
  {
    "input_order": 875,
    "component": "card-progress-sorted-times",
    "object_name": "sorted_times.o",
    "private_object_sha256": "16ccac39d5ca1dd88f74716964fc1bedf82258f119099484c12090ab1f69f9ae",
    "emitted_functions": [],
    "kind": "independently_authored_source",
    "source": "sources/card-progress-sorted-times/sorted_times.cpp"
  }
]
```

## Authored but never executed

`newMode__Fii`, `stopCardQuery__Fv`, and `sceMcSync` were compiled with the reviewed EE toolchain and checked by host contracts. The host contracts fail if the mode save, the card-query clear, or the semaphore completion path is wrong. The data objects were checked against the observed sizes and initial bytes. None of these functions ran on the EE, under the original process, or against gameplay. The semaphore calls were not issued to the IOP. The icon header and sorted-time records were not filled by `MC_Init` or the browser.

## Unknown

The mode numbers stored in `asyncMode` are unknown. The original nonzero-argument path of `newMode__Fii` leaves the return register unchanged; this C function returns the previous mode there instead. What `MCA_Reset__Fv` and `MCA_Sync__FPi` change, besides the words reloaded after them, is unknown. Which routine reads `oldAsyncMode` is unknown. What the floating-point loads of `flareAlpha`, `slotClusters`, and `totalClusters` expect from those integer bits is unknown. The two other local `flareAlpha` symbols and the other local `flareDelta` are not this package. The bytes inside a 16-byte sorted-time record, and which of the 964 icon-header bytes stay zero after `MC_Init`, are unknown. Retail revision beyond the pinned executable hash is unknown.
