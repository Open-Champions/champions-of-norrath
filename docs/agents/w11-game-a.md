# w11-game-a: game world, net and object helpers

Package `w11-game-a`, branch `agents/w11-game-a`. Private reference: `SLUS_205.65` (the pinned executable). Addresses
below are that file's symbol values. No game bytes or listings are committed. Every source compiles with the reviewed
`authored_cpp` profile. Nothing here ran on a PS2 or in the game; "paired" means the authored object was run next to the
original function in a private R5900 interpreter (kept outside git, per-package copy `priv-w11-game-a`).

## How each function was checked

- Paired run (private): the unit is compiled, linked with every undefined symbol bound to the original's address, then the
  original and the authored function run from identical memory snapshots with identical stub callees (hooks keyed by symbol or
  by raw address for function-pointer slots). Compared: the ordered callee log (argument registers, float arguments, pointed-to
  data), and every changed byte outside the stack. Instruction coverage of the original was measured.
- Host contract (`host_check.cpp` in each directory, not a build input): restates the expected behaviour with stub callees and
  in-memory objects. Units that read 32-bit pointer words from game memory use `NORRATH_HOST_BASE` / `NORRATH_HOST_CONTRACT`
  in the host check to turn those words into offsets or table indexes.
- Mutations of constants, bounds and branch senses were run against the host check or the paired run and detected.

## Functions

| directory | symbol | original extent | check |
|---|---|---|---|
| `w11-game-a-list-head-copy` | `__8ListHeadRC8ListHead` | 0x194498, 8 | paired + host |
| `w11-game-a-more-recent` | `moreRecent__Fii` | 0x2562d8, 192 | paired + host |
| `w11-game-a-compare-state-packets` | `compareStatePackets__FP11statePacketT0i` | 0x129f00, 232 | paired + host |
| `w11-game-a-jump-gate-init` | `jumpGateInit__FiPcc` | 0x2845f0, 144 | paired + host |
| `w11-game-a-jump-gate-add-destination` | `jumpGateAddDestination__Fv` | 0x2846f8, 164 | paired + host |
| `w11-game-a-object-add-string` | `objectAddString__FPcP10GameObject` | 0x140410, 368 | 300 paired + host |
| `w11-game-a-object-add-to-grid` | `objectAddToGrid__FP10GameObject` | 0x143720, 200 | 300 paired + host |
| `w11-game-a-world-get-door-height` | `worldGetDoorHeight__FP12_worldHeaderi` | 0x17d3d0, 64 | paired + host |
| `w11-game-a-net-disconnect` | `netDisconnect__Fv` | 0x1351c0, 120 | paired + host |
| `w11-game-a-set-resource` | `setResource__10GameObjectPQ210GameObject11resourceMap` | 0x143bb0, 136 | paired + host |
| `w11-game-a-player-raise-player` | `playerRaisePlayer__FiG6Point3` | 0x1ab388, 312 | paired + host |
| `w11-game-a-world-can-pass` | `worldCanPass__FP12_worldHeaderG6Point3T1fi` | 0x176e88, 624 | 312 paired + host |
| `w11-game-a-world-hitscan-with-objects` | `worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii` | see source | paired + host |
| `w11-game-a-ant-wave-ctor` | `__7AntWaveP10GameObjectG6Point3si` | 0x208be8, 228 | paired + host |
| `w11-game-a-set-iop-function` | `SetIopFunction__FUii` | 0x1127040, 152 | 120 paired + host |
| `w11-game-a-floor-add` | `floorAdd__FG6Point3sfii` | 0x191ef8, 956 | 500 paired (234 of 239 words), 4 host mutants |
| `w11-game-a-random-teleport-spell` | `RandomTeleportSpell__FP10GameObjectP6Point3f` | 0x108b68, 752 | 400 paired (all 188 words) + host |
| `w11-game-a-spell-harm-touch-attack` | `attack__14SpellHarmTouchP10GameObjectPiT2` | 0x29aa00, 944 | 120 paired (all 236 words) + host |
| `w11-game-a-receive-control-packet` | `receiveControlPacket__FPiib` | 0x132d40, 2596 | 700 paired with generated bit streams (640 of 649 words), 6 mutants detected, host |

