# Spell, skill and actor virtuals, Gold to Lamp (w10-actor-h)

Branch `agents/w10-actor-h`. Package: the 32 unresolved imports of the partial build named in the task,
from `_vt$4Gold` through `msg_run__4Lamp`: the Gold and GutChunk virtual tables, GothTrap and HateHole
`msg_run`, the Ice, Innoruuk, JumpGate and Lamp class functions, and the ItemProp constructor.

Private reference: `work/private/SLUS_205.65` of the main checkout, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses below are that file's
symbol values and extents. No game bytes, disassembly listings or decompiler output are committed;
the sources are independently written C++ and prose.

## How each function was checked

- **Compile.** Every unit compiles with the reviewed EE g++ and the plan's `authored_cpp` flags, one
  unit at a time. No full partial build was run (the task said to prefer single units).
- **Host contract.** Each unit has a `host_check.cpp` (not an EE input) whose `main` returns non-zero
  when a store, a call argument, the call order or a result is wrong. Objects that hold 32-bit pointer
  words use `w10-actor-h-common/words.hpp`, whose host mode turns words into offsets from a base so the
  real field offsets are kept on a 64-bit host.
- **Paired run against the original image.** In a private R5900 interpreter (outside git) each authored
  function was compiled, linked with every undefined symbol bound to the original's address, and run next
  to the original function from identical memory snapshots with identical Python stand-ins for every
  callee, virtual slot and soft-float helper. A scenario passes only when the call logs (callee, argument
  words, float argument registers, the bytes behind pointer arguments, selected stack arguments and a
  hash of the object memory at each call) and every changed byte outside the stack are identical. Where a
  function has several paths, the scenario generator was biased until the original's instruction
  coverage stopped growing. The interpreter's float model is double arithmetic rounded to single, not the
  R5900 FPU, so scenarios that reach infinity or NaN are not evidence; none of the reported coverage
  relies on them (the one function that divides by a value that can be zero, `msg_run__4Lamp`'s size,
  cannot be zero there).
- **Mutations.** One-constant changes were caught for `clone__4Lamp` (a copy range), `msg_trigger__8Innoruuki`
  (the music mode) and `msg_run__8Innoruuk` (the ring angle step and a shock-field halfword).
- **Vtables.** `_vt$4Gold` and `_vt$8GutChunk` are data: the units were linked with every slot symbol
  bound to the original's address and the 144 bytes of the table compared with the image; they match.
- **Not byte matches.** Compiled sizes differ from the originals (see the table). These are behaviour
  reconstructions; nothing claims instruction equivalence or a percentage.

## Toolchain facts found on the way

- Decimal float literals can be one unit in the last place off in this compiler; every constant with a
  fraction is therefore written as a hexadecimal float literal (the paired runs caught two hand-written
  constants, 19000.0 against 7963.0 and a truncated 1.8).
- The compiler folds `1.0f / (300.0f * 300.0f)` at build time with its own rounding, which differs by one unit in
  the last place from the run-time `div.s` of the image. `msg_run__8JumpGate` therefore keeps that value
  opaque to the optimizer with an empty inline-asm statement (guarded by `__mips__`).
- `rsqrt.s` (used by `msg_run__4Lamp`, `msg_run__3Ice`) is written as inline assembly under `__mips__`
  with a portable host fallback, as in other units. `(int)f` compiles to `cvt.w.s` like the image.
- A `Point3` passed by value (`G6Point3`) is a pointer argument; the sources pass a pointer to a local
  12-byte copy, as the neighbouring units do.

## Functions

