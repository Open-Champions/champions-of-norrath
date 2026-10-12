# Network play and player state (2/3)

Package `agents/w8-net-play-b`. Addresses are from the pinned `SLUS_205.65` image. Everything here was read from that image's code and data and compiled with the reviewed EE toolchain (`authored_cpp` flags, one unit at a time). No file here is in `experimental/build-plan.json` or `experimental/snapshot.json`; the entries to add are listed at the end. Nothing was executed on an EE or in the game.

## Functions

| Symbol | Address | Image bytes | Compiled bytes |
|---|---|---|---|
| `norrath_player_proxy_mesh_id` (image name `getProxyMeshId__4itemPQ24item11description`) | `0x00108498` | 68 | 48 |
| `netKickPlayer__Fi` | `0x0012a190` | 184 | 196 |
| `netWaitForPeersToCatchUp__Fv` | `0x0012b198` | 180 | 228 |
| `netGameStep__FbT0` | `0x0012b250` | 5796 | 6584 |
| `netStartNetworkGameFrame__Fv` | `0x0012c8f8` | 1636 | 1424 |
| `netStartNetworkGameInit__Fv` | `0x0012cf60` | 1788 | 1732 |
| `netDoNewLaunch__Fv` | `0x0012d790` | 8064 | 7812 |
| `netRestoreButtonConfigs__Fv` | `0x0012f710` | 300 | 236 |
| `netInit__Fv` | `0x00131170` | 1628 | 1440 |
| `netStartLevel__Fv` | `0x00135108` | 8 | 12 |
| `netStart__Fv` | `0x00135110` | 60 | 92 |
| `netPlayerIsConnected__Fi` | `0x00135150` | 60 | 84 |
| `netDisconnectPlayer__Fi` | `0x00135190` | 44 | 56 |
| `netClearContact__Fv` | `0x00135238` | 60 | 64 |
| `netInContact__Fv` | `0x00135278` | 108 | 100 |
| `playerClearSpellEffect__Fv` | `0x001ab878` | 104 | 100 |

Compiled sizes differ because the compiler repacks, tail-calls and adds a divide-by-zero check; no byte match is claimed. Each function is its own commit with the unit's own `host_check.cpp` (not a plan input).

## What each function does