Storage units (addresses and sizes in each file header): `w11-game-a-group-tables` (`nmEqGroups`, `nmWGroups`, `eqGroup`,
`wGroup`), `w11-game-a-jump-gate-pos-hidden` (`jumpGatePos`, `jumpHidden`), `w11-game-a-net-flags` (`netTrueStep`,
`netPrediction`, `netPredictionFastForward`, `netLogDisable`) and `w11-game-a-route-dirs` (`route_dirs`).

## Observed / inferred / unknown

Each source file opens with its own observed / inferred / unknown comment; the summary lines are only a pointer.

- `receiveControlPacket` is the receive side of a lock-step input history. Observed: partner record stride 17632, record
  history of 256 x 24 bytes at +172, 256 state packets of 12 bytes at +6316, an 8192-byte side-data ring at +9388, the
  marker word 0xbd40. Inferred: names such as "frame", "ring". Unknown: what the byte array at `0x0048681c` (indexed by a
  packet byte) is for, and the meaning of the fields at +17600, +17604, +17608 and +17624.
- `floorAdd`, `RandomTeleportSpell` and `SpellHarmTouch::attack` are described in their headers, including the unnamed
  read-only float constants (used by bit pattern).

## Imports closed

__7AntWaveP10GameObjectG6Point3si, setResource__10GameObjectPQ210GameObject11resourceMap, __8ListHeadRC8ListHead,
RandomTeleportSpell__FP10GameObjectP6Point3f, SetIopFunction__FUii, attack__14SpellHarmTouchP10GameObjectPiT2,
compareStatePackets__FP11statePacketT0i, eqGroup, floorAdd__FG6Point3sfii, jumpGateAddDestination__Fv,
jumpGateInit__FiPcc, jumpGatePos, jumpHidden, moreRecent__Fii, netDisconnect__Fv, netLogDisable, netPrediction,
netPredictionFastForward, netTrueStep, nmEqGroups, nmWGroups, objectAddString__FPcP10GameObject,
objectAddToGrid__FP10GameObject, playerRaisePlayer__FiG6Point3, receiveControlPacket__FPiib, route_dirs, wGroup,
worldCanPass__FP12_worldHeaderG6Point3T1fi, worldGetDoorHeight__FP12_worldHeaderi,
worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii.

## Not closed

Two symbols of the package are not authored. Both were read and measured; neither was started as source, because a partial
body would not be a verifiable function and the notes say "no credit for placeholders".

- `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb` at 0x001ce218, 9184 bytes (2297 instructions, about
  240 branches). Observed outline of the first ~45% of the body, for whoever takes it: the arguments are victim, attacker,
  a low and high damage float, four ints (the first is read as a rating, the third as a flag word with bits 0x40, 0x20000,
  0x08000000, 0x01000000 and 0x40000000 tested), an optional item description (null selects a zeroed local with four
  8-byte modifier slots at +36: a type short at +0, three value shorts at +2, +4, +6; empty slots hold -1) and a bool. It
  uses soft-double arithmetic (`litodp`, `fptodp`, `fmod`, `dpadd`, `dptofp`) for the random damage roll in
  [low, high], RTTI comparisons through vtable slot 1 (`__tf8Innoruuk`, `__eq__` on `type_info`), about 13
  `lumpFindResource` plus `soundPlaySequence` effect plays, 50-iteration spark loops using `eRand`, a recursive call to
  itself (reflect back at the attacker), `playerGetSkillRank` with the two critical-chance routines, and `blockAlloc`
  calls that build `poisonGasSpell`, `SpellDiseaseEffect` and `FlyingText` objects. A modifier lookup (scan of the four
  slots for one type number, then a value short scaled by an unnamed float constant and used as a roll threshold against a
  16-bit random number) is inlined about two dozen times with the out-of-line "found" blocks the compiler placed after the
  loops, so a faithful reconstruction needs a full control-flow map first. Unknown: the item description layout beyond the
  modifier slots, the names of the rating and flag arguments, and what most modifier types (27, 29, 31 to 34, 41 and others)
  mean.
- `routePlot__FR10RouteQueue` at 0x002c0a10, 19812 bytes (4954 instructions, 319 branches). It inlines the `RouteHeap`
  template code (`swap__H1Z9RouteHeap_RX01T0_v` and the int/float/`RouteBuffer` swaps), calls `routeUnrollVnode__Fi` twelve
  times, `qsort` four times, `worldHitscan` four times and `worldHitscanWithObjects` twice, and uses soft-double compares.
  It was not decoded.

