# Network play part 2 (w10-net-b)

Branch `agents/w10-net-b`. Addresses are from the pinned `SLUS_205.65` image. Nothing here is in `experimental/build-plan.json` or `experimental/snapshot.json`. Nothing was run on an EE or in the game.

## Functions (one commit each)

| Symbol | Address | Unit |
|---|---|---|
| `netSendPacket__FiiPUci` | 0x00129fe8 | `w10-net-b-net-send-packet` |
| `peerSendDataMsg__FPciiP14sceInetAddressi` | 0x0012d660 | `w10-net-b-peer-send-data-msg` |
| `sendAlertMessage__FifG6Point3P10GameObject` | 0x001134e0 | `w10-net-b-send-alert-message` |
| `receivePackets__Fv` | 0x00133768 | `w10-net-b-receive-packets` |
| `showNetPlayerStatus__Fv` | 0x001343b0 | `w10-net-b-show-net-player-status` |
| `sendControlPacket__FiiiiiR9CCompressb` | `0x0012f840` (5532 bytes) | `w10-net-b-send-control-packet` |
| `runPrediction__Fi` | `0x0012a248` (3920 bytes) | `w10-net-b-run-prediction` |

Data units (storage only, one commit each): frame counters (`trueNetFrameNm`, `sentStatePacketFrameNm`, `packetSendCycleLength`, `packetSendCycleTime`), network flags (`netTrouble`, `netKickedOut`, `totallyDisconnected`), launch state (`netLaunchTimer`, `netLaunchExpirationCount`, `netNewLaunchMaybeLaunch`, `netNewLaunchMaybeQuit`, `udpLaunchConnection`), comm counters (`readcount`, `writecount`, `timeout`), `netLocalPadState`, `net_buf`/`sLibNetInitialized`, `scanMask`/`sessionMark`. Initial values and sizes follow the image symbol table entries.

Each source unit has a `host_check.cpp` (host compiler, fake callees; not a plan input). The checks pass and fail when a constant is mutated (`sendControlPacket` 0xbd40 -> 0xbd41 fails; `runPrediction` counter increment 1 -> 2 fails).

Behaviour of `sendControlPacket` and `runPrediction` is described in the header comment of each source file (observed / inferred / unknown are separated there).

## Paired runs against the image (private, not in git)

Tool kept outside git: `/Users/case/norrath-agents/priv-w10-net-b` (R5900 interpreter, authored object linked beside the image, identical snapshots and stub callees; call logs and every changed byte outside the stack compared).

| Function | Mismatches | Original words reached |
|---|---|---|
| `sendControlPacket` | 0 | 1251 of 1383 |
| `runPrediction` | 0 | 813 of 980 |
| `receivePackets` | 0 | 198 of 201 |

Not reached in `sendControlPacket`: the copy variant that depends on the run-time alignment of the compression object (the authored copy has no such branch) and a few run-length flush edges of the three bit coders. Unreached words in `runPrediction` were not itemised.

## Imports closed (27)

`netKickedOut`, `netLaunchExpirationCount`, `netLaunchTimer`, `netLocalPadState`, `netNewLaunchMaybeLaunch`, `netNewLaunchMaybeQuit`, `netSendPacket__FiiPUci`, `netTrouble`, `net_buf`, `packetSendCycleLength`, `packetSendCycleTime`, `peerSendDataMsg__FPciiP14sceInetAddressi`, `readcount`, `receivePackets__Fv`, `runPrediction__Fi`, `sLibNetInitialized`, `scanMask`, `sendAlertMessage__FifG6Point3P10GameObject`, `sendControlPacket__FiiiiiR9CCompressb`, `sentStatePacketFrameNm`, `sessionMark`, `showNetPlayerStatus__Fv`, `timeout`, `totallyDisconnected`, `trueNetFrameNm`, `udpLaunchConnection`, `writecount`.

Not closed by this package: `norrath_construct_Vehicle` (authored in `vehicle-actors-vehicle-construct`) and `norrath_web_construct` (authored in `w9-camera-map-web-construct`). Both belong to other packages and need only to be present in the plan.

## Imports opened

Referenced and not defined by these units: `MC_Debug`, `Update__6Camera`, `analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci`, `analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii`, `broadcastConnection`, `broadcastConnectionFlags`, `cd`, `compImmediateCompressor`, `compImmediateDecompressor`, `engineGetAIStepCount__Fv`, `fontColor__FUiii`, `fontEnd__Fv`, `fontPrintCentered__FiiPci`, `fontPrintCenteredu__FiiPUsi`, `fontPrintWrapCenteredu__FiiPUsiiii`, `fontStart__FP4FontiUii`, `frameNm`, `g_net_buf`, `gameCam`, `hostStartedWithoutMe`, `hudFooterUni`, `hudShowMsg__FPUsN20iii`, `launchPeers`, `lnet_buf`, `localNetFrameNm`, `localPlayerPadState`, `localPredictedPlayer`, `lumpFindResource__FPcT0`, `memcpy`, `memset`, `netCameraYaw`, `netGameLaunchInfo`, `netGetLocalPlayerNm__Fi`, `netLogDisable`, `netNetworkMode`, `netPrediction`, `netPredictionFastForward`, `nmPartners`, `objectFindInBox__FG6Point3T0PP10GameObjecti`, `objectIgnoreFlags`, `pad`, `padAcceptButtonFontChar`, `padBackButtonFontChar`, `padProcessData__FP3padP18padUnprocessedDataP12padStateData`, `partners`, `pktHandler`, `player`, `receiveControlPacket__FPiib`, `sceInetRecvFrom`, `sceInetSendTo`, `smallFont`, `sprintf`, `strcat`, `strlen`, `syncRandState`, `textGetString__FPc`, `textureDrawBorder__FiiiiiUiPcii`, `textureDrawColor__FP8_textureiiiUi`, `usprintf__FPUsT0ffffffff`, `ustrcpy__FPUsT0`. Many are defined by other packages (w8-net-play-a/b/c, fonts, text); the list is not intersected with the integrator's current unresolved list.