**Observed.**
- `netStartLevel` stores zero in `localPredictedPlayer`. `netStart` copies `trueNetFrameNm` into `sentStatePacketFrameNm` and `localNetFrameNm`, clears `syncRandState`, calls `worldPlotRouteSetEnable(0)`, clears `noAdvanceTime` and stores `localNetFrameNm - 10` in `derivedFrameNm` (read after the call).
- The partner table is `partners` (stride 17632). `netPlayerIsConnected` returns 1 when `netNetworkMode` is zero, 0 for an index not below `nmPartners`, else whether the byte at +17621 is zero. `netDisconnectPlayer` sets that byte. `netClearContact` zeroes the word at +17616 of each record; `netInContact` accepts a record when one of the bytes +17620..17622 is nonzero or the word at +17616 is nonzero, and returns 0 at the first record that fails.
- `netKickPlayer` marks a remote record (byte 17622 = 1, word 17624 = -1, word 17628 = 1), clears the 8-byte `kickState` with `memset` and sets its first word to 1, only for a nonlocal player whose three flag bytes are clear. `netWaitForPeersToCatchUp` sets `engineDisplayDisable` to 3 and calls `netGameStep(true, true)` plus `engineSyncVBlank` up to 121 times while an unflagged record's word at +17600 is below `trueNetFrameNm`.
- `netRestoreButtonConfigs` copies the nine button words of each 36-byte config record at `MC_GameConfig + 52` back into the nine four-word button tables.
- `playerClearSpellEffect` zeroes four per-player word arrays (`lightIntensity`, `spellAlpha`, `lightInc`, `spellAlphaInc`).
- `netInit` runs once (guard byte `netInitState`): it sets the frame counters to 1, clears pad and camera buffers, builds four 76-byte compression objects (four 16-byte slots with the float 128.0 in the third word, then a zero word), clears `kickState` and two more objects, calls `sceSifMInitRpc(0)`, `sceLibnetInitialize(cd, 2048, 8192, 32)`, asks `sceInetGetInterfaceList` for one interface, calls a six-argument `sceInetInterfaceControl` unless the list failed, and sets `isModem` when `sceSifSearchModuleByName` finds the modem driver module.
- `netStartNetworkGameFrame` is the lobby discovery frame: it broadcasts a 4-byte announce (0xbd3d) every 64th frame, eight start broadcasts (0xbd3e) once two partners are known or a pad bit is held, receives one datagram, adds new senders to the table (sorted by first address octet, whole 17632-byte records swapped), and on a start message initialises the players, copies the level name and returns 1; otherwise it prints the known addresses.
- `netStartNetworkGameInit` clears `nmPartners` and, in broadcast mode, creates the socket and seeds partner 0 from `myaddr`; in launch mode it moves temporary player storage, creates the launch socket, fills `launchPeers` from `netGameLaunchInfo` and the player record, and stores sizes by launch-flag case. The image stores the `sceInetCreate` result in `broadcastConnection` and `udpLaunchConnection` and ignores the `sceInetOpen` result (the store sits in the branch delay slot of the open call); a first reading of this was wrong and was caught by the paired run below.
- `netGameStep` is the lock-step network frame: pings, timeout marking, kick-state broadcasts, local pad input into a 256-entry frame ring, the per-partner packet encoder (bit writer, run-length codes for the button halfword and the yaw bit, four `analCompress` calls, a yaw delta coder, marker 0xa5a5), `sendControlPacket(..., 0xbd3f, ...)`, leaver messages, how many real steps `advanceRealState` takes, `netTrouble`, `runPrediction`, and the pad record restore. The ring entry's word at +16 is not written by this port (the image stores uninitialised stack there; it is overwritten before use).
- `netDoNewLaunch` is one step of the launch lobby: confirmation prompts, one datagram receive, a nine-way type switch (join, size, data chunks with a 16-bit checksum that adds the first payload byte on every step, acks, ready query, launch mask), a per-entry nine-state resend machine with 0.5 s and 1.0 s gates, the launch decision and timer, and the hand-off that rebuilds `partners`, the button tables and the broadcast socket.
- `norrath_player_proxy_mesh_id` returns the signed halfword at +52 of the record pointed to by the description's first word, plus bits 11..15 of the 64-bit word at +8 when bit 36 of that word is set.

**Inferred.** The purposes above (lobby, lock step, kick handling), the names of fields only where the image gives a symbol, and that `launchPeers[i]` word +64 is the peer's index.

**Unknown.** What most record fields mean; what `sceInetInterfaceControl`'s six words are; what the analog byte coder `analCompress` does; the receive side of the 0xbd3f stream; what bit 0x40 of the `pad` word at +12 is in the broadcast frame; the uninitialised word read when no announce was sent in the same frame (this port reads zero); how much of each 512-byte datagram is meaningful.

## Verification

Each unit has a `host_check.cpp` (host compiler, fake callees). The small units compare against an independently written table or log; the large ones check call order, packet words, the exact bit string of a short encoded packet (`netGameStep`) and state transitions (`netDoNewLaunch`). Each check was run against a mutated source and fails.

Beyond that, every function (except the four storage units) was compared against the original code with a private tool kept outside git (`/Users/case/norrath-agents/priv-w8-net-play-b`; it loads the private executable, so it is not published). It is a small R5900 interpreter. The authored object is linked next to the image with every external symbol bound to the original's address; the original function and the authored one run from identical memory snapshots with identical stub callees; the call logs (arguments, data read from stack buffers, bit writer contents) and every changed byte outside the stack must match. Scenarios are seeded random states steered toward each branch; instruction coverage of the original function is recorded. Results at the last commit:

| Function | Scenarios | Mismatches | Original instructions reached |
|---|---|---|---|
| `netStartLevel` | 3 | 0 | 2 of 2 |
| `netStart` | 4 | 0 | 15 of 15 |
| `netPlayerIsConnected` | 864 | 0 | 15 of 15 |
| `netDisconnectPlayer` | 864 | 0 | 11 of 11 |
| `netClearContact` | 144 | 0 | 15 of 15 |
| `netInContact` | 144 | 0 | 27 of 27 |
| `playerClearSpellEffect` | 6 | 0 | 26 of 26 |
| `netRestoreButtonConfigs` | 1 | 0 | 75 of 75 |
| `netKickPlayer` | 576 | 0 | 46 of 46 |
| `netWaitForPeersToCatchUp` | 240 | 0 | 44 of 45 (the other is padding) |
| `netInit` | 162 | 0 | 310 of 407 (the rest are alignment fallbacks) |
| `netStartNetworkGameFrame` | 1500 | 0 | 336 of 409 (the rest are alignment fallbacks) |
| `netStartNetworkGameInit` | 600 (0 mismatches after the delay-slot fix) | 0 | 445 of 447 (padding) |
| `norrath_player_proxy_mesh_id` | 400 | 0 | 17 of 17 |
| `netGameStep` | 2300 in three families (normal, forced send, kick state 2) | 0 | 1367 + 1234 reached across families; every reachable instruction hit except the alignment fallback of the pad-record copy and padding |
| `netDoNewLaunch` | 3500 + 1500 | 0 | 1810 of 2016 in the first run (the rest are alignment fallbacks of struct copies, branch delay slots that cannot run, and padding), plus the second run reached the second join packet |

"Alignment fallback" means the image copies large structures with a runtime test of the source and destination alignment and an unaligned path; all objects here are aligned, so that path never runs and was not exercised. Because the compiler used for the authored code copies structures inline too, the same difference cannot be observed from the outside.

The interpreter, the fake callees and the fixtures are not equivalent to hardware: timing, exceptions, the real callees, physical frame behaviour and the EE's floating point rounding beyond what the instructions used here need are not covered. In the paired runs, bytes the image leaves uninitialised on the stack (the 512-byte tail of each datagram, and the ring entry word at +16 in `netGameStep`) are excluded or not compared.

## Data defined

`launch_state.cpp`: `netBroadCastStart` (1 byte, initial 1), `netAttemptLaunch` (1), `netTrouble` (4), `multiHelpMessageOn` (4), `nmPlayersAlive` (4), `localNmPlayersAlive` (4), `nmBatchCommands` (4; a local symbol in the image), `multiRelease` (4), `multiTitle`, `multiBody`, `multiSide` (16 each; sizes are gaps to the next symbol). All are zero in the image except the first. `player_storage.cpp`: `player`, 95936 bytes = four 23984-byte records, all zero in the file. `heap_pointer.cpp`: `norrath_player_heap_pointer_word`, the word at `0x00444954` (`heap_ptr.39`), initialised from `_end` (file value `0x008ec91c`, the address of both `end` and `_end`). `services_view.cpp`: `norrath_player_services_view_48068c`, the four bytes `cc cc cc 3e` at `0x0048068c` (float bits `0x3ecccccc`, one unit below 0.4). Static locals of `netGameStep` (`count`, `minStepsToTake`, `minStepsToTakeTimer` at `0x00483874`, `0x00483878`, `0x0048387c`, initial 0, 100, 0) are file-local statics of the unit, and its seven-word table (6, 2, 4, 6, 6, 6, 6) is a const array in the unit.

`nmPlayersAlive` is also defined by `session-roster-player-state/player_state.cpp`, which the integrator left out of the plan because three of its other names clash. Select only one of the two definitions; if that unit is later selected, remove `nmPlayersAlive` from one of them.

## Strings in the sources

