# Session roster

Package `agents/session-roster`. Addresses are from the pinned `SLUS_205.65` image. Everything below was read from that image's code and data and compiled with the reviewed EE toolchain. Nothing was executed on an EE or in the game. The partial link before this package listed 950 unresolved imports (908 inputs); the same link with these 11 units added (private plan copy, outside git) lists 939.

## Functions

| Symbol | Address | Image bytes | Compiled bytes |
|---|---|---|---|
| `netWaitCallBack__FPUci` | `0x001c8070` | 8 | 8 |
| `commHandler__FPUci` | `0x002cd110` | 32 | 12 |
| `commStatusHandler__FPUci` | `0x002cd130` | 56 | 60 |
| `checkForMonsters__FP10GameObject` | `0x0013b760` | 32 | 44 |
| `playerSelectionSetup__Fv` | `0x002804e8` | 32 | 8 |
| `netRegisterMessageHandler__FPFPUci_v` | `0x00134ec8` | 160 | 100 |
| `netRegisterPacketHandler__FPFPUci_v` | `0x00135068` | 160 | 100 |
| `playerSelectionStart__Fv` | `0x00279cb8` | 384 | 320 |
| `playerInit__FiiPcScScScScT2` | `0x001a1630` | 1640 | 1252 |

Compiled sizes differ because the compiler tail-calls and repacks; no byte match is claimed.

**Observed.** `netWaitCallBack` is a bare return. `commHandler` passes the byte at packet+132 and the packet to `recievePacket__FiPUc`. `commStatusHandler` ORs or clears the low nibble of the first byte in `commSendmask` according to bit 7. `checkForMonsters` clears `noMonsters` when the word at object+12 has bit 6 clear and `(word & 0x28) == 0x20`. `playerSelectionSetup` calls `init__Fv`. Both registrars search a handler table, then take the first empty slot, and with a full table store and return the index one past the end (16 for messages, 4 for packets); that word is the first word of the next object (`pktHandler`, `scriptParticle0`). `playerSelectionStart` sets a byte, fades, sets the camera mode, pushes one scene light, loads `interface.lmp`, loads one of three language-selected selection sound lumps (language 3, 4, else), adds the selection task (priority 9, flag 1) and sets `playerSelRunning`. `playerInit` clears one 23984-byte player record and fills the offsets listed in the source comment.

**Inferred.** The Point3 argument of `lightPushSceneLights` is passed by address of a stack copy; the three-float value is (0, 0, -1). The mapping of the second `playerInit` argument to the word at +80 (values 0..9 to 0..4) is read from the compare chain. The float 0.075 stored at +184 and +188 has bits `0x3d999999`, one unit below `0.075f`; the host check found this, so the source stores the bits.

**Unknown.** What any record field means. What the nine integers to `lightPushSceneLights` mean. What `init__Fv` does. Whether the full-table registrar overflow is reachable in play. Which of the five values stored at +196 by class is a distance, speed or other quantity.

## Host checks

Each unit has `host_check.cpp` (not a plan input) built with the host compiler. The register and comm units use stub callees that record arguments and order. `playerInit` is compared byte-for-byte with an independently written table of stores for second-argument values -3..12, all four record indexes, both tint paths, with sentinel bytes around the record and checks that only the right row of `fadeTimer` and `fadeIconY` is cleared. Mutating one stored float made that check fail.

## Data defined

`net_state.cpp`: `netNmPartners` (1 byte, initial 1), `nmPartners` (volatile word, 0), `broadcastConnection` (word, -1), `commHandle`, `commStatusHandle`, `netWaitMsgHandler` (words, 0), `partners` (70528 bytes = 4 records of 17632), `g_net_buf` (2048 bytes). `player_state.cpp`: `playersAlive` (16), `startEntranceName` (32), `syncRandState`, `activePlayer`, `nmPlayersAlive`, `noMonsters` (byte), `nmSingle`, `nmLines`, `invisible` (80). `net_handlers.cpp`: `commSendmask`, `msgHandler` (16 words), `pktHandler` (4 words). `player_selection_state.cpp`: `playerSelRunning`, `playerSelectSoundLump` (32). Sizes for symbols with ELF size zero are gaps to the next symbol and are marked in the sources. All are zero-initialized in the file except the three non-zero initials above, and are defined here as normal globals; section placement and the 8-byte gap after `noMonsters` are not reproduced. The image symbol `firstTime` exists twice (`0x00483e40` and `0x00486d88`); the selection start uses the second.

## Imports closed (26)

`activePlayer`, `broadcastConnection`, `checkForMonsters__FP10GameObject`, `commHandle`, `commHandler__FPUci`, `commStatusHandle`, `commStatusHandler__FPUci`, `g_net_buf`, `invisible`, `netNmPartners`, `netRegisterMessageHandler__FPFPUci_v`, `netRegisterPacketHandler__FPFPUci_v`, `netWaitCallBack__FPUci`, `netWaitMsgHandler`, `nmLines`, `nmPartners`, `nmPlayersAlive`, `nmSingle`, `noMonsters`, `partners`, `playerInit__FiiPcScScScScT2`, `playerSelectionSetup__Fv`, `playerSelectionStart__Fv`, `playersAlive`, `startEntranceName`, `syncRandState`.

