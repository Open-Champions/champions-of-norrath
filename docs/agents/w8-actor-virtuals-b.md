# w8-actor-virtuals-b

Package: actor virtual methods, destructors and type info (2/3). Work is on
`agents/w8-actor-virtuals-b`; one commit per function, each with a host check next to its
source (`host_check.cpp`, not a plan input). Nothing in `experimental/build-plan.json`,
`experimental/snapshot.json`, a receipt, or `tools/` was edited.

## Observed

Private executable SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`
(the image used for every address below). Addresses, sizes and symbol names were read with
the reviewed EE binutils from that image's symbol table and code; no listing was committed.

| Function | Address | Size | Source directory (under `experimental/sources/`) |
| --- | --- | --- | --- |
| `getModel__10AutoObject` | 0x00314de8 | 8 | `w8-actor-virtuals-b-get-model-autoobject` |
| `msg_run__10HateBridge` | 0x00226468 | 124 | `w8-actor-virtuals-b-msg-run-hatebridge` |
| `msg_draw__10Teleporter` | 0x0021ce78 | 8 | `w8-actor-virtuals-b-msg-draw-teleporter` |
| `msg_run__10Teleporter` | 0x00216380 | 632 | `w8-actor-virtuals-b-msg-run-teleporter` |
| `clone__7Trigger` | 0x00115d80 | 336 | `w8-actor-virtuals-b-clone-trigger` |
| `clone__11MissileTrap` | 0x00124a70 | 272 | `w8-actor-virtuals-b-clone-missiletrap` |
| `clone__8FireBomb` | 0x001284e0 | 272 | `w8-actor-virtuals-b-clone-firebomb` |
| `clone__9Container` | 0x001154d8 | 368 | `w8-actor-virtuals-b-clone-container` |
| `clone__9WorldPart` | 0x00127580 | 272 | `w8-actor-virtuals-b-clone-worldpart` |
| `clone__7Shooter` | 0x00124b80 | 332 | `w8-actor-virtuals-b-clone-shooter` |
| `clone__15PushPhysicsProp` | 0x00129cb0 | 588 | `w8-actor-virtuals-b-clone-pushphysicsprop` |
| `clone__10Teleporter` | 0x001293a0 | 1348 | `w8-actor-virtuals-b-clone-teleporter` |
| `msg_load__7TriggerPc` | 0x001d1a90 | 36 | `w8-actor-virtuals-b-msg-load-trigger` |
| `msg_load__7ShooterPc` | 0x001e5d98 | 24 | `w8-actor-virtuals-b-msg-load-shooter` |
| `msg_poke__9Containeri` | 0x0021da08 | 96 | `w8-actor-virtuals-b-msg-poke-container` |
| `msg_hurt__9ContainerfP10GameObjecti` | 0x0021d870 | 144 | `w8-actor-virtuals-b-msg-hurt-container` |
| `msg_draw__9Container` | 0x0021d900 | 192 | `w8-actor-virtuals-b-msg-draw-container` |
| `msg_load__9ContainerPc` | 0x00217860 | 252 | `w8-actor-virtuals-b-msg-load-container` |
| `msg_getPokeName__9ContainerPUsi` | 0x0021d9c0 | 72 | `w8-actor-virtuals-b-msg-getpokename-container` |
| `msg_collision__9ContainerP10GameObject` | 0x00217960 | 212 | `w8-actor-virtuals-b-msg-collision-container` |
| `msg_poke__8AutoPropi` | 0x00224e78 | 76 | `w8-actor-virtuals-b-msg-poke-autoprop` |
| `msg_load__8AutoPropPc` | 0x00224d18 | 84 | `w8-actor-virtuals-b-msg-load-autoprop` |
| `msg_hurt__8AutoPropfP10GameObjecti` | 0x00224d70 | 264 | `w8-actor-virtuals-b-msg-hurt-autoprop` |
| `msg_getPokeName__8AutoPropPUsi` | 0x00220a38 | 392 | `w8-actor-virtuals-b-msg-getpokename-autoprop` |
| `advanceState__8AutoProp` | 0x00220bc0 | 876 | `w8-actor-virtuals-b-advance-state-autoprop` |
| `msg_collision__15PushPhysicsPropP10GameObject` | 0x001503e8 | 488 | `w8-actor-virtuals-b-msg-collision-pushphysicsprop` |
| `msg_draw__15PushPhysicsProp` | 0x00155d68 | 220 | `w8-actor-virtuals-b-msg-draw-pushphysicsprop` |
| `msg_load__15PushPhysicsPropPc` | 0x00150100 | 384 | `w8-actor-virtuals-b-msg-load-pushphysicsprop` |
| `msg_draw__10AutoObject` | 0x0021ec10 | 728 | `w8-actor-virtuals-b-msg-draw-autoobject` |
| `__tf9WorldPart` | 0x0030d9f8 | 76 | `w8-actor-virtuals-b-tf-9worldpart` |

Behaviour of each function is written at the top of its source file (offsets read, stores,
calls and their order, branch conditions). The AutoProp table slots called by
`msg_poke`, `msg_load`, `msg_hurt` and `advanceState` are identified from the existing
GothTrap table view (`advanceState__8AutoProp` at byte offset 136, `setState__8AutoPropi` at
144, `setPassableState` at 152); the other AutoProp family tables are consistent with that.
`clone__10Teleporter` has exactly the Creature clone's copy sequence with the Teleporter
table as the final store (the Creature clone copy spans were compared to it).
`clone__15PushPhysicsProp` and `msg_collision__9Container` differ from sibling clones and
messages only in sizes, offsets and constants listed in their files.

`netNetworkMode`, `modelClippingOn` and `currentRunningObject` are ELF symbols in the image
(bytes at 0x00483829, 0x0048407c and 0x00483acc); `teleportParticle1` and `teleportParticle2`
are 0x50-byte ELF data objects at 0x0042c670 and 0x0042c6c0.

## Inferred

The names of fields are not guessed anywhere; sources say "the word at +N". Inferences that
are stated as such in the sources: the AutoProp table slot identities above; that the
`+72` word of an object is a table address (observed as a table of 8-byte entries with a
16-bit adjustment then a function address); that the run-time type test in
`msg_collision__15PushPhysicsProp` is the compiler's type-identity test (table entry 0 is
called, the result compared with `__tf15PushPhysicsProp()` through `__eq__C9type_info
RC9type_info`).

## Unknown

- What almost every field means (state words, masks, float pairs, counters).
- Whether the object memory the allocator returns is zeroed; the clone gap bytes are
  documented as left untouched.
- The EE `rsqrt.s` result for a zero length (the host checks use IEEE division).
- Whether `objectFindInBox` terminates a full list of 16; the PushPhysicsProp collision
  reads the list as zero-terminated.
- Byte identity with the image: none is claimed. Compiled bodies differ in size from the
  originals (for example `msg_run__10HateBridge` 140 vs 124 bytes, `msg_draw__10AutoObject`
  852 vs 728), so every function here is a behaviour reconstruction checked by host tests,
  not a matched function.

## Checks performed

- Each source compiles with the reviewed EE `g++ 15.2.0` `authored_cpp` flags (no warnings
  treated as errors in that profile; the object defines only the named function, plus
  one local helper symbol `_ZN12_GLOBAL__N_1L4emitEPvPKh` in the Teleporter `msg_run`).
- Each `host_check.cpp` passes with the system `c++ -std=c++17 -Wall -Wextra -Werror`.
  The checks replace the callees with recording stubs and assert call order, arguments,
  stored bytes and untouched bytes for every branch listed in the source comment. Clone
  checks fill the destination with 0xa5 to detect stray or missing stores.
- A private copy of the plan and snapshot (outside git, copy of `experimental/` with
  the 30 entries below appended and `default_authored_inputs` raised by 30) built with
  `tools/build_partial.py`: 1437 compilations, one link, `partial_build_complete`.
  Unresolved imports went from 1195 to 1180 (30 closed, 15 opened).
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.

## Imports closed

`__tf9WorldPart`, `advanceState__8AutoProp`, `clone__10Teleporter`, `clone__11MissileTrap`,
`clone__15PushPhysicsProp`, `clone__7Shooter`, `clone__7Trigger`, `clone__8FireBomb`,
`clone__9Container`, `clone__9WorldPart`, `getModel__10AutoObject`,
`msg_collision__15PushPhysicsPropP10GameObject`, `msg_collision__9ContainerP10GameObject`,
`msg_draw__10AutoObject`, `msg_draw__10Teleporter`, `msg_draw__15PushPhysicsProp`,
`msg_draw__9Container`, `msg_getPokeName__8AutoPropPUsi`, `msg_getPokeName__9ContainerPUsi`,
`msg_hurt__8AutoPropfP10GameObjecti`, `msg_hurt__9ContainerfP10GameObjecti`,
`msg_load__15PushPhysicsPropPc`, `msg_load__7ShooterPc`, `msg_load__7TriggerPc`,
`msg_load__8AutoPropPc`, `msg_load__9ContainerPc`, `msg_poke__8AutoPropi`,
`msg_poke__9Containeri`, `msg_run__10HateBridge`, `msg_run__10Teleporter`.

## Imports opened

Newly unresolved after adding these units (not defined by any package on the integration
tip I built against):

`__6PhPropG6Point3PcN22liPQ24item11descriptionf`, `addEffect__FiG6Point3`,
`bodyInit__FP5_bodyfffff`, `bodyUpdateMatrix__FP5_body`, `eRand__10GameObject`,
`lightSetAmbientColor__FPs`, `lightSetAmbientOnly__Fv`,
`modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec`,
`modelSetShadowScale__Ff`, `objectActiveProcessing__FP10GameObjectii`,
`objectAddToRunList__FP10GameObject`, `objectAlwaysDraw__FP10GameObject`,
`objectCancelAllAlwaysDraw__Fv`, `teleportParticle1` and `teleportParticle2` (data).

Already unresolved before and still unresolved; now also referenced by my units:
`__15AnimationState8RC15AnimationState8`, `__eq__C9type_infoRC9type_info`, `__rtti_si`,
`__tf15PushPhysicsProp`, `animAdd__FP14AnimationStateP15AnimationHeaderi`,
`currentRunningObject`, `icos__Fi`, `isin__Fi`, `matMakeTransYaw__FP8Matrix34G6Point3s`,
`modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi`,
`modelGetActiveChunks__FP12_modelHeaderi`, `modelResetOptionalParams__Fv`, `msg_run__8AutoProp`,
`objectActivationCheck__8Creaturei`, `objectSaveGetFromStringPool__Fi`, `scriptStart__FPc`,
`strcasecmp`, `textGetString__FPc`, `ustrcpy__FPUsPc`, `ustrcpy__FPUsT0`.
(`__throw_bad_typeid` is called by the PushPhysicsProp collision; it is also unresolved.)
Resolved by other packages on the tip: `norrath_block_alloc`,
`norrath_list_head_copy_construct`, `norrath_game_object_vtable_view`, the `norrath_actors_*`
and `norrath_actor_view_4488f0` table views, `norrath_ti_10GameObject`,
`norrath_vt_tf_10GameObject`, `findTagString__10GameObjectPci`,
`lumpFindResource__FPcT0`, `P_AddParticle__FP11ParticleDefP6Point3T1f`,
`lightResetOptionalParams__Fv`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`,
`objectFindInBox__FG6Point3T0PP10GameObjecti`, `sprintf`, `netNetworkMode`,
`modelClippingOn`.