Short identifiers only: the level name `vtest2` and the four default character names used by the lobby start path, the text keys `netMessagePlayerExit`, `netLaunchCancelConfirm`, `netLaunchHostWarn`, the allocation tag `Net Launch buff`, and the module name `INET_SPEED_UART_driver`.

## Imports closed (30)

`localNmPlayersAlive`, `multiBody`, `multiHelpMessageOn`, `multiRelease`, `multiSide`, `multiTitle`, `netAttemptLaunch`, `netBroadCastStart`, `netClearContact__Fv`, `netDisconnectPlayer__Fi`, `netDoNewLaunch__Fv`, `netGameStep__FbT0`, `netInContact__Fv`, `netInit__Fv`, `netKickPlayer__Fi`, `netPlayerIsConnected__Fi`, `netRestoreButtonConfigs__Fv`, `netStartLevel__Fv`, `netStartNetworkGameFrame__Fv`, `netStartNetworkGameInit__Fv`, `netStart__Fv`, `netTrouble`, `netWaitForPeersToCatchUp__Fv`, `nmBatchCommands`, `nmPlayersAlive`, `norrath_player_heap_pointer_word`, `norrath_player_proxy_mesh_id`, `norrath_player_services_view_48068c`, `player`, `playerClearSpellEffect__Fv`

## Imports opened (69)

Referenced by these units, not defined by them, and not unresolved in the planner's build (`/Users/case/norrath-agents/repo/work/plan8-build`): `MC_BuildPackedPlayer__FP11playerEntryi`, `MC_ByteCopy__FPvT0i`, `MC_ConvertTempPlayerToReal__Fi`, `MC_GetRamPlayerPtr__Fi`, `MC_GetRamSlot__FPi`, `MC_GetTempPlayerStorage__Fi`, `MC_NetPack__Fv`, `MC_NetUnPack__Fv`, `MC_ReplicateFirstPlayersImportantThings__Fv`, `MC_SetRamNmPlayers__Fi`, `advanceRealState__Fi`, `analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci`, `broadcastConnectionFlags`, `compImmediateCompressor`, `compImmediateDecompressor`, `compLocalCompressor`, `compLocalDecompressor`, `compressSize`, `currentTime`, `derivedFrameNm`, `dumpMem__FPvi`, `hostForcedLaunch`, `hostLaunchMask`, `hostStartedWithoutMe`, `isModem`, `kickState`, `lastPingTime`, `launchPeers`, `lightInc`, `lightIntensity`, `lnet_buf`, `localNetFrameNm`, `localNetSentFrameNm`, `localPlayerPadState`, `localPredictedPlayer`, `logChecksum`, `msgBufferSent`, `myaddr`, `netCameraYaw`, `netKickedOut`, `netLaunchExpirationCount`, `netLaunchTimer`, `netLocalPadState`, `netNewLaunchMaybeLaunch`, `netNewLaunchMaybeQuit`, `noAdvanceTime`, `packetSendCycleLength`, `packetSendCycleTime`, `padBlank`, `peerSendDataMsg__FPciiP14sceInetAddressi`, `playerSelectInfo`, `playerSpecStorage`, `receivePackets__Fv`, `runPrediction__Fi`, `sceInetCreate`, `sceInetInterfaceControl`, `sceInetOpen`, `sceInetRecvFrom`, `sceInetSendTo`, `sceSifMInitRpc`, `sendControlPacket__FiiiiiR9CCompressb`, `sentStatePacketFrameNm`, `showNetPlayerStatus__Fv`, `spellAlpha`, `spellAlphaInc`, `totallyDisconnected`, `trueNetFrameNm`, `udpLaunchConnection`, `worldPlotRouteSetEnable__Fi`. None was in any package's scope list when the waves were planned (the planner list was derived from the unresolved names of that build, which did not contain these because nothing referenced them). Several are in startup/SCE or memory-card territory (`sceInet*`, `sceSifMInitRpc`, `MC_*`); the net-state words (`trueNetFrameNm`, `launchPeers`, `kickState`, ...) and `receivePackets`, `sendControlPacket`, `analCompress`, `advanceRealState`, `runPrediction`, `peerSendDataMsg`, `showNetPlayerStatus` have no owner yet.