Not closed from the 32 names: `sceInetClose`, `sceInetCtlDownInterface`, `sceLibnetTerminate` (Sony SDK library code, not game code, left as imports), and the three aliases `norrath_got_player_start_this_level_bytes`, `norrath_keep_player_positions_bytes`, `norrath_restore_broadcast_mask_word`. Per `world-state.md` the last is the same word as `objectIgnoreFlags` (already defined as `norrath_grid_ignore_flags_view`) and per `items-player.md` the first is the same byte as `norrath_player_view_484ca8`; closing them needs a link alias or a rename by the integrator, which this package does not do (no second symbol was emitted). `norrath_keep_player_positions_bytes` has no owner identified here.

## Imports opened (15)

`SetMode__6Camerai`, `abilityTable`, `fadeIconY`, `fadeTimer`, `firstTime`, `hudFadeScreen__Fi`, `hudResetFadeScreen__Fi`, `init__Fv`, `language`, `lightPushSceneLights__FG6Point3iiiiiiiii`, `playerRecomputeAbilityModifiers__Fv`, `playerRecomputeWeight__Fi`, `playerSelectionTask__Fv`, `recievePacket__FiPUc`, `setupDefaultEquipment__Fi`. Net change 950 to 939. `abilityTable` is 80 bytes at `0x00422aa8` (five rows of four words); `fadeTimer` and `fadeIconY` are 112 bytes each at `0x0034b7b8` and `0x0034b828`.

## Build-plan entries needed

Profile `authored_cpp`; the integrator assigns `input_order` and must add the sources to the snapshot. Do not add `host_check.cpp`. All have empty `header_closure`.

```json
{"source":"sources/session-roster-net-wait-callback/net_wait_callback.cpp","object_name":"net_wait_callback.o","sha256":"a692062942ec14c783f652bcbf49b959013aa69c48c5455cba53e9b92c7b03aa","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netWaitCallBack__FPUci","compiled_body_bytes":8}]}
{"source":"sources/session-roster-comm-handler/comm_handler.cpp","object_name":"comm_handler.o","sha256":"0c629dbd872feef99abb8b227bbeb58570bfbc0671a6b5ea514601f2bc710d91","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"commHandler__FPUci","compiled_body_bytes":12}]}
{"source":"sources/session-roster-comm-status-handler/comm_status_handler.cpp","object_name":"comm_status_handler.o","sha256":"914fe6dca1c4fd6cf20a368e49a2ecdccb4f5ae227fdecee0146a236b621ef11","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"commStatusHandler__FPUci","compiled_body_bytes":60}]}
{"source":"sources/session-roster-check-for-monsters/check_for_monsters.cpp","object_name":"check_for_monsters.o","sha256":"5755df41cd93e319e7583b32859d0446d884a5183f4cefe4886ec173c79b11ef","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"checkForMonsters__FP10GameObject","compiled_body_bytes":44}]}
{"source":"sources/session-roster-player-selection-setup/player_selection_setup.cpp","object_name":"player_selection_setup.o","sha256":"cfaf7ac5c135e2e15e33c15c937b817edae8ec785a405f7ce5adcd0a24c768c9","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"playerSelectionSetup__Fv","compiled_body_bytes":8}]}
{"source":"sources/session-roster-net-register-message-handler/register_message_handler.cpp","object_name":"register_message_handler.o","sha256":"6c4f9e67dc3164e01626feae3d0cfeef3cd21ffef9a9ebee21d03e9d1622944b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netRegisterMessageHandler__FPFPUci_v","compiled_body_bytes":100}]}
{"source":"sources/session-roster-net-register-packet-handler/register_packet_handler.cpp","object_name":"register_packet_handler.o","sha256":"fb1f949ce32ec87e570139e47a80b9c795ab01feedee5e2a509262ccdc8ac8bb","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netRegisterPacketHandler__FPFPUci_v","compiled_body_bytes":100}]}
{"source":"sources/session-roster-player-selection-start/player_selection_start.cpp","object_name":"player_selection_start.o","sha256":"cf158485ff757138370ff58bc6fa6ebd20e34ef01645490baf0ab14a96fc7d68","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"playerSelectionStart__Fv","compiled_body_bytes":320}]}
{"source":"sources/session-roster-player-init/player_init.cpp","object_name":"player_init.o","sha256":"120f707d1866d39071817d5ca5d6f3df69e264ba53d84630e126fca8dff90cc9","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"playerInit__FiiPcScScScScT2","compiled_body_bytes":1252}]}
{"source":"sources/session-roster-net-state/net_state.cpp","object_name":"net_state.o","sha256":"e471be296cf21497f34d72ebde594f2993a0766e62a7bb1800207e93994d4e03","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
{"source":"sources/session-roster-player-state/player_state.cpp","object_name":"player_state.o","sha256":"c82768d75f3e8e9535876595676116219dc7883ddee86fccd31d3eab576af0c5","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
{"source":"sources/session-roster-net-handlers/net_handlers.cpp","object_name":"net_handlers.o","sha256":"0877378ba033dcc0b9fb28154d0fd69c41550b911cc51b58637875f06b91ce04","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
{"source":"sources/session-roster-player-selection-state/player_selection_state.cpp","object_name":"player_selection_state.o","sha256":"1e4949a8e6dc2844f6c31fb21f994ce289ae9fcfd79f0086a8d86f5d90707fb0","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
```

## Authored but never executed

All eleven source units that contain functions or data above were compiled with the reviewed EE toolchain and linked into the partial relocatable object only. Host checks ran on this machine against stubs. No EE, original-process or gameplay run. `playerSelectionStart` and `playerInit` were never run against the real callees.

## Unknown

See the field and meaning list above. Also not established: section placement of the data, the real storage owner of the net buffers, and whether `partners` and `g_net_buf` are laid out as plain byte arrays beyond the one flag byte read by `netGetLocalPlayerNm`.