Columns: source, symbol, image address and bytes, compiled bytes, paired scenarios (and the fraction of
the original's instructions executed, where it was measured), sha256 of the source file. The "paired" column
is a scenario count; for clones it is byte-image comparisons of the whole new object.

| source | symbol | original address, bytes | compiled bytes | paired scenarios / original words executed | sha256 |
|---|---|---|---|---|---|
| `w10-actor-h-clone-ice/clone.cpp` | `clone__3Ice` | `0x00115648`, 312 | 272 | 12 byte-image comparisons | `bc1f1bc74cdba90195d61045ae625176502ba2433299372b0bee10babf441354` |
| `w10-actor-h-clone-jumpgate/clone.cpp` | `clone__8JumpGate` | `0x00128248`, 660 | 520 | 12 byte-image comparisons | `8d960270de1b8c64ff9b56ef2c53a7bf4ad10519a858e30f5f069ae4c91f1a33` |
| `w10-actor-h-clone-lamp/clone.cpp` | `clone__4Lamp` | `0x00123d30`, 432 | 332 | 12 byte-image comparisons | `70929cfba4e0b70d69a7e483cb78834d24b42e012c779d424a6405e0b6a0a941` |
| `w10-actor-h-ctor-8itemprop/ctor.cpp` | `__8ItemPropG6Point3PQ24item11descriptioni` | `0x0014b9e8`, 480 | 416 | 30 / 120 of 120 | `8996e1e3ebb2733d8fc9f2b5f860af5c174a27e06ccc01d39928594c58102e4f` |
| `w10-actor-h-dtor-ice/dtor.cpp` | `_$_3Ice` | `0x0030e068`, 100 | 88 | 10 | `69b20119579e5c837326add3f244e0fdb4f1d8b7917a0fcc4e2db46110ff1f50` |
| `w10-actor-h-dtor-innoruuk/dtor.cpp` | `_$_8Innoruuk` | `0x00314708`, 124 | 128 | 10 | `110e020321040154c09bab2756f4ac214375e59c9ffadda1d6013e1721ace6f1` |
| `w10-actor-h-dtor-jumpgate/dtor.cpp` | `_$_8JumpGate` | `0x00284450`, 160 | 164 | 10 | `052ce83e1af2348656061e12102be473374f39407f9b23feb155acd5e880b923` |
| `w10-actor-h-dtor-lamp/dtor.cpp` | `_$_4Lamp` | `0x00310370`, 116 | 120 | 10 | `21b7a26756025b703c47b22b782db3e3b075f54ab656195e665fa3e8195257ce` |
| `w10-actor-h-msg-draw-8innoruuk/msg_draw.cpp` | `msg_draw__8Innoruuk` | `0x001ff458`, 172 | 184 | 8 | `bdd53a03daea78da328ca60bd91c5511d331d57d262e6763eeb0aa2ea356d7bf` |
| `w10-actor-h-msg-draw-8jumpgate/msg_draw.cpp` | `msg_draw__8JumpGate` | `0x00282fc0`, 276 | 364 | 16 | `5884ffb081674c8eb8b5fc4748e783fbcfac853882cb67541b73cd652a2d7c1a` |
| `w10-actor-h-msg-draw-ice/msg_draw.cpp` | `msg_draw__3Ice` | `0x001e6330`, 168 | 200 | 8 | `ad249c16335018feff8f7a0e5e63149c943f894d1a8e2944a3fbeee870bdd909` |
| `w10-actor-h-msg-draw-lamp/msg_draw.cpp` | `msg_draw__4Lamp` | `0x00231318`, 140 | 176 | 10 | `2742be0f6acfa0ef001f578909b24298e6ca9f45c15e3175d0c6495719c66470` |
| `w10-actor-h-msg-get-poke-name-8jumpgate/msg_get_poke_name.cpp` | `msg_getPokeName__8JumpGatePUsi` | `0x002830d8`, 472 | 536 | 200 / 117 of 118 | `9e16bb3a733373f9761e3d62c40f4ed587f10bdd5bb8941310ae613bd6a40242` |
| `w10-actor-h-msg-hurt-8innoruuk/msg_hurt.cpp` | `msg_hurt__8InnoruukfP10GameObjecti` | `0x001ff568`, 64 | 72 | 8 | `5dd8369d1cd8c4849627736a5532ce88fd4235c3cd5e8b0db0fdbdfccda3723d` |
| `w10-actor-h-msg-load-8jumpgate/msg_load.cpp` | `msg_load__8JumpGatePc` | `0x00284560`, 140 | 132 | 8 | `e03744fa38ea1c9bb61627f012998ba68c12ca593639ed8dc652326df3906c2f` |
| `w10-actor-h-msg-poke-8jumpgate/msg_poke.cpp` | `msg_poke__8JumpGatei` | `0x002844f0`, 72 | 88 | 8 | `69453df2b0631e3af084e8eb69aa0a6c739f5857c31241cd8a6cf3775cd5ba96` |
| `w10-actor-h-msg-run-8gothtrap/msg_run.cpp` | `msg_run__8GothTrap` | `0x00226290`, 8 | 8 | 4 | `a76abc573c962827310de6254f7ea882296af4d20f899ad7b474b9124bb1f94d` |
| `w10-actor-h-msg-run-8hatehole/msg_run.cpp` | `msg_run__8HateHole` | `0x00226318`, 28 | 8 | 4 | `6fe251f3dab2e63dc12454e4cf952ee30c0b60af2f9a22321577a017ca7435b1` |
| `w10-actor-h-msg-run-8innoruuk/msg_run.cpp` | `msg_run__8Innoruuk` | `0x001f9150`, 11684 | 9788 | about 4,700 / 2891 of 2921 | `5681e9ef16d6af4d2a20d0ccd56de461ece02b4911d5e48bff42b9a104a7a0b2` |
| `w10-actor-h-msg-run-8jumpgate/msg_run.cpp` | `msg_run__8JumpGate` | `0x00282520`, 2720 | 2584 | 300 / 677 of 680 | `e8dfef644866eb0663b4cb17d4e17e6f0c8532151a33418703d29d05d8691652` |
| `w10-actor-h-msg-run-ice/msg_run.cpp` | `msg_run__3Ice` | `0x001df800`, 884 | 940 | 40 / 220 of 221 | `156763d700e2d7070de838514de652116b4592dcf98ce6173316c4ce7d2363bd` |
| `w10-actor-h-msg-run-lamp/msg_run.cpp` | `msg_run__4Lamp` | `0x00230b20`, 548 | 536 | 40 / 137 of 137 | `c20aa0cc094085ec7895b3689ee51dabb8d19dd899d16b861249844ebc02200d` |
| `w10-actor-h-msg-save-8jumpgate/msg_save.cpp` | `msg_save__8JumpGatePc` | `0x00284538`, 36 | 24 | 6 | `e55bada2576a033a6b53e34673da710bfd1440435ca4c3a5031e7975d4f9b92c` |
| `w10-actor-h-msg-trigger-8innoruuk/msg_trigger.cpp` | `msg_trigger__8Innoruuki` | `0x001ff508`, 96 | 96 | 8 | `f88882dae9210590b14558148b234f77fcc7f84354a65729905a5cfd1126803f` |
| `w10-actor-h-set-state-8innoruuk/set_state.cpp` | `setState__8Innoruuki` | `0x001fbef8`, 1724 | 2376 | about 1,300 / 416 of 431 | `e4f5a39089fa7a253a7f5f83b581474353a51b18b48572388ef70bc9c27ef8a2` |
| `w10-actor-h-tf-ice/typeinfo.cpp` | `__tf3Ice` | `0x0030e0d0`, 76 | 92 | 9 | `aa08636e23a9c5fba1d356262a6fd41fd12a57ba7ae693ea08c0cf509e50a04f` |
| `w10-actor-h-tf-ice/typeinfo.cpp` | `__ti3Ice` | `0x008eb278`, 12 | 12 (bss) |  | `aa08636e23a9c5fba1d356262a6fd41fd12a57ba7ae693ea08c0cf509e50a04f` |
| `w10-actor-h-tf-innoruuk/typeinfo.cpp` | `__tf8Innoruuk` | `0x00314788`, 80 | 92 | 9 | `094a774c77e2f15d0254e95b689de0dd0fcd70059ce5474e43959728273fda63` |
| `w10-actor-h-tf-innoruuk/typeinfo.cpp` | `__ti8Innoruuk` | `0x008eb438`, 12 | 12 (bss) |  | `094a774c77e2f15d0254e95b689de0dd0fcd70059ce5474e43959728273fda63` |
| `w10-actor-h-tf-jumpgate/typeinfo.cpp` | `__tf8JumpGate` | `0x003120b0`, 76 | 92 | 9 | `3a5ebfabe1e939bed79dd12fef660225fd3c51caf8ee5aefb2f875660388f1a6` |
| `w10-actor-h-tf-jumpgate/typeinfo.cpp` | `__ti8JumpGate` | `0x008eb1f8`, 12 | 12 (bss) |  | `3a5ebfabe1e939bed79dd12fef660225fd3c51caf8ee5aefb2f875660388f1a6` |
| `w10-actor-h-tf-lamp/typeinfo.cpp` | `__tf4Lamp` | `0x003103e8`, 76 | 92 | 9 | `214340b1faaff6b652fc5cfe8a4df989f87133c4d34e44ad3b88099a940d4ef3` |
| `w10-actor-h-tf-lamp/typeinfo.cpp` | `__ti4Lamp` | `0x008eaef8`, 12 | 12 (bss) |  | `214340b1faaff6b652fc5cfe8a4df989f87133c4d34e44ad3b88099a940d4ef3` |
| `w10-actor-h-update-enemy-8innoruuk/update_enemy.cpp` | `updateEnemy__8Innoruuk` | `0x001fc5b8`, 772 | 916 | 400 / 193 of 193 | `def62858ab8ccf58dd2876312ff176be28cb71a94db5365edfac104cc16ec4af` |
| `w10-actor-h-vt-gold/vt_gold.cpp` | `_vt$4Gold` | `0x0044cd48`, 144 | 144 (data) | 144 data bytes equal | `ed415aa0e8e856be981c18c7a0a9da1d8d486f358bc9e15a3aaff9572bcb21a5` |
| `w10-actor-h-vt-gutchunk/vt_gutchunk.cpp` | `_vt$8GutChunk` | `0x0044ad20`, 144 | 144 (data) | 144 data bytes equal | `6e7bff8008bc2a2798f38ad7dcc0229639c3986789820b19c78b60f44235181f` |

The unit `msg-run-8innoruuk` also emits file-local helpers (`spawn_soul`, `ring`, `unit_draw`, `damage`,
`light_effect`); `msg-run-8jumpgate` emits a local `unit_draw`. They are not imports.

## Imports closed (names)

_vt$4Gold, msg_run__8GothTrap, _vt$8GutChunk, msg_run__8HateHole, _$_3Ice, __tf3Ice, clone__3Ice, msg_draw__3Ice, msg_run__3Ice, _$_8Innoruuk, __tf8Innoruuk, msg_draw__8Innoruuk, msg_hurt__8InnoruukfP10GameObjecti, msg_run__8Innoruuk, msg_trigger__8Innoruuki, setState__8Innoruuki, updateEnemy__8Innoruuk, __8ItemPropG6Point3PQ24item11descriptioni, _$_8JumpGate, __tf8JumpGate, clone__8JumpGate, msg_draw__8JumpGate, msg_getPokeName__8JumpGatePUsi, msg_load__8JumpGatePc, msg_poke__8JumpGatei, msg_run__8JumpGate, msg_save__8JumpGatePc, _$_4Lamp, __tf4Lamp, clone__4Lamp, msg_draw__4Lamp, msg_run__4Lamp

## Imports opened (new unresolved symbols the authored units reference)

These are the undefined symbols of the 32 objects that no unit of this package defines. Many are classes'
constructors, base destructors and slot functions that other packages own; run the partial build to see
which remain after the lead integrator merges.

`DLG_CheckMutex__Fv`, `MC_SaveCheckPoint__Fiii`, `P_AddParticle__FP11ParticleDefP6Point3T1f`, `P_DeleteTrail__Fi`, `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`, `P_RevolveTrail__FiG6Point3N31i`, `P_StartTrail__Fiifiiiii`, `Pop__6Camera`, `Push__6Camera`, `SetMode__6Camerai`, `_$_10GameObject`, `_$_14AnimationState`, `_$_15FxCreatureState`, `_$_4Gold`, `_$_5Light`, `_$_8Creature`, `_$_8GutChunk`, `_4item$globalItem`, `__11LightEffectG6Point3ffffffffffffP10GameObjecti`, `__14AnimationStateRC14AnimationState`, `__14HateProjectileG6Point3sP10GameObjectP5SkilliP15AnimationHeader`, `__4PropG6Point3PcN22li`, `__6PhPropG6Point3P8Matrix34PcN23liPQ24item11descriptionf`, `__6PhPropG6Point3PcN22liPQ24item11descriptionf`, `__8HateBeamiG6Point3T2P8CreatureP5Skill`, `__8HateSoulG6Point3T1iii`, `__8ListHeadRC8ListHead`, `__rtti_si`, `__tf10GameObject`, `__tf4Gold`, `__tf8Creature`, `__tf8GutChunk`, `__ti10GameObject`, `__ti8Creature`, `_vt$10GameObject`, `_vt$3Ice`, `_vt$4Lamp`, `_vt$5PhIce`, `_vt$8Innoruuk`, `_vt$8ItemProp`, `_vt$8JumpGate`, `alternateEnemyInZone__8Creatureffss`, `animAddOneShot__FP14AnimationStateP15AnimationHeaderffi`, `animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi`, `animAdd__FP14AnimationStateP15AnimationHeaderi`, `animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps`, `animGlobalSync__FP14AnimationState`, `bExitStatePersueIdle__8Creature`, `blockAlloc__Fi`, `blockFree__FPv`, `bossMeterCurrent`, `bossMeterName`, `clone__4Gold`, `clone__8GutChunk`, `coreStartSwitchScene__Fi`, `creatureAdvanceAnimation__8Creaturef`, `creatureFindLeadYaw__8CreatureP10GameObjectf`, `deathEffect__8InnoruukP10GameObjectP12_modelHeaderP14AnimationStateG6Point3sff`, `dpcmp`, `dpmul`, `draw__8CreatureP12_modelHeaderP8_textureT1`, `dropTreasure__8CreatureG6Point3iffffffPc`, `eRand__10GameObject`, `ef1Noise__Fi`, `enemyInZone2__8Creatureffssfi`, `enemyInZone__8Creaturefs`, `engineSetScreenColor__Fiiii`, `engineTaskActive__FPc`, `everyFrame__8CreatureP12_modelHeader`, `findEnemyLeadPosition__8CreaturefP6Point3`, `findTagString__10GameObjectPci`, `frameNm`, `fxRunCreatureShock__FP12_modelHeaderP14AnimationStateG6Point3sP15FxCreatureStatefffUi`, `fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34`, `gameCam`, `gameNetKick__Fi`, `gameSetCameraShake__Ffb`, `getModel__10GameObject`, `getProxyMeshId__4itemPQ24item11description`, `getSmartRandomDestination__8Creaturei`, `haloTexture1`, `haloTexture2`, `haloTexture5`, `hudSetInfoMessage__FPUsiic`, `icos__Fi`, `initStuff__6PhPropi`, `innoruukParticle2`, `innoruukParticle3`, `innoruukResourceMap`, `isin__Fi`, `jumpGateAddDestination__Fv`, `jumpGateDestCount`, `jumpGateInit__FiPcc`, `jumpGatePos`, `jumpParticle2`, `jumpParticleOn`, `jumping`, `lampParticle`, `levelName`, `lightActivate__FP5Light`, `lightAdd__FP5Light`, `lightDeactivate__FP5Light`, `lightResetOptionalParams__Fv`, `lightSetAmbientOffset__FPs`, `lightUpdatePosition__FP5Light`, `listDestroy__FP8ListHead`, `listOnewayLink__FP8ListHeadT0Pv`, `litodp`, `lumpFindResource__FPcT0`, `matMakeTransYaw__FP8Matrix34G6Point3s`, `modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii`, `modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec`, `modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi`, `modelGetAttachmentPos__FP12_modelHeaderP14AnimationStateiP6Point3T3`, `modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f`, `modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f`, `modelGetNmShadowPoints__FP12_modelHeader`, `modelGetShadowPos__FP12_modelHeaderP8Matrix34iP6Point3Pf`, `modelResetOptionalParams__Fv`, `modelSetRelightAttachment__FG6Point3`, `move2__8CreatureG6Point3`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_collision__10GameObjectP10GameObject`, `msg_collision__4GoldP10GameObject`, `msg_draw__4Gold`, `msg_draw__8GutChunk`, `msg_getPokeName__10GameObjectPUsi`, `msg_getPokeName__6PhPropPUsi`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_hurt__8CreaturefP10GameObjecti`, `msg_load__4GoldPc`, `msg_load__4PropPc`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_poke__10GameObjecti`, `msg_poke__6PhPropi`, `msg_route__10GameObjecti`, `msg_run__4Gold`, `msg_run__8AutoProp`, `msg_run__8GutChunk`, `msg_save__4GoldPc`, `msg_save__4PropPc`, `msg_trigger__10GameObjecti`, `netDisconnect__Fv`, `netNetworkMode`, `netPlayerIsLocal__Fi`, `newDamageHandler__8CreatureP12_modelHeader`, `nmPlayersAlive`, `objectActivationCheck__8Creaturei`, `objectActiveProcessing__FP10GameObjectii`, `objectAddToRunList__FP10GameObject`, `objectFindInBox__FG6Point3T0PP10GameObjecti`, `objectRemoveFromRunList__FP10GameObject`, `objectUpdateInGrid__FP10GameObject`, `player`, `playersAlive`, `produceGeneralDamage__8CreatureffssiiiiPQ24item11description`, `resetRouting__8Creature`, `scriptGetGameState__FPc`, `scriptSetGameState__FPci`, `scriptStart__FPc`, `setStateProcessing__8CreaturePi`, `shockwaveInnoruukParticle`, `shockwaveInnoruukParticle2`, `skills`, `slideBack__8Creaturef`, `smartWander__8Creature`, `soundFadeStream__Fi`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`, `soundResetStream__Fi`, `soundStartMusic__FPci`, `soundStopSound__Fi`, `sprintf`, `steerAndRouteToEnemy__8Creature`, `summonhammerParticle`, `textGetString__FPc`, `turnToFaceEnemy__8Creature`, `ustrcpy__FPUsT0`, `vaporParticle`

## Build-plan entries needed (for the lead integrator)

All 32 units use profile `authored_cpp` and kind `independently_authored_source`. Units that include
`words.hpp` list it in `header_closure`; add it to `files` as well. Host contracts (`host_check.cpp`) are
not plan inputs. `input_order` is left to the integrator. Object names are made unique.

```json
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_clone_ice.o", "profile": "authored_cpp", "sha256": "bc1f1bc74cdba90195d61045ae625176502ba2433299372b0bee10babf441354", "source": "sources/w10-actor-h-clone-ice/clone.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_clone_jumpgate.o", "profile": "authored_cpp", "sha256": "8d960270de1b8c64ff9b56ef2c53a7bf4ad10519a858e30f5f069ae4c91f1a33", "source": "sources/w10-actor-h-clone-jumpgate/clone.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_clone_lamp.o", "profile": "authored_cpp", "sha256": "70929cfba4e0b70d69a7e483cb78834d24b42e012c779d424a6405e0b6a0a941", "source": "sources/w10-actor-h-clone-lamp/clone.cpp"},
{"header_closure": ["sources/w10-actor-h-common/words.hpp"], "kind": "independently_authored_source", "object_name": "w10_actor_h_ctor_8itemprop.o", "profile": "authored_cpp", "sha256": "8996e1e3ebb2733d8fc9f2b5f860af5c174a27e06ccc01d39928594c58102e4f", "source": "sources/w10-actor-h-ctor-8itemprop/ctor.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_dtor_ice.o", "profile": "authored_cpp", "sha256": "69b20119579e5c837326add3f244e0fdb4f1d8b7917a0fcc4e2db46110ff1f50", "source": "sources/w10-actor-h-dtor-ice/dtor.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_dtor_innoruuk.o", "profile": "authored_cpp", "sha256": "110e020321040154c09bab2756f4ac214375e59c9ffadda1d6013e1721ace6f1", "source": "sources/w10-actor-h-dtor-innoruuk/dtor.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_dtor_jumpgate.o", "profile": "authored_cpp", "sha256": "052ce83e1af2348656061e12102be473374f39407f9b23feb155acd5e880b923", "source": "sources/w10-actor-h-dtor-jumpgate/dtor.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_dtor_lamp.o", "profile": "authored_cpp", "sha256": "21b7a26756025b703c47b22b782db3e3b075f54ab656195e665fa3e8195257ce", "source": "sources/w10-actor-h-dtor-lamp/dtor.cpp"},
{"header_closure": ["sources/w10-actor-h-common/words.hpp"], "kind": "independently_authored_source", "object_name": "w10_actor_h_msg_draw_8innoruuk.o", "profile": "authored_cpp", "sha256": "bdd53a03daea78da328ca60bd91c5511d331d57d262e6763eeb0aa2ea356d7bf", "source": "sources/w10-actor-h-msg-draw-8innoruuk/msg_draw.cpp"},
{"header_closure": ["sources/w10-actor-h-common/words.hpp"], "kind": "independently_authored_source", "object_name": "w10_actor_h_msg_draw_8jumpgate.o", "profile": "authored_cpp", "sha256": "5884ffb081674c8eb8b5fc4748e783fbcfac853882cb67541b73cd652a2d7c1a", "source": "sources/w10-actor-h-msg-draw-8jumpgate/msg_draw.cpp"},
{"header_closure": ["sources/w10-actor-h-common/words.hpp"], "kind": "independently_authored_source", "object_name": "w10_actor_h_msg_draw_ice.o", "profile": "authored_cpp", "sha256": "ad249c16335018feff8f7a0e5e63149c943f894d1a8e2944a3fbeee870bdd909", "source": "sources/w10-actor-h-msg-draw-ice/msg_draw.cpp"},
{"header_closure": ["sources/w10-actor-h-common/words.hpp"], "kind": "independently_authored_source", "object_name": "w10_actor_h_msg_draw_lamp.o", "profile": "authored_cpp", "sha256": "2742be0f6acfa0ef001f578909b24298e6ca9f45c15e3175d0c6495719c66470", "source": "sources/w10-actor-h-msg-draw-lamp/msg_draw.cpp"},
{"header_closure": ["sources/w10-actor-h-common/words.hpp"], "kind": "independently_authored_source", "object_name": "w10_actor_h_msg_get_poke_name_8jumpgate.o", "profile": "authored_cpp", "sha256": "9e16bb3a733373f9761e3d62c40f4ed587f10bdd5bb8941310ae613bd6a40242", "source": "sources/w10-actor-h-msg-get-poke-name-8jumpgate/msg_get_poke_name.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_msg_hurt_8innoruuk.o", "profile": "authored_cpp", "sha256": "5dd8369d1cd8c4849627736a5532ce88fd4235c3cd5e8b0db0fdbdfccda3723d", "source": "sources/w10-actor-h-msg-hurt-8innoruuk/msg_hurt.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_msg_load_8jumpgate.o", "profile": "authored_cpp", "sha256": "e03744fa38ea1c9bb61627f012998ba68c12ca593639ed8dc652326df3906c2f", "source": "sources/w10-actor-h-msg-load-8jumpgate/msg_load.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_msg_poke_8jumpgate.o", "profile": "authored_cpp", "sha256": "69453df2b0631e3af084e8eb69aa0a6c739f5857c31241cd8a6cf3775cd5ba96", "source": "sources/w10-actor-h-msg-poke-8jumpgate/msg_poke.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_msg_run_8gothtrap.o", "profile": "authored_cpp", "sha256": "a76abc573c962827310de6254f7ea882296af4d20f899ad7b474b9124bb1f94d", "source": "sources/w10-actor-h-msg-run-8gothtrap/msg_run.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_msg_run_8hatehole.o", "profile": "authored_cpp", "sha256": "6fe251f3dab2e63dc12454e4cf952ee30c0b60af2f9a22321577a017ca7435b1", "source": "sources/w10-actor-h-msg-run-8hatehole/msg_run.cpp"},
{"header_closure": ["sources/w10-actor-h-common/words.hpp"], "kind": "independently_authored_source", "object_name": "w10_actor_h_msg_run_8innoruuk.o", "profile": "authored_cpp", "sha256": "5681e9ef16d6af4d2a20d0ccd56de461ece02b4911d5e48bff42b9a104a7a0b2", "source": "sources/w10-actor-h-msg-run-8innoruuk/msg_run.cpp"},
{"header_closure": ["sources/w10-actor-h-common/words.hpp"], "kind": "independently_authored_source", "object_name": "w10_actor_h_msg_run_8jumpgate.o", "profile": "authored_cpp", "sha256": "e8dfef644866eb0663b4cb17d4e17e6f0c8532151a33418703d29d05d8691652", "source": "sources/w10-actor-h-msg-run-8jumpgate/msg_run.cpp"},
{"header_closure": ["sources/w10-actor-h-common/words.hpp"], "kind": "independently_authored_source", "object_name": "w10_actor_h_msg_run_ice.o", "profile": "authored_cpp", "sha256": "156763d700e2d7070de838514de652116b4592dcf98ce6173316c4ce7d2363bd", "source": "sources/w10-actor-h-msg-run-ice/msg_run.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_msg_run_lamp.o", "profile": "authored_cpp", "sha256": "c20aa0cc094085ec7895b3689ee51dabb8d19dd899d16b861249844ebc02200d", "source": "sources/w10-actor-h-msg-run-lamp/msg_run.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_msg_save_8jumpgate.o", "profile": "authored_cpp", "sha256": "e55bada2576a033a6b53e34673da710bfd1440435ca4c3a5031e7975d4f9b92c", "source": "sources/w10-actor-h-msg-save-8jumpgate/msg_save.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_msg_trigger_8innoruuk.o", "profile": "authored_cpp", "sha256": "f88882dae9210590b14558148b234f77fcc7f84354a65729905a5cfd1126803f", "source": "sources/w10-actor-h-msg-trigger-8innoruuk/msg_trigger.cpp"},
{"header_closure": ["sources/w10-actor-h-common/words.hpp"], "kind": "independently_authored_source", "object_name": "w10_actor_h_set_state_8innoruuk.o", "profile": "authored_cpp", "sha256": "e4f5a39089fa7a253a7f5f83b581474353a51b18b48572388ef70bc9c27ef8a2", "source": "sources/w10-actor-h-set-state-8innoruuk/set_state.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_tf_ice.o", "profile": "authored_cpp", "sha256": "aa08636e23a9c5fba1d356262a6fd41fd12a57ba7ae693ea08c0cf509e50a04f", "source": "sources/w10-actor-h-tf-ice/typeinfo.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_tf_innoruuk.o", "profile": "authored_cpp", "sha256": "094a774c77e2f15d0254e95b689de0dd0fcd70059ce5474e43959728273fda63", "source": "sources/w10-actor-h-tf-innoruuk/typeinfo.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_tf_jumpgate.o", "profile": "authored_cpp", "sha256": "3a5ebfabe1e939bed79dd12fef660225fd3c51caf8ee5aefb2f875660388f1a6", "source": "sources/w10-actor-h-tf-jumpgate/typeinfo.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_tf_lamp.o", "profile": "authored_cpp", "sha256": "214340b1faaff6b652fc5cfe8a4df989f87133c4d34e44ad3b88099a940d4ef3", "source": "sources/w10-actor-h-tf-lamp/typeinfo.cpp"},
{"header_closure": ["sources/w10-actor-h-common/words.hpp"], "kind": "independently_authored_source", "object_name": "w10_actor_h_update_enemy_8innoruuk.o", "profile": "authored_cpp", "sha256": "def62858ab8ccf58dd2876312ff176be28cb71a94db5365edfac104cc16ec4af", "source": "sources/w10-actor-h-update-enemy-8innoruuk/update_enemy.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_vt_gold.o", "profile": "authored_cpp", "sha256": "ed415aa0e8e856be981c18c7a0a9da1d8d486f358bc9e15a3aaff9572bcb21a5", "source": "sources/w10-actor-h-vt-gold/vt_gold.cpp"},
{"header_closure": [], "kind": "independently_authored_source", "object_name": "w10_actor_h_vt_gutchunk.o", "profile": "authored_cpp", "sha256": "6e7bff8008bc2a2798f38ad7dcc0229639c3986789820b19c78b60f44235181f", "source": "sources/w10-actor-h-vt-gutchunk/vt_gutchunk.cpp"}
```

Files map additions:

```json
{
 "sources/w10-actor-h-clone-ice/clone.cpp": "bc1f1bc74cdba90195d61045ae625176502ba2433299372b0bee10babf441354",
 "sources/w10-actor-h-clone-jumpgate/clone.cpp": "8d960270de1b8c64ff9b56ef2c53a7bf4ad10519a858e30f5f069ae4c91f1a33",
 "sources/w10-actor-h-clone-lamp/clone.cpp": "70929cfba4e0b70d69a7e483cb78834d24b42e012c779d424a6405e0b6a0a941",
 "sources/w10-actor-h-common/words.hpp": "ceab32d787d615fcfd4432bd4bb949df76a55f897f1ecab6d02a7b70ad260dbf",
 "sources/w10-actor-h-ctor-8itemprop/ctor.cpp": "8996e1e3ebb2733d8fc9f2b5f860af5c174a27e06ccc01d39928594c58102e4f",
 "sources/w10-actor-h-dtor-ice/dtor.cpp": "69b20119579e5c837326add3f244e0fdb4f1d8b7917a0fcc4e2db46110ff1f50",
 "sources/w10-actor-h-dtor-innoruuk/dtor.cpp": "110e020321040154c09bab2756f4ac214375e59c9ffadda1d6013e1721ace6f1",
 "sources/w10-actor-h-dtor-jumpgate/dtor.cpp": "052ce83e1af2348656061e12102be473374f39407f9b23feb155acd5e880b923",
 "sources/w10-actor-h-dtor-lamp/dtor.cpp": "21b7a26756025b703c47b22b782db3e3b075f54ab656195e665fa3e8195257ce",
 "sources/w10-actor-h-msg-draw-8innoruuk/msg_draw.cpp": "bdd53a03daea78da328ca60bd91c5511d331d57d262e6763eeb0aa2ea356d7bf",
 "sources/w10-actor-h-msg-draw-8jumpgate/msg_draw.cpp": "5884ffb081674c8eb8b5fc4748e783fbcfac853882cb67541b73cd652a2d7c1a",
 "sources/w10-actor-h-msg-draw-ice/msg_draw.cpp": "ad249c16335018feff8f7a0e5e63149c943f894d1a8e2944a3fbeee870bdd909",
 "sources/w10-actor-h-msg-draw-lamp/msg_draw.cpp": "2742be0f6acfa0ef001f578909b24298e6ca9f45c15e3175d0c6495719c66470",
 "sources/w10-actor-h-msg-get-poke-name-8jumpgate/msg_get_poke_name.cpp": "9e16bb3a733373f9761e3d62c40f4ed587f10bdd5bb8941310ae613bd6a40242",
 "sources/w10-actor-h-msg-hurt-8innoruuk/msg_hurt.cpp": "5dd8369d1cd8c4849627736a5532ce88fd4235c3cd5e8b0db0fdbdfccda3723d",
 "sources/w10-actor-h-msg-load-8jumpgate/msg_load.cpp": "e03744fa38ea1c9bb61627f012998ba68c12ca593639ed8dc652326df3906c2f",
 "sources/w10-actor-h-msg-poke-8jumpgate/msg_poke.cpp": "69453df2b0631e3af084e8eb69aa0a6c739f5857c31241cd8a6cf3775cd5ba96",
 "sources/w10-actor-h-msg-run-8gothtrap/msg_run.cpp": "a76abc573c962827310de6254f7ea882296af4d20f899ad7b474b9124bb1f94d",
 "sources/w10-actor-h-msg-run-8hatehole/msg_run.cpp": "6fe251f3dab2e63dc12454e4cf952ee30c0b60af2f9a22321577a017ca7435b1",
 "sources/w10-actor-h-msg-run-8innoruuk/msg_run.cpp": "5681e9ef16d6af4d2a20d0ccd56de461ece02b4911d5e48bff42b9a104a7a0b2",
 "sources/w10-actor-h-msg-run-8jumpgate/msg_run.cpp": "e8dfef644866eb0663b4cb17d4e17e6f0c8532151a33418703d29d05d8691652",
 "sources/w10-actor-h-msg-run-ice/msg_run.cpp": "156763d700e2d7070de838514de652116b4592dcf98ce6173316c4ce7d2363bd",
 "sources/w10-actor-h-msg-run-lamp/msg_run.cpp": "c20aa0cc094085ec7895b3689ee51dabb8d19dd899d16b861249844ebc02200d",
 "sources/w10-actor-h-msg-save-8jumpgate/msg_save.cpp": "e55bada2576a033a6b53e34673da710bfd1440435ca4c3a5031e7975d4f9b92c",
 "sources/w10-actor-h-msg-trigger-8innoruuk/msg_trigger.cpp": "f88882dae9210590b14558148b234f77fcc7f84354a65729905a5cfd1126803f",
 "sources/w10-actor-h-set-state-8innoruuk/set_state.cpp": "e4f5a39089fa7a253a7f5f83b581474353a51b18b48572388ef70bc9c27ef8a2",
 "sources/w10-actor-h-tf-ice/typeinfo.cpp": "aa08636e23a9c5fba1d356262a6fd41fd12a57ba7ae693ea08c0cf509e50a04f",
 "sources/w10-actor-h-tf-innoruuk/typeinfo.cpp": "094a774c77e2f15d0254e95b689de0dd0fcd70059ce5474e43959728273fda63",
 "sources/w10-actor-h-tf-jumpgate/typeinfo.cpp": "3a5ebfabe1e939bed79dd12fef660225fd3c51caf8ee5aefb2f875660388f1a6",
 "sources/w10-actor-h-tf-lamp/typeinfo.cpp": "214340b1faaff6b652fc5cfe8a4df989f87133c4d34e44ad3b88099a940d4ef3",
 "sources/w10-actor-h-update-enemy-8innoruuk/update_enemy.cpp": "def62858ab8ccf58dd2876312ff176be28cb71a94db5365edfac104cc16ec4af",
 "sources/w10-actor-h-vt-gold/vt_gold.cpp": "ed415aa0e8e856be981c18c7a0a9da1d8d486f358bc9e15a3aaff9572bcb21a5",
 "sources/w10-actor-h-vt-gutchunk/vt_gutchunk.cpp": "6e7bff8008bc2a2798f38ad7dcc0229639c3986789820b19c78b60f44235181f"
}
```

## Authored but never executed

Nothing here has run on the EE, in the game or in the linked partial build. The paired runs are an
interpreter comparison on synthetic memory. The two virtual tables were only compared as data bytes. The
`__ti3Ice`, `__ti4Lamp`, `__ti8JumpGate` and `__ti8Innoruuk` type nodes are defined by the `__tf` units as
12-byte zeroed objects; their initialization by the first call to `__tf*` was exercised only in the host and
paired runs. `msg_run__8Innoruuk`'s remaining un-executed original words are alignment nops and three
arms that cannot be reached (a three-way sound pick whose index can only be 0..2, a two-way pick whose
value can only be 0 or 1).

## Unknown

- What the states of Innoruuk (values 0 to 21 of the halfword at +572) mean, and the fields at +256,
  +576, +752, +960 through +1008, +1216; the same for JumpGate's bytes at +76 to +79, +208, +220, and Lamp's
  bytes at +136/+137. Names in the sources come only from the symbols the code calls.
- `msg_run__8JumpGate`'s call of `jumpGateInit` after a full scan uses the slot one past the last player
  (`playersAlive[nmPlayersAlive]`). That is what the image does; whether it is intended is unknown.
- The two 64-byte runs at JumpGate +80 and +144 (a name and a lump name by use), the 40-byte region at
  +316 that is destroyed as an FxCreatureState but cloned bytewise, and the Gold/GutChunk slot classes
  beyond what the slot names say.
- The stores of -1 and 0 into ItemProp +360..+388 are overwritten by the 72-byte description copy that
  follows; they are kept in the order of the image but are not observable.