## Build-plan entries needed

Profile `authored_cpp`; the integrator assigns `input_order`. Do not add `host_check.cpp`. Hashes are of the committed sources at the last commit; recompute if a file changes.

```json
{"source":"sources/w10-net-b-comm-counters/comm_counters.cpp","object_name":"comm_counters.o","sha256":"e1ab6863403a09e1670db48cf3b2df834e4c40ab8e6f5e503f3f311d57039ec7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
{"source":"sources/w10-net-b-frame-counters/frame_counters.cpp","object_name":"frame_counters.o","sha256":"488be05e055b5013d650d435b15b4a1e3e4ad555112c0528baaf9abafe6c29fa","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
{"source":"sources/w10-net-b-launch-state/launch_state.cpp","object_name":"launch_state.o","sha256":"ebc2de80b8cecf5b24837693405db84d2f44f22876540b74544f6c8bce22b820","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
{"source":"sources/w10-net-b-lib-net/lib_net.cpp","object_name":"lib_net.o","sha256":"378915c329d9b69712f9811e6f8a52dab82537e826f43e10fb704704feb4c26a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
{"source":"sources/w10-net-b-net-flags/net_flags.cpp","object_name":"net_flags.o","sha256":"fa8eca728fbe4ec7fd5dfac79ac4342f4bd2e04a2ac578584ce2e90085ed9c22","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
{"source":"sources/w10-net-b-net-send-packet/net_send_packet.cpp","object_name":"net_send_packet.o","sha256":"9bb42dd0b11979d54c7b4f2a26c4c01ed17606653d3d1154fa07d1d13957c068","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"netSendPacket__FiiPUci","compiled_body_bytes":420}]}
{"source":"sources/w10-net-b-pad-state/pad_state.cpp","object_name":"pad_state.o","sha256":"4db34660753d044ec53cdc8b9d13c3ab69f97188695e0b08041b44370c16d8bc","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
{"source":"sources/w10-net-b-peer-send-data-msg/peer_send_data_msg.cpp","object_name":"peer_send_data_msg.o","sha256":"3d05e5d81c9a308262b61eeecb51500f1c05ebbdb23562ed14677fb4b324e71b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"peerSendDataMsg__FPciiP14sceInetAddressi","compiled_body_bytes":304}]}
{"source":"sources/w10-net-b-receive-packets/receive_packets.cpp","object_name":"receive_packets.o","sha256":"06981c97bf1828a61a66d9bec56fc4d42a85ca37bc186682b8252d7bb1291109","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"receivePackets__Fv","compiled_body_bytes":776}]}
{"source":"sources/w10-net-b-run-prediction/run_prediction.cpp","object_name":"run_prediction.o","sha256":"32d6a21e2481533ae12888640a41cb008e13f64e7733954b1cbce31a3b9dfe0e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"runPrediction__Fi","compiled_body_bytes":3068}]}
{"source":"sources/w10-net-b-scan-session/scan_session.cpp","object_name":"scan_session.o","sha256":"b5cd3a1f358738bfbdc1def1da9829ae33ab9add43bb96a379da25cf68a450d9","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source"}
{"source":"sources/w10-net-b-send-alert-message/send_alert_message.cpp","object_name":"send_alert_message.o","sha256":"8faeb5b7035b8d0f353e0bd82479e26a128ce4e938343ab1ed2232f70ec3bd4e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"sendAlertMessage__FifG6Point3P10GameObject","compiled_body_bytes":228}]}
{"source":"sources/w10-net-b-send-control-packet/send_control_packet.cpp","object_name":"send_control_packet.o","sha256":"4c130668ca269a63cdde4f9b9d05692a03343260c0d1d09fa4541283ba4f0c83","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"sendControlPacket__FiiiiiR9CCompressb","compiled_body_bytes":1344}]}
{"source":"sources/w10-net-b-show-net-player-status/show_net_player_status.cpp","object_name":"show_net_player_status.o","sha256":"e4c717a6be98a1dbf471d21c6b2903eb8777c17f9424b55226b20197639746e3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"showNetPlayerStatus__Fv","compiled_body_bytes":2008}]}
```

## Authored but never executed

Everything. No unit ran on an EE or in the game; the paired runs used an interpreter with stub callees and the host checks use fake callees. The partial build was not rerun with these units added (single units were compiled with the reviewed toolchain; their symbol sizes are in the plan entries). Compiled sizes differ from the image because the compiler repacks code; no byte match is claimed.

## Unknown

Why `sendControlPacket` discards the first compression pass's output and keeps only its state effect; its behaviour when `second < first - 1`; the content of the stack buffer past the written bytes; the meaning of most `partners` record fields; what `analCompress` and `analDecompress` do internally (other packages); the gameplay purpose of `runPrediction`'s rewind steps beyond what its header comment records as inferred.
