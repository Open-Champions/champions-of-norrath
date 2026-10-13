# Game-loop plumbing, net command queue and misc state (b) — `w9-other-b`

Branch `agents/w9-other-b`. One function and 25 data objects are authored, each as its own commit under
`experimental/sources/w9-other-b-*`. Addresses and sizes come from the symbol table of the private
`SLUS_205.65` and the instruction and data bytes; nothing from the image is committed. Each data unit's
header comment gives the observed / inferred / unknown split for that object.

## Imports closed

From the assigned list, 25 of 26 names are now defined:
`nextNmDeferedCalls`, `nmDeferedCalls`, `nmDirectQWords`, `nullBuff`, `numEnvironments`, `p`, `packetbuffer`,
`packetshift`, `pnums`, `raceStrings`, `recallEnter`, `recallPos`, `scaleScale`,
`serviceResult__FP8queueCmdPi`, `sortedDir`, `sortedDirCount`, `syncRandState`, `syncState`, `t`,
`targetObjs`, `targets`, `tempBuffer`, `thereIsACutPlayer`, `timeLastTransition`, `unscrambleTable`.

Not closed: `setActiveChunks__10Teleporter`. It is already authored on the shared tree in
`w8-actor-virtuals-c-teleporter-set-active-chunks` (8 bytes at 0x00312880) but that unit is not in the
`build-plan.json` of this branch, so it still shows as unresolved. I did not duplicate it; the integrator
only needs to select the existing unit.

## Imports opened

- `file` (0x00486ce4, a local word of the original memory card unit): `serviceResult__FP8queueCmdPi` passes it
  to `sceMcClose`. The authored `MCA_Process__FPi` keeps its own `file` in an anonymous namespace, so nothing
  exports the symbol yet. It needs `file` made external there (or defined once and shared). Left unresolved
  on purpose.
- `MCA_ErrorMsg` was opened and closed in the same package (`w9-other-b-mca-error-msg`).
- Existing imports reused, defined by other units or still open for others: `MCA_Result`, `MCA_FileOpen`,
  `MCA_LastDirCount`, `MCA_Formatted`, `MCA_Present`, `cardType`, `queueStart`, `queueEnd`,
  `scePadGetSlotMax`, `sceMcClose`.

## Verification

- `serviceResult__FP8queueCmdPi` (0x0025c350, 740 bytes; authored body 800 bytes): paired run in the private
  R5900 harness (kept outside git) against the original function. 9,504 scenarios cover every command value
  0 to 14 and 0xffffffff, thirteen result values, slot and port variations, card type 0/1/2, slot maximum 1/2
  and the file-open flag. Call logs (`scePadGetSlotMax`, `sceMcClose` arguments), every changed byte outside
  the stack, and the string `MCA_ErrorMsg` points to all matched; 178 of the 185 original instruction words
  were executed (the remaining seven were not reached by any scenario and were not inspected further). Replacing the `-9` bound by `-8` produced
  120 mismatches. The committed `host_check.cpp` checks representative commands on the host.
  The paired run compared behavior; the compiled bytes do not match the original.
- Data: each unit has a host check of the initial value and a size assertion, and compiles with the
  `authored_cpp` flags to an object whose symbol size equals the original size or gap (`packetbuffer` 8512,
  `targets` 6360, `tempBuffer` 95936, `nullBuff` 16384, `sortedDir` 640 and so on). For `targets` and
  `raceStrings` the object was linked in a private image and compared word by word with the original data:
  all non-pointer words are identical and every pointer leads to an identical string.
- Initialized-to-zero objects are emitted in zero-filled storage by the compiler rather than file-backed data.

## Build-plan entries needed

Append after the current last input (profile `authored_cpp`, header closure empty, kind
`independently_authored_source`). `snapshot.json` must record each source and its `emitted_functions`
(`serviceResult__FP8queueCmdPi`, 800 compiled body bytes, for the first unit; the rest define data only).
Do not add any `host_check.cpp`.