Strings: the short resource names, format texts and text-table keys these functions use
(for example the AutoProp hurt sounds, the `action%d`/`message%d` formats, a Container
text-table key) are held as literals in the sources that use them, not imported. The image
stores them in its shared string area; sharing/pooling is not reproduced. They are
resource identifiers and format strings, not dialogue.

## Build-plan entries needed

Append these 30 authored inputs after the current last input, assigning consecutive fresh
`input_order` values, raise `default_authored_inputs` by 30, add the `files` hashes below
to both the plan and the snapshot `files` maps, and add a snapshot input per entry with
the same `emitted_functions` (component = the source directory name). `host_check.cpp`
files are not inputs. `private_object_sha256` is not claimed.
`sources/w8-actor-virtuals-b-common/virtual_call.hpp` is a shared header used by five of
the sources; it is in their `header_closure`.

```json
{"emitted_functions":[{"compiled_body_bytes":1088,"name":"advanceState__8AutoProp"}],"header_closure":["sources/w8-actor-virtuals-b-common/virtual_call.hpp"],"kind":"independently_authored_source","object_name":"advance_state.o","profile":"authored_cpp","sha256":"aafc23d959ab96884441b67f666fb39bc08404d33d79b36a6318b6a519873c3d","source":"sources/w8-actor-virtuals-b-advance-state-autoprop/advance_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":364,"name":"clone__9Container"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"59c467e7a7359a1a1741422878a5a761953695cb3718ce35b6dc6ab92f3131ab","source":"sources/w8-actor-virtuals-b-clone-container/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":324,"name":"clone__8FireBomb"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"afeebc5e4f90103c6f8fe214aae863ce6713103530027cfe05f92e1bb5ba1620","source":"sources/w8-actor-virtuals-b-clone-firebomb/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":324,"name":"clone__11MissileTrap"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"9f102b8046076fdc21817c0c9ff47c85b88b31547f7e1fcac1bc4d5a73e3ea61","source":"sources/w8-actor-virtuals-b-clone-missiletrap/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":324,"name":"clone__15PushPhysicsProp"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"ac2fa7d07b686ed585fd78b4c5941a1848315673c3fe257901ffde2ea7d9eb96","source":"sources/w8-actor-virtuals-b-clone-pushphysicsprop/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":364,"name":"clone__7Shooter"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"8b07dd740f82a698f54ee5987f460a7531fffd91b90221610dd740112b7831ec","source":"sources/w8-actor-virtuals-b-clone-shooter/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":572,"name":"clone__10Teleporter"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"03a3a91b58d48ec9488d1e45c4ecf181f519e7d6962b85d6833a653f5ab499d4","source":"sources/w8-actor-virtuals-b-clone-teleporter/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":364,"name":"clone__7Trigger"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"21f1629fbe804b36b17c78670816672a390dd49568688f37eeb8ca5972faaef7","source":"sources/w8-actor-virtuals-b-clone-trigger/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":324,"name":"clone__9WorldPart"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"ad8bf26acb592dc856eeee4b0066f6f541d0c195d739b7a210b17c206d8ea69c","source":"sources/w8-actor-virtuals-b-clone-worldpart/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"getModel__10AutoObject"}],"header_closure":[],"kind":"independently_authored_source","object_name":"get_model.o","profile":"authored_cpp","sha256":"8750f47f913b11ec314cac14de6d0ad155c9b4bb2f0675309d6f938e8048a6a3","source":"sources/w8-actor-virtuals-b-get-model-autoobject/get_model.cpp"}
{"emitted_functions":[{"compiled_body_bytes":196,"name":"msg_collision__9ContainerP10GameObject"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_collision.o","profile":"authored_cpp","sha256":"1ab4c8004a3b55a0e8baa4154b527246ea876aa852176fc4503e97574ecda98a","source":"sources/w8-actor-virtuals-b-msg-collision-container/msg_collision.cpp"}
{"emitted_functions":[{"compiled_body_bytes":456,"name":"msg_collision__15PushPhysicsPropP10GameObject"}],"header_closure":["sources/w8-actor-virtuals-b-common/virtual_call.hpp"],"kind":"independently_authored_source","object_name":"msg_collision.o","profile":"authored_cpp","sha256":"aec103ceca42812f22baab862b3a60e0769b04a4285af1bfaa9d1e2fc0b4c777","source":"sources/w8-actor-virtuals-b-msg-collision-pushphysicsprop/msg_collision.cpp"}
{"emitted_functions":[{"compiled_body_bytes":852,"name":"msg_draw__10AutoObject"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_draw.o","profile":"authored_cpp","sha256":"6c01771f616eb50cd6f46948e949335152679977b2fb6690d257fd141a4a9b25","source":"sources/w8-actor-virtuals-b-msg-draw-autoobject/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":236,"name":"msg_draw__9Container"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_draw.o","profile":"authored_cpp","sha256":"c294ab426d0ad209f45b1eef049315dd7a3f57cdecf4c3910a450fb42ccd3898","source":"sources/w8-actor-virtuals-b-msg-draw-container/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":240,"name":"msg_draw__15PushPhysicsProp"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_draw.o","profile":"authored_cpp","sha256":"9052837f6c81cead3452811173a7db0151e68b2bd977e5d1b3bfc82ec3a7118b","source":"sources/w8-actor-virtuals-b-msg-draw-pushphysicsprop/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"msg_draw__10Teleporter"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_draw.o","profile":"authored_cpp","sha256":"f15a896d3c10ba9f90bd36c2b7cd59497fed19b98af4d25798dd79c8f8570742","source":"sources/w8-actor-virtuals-b-msg-draw-teleporter/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":384,"name":"msg_getPokeName__8AutoPropPUsi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_get_poke_name.o","profile":"authored_cpp","sha256":"87b92b5654e7dbc2e5f744e1d1b5d237532d7e83ede00f7161a1b793d3063180","source":"sources/w8-actor-virtuals-b-msg-getpokename-autoprop/msg_get_poke_name.cpp"}
{"emitted_functions":[{"compiled_body_bytes":84,"name":"msg_getPokeName__9ContainerPUsi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_get_poke_name.o","profile":"authored_cpp","sha256":"a8c5f45798c85177eb195274b1e7d838b245aa51f7d7ad7b8affc6f45104e739","source":"sources/w8-actor-virtuals-b-msg-getpokename-container/msg_get_poke_name.cpp"}
{"emitted_functions":[{"compiled_body_bytes":248,"name":"msg_hurt__8AutoPropfP10GameObjecti"}],"header_closure":["sources/w8-actor-virtuals-b-common/virtual_call.hpp"],"kind":"independently_authored_source","object_name":"msg_hurt.o","profile":"authored_cpp","sha256":"8cd8a1155c691b36f8f78cad61e712946631d45f574a0235e06504a37b49afc6","source":"sources/w8-actor-virtuals-b-msg-hurt-autoprop/msg_hurt.cpp"}
{"emitted_functions":[{"compiled_body_bytes":164,"name":"msg_hurt__9ContainerfP10GameObjecti"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_hurt.o","profile":"authored_cpp","sha256":"0c7dcdfb1af6de217dab6b5b5972a3b9337ff09d9e56dd40b387f4f3e7886dfc","source":"sources/w8-actor-virtuals-b-msg-hurt-container/msg_hurt.cpp"}
{"emitted_functions":[{"compiled_body_bytes":52,"name":"msg_load__8AutoPropPc"}],"header_closure":["sources/w8-actor-virtuals-b-common/virtual_call.hpp"],"kind":"independently_authored_source","object_name":"msg_load.o","profile":"authored_cpp","sha256":"6a6a4c66910ec5e265e1c31505902362042dd7ede107de1dbfff71d3748c36f8","source":"sources/w8-actor-virtuals-b-msg-load-autoprop/msg_load.cpp"}
{"emitted_functions":[{"compiled_body_bytes":212,"name":"msg_load__9ContainerPc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_load.o","profile":"authored_cpp","sha256":"cf6f5e080608d1619ead3c58a8b11c96c1920fa61d7e776750f7fc351279be05","source":"sources/w8-actor-virtuals-b-msg-load-container/msg_load.cpp"}
{"emitted_functions":[{"compiled_body_bytes":232,"name":"msg_load__15PushPhysicsPropPc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_load.o","profile":"authored_cpp","sha256":"36b02c8e7b44563249f0e80186a90819f029dfb8ca0a99a8454418c82851c478","source":"sources/w8-actor-virtuals-b-msg-load-pushphysicsprop/msg_load.cpp"}
{"emitted_functions":[{"compiled_body_bytes":16,"name":"msg_load__7ShooterPc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_load.o","profile":"authored_cpp","sha256":"4aaf70a11cad8199aa74eaa2f38a1cdc6a96a0d8cf5608f1234b30bc289f5834","source":"sources/w8-actor-virtuals-b-msg-load-shooter/msg_load.cpp"}
{"emitted_functions":[{"compiled_body_bytes":24,"name":"msg_load__7TriggerPc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_load.o","profile":"authored_cpp","sha256":"a4dc8a25c7faa4f37a1b0e6e871fe77e13ad1b049ba878773dae045f45c1c3df","source":"sources/w8-actor-virtuals-b-msg-load-trigger/msg_load.cpp"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"msg_poke__8AutoPropi"}],"header_closure":["sources/w8-actor-virtuals-b-common/virtual_call.hpp"],"kind":"independently_authored_source","object_name":"msg_poke.o","profile":"authored_cpp","sha256":"7f36cb25bf6190a1a8e8b4f215ce8272422d65268a505cbcbcaf37327d00a845","source":"sources/w8-actor-virtuals-b-msg-poke-autoprop/msg_poke.cpp"}
{"emitted_functions":[{"compiled_body_bytes":104,"name":"msg_poke__9Containeri"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_poke.o","profile":"authored_cpp","sha256":"87de9370cce1c4c620e6c619d0d6ade8898b6924061a63d6ce619ae098d4117c","source":"sources/w8-actor-virtuals-b-msg-poke-container/msg_poke.cpp"}
{"emitted_functions":[{"compiled_body_bytes":140,"name":"msg_run__10HateBridge"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_run.o","profile":"authored_cpp","sha256":"e0d262812cc2c230479dc54d122816f746f9e56485a4d6179d7e5547b554251f","source":"sources/w8-actor-virtuals-b-msg-run-hatebridge/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":284,"name":"_ZN12_GLOBAL__N_1L4emitEPvPKh"},{"compiled_body_bytes":68,"name":"msg_run__10Teleporter"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_run.o","profile":"authored_cpp","sha256":"50fbbd9655e8360a98fecc48131b470d004d21464d0b472c37520602b81e2a50","source":"sources/w8-actor-virtuals-b-msg-run-teleporter/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"__tf9WorldPart"}],"header_closure":[],"kind":"independently_authored_source","object_name":"typeinfo.o","profile":"authored_cpp","sha256":"81d1084c180bfaa99e8a7376c253f5f0259ce14e664aa2f056bfbe4b92e1b5ba","source":"sources/w8-actor-virtuals-b-tf-9worldpart/typeinfo.cpp"}
```