## Imports opened (undefined in my units and not defined by them)

The units leave these undefined (some are other packages' symbols, some are image data/functions nobody has authored yet):

MCA_DirContents, MC_Debug, P_AddParticle__FP11ParticleDefP6Point3T1f, __11LightEffectG6Point3ffffffffffffP10GameObjecti, __6PlayerG6Point3sii, __8CreatureG6Point3sPQ210GameObject11resourceMapfPPc, _ctype_, _vt$7AntWave, analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii, blockAlloc__Fi, eRandState, eRand__5Spell, engineAddTask__FPFv_viPci, engineSetScreenColor__Fiiii, expandList, freeList, g_BloodLevel, gameGetFullLevelName__FPc, gameNetKick__Fi, ghostObjectGrid, harm1, harm2, hudSetInfoMessage__FPUsiic, iatan2__Fff, icos__Fi, isin__Fi, jumpGateDestCount, jumpGateEntries, jumpGateHurtDest, jumpGateState, jumpGateTaskDraw__Fv, jumpGateTaskRun__Fv, jumpGateWho, jumping, lightFloorSprite__FG6Point3, listOnewayLink__FP8ListHeadT0Pv, lumpFindResource__FPcT0, megatileRot, megatileSeed, memcpy, modelGetNmShadowPoints__FP12_modelHeader, modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f, netNetworkMode, netPlayerIsLocal__Fi, nmPartners, norrath_net_flag_bytes_48681c, norrath_overlay_sFunctionIndex_11edf00, norrath_overlay_sIopFunctionData_11edf08, norrath_overlay_sSend_11edb00, objectAddToRunList__FP10GameObject, objectFindEmptySpot__FG6Point3iiPc, objectFindInBox__FG6Point3T0PP10GameObjecti, objectGrid, objectHash, objectStrings, objectUpdateInGrid__FP10GameObject, offTheGrid, partners, player, resourceFrame, resurectEffect__6Player, soundPlaySequence__FP15AnimationHeaderP10GameObjectf, sprGrid, sprintf, spriteCount, strcasecmp, strchr, strcmp, strcpy, stringHash, strlen, syncRandState, totallyDisconnected, ustrcpy__FPUsPc, world, worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3, worldFindHeight__FP12_worldHeaderG6Point3f, worldFindStandHeight__FP12_worldHeaderG6Point3f, worldHitscan__FP12_worldHeaderG6Point3T1fi

Names I introduced that do not exist in the image's symbol table: `norrath_net_flag_bytes_48681c` (the byte array at
0x0048681c, unnamed in the image), `norrath_overlay_sSend_11edb00`, `norrath_overlay_sFunctionIndex_11edf00` and
`norrath_overlay_sIopFunctionData_11edf08` (the overlay-local buffers of `SetIopFunction`, left as address-carrying imports).

## Build-plan entries needed

The lead integrator should append these `files` entries to `experimental/build-plan.json` (one JSON object per line, no
header closure needed). I kept a private copy of the plan outside git.