| Source | Object name | SHA-256 |
| --- | --- | --- |
| `sources/w9-other-b-mca-error-msg/mca_error_msg.cpp` | `mca_error_msg.o` | `deef842a10b356989899331c42b45a0c1ada6c66e1d4ac96dc6328fe8c9faa3b` |
| `sources/w9-other-b-next-nm-defered-calls/next_nm_defered_calls.cpp` | `next_nm_defered_calls.o` | `1c282873d4f41391b501a753ca284cef22d04a5187f77861f4ec3a2695852b64` |
| `sources/w9-other-b-nm-defered-calls/nm_defered_calls.cpp` | `nm_defered_calls.o` | `b589410c9dc48db951cefa2c92dfbde2534b71d802efd0788e33b11f2337e349` |
| `sources/w9-other-b-nm-direct-qwords/nm_direct_qwords.cpp` | `nm_direct_qwords.o` | `1ebeb1a3ae2233424ade18c93fd49f72f0af20a01d33407300e61c93ad9c5b99` |
| `sources/w9-other-b-null-buff/null_buff.cpp` | `null_buff.o` | `67911d9e2b1de2e94dd9f86e218492b2271a1810671fd684e4706476e258f41b` |
| `sources/w9-other-b-num-environments/num_environments.cpp` | `num_environments.o` | `3cd30f81a923b9bf8e6d1d8592a7ff76538447cf10e672967a3e6abc56133b8d` |
| `sources/w9-other-b-p/p.cpp` | `p.o` | `ff24a964cf81118817d6497bd6cf6e83b1352bb770d07acbef06dede4969e496` |
| `sources/w9-other-b-packetbuffer/packetbuffer.cpp` | `packetbuffer.o` | `0d467df64b2d9f00c6bef3ab9714d5a649e56dc37dd8b71b2dc07d547ff0e179` |
| `sources/w9-other-b-packetshift/packetshift.cpp` | `packetshift.o` | `5516beeed0c3e557091c0a718af6666647b2624178c6517aa53a118540faf432` |
| `sources/w9-other-b-pnums/pnums.cpp` | `pnums.o` | `21e234800e1d91a41a5cb7325121276750ed96d084cbbe475558da5993b06c4e` |
| `sources/w9-other-b-race-strings/race_strings.cpp` | `race_strings.o` | `48a5ebcb48320099a876c71bfc96a1e333dba0620093dff1780237b5269feccc` |
| `sources/w9-other-b-recall-enter/recall_enter.cpp` | `recall_enter.o` | `c644140d4b975ffa2975825f3f6e082004727f99eba79ab82dc4ff87ba5cee40` |
| `sources/w9-other-b-recall-pos/recall_pos.cpp` | `recall_pos.o` | `e73555fd8fd63d46775f5ff1398ad5085827c8ad3b49d18e438b1bf935da2abd` |
| `sources/w9-other-b-scale-scale/scale_scale.cpp` | `scale_scale.o` | `c2759db9b5f2db3a51ead09b230d78a4a7b9af0e06e9b2a0764ef334e626bbdf` |
| `sources/w9-other-b-service-result/service_result.cpp` | `service_result.o` | `dce87063f495a120bcc1687ea35b1f6fed045d31595183aab8710b5e4f0f7aa6` |
| `sources/w9-other-b-sorted-dir-count/sorted_dir_count.cpp` | `sorted_dir_count.o` | `7250e2afa2612e1c4e19cfc889c0f3a6c9ae1188797c999c2ff7ffaa78fb073b` |
| `sources/w9-other-b-sorted-dir/sorted_dir.cpp` | `sorted_dir.o` | `c9491930225b3f659edefbe95b66c3352eb8d62499583c731394dea5c37bb818` |
| `sources/w9-other-b-sync-rand-state/sync_rand_state.cpp` | `sync_rand_state.o` | `0e27ade4476db120751d9777e79f869acd0aaf9c949beac644df233ed6b31f5e` |
| `sources/w9-other-b-sync-state/sync_state.cpp` | `sync_state.o` | `8faab2ac2bcf0089ed8d64701a2b7ce177cc2ddeed1249b55872faaa3460fc23` |
| `sources/w9-other-b-t/t.cpp` | `t.o` | `e32b0e5ad58aaaa70c14453c39012228cc2fb099e38a1cd45c218021608cd176` |
| `sources/w9-other-b-target-objs/target_objs.cpp` | `target_objs.o` | `8d990371f22bcf12cd73e0b7b51ac232f7713a695d334b305702393f8250144e` |
| `sources/w9-other-b-targets/targets.cpp` | `targets.o` | `3dde9d71af2e911e8a46e41e53fffe13b69e8cb98ac55d58f562f5ccf5b08e47` |
| `sources/w9-other-b-temp-buffer/temp_buffer.cpp` | `temp_buffer.o` | `585203b10e191b640fdc693d53c26f968eae4aa43317ea8391925943741d1878` |
| `sources/w9-other-b-there-is-a-cut-player/there_is_a_cut_player.cpp` | `there_is_a_cut_player.o` | `ba77e339bdb2b7bfb8023ec9e52fc1d291a34d64d58bb3f5b30472e47e23239b` |
| `sources/w9-other-b-time-last-transition/time_last_transition.cpp` | `time_last_transition.o` | `cfd76c87e8c5b3c36acb1a01a74e2f6192c06a3d30f3eaf4a9736e44df1412b5` |
| `sources/w9-other-b-unscramble-table/unscramble_table.cpp` | `unscramble_table.o` | `6f8cf2cf884961ff3b93d54b683aad0c111a4d7f3e65e5af97468f1299e58733` |

## Authored but never executed

- Only `serviceResult__FP8queueCmdPi` has been run against the original. Its inputs came from a hand-built
  queue entry and globals, not from a live memory card; real sceMc result codes and timing are untested.
- The data units were checked only for their initial values and sizes. No reader or writer of the original
  was run against the authored storage.

## Unknown

- The names behind most of these objects (`t`, `p`, `pnums`, `targets` fields); only descriptive field names
  are used, labeled as such in the comments.
- `tempBuffer` (95,936 bytes), `sortedDir` (640) and `targetObjs` use the gap to the next symbol as their size,
  which can include padding: the 8 spare bytes after the ten `targetObjs` pointers are not explained.
- The four byte fields of each `targets` record and its 612-byte working area.
- The meaning of the sceMc result codes handled by `serviceResult`, and any reader of `MCA_ErrorMsg`.
- `nullBuff`: the authored `memcard-calls-mca-init` keeps a private file-local copy of the same name, so its
  zeroing does not reach this definition until that unit uses the shared symbol.
- `t` (0x00484c80) is stored as an integer zero and loaded as a float; the file-local `t` at 0x004844e8 used by
  the menu code is a different object and is not defined here.