`files` hashes to add:

```json
{"sources/w8-actor-virtuals-b-advance-state-autoprop/advance_state.cpp":"aafc23d959ab96884441b67f666fb39bc08404d33d79b36a6318b6a519873c3d"}
{"sources/w8-actor-virtuals-b-clone-container/clone.cpp":"59c467e7a7359a1a1741422878a5a761953695cb3718ce35b6dc6ab92f3131ab"}
{"sources/w8-actor-virtuals-b-clone-firebomb/clone.cpp":"afeebc5e4f90103c6f8fe214aae863ce6713103530027cfe05f92e1bb5ba1620"}
{"sources/w8-actor-virtuals-b-clone-missiletrap/clone.cpp":"9f102b8046076fdc21817c0c9ff47c85b88b31547f7e1fcac1bc4d5a73e3ea61"}
{"sources/w8-actor-virtuals-b-clone-pushphysicsprop/clone.cpp":"ac2fa7d07b686ed585fd78b4c5941a1848315673c3fe257901ffde2ea7d9eb96"}
{"sources/w8-actor-virtuals-b-clone-shooter/clone.cpp":"8b07dd740f82a698f54ee5987f460a7531fffd91b90221610dd740112b7831ec"}
{"sources/w8-actor-virtuals-b-clone-teleporter/clone.cpp":"03a3a91b58d48ec9488d1e45c4ecf181f519e7d6962b85d6833a653f5ab499d4"}
{"sources/w8-actor-virtuals-b-clone-trigger/clone.cpp":"21f1629fbe804b36b17c78670816672a390dd49568688f37eeb8ca5972faaef7"}
{"sources/w8-actor-virtuals-b-clone-worldpart/clone.cpp":"ad8bf26acb592dc856eeee4b0066f6f541d0c195d739b7a210b17c206d8ea69c"}
{"sources/w8-actor-virtuals-b-common/virtual_call.hpp":"6d841cbaa8cf2fe3d00e01010e4979a1e271f47a74e513a58f62e0f5618b4dab"}
{"sources/w8-actor-virtuals-b-get-model-autoobject/get_model.cpp":"8750f47f913b11ec314cac14de6d0ad155c9b4bb2f0675309d6f938e8048a6a3"}
{"sources/w8-actor-virtuals-b-msg-collision-container/msg_collision.cpp":"1ab4c8004a3b55a0e8baa4154b527246ea876aa852176fc4503e97574ecda98a"}
{"sources/w8-actor-virtuals-b-msg-collision-pushphysicsprop/msg_collision.cpp":"aec103ceca42812f22baab862b3a60e0769b04a4285af1bfaa9d1e2fc0b4c777"}
{"sources/w8-actor-virtuals-b-msg-draw-autoobject/msg_draw.cpp":"6c01771f616eb50cd6f46948e949335152679977b2fb6690d257fd141a4a9b25"}
{"sources/w8-actor-virtuals-b-msg-draw-container/msg_draw.cpp":"c294ab426d0ad209f45b1eef049315dd7a3f57cdecf4c3910a450fb42ccd3898"}
{"sources/w8-actor-virtuals-b-msg-draw-pushphysicsprop/msg_draw.cpp":"9052837f6c81cead3452811173a7db0151e68b2bd977e5d1b3bfc82ec3a7118b"}
{"sources/w8-actor-virtuals-b-msg-draw-teleporter/msg_draw.cpp":"f15a896d3c10ba9f90bd36c2b7cd59497fed19b98af4d25798dd79c8f8570742"}
{"sources/w8-actor-virtuals-b-msg-getpokename-autoprop/msg_get_poke_name.cpp":"87b92b5654e7dbc2e5f744e1d1b5d237532d7e83ede00f7161a1b793d3063180"}
{"sources/w8-actor-virtuals-b-msg-getpokename-container/msg_get_poke_name.cpp":"a8c5f45798c85177eb195274b1e7d838b245aa51f7d7ad7b8affc6f45104e739"}
{"sources/w8-actor-virtuals-b-msg-hurt-autoprop/msg_hurt.cpp":"8cd8a1155c691b36f8f78cad61e712946631d45f574a0235e06504a37b49afc6"}
{"sources/w8-actor-virtuals-b-msg-hurt-container/msg_hurt.cpp":"0c7dcdfb1af6de217dab6b5b5972a3b9337ff09d9e56dd40b387f4f3e7886dfc"}
{"sources/w8-actor-virtuals-b-msg-load-autoprop/msg_load.cpp":"6a6a4c66910ec5e265e1c31505902362042dd7ede107de1dbfff71d3748c36f8"}
{"sources/w8-actor-virtuals-b-msg-load-container/msg_load.cpp":"cf6f5e080608d1619ead3c58a8b11c96c1920fa61d7e776750f7fc351279be05"}
{"sources/w8-actor-virtuals-b-msg-load-pushphysicsprop/msg_load.cpp":"36b02c8e7b44563249f0e80186a90819f029dfb8ca0a99a8454418c82851c478"}
{"sources/w8-actor-virtuals-b-msg-load-shooter/msg_load.cpp":"4aaf70a11cad8199aa74eaa2f38a1cdc6a96a0d8cf5608f1234b30bc289f5834"}
{"sources/w8-actor-virtuals-b-msg-load-trigger/msg_load.cpp":"a4dc8a25c7faa4f37a1b0e6e871fe77e13ad1b049ba878773dae045f45c1c3df"}
{"sources/w8-actor-virtuals-b-msg-poke-autoprop/msg_poke.cpp":"7f36cb25bf6190a1a8e8b4f215ce8272422d65268a505cbcbcaf37327d00a845"}
{"sources/w8-actor-virtuals-b-msg-poke-container/msg_poke.cpp":"87de9370cce1c4c620e6c619d0d6ade8898b6924061a63d6ce619ae098d4117c"}
{"sources/w8-actor-virtuals-b-msg-run-hatebridge/msg_run.cpp":"e0d262812cc2c230479dc54d122816f746f9e56485a4d6179d7e5547b554251f"}
{"sources/w8-actor-virtuals-b-msg-run-teleporter/msg_run.cpp":"50fbbd9655e8360a98fecc48131b470d004d21464d0b472c37520602b81e2a50"}
{"sources/w8-actor-virtuals-b-tf-9worldpart/typeinfo.cpp":"81d1084c180bfaa99e8a7376c253f5f0259ce14e664aa2f056bfbe4b92e1b5ba"}
```

## Authored but never executed

All 30 functions and the shared header are authored, compiled with the reviewed toolchain
and exercised only by host tests on this Mac with stubbed callees. None was executed on an
EE, in an emulator, or as part of gameplay, and none is on the startup path to
`engineFrameEnd`. Table dispatch (`virtual_call.hpp`) and the type-identity test are
tested through host-side address mapping, not real EE tables.