```
{"emitted_functions":[{"compiled_body_bytes":236,"name":"__7AntWaveP10GameObjectG6Point3si"}],"header_closure":[],"kind":"independently_authored_source","object_name":"ant_wave_ctor.o","profile":"authored_cpp","sha256":"22f41a3d548fd9ae24406ddf55fde7a861f6bd0d3eb8350fa640e00503ecfab2","source":"sources/w11-game-a-ant-wave-ctor/ant_wave_ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":292,"name":"compareStatePackets__FP11statePacketT0i"}],"header_closure":[],"kind":"independently_authored_source","object_name":"compare_state_packets.o","profile":"authored_cpp","sha256":"a22f808fe93439df9d2ee349e29099a8ae56780ce8668dfccfa74c429e2f27dc","source":"sources/w11-game-a-compare-state-packets/compare_state_packets.cpp"}
{"emitted_functions":[{"compiled_body_bytes":840,"name":"floorAdd__FG6Point3sfii"}],"header_closure":[],"kind":"independently_authored_source","object_name":"floor_add.o","profile":"authored_cpp","sha256":"861460b34cdf72d525921b4cffe1d2186361212907d88d7ff479a2367591b66d","source":"sources/w11-game-a-floor-add/floor_add.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"group_tables.o","profile":"authored_cpp","sha256":"c62078bf015d0b3ceaabe0246fbbf57bf150806f6ac37bc22af675215154657e","source":"sources/w11-game-a-group-tables/group_tables.cpp"}
{"emitted_functions":[{"compiled_body_bytes":200,"name":"jumpGateAddDestination__Fv"}],"header_closure":[],"kind":"independently_authored_source","object_name":"jump_gate_add_destination.o","profile":"authored_cpp","sha256":"9fc4fe36ce18d21c3062ef7d423a2ccb345443d6ba08b57dce3bd71096dba07e","source":"sources/w11-game-a-jump-gate-add-destination/jump_gate_add_destination.cpp"}
{"emitted_functions":[{"compiled_body_bytes":172,"name":"jumpGateInit__FiPcc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"jump_gate_init.o","profile":"authored_cpp","sha256":"fb1e140e1550f3684df470475ff96d84c912922dfd3ac13265fbc4bfcf884309","source":"sources/w11-game-a-jump-gate-init/jump_gate_init.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"jump_gate_pos_hidden.o","profile":"authored_cpp","sha256":"30500d2d99bd3a2f283e0b8f9f4bd90dcc614e466d763f7631a6d029bc6638c4","source":"sources/w11-game-a-jump-gate-pos-hidden/jump_gate_pos_hidden.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"__8ListHeadRC8ListHead"}],"header_closure":[],"kind":"independently_authored_source","object_name":"list_head_copy.o","profile":"authored_cpp","sha256":"7116294624a09d10d57cf53abf37f0a1abb93e3b225f2d0bb3e278d8fa7904fe","source":"sources/w11-game-a-list-head-copy/list_head_copy.cpp"}
{"emitted_functions":[{"compiled_body_bytes":208,"name":"moreRecent__Fii"}],"header_closure":[],"kind":"independently_authored_source","object_name":"more_recent.o","profile":"authored_cpp","sha256":"b502742a16207f95ede8afdbcd61e282f558d8f12756d042271ec7fba2118db5","source":"sources/w11-game-a-more-recent/more_recent.cpp"}
{"emitted_functions":[{"compiled_body_bytes":136,"name":"netDisconnect__Fv"}],"header_closure":[],"kind":"independently_authored_source","object_name":"net_disconnect.o","profile":"authored_cpp","sha256":"b065bda709cd5aabc12c9e2294d97a5a83e03f84c6ed56d84e3983b7660ef0af","source":"sources/w11-game-a-net-disconnect/net_disconnect.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"net_flags.o","profile":"authored_cpp","sha256":"1bdf03c0bb70eff1bdf28eb03865320c989774d3e215a88ab90efc83b0f59d1f","source":"sources/w11-game-a-net-flags/net_flags.cpp"}
{"emitted_functions":[{"compiled_body_bytes":444,"name":"objectAddString__FPcP10GameObject"}],"header_closure":[],"kind":"independently_authored_source","object_name":"object_add_string.o","profile":"authored_cpp","sha256":"2fc7f42fe45776cd53f5612c3d76f8c85ab9f1bed6ac1ff929f21036392dc14d","source":"sources/w11-game-a-object-add-string/object_add_string.cpp"}
{"emitted_functions":[{"compiled_body_bytes":168,"name":"objectAddToGrid__FP10GameObject"}],"header_closure":[],"kind":"independently_authored_source","object_name":"object_add_to_grid.o","profile":"authored_cpp","sha256":"fde8f700fdce603df71e97227b3e9374a2928f03cc1fcbfd692cf7d60abd35db","source":"sources/w11-game-a-object-add-to-grid/object_add_to_grid.cpp"}
{"emitted_functions":[{"compiled_body_bytes":440,"name":"playerRaisePlayer__FiG6Point3"}],"header_closure":[],"kind":"independently_authored_source","object_name":"player_raise_player.o","profile":"authored_cpp","sha256":"4cb09d395738c38c3dbe54220dfaecf0706ef802e806975f1f61d92418e77830","source":"sources/w11-game-a-player-raise-player/player_raise_player.cpp"}
{"emitted_functions":[{"compiled_body_bytes":812,"name":"RandomTeleportSpell__FP10GameObjectP6Point3f"}],"header_closure":[],"kind":"independently_authored_source","object_name":"random_teleport_spell.o","profile":"authored_cpp","sha256":"16422d02250e19f52814dc75468088023ed3b4fbcbd08c8c647a2e5c82ced1e3","source":"sources/w11-game-a-random-teleport-spell/random_teleport_spell.cpp"}
{"emitted_functions":[{"compiled_body_bytes":2796,"name":"receiveControlPacket__FPiib"}],"header_closure":[],"kind":"independently_authored_source","object_name":"receive_control_packet.o","profile":"authored_cpp","sha256":"1ca2d321af5f0217ad9b3bf4994250611e27e40c7776fd6ef2138c731624a597","source":"sources/w11-game-a-receive-control-packet/receive_control_packet.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"route_dirs.o","profile":"authored_cpp","sha256":"3e753455f981f83528964354835dc7d2b6919a818801631d96b3d7b51967f24c","source":"sources/w11-game-a-route-dirs/route_dirs.cpp"}
{"emitted_functions":[{"compiled_body_bytes":152,"name":"SetIopFunction__FUii"}],"header_closure":[],"kind":"independently_authored_source","object_name":"set_iop_function.o","profile":"authored_cpp","sha256":"42d66956606b5a8b81716cd5d295e3699e95a83fce537a97f3d41da94ce958ad","source":"sources/w11-game-a-set-iop-function/set_iop_function.cpp"}
{"emitted_functions":[{"compiled_body_bytes":224,"name":"setResource__10GameObjectPQ210GameObject11resourceMap"}],"header_closure":[],"kind":"independently_authored_source","object_name":"set_resource.o","profile":"authored_cpp","sha256":"7cce08ba449be8a0764d01570c8c3d32ca35de5c26ca632feea37ae08740b6c6","source":"sources/w11-game-a-set-resource/set_resource.cpp"}
{"emitted_functions":[{"compiled_body_bytes":884,"name":"attack__14SpellHarmTouchP10GameObjectPiT2"}],"header_closure":[],"kind":"independently_authored_source","object_name":"spell_harm_touch_attack.o","profile":"authored_cpp","sha256":"52e6368da57c9a7eb6043c0435e34854488623d8ce1ced49989a7e72e5b9a826","source":"sources/w11-game-a-spell-harm-touch-attack/spell_harm_touch_attack.cpp"}
{"emitted_functions":[{"compiled_body_bytes":680,"name":"worldCanPass__FP12_worldHeaderG6Point3T1fi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"world_can_pass.o","profile":"authored_cpp","sha256":"e70d960e77c39107fa19b331c98d20c13de33766a1b18dd8c6d9734cba6134e8","source":"sources/w11-game-a-world-can-pass/world_can_pass.cpp"}
{"emitted_functions":[{"compiled_body_bytes":192,"name":"worldGetDoorHeight__FP12_worldHeaderi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"world_get_door_height.o","profile":"authored_cpp","sha256":"6b9a489f38ec8c9d0a3de4c0607d598b4cb8f31e3c4fae15687812d07bef8b9a","source":"sources/w11-game-a-world-get-door-height/world_get_door_height.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1056,"name":"worldHitscanWithObjects__FP12_worldHeaderG6Point3T1fP10GameObjectPP10GameObjectii"}],"header_closure":[],"kind":"independently_authored_source","object_name":"world_hitscan_with_objects.o","profile":"authored_cpp","sha256":"e761899063d459a21936e67ad89a283cef4b60a595f9249733016a29250aae64","source":"sources/w11-game-a-world-hitscan-with-objects/world_hitscan_with_objects.cpp"}
```

## Authored but never executed

Every function and table here has a host contract with stubbed callees, EE compilation with the reviewed flags and a
private emulated paired run against the original with stand-in callees. None ran on the EE, in the game, or against real
callee bodies. The paired scenarios use constructed memory and scripted callee results; they establish agreement of
control flow, arguments and stores under those conditions, not gameplay. Nothing is an original-byte match.

## Unknown

- The meaning of most object fields touched (offsets and masks are stated as observed in each header).
- The bodies of the callees named in the sources (`analDecompress`, `modelGetShadowPos`, `worldClipMotion` and so on) are
  other packages' or unauthored; several stores here may be overwritten or reinterpreted by them.
- Float behaviour: the paired runs use the interpreter's single-float model, the host checks use host floats.
- The private harness has no MMI opcodes: the image's `memcpy`/`strcpy`/`memset` were replaced in the runs that reach them.
