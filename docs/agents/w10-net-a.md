# w10-net-a: network play part 1 of 2

Package branch: `agents/w10-net-a`. Every function source has a `host_check.cpp` (not an EE plan input) next to it and
compiles with the reviewed `authored_cpp` flags. Verification of each function was also a private paired run of the
authored object against the pinned `SLUS_205.65` image (return values where meaningful, call logs, and every changed
byte outside the stack, from identical memory snapshots), with logic mutants that the runs had to catch. The private
harness and its output are outside git. No byte match with the original is claimed.

## Functions (observed behavior; details are in each source header)

- `netIsModem__Fv` (0x135798): returns the signed byte `isModem` widened to int.
- `netGetNmConnectedPartners__Fv` (0x1352e8): counts the first `nmPartners` partner records whose bytes at 17621 and
  17622 are both zero.
- `netGetCameraCustomControl__Fi` (0x135378): without a network mode, `gameCam->word52 != 1`; otherwise bit 0 of the
  halfword `netCameraYaw[player]`.
- `commanderValid__14WoodElfSoldier` (0x249e60): non-null commander at +960 whose descriptor word (+12 of +12) has bit 8 clear.
- `netCheckActivation__FG6Point3f` (0x135658): true when any player object lies within the range (strict squared compare).
- `analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci` (0x133a90): three-pass 2-bit delta/run coder
  of analog bytes. The paired run covered all 307 original instructions.
- `advanceRealState__Fi` (0x1317d0, 5488 bytes): the per-step lock-step re-simulation; see the header for the six
  parts. Paired run: 1155 of 1372 original words executed; the unexecuted words are provably unreachable for the
  one-frame coder (multi-frame branches), the unaligned variants of struct copies and `k < 0` guards.
- Data (size from the image; initial values from the image): `netBroadCastStart` (1), `netAttemptLaunch`, `isModem`,
  `lastPingTime`, `compressSize`, `lnet_buf`, `launchPeers` (1008), `hostForcedLaunch`, `hostLaunchMask`,
  `hostStartedWithoutMe`, `myaddr` (16); `localNetFrameNm` (1), `localNetSentFrameNm` (1), `localPredictedPlayer`,
  `logChecksum`, `netCameraYaw` (8), `kickState` (8), `broadcastConnectionFlags`, `msgBufferSent`,
  `localPlayerPadState` (8); `commCVol`, `commNameMask`, `commRVInternalLast`.

## Notes

- `w8-net-play-b-launch-state/launch_state.cpp` also defines `netBroadCastStart` and `netAttemptLaunch`; it is not a
  plan input, so only my launch-state-data unit may be selected for those two.
- Zero-initialised items that the image keeps in its data sections fall into bss here, as in the other data units.
- `advanceRealState` reproduces two oddities as observed: a step is skipped (objectIgnoreFlags stays 0x20000) when
  `levelName[0]` is nonzero, and three per-player words are counted down in an empty loop every 64th frame.
  `cvt.w.s` is used for the float-to-int log sum (inline asm; the compiler would otherwise emit `trunc.w.s`).

## Imports closed

advanceRealState__Fi, analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci,
broadcastConnectionFlags, commCVol, commNameMask, commRVInternalLast, commanderValid__14WoodElfSoldier,
compressSize, hostForcedLaunch, hostLaunchMask, hostStartedWithoutMe, isModem, kickState, lastPingTime, launchPeers,
lnet_buf, localNetFrameNm, localNetSentFrameNm, localPlayerPadState, localPredictedPlayer, logChecksum,
msgBufferSent, myaddr, netAttemptLaunch, netBroadCastStart, netCameraYaw, netCheckActivation__FG6Point3f,
netGetCameraCustomControl__Fi, netGetNmConnectedPartners__Fv, netIsModem__Fv.

## Imports opened