Already unresolved and referenced here: `GetYaw__6Camera`, `_end`, `cd`, `engineGetAIStepCount__Fv`, `fontEnd__Fv`, `fontPrintCentered__FiiPci`, `fontStart__FP4FontiUii`, `hudRunYesNo__FPUsiiii`, `memAlloc__FiPc`, `memFree__FPv`, `netLocalPad`, `padGetUnprocessedData__FiP18padUnprocessedData`, `padProcessData__FP3padP18padUnprocessedDataP12padStateData`, `sceInetClose`, `sceInetGetInterfaceList`, `sceLibnetInitialize`, `strcpy`, `syncRandState`, `textGetString__FPc`, `ustrcat__FPUsT0`, `ustrcpy__FPUsPc`. `cd` is ambiguous in the image (four symbols); these sources use the one at `0x004f3e90`.

## Build-plan entries needed

Profile `authored_cpp`; the integrator assigns `input_order` and adds the sources to the snapshot. Do not add `host_check.cpp`. All have empty `header_closure`.

```json
{"source":"sources/w8-net-play-b-clear-contact/net_clear_contact.cpp","object_name":"net_clear_contact.o","sha256":"02da5f31225537a7e006e19992119e018b9f7a65ceba222fe6cca941f28a8c36","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netClearContact__Fv","compiled_body_bytes":64}]}
{"source":"sources/w8-net-play-b-clear-spell-effect/player_clear_spell_effect.cpp","object_name":"player_clear_spell_effect.o","sha256":"3f085866278707c74ba2b97c879e515b397cde379416d3244c9b4b63716eab29","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"playerClearSpellEffect__Fv","compiled_body_bytes":100}]}
{"source":"sources/w8-net-play-b-disconnect-player/net_disconnect_player.cpp","object_name":"net_disconnect_player.o","sha256":"90a8c92a8a48b83e92967e28ef9034fd5c6d55d46199029110a8e8fd289c8459","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netDisconnectPlayer__Fi","compiled_body_bytes":56}]}
{"source":"sources/w8-net-play-b-do-new-launch/net_do_new_launch.cpp","object_name":"net_do_new_launch.o","sha256":"abc2f381e95c350f3b548524e498fd1c8945552d9f629a2ebc0e084f772d9703","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netDoNewLaunch__Fv","compiled_body_bytes":7812}]}
{"source":"sources/w8-net-play-b-game-step/net_game_step.cpp","object_name":"net_game_step.o","sha256":"e202f0243faf08240e3acaae8b14d3a45d27d8bcdd1f3e5568476fe769780281","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netGameStep__FbT0","compiled_body_bytes":6584}]}
{"source":"sources/w8-net-play-b-heap-pointer/heap_pointer.cpp","object_name":"heap_pointer.o","sha256":"1376fe1814f9d4a477fe66470cdd48ba466e7198388c04a5c44305717a5fe799","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
{"source":"sources/w8-net-play-b-in-contact/net_in_contact.cpp","object_name":"net_in_contact.o","sha256":"98df78b25463a585b6107c1efe0a6af6b5f19c850aa99e18307f94ca1dbba13f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netInContact__Fv","compiled_body_bytes":100}]}
{"source":"sources/w8-net-play-b-init/net_init.cpp","object_name":"net_init.o","sha256":"7eba1259eff246720d4f895064ff29a21cb1b725c27e7dcfb6ff524a93ebc715","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netInit__Fv","compiled_body_bytes":1440}]}
{"source":"sources/w8-net-play-b-kick-player/net_kick_player.cpp","object_name":"net_kick_player.o","sha256":"41c0658d130955433826c31bff2aca9e5ae862382f1d0299c565027b337eedc7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netKickPlayer__Fi","compiled_body_bytes":196}]}
{"source":"sources/w8-net-play-b-launch-state/launch_state.cpp","object_name":"launch_state.o","sha256":"189276675640d4361f5b1a18fd0fe5431118370a5679abd27baa7a987ffab737","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
{"source":"sources/w8-net-play-b-player-is-connected/net_player_is_connected.cpp","object_name":"net_player_is_connected.o","sha256":"8ca62d0b6334395ce9f1e033c8a16aa980a05a10b66d16f50df256fb7c75d243","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netPlayerIsConnected__Fi","compiled_body_bytes":84}]}
{"source":"sources/w8-net-play-b-player-storage/player_storage.cpp","object_name":"player_storage.o","sha256":"7d08470944cff4908b1ab0a27452b1b0347cc729611f2573f399e9decbd15772","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
{"source":"sources/w8-net-play-b-proxy-mesh-id/proxy_mesh_id.cpp","object_name":"proxy_mesh_id.o","sha256":"3c5f21a73b884a20cb5436bcce51b6ecdf2c3a442564fb71a674048d9b464bee","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"norrath_player_proxy_mesh_id","compiled_body_bytes":48}]}
{"source":"sources/w8-net-play-b-restore-button-configs/net_restore_button_configs.cpp","object_name":"net_restore_button_configs.o","sha256":"edc8c2b54996fb9da4c72f8566ab0121d02dce1d44602755686aa17bdf926823","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netRestoreButtonConfigs__Fv","compiled_body_bytes":236}]}
{"source":"sources/w8-net-play-b-services-view/services_view.cpp","object_name":"services_view.o","sha256":"fed2a57ec94e9f83c097b913450b09387c62eb8fec57fe7d84a55bd38e744666","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
{"source":"sources/w8-net-play-b-start/net_start.cpp","object_name":"net_start.o","sha256":"3af8bb7a06896014324b5d272bd413f18c7ca1e9be938627d5fac0c4264161da","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netStart__Fv","compiled_body_bytes":92}]}
{"source":"sources/w8-net-play-b-start-level/net_start_level.cpp","object_name":"net_start_level.o","sha256":"5831e10c793399648830b1ca88985f783decb89fbac673cad4b5716b2a884347","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netStartLevel__Fv","compiled_body_bytes":12}]}
{"source":"sources/w8-net-play-b-start-network-game-frame/net_start_network_game_frame.cpp","object_name":"net_start_network_game_frame.o","sha256":"3105b37675dfe124bbc36abb3314bf76559f76f27d17b5d4acb00c56bec898bd","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netStartNetworkGameFrame__Fv","compiled_body_bytes":1424}]}
{"source":"sources/w8-net-play-b-start-network-game-init/net_start_network_game_init.cpp","object_name":"net_start_network_game_init.o","sha256":"7d8dc279361afe83fb289df36e3b6422727568a78cfe4dd333b6e0a7eed1e6e6","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netStartNetworkGameInit__Fv","compiled_body_bytes":1732}]}
{"source":"sources/w8-net-play-b-wait-for-peers/net_wait_for_peers.cpp","object_name":"net_wait_for_peers.o","sha256":"559d20152df30df5de353994033c5c0ea5fdb07021e870a600f4a19d3d1f569a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netWaitForPeersToCatchUp__Fv","compiled_body_bytes":228}]}
```

## Authored but never executed

Everything. No unit has been run on an EE or in the game; the paired runs above used an interpreter and fake callees, and the host checks use fake callees. The partial link has not been rebuilt with these units added. `netGameStep` and `netDoNewLaunch` in particular depend on callees (`analCompress`, `sendControlPacket`, `receivePackets`, `peerSendDataMsg`, the `MC_*` storage calls) that this package did not author.

## Unknown

See the per-function lists. Also: whether the `launchPeers` record layout (252 bytes) and the 136-byte block at +104 match other users; what `netLocalPad`, `pad`, `padBlank` records contain beyond the offsets read; and whether the single 512-byte send length is ever shortened elsewhere.