Only by `advanceRealState`; several are probably closed by other packages already: 
analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii,
compareStatePackets__FP11statePacketT0i, padProcessData__FP3padP18padUnprocessedDataP12padStateData,
coreGameStep__Fii, worldPlotRouteProcess__Fi, playerDamageHandler__6Player, objectAddToGrid__FP10GameObject,
and the data msgHandler, levelName, netLocalPad, pad, objectIgnoreFlags, syncRandState, netTrueStep, netPrediction,
netLogDisable, netForceLocalUpdate, derivedFrameNm, trueNetFrameNm, frameNm, nmPartners, nmPlayers, netNetworkMode,
partners, player, compLocalCompressor, compLocalDecompressor, compImmediateCompressor, compImmediateDecompressor.
`analCompress` opens nothing.

## Build-plan entries needed (path under `experimental/`, sha-256; profile authored_cpp, no header closure)

- `sources/w10-net-a-advance-real-state/advance_real_state.cpp` bf2e973d4fddf543b0faa699ee122930f83b6b6f09952919b2b4f9de0c0db6f3
- `sources/w10-net-a-anal-compress/anal_compress.cpp` 0406144e8f20c2d9b303290b7ca75a3ee8a0ad137827f769380d63eb96d77e8b
- `sources/w10-net-a-comm-state-data/comm_state_data.cpp` 8991989e57ee696055e0ff10565ce3a75cd6a45a9a3eced7ed195a88eb616d55
- `sources/w10-net-a-commander-valid/commander_valid.cpp` 85ccf6230c61c883412b7f471bab36264f238c106243a77ab763a2543cebdcc3
- `sources/w10-net-a-frame-state-data/frame_state_data.cpp` 6573a76ad93935d330bf6f187d6d0c5b0376bfb13f2f23a5bef0938efbf66fb9
- `sources/w10-net-a-launch-state-data/launch_state_data.cpp` 51bb72b90778f3adffcc4bd1ed6540edc7d738ca75c4d43d5ed75137bf2b0e05
- `sources/w10-net-a-net-check-activation/net_check_activation.cpp` 6147ba6b4901394f2c2dc41836cabbf758176c5505051c2ca7bfbc5015553328
- `sources/w10-net-a-net-get-camera-custom-control/net_get_camera_custom_control.cpp` 5898128e0861fb469b5608879e610484437d0ff6ecfd4ab592204b7cfbc830a8
- `sources/w10-net-a-net-get-nm-connected-partners/net_get_nm_connected_partners.cpp` cd5d62f807bea92ae3ed423a76614a9fc046505fe57291a6f4239fa3d16f7c21
- `sources/w10-net-a-net-is-modem/net_is_modem.cpp` cb1f345c4953da71df991b3d4c4facf23d9a2cdcf57e04d76ab57e6285298215

Host-check builds of `advance_real_state.cpp` also need `-DNORRATH_W10NA_HOST=1` and `anal_compress.cpp` as a second file.

## Authored but never executed as part of a running game

All of it: nothing here has run outside the host checks and the private interpreter comparisons, which stub every
callee and use synthetic memory (no real network, hardware, frame timing or interrupts). `analCompress` and the
original `analDecompress` were run only from the image inside those comparisons. The data units were compared for
size and initial values only.

## Unknown

- Meaning of the second partner flag (byte 17622) in `netGetNmConnectedPartners` and of the kick word 17624.
- Why `advanceRealState` skips on `levelName[0] != 0`, and what the empty counting loop is for.
- Layout of `launchPeers` entries beyond the offsets the launch code uses; address format of `myaddr`.
- Names/types of the objects behind `gameCam` word 52, the descriptor word at +12, `lnet_buf` and the vtable entries
  used (+8 with 3, +16 damage-like call, +120 clone-like call); only their shapes are observed.
- Behaviour of `analCompress` for a negative count and of `advanceRealState` for a local index of -1 beyond reading
  the memory before `player`/`partners`.
