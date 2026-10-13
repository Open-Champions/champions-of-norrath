# w11-actor-b: actor class methods (2 of 3)

Branch `agents/w11-actor-b`. Package: Orc, PhProp, Player, Projectile, Prop, RecallEffect, Scorpion, Shooter and
Sparks methods that the partial build still imported. One verified function per commit; 37 of the 38 listed
imports are closed. `msg_run__6Player` is not (see "Unknown / not done").

Private reference: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`. Addresses are that file's symbol values; no game
bytes, listings or decompiler output are committed.

## Method (every function)

1. Observed: the original's instructions were read from the private image with the reviewed binutils and decoded
   by hand into the source; every constant, string, jump table and float bit pattern was read from the image.
   Branch delay slots (which execute before the branch target, including the `li`/`lw` that set a register used
   after the jump) were traced explicitly; two of the three decoding mistakes found during this package came from
   there (see below).
2. Each unit compiles alone with the reviewed EE `g++` `authored_cpp` flags. Compiled sizes differ from the image
   extents (table below): these are behaviour reconstructions, not byte matches.
3. Each unit directory has a `host_check.cpp` (host compiler, not an EE input) with mocked callees that fails if a
   field, argument, call order, boundary or return is wrong. Sources that call `strcmp`, `strlen`, `strstr`,
   `sprintf`, `memset` or `strcpy` by name give the call a local name with an `__asm__` label and the host check
   must be compiled with `-fno-builtin`.
4. Each function was also run against the original in a private R5900 interpreter kept outside git: the original and
   the authored object start from identical memory snapshots with identical stand-in callees; call logs (with
   arguments, floats and key fields at call time) and every changed byte outside the stack are compared. Scenario
   counts and instruction coverage of the original are in each commit message; uncovered words are nops, annulled
   branch-likely slots or unreachable code.
5. Mutants of each function (changed constants, offsets, flags, branch order) were run through the paired test;
   survivors are named in the commit messages (comment hits in the mutation tool, equivalent forms, or one-ulp
   constant changes the random scenarios never reached).
6. Nothing ran on an EE, an emulator or as gameplay. No claim of compatibility is made. Targets with no shared
   verifier yet; this package does not create one.

Decoding mistakes the paired runs caught (all fixed before their commit): a branch-delay-slot `li v0,4` that made a
comparison against 3 look like a comparison against 4 (Orc initType); a constant 0xafc7 read as 45015 instead of 44999
(Player msg_hurt, found only when generator-boundary scenarios were added); a light-registration test that a
"copy then clear" reading had turned into an unconditional call (Player clone).

## Shared helper

`sources/w11-actor-b-common/access.hpp` (byte-offset field access, host mapping of 32-bit address words, the
virtual-table entry call used by several units). Every unit that includes it lists it in `header_closure`.

## Observed, inferred, unknown (summary; per-function detail is in each source's header comment)

- Observed: all offsets, constants (including single-precision bit patterns one ulp below the nearest literal, such
  as 0.2 = 0x3e4ccccc), string literals, jump-table contents (Player drawAttachments: 14-entry slot table and 10-entry
  shift table), call order and argument registers.
- Observed: `msg_draw__6PhProp` owns a function-local 8-byte table (the image symbol `halos.1170`) initialised to
  `ff ff ff ff 00 80 cc 43`; `drawAttachments__6Player` keeps its model/texture locals between passes (a skipped pass
  reuses the previous pass's pointers).
- Inferred (from names only): that `_$_X` are destructors and `__tfX` type-descriptor accessors, that `msg_run`/`msg_draw`
  are the per-frame update/draw virtuals, that Player +960 is the player index and player + index * 23984 its record,
  that `cache` at 0x7cb770 is an armor cache with 88-byte elements and 352-byte slot stride.
- Unknown: the meaning of most fields and flag bits (listed in each source), the names of Player states, what the item
  flag bits (0x800000 and friends) select beyond the sound names they choose in PhProp msg_run, what `+564` counts.

## Functions

| source | symbol | image | image bytes | compiled bytes |
|---|---|---|---|---|
| `w11-actor-b-dtor-10projectile/dtor.cpp` | `_$_10Projectile` | `0x0030d658` | 108 | 112 |
| `w11-actor-b-dtor-12recalleffect/dtor.cpp` | `_$_12RecallEffect` | `0x003104f8` | 100 | 88 |
| `w11-actor-b-dtor-6phprop/dtor.cpp` | `_$_6PhProp` | `0x0030dce0` | 100 | 88 |
| `w11-actor-b-dtor-6player/dtor.cpp` | `_$_6Player` | `0x001947d0` | 348 | 352 |
| `w11-actor-b-dtor-6sparks/dtor.cpp` | `_$_6Sparks` | `0x00310af8` | 132 | 136 |
| `w11-actor-b-tf-10projectile/typeinfo.cpp` | `__tf10Projectile` | `0x0030d6c8` | 76 | 92 |
| `w11-actor-b-tf-12recalleffect/typeinfo.cpp` | `__tf12RecallEffect` | `0x00310560` | 76 | 92 |
| `w11-actor-b-tf-6player/typeinfo.cpp` | `__tf6Player` | `0x0030e7c8` | 80 | 92 |
| `w11-actor-b-clone-10projectile/clone.cpp` | `clone__10Projectile` | `0x00114310` | 276 | 252 |
| `w11-actor-b-clone-12recalleffect/clone.cpp` | `clone__12RecallEffect` | `0x00124008` | 304 | 312 |
| `w11-actor-b-clone-6phprop/clone.cpp` | `clone__6PhProp` | `0x00114b98` | 980 | 400 |
| `w11-actor-b-clone-6player/clone.cpp` | `clone__6Player` | `0x00116388` | 2280 | 1444 |
| `w11-actor-b-draw-attachments-6player/draw_attachments.cpp` | `drawAttachments__6PlayerP11_drawRecordf` | `0x001960f0` | 2224 | 2000 |
| `w11-actor-b-find-commander-3orc/find_commander.cpp` | `findCommander__3Orc` | `0x00229c20` | 216 | 200 |
| `w11-actor-b-get-model-6player/get_model.cpp` | `getModel__6Player` | `0x0030e828` | 8 | 8 |
| `w11-actor-b-init-type-3orc/init_type.cpp` | `initType__3Orcb` | `0x002270b8` | 1408 | 1612 |
| `w11-actor-b-init-4prop/init.cpp` | `init__4Propi` | `0x00147a48` | 996 | 1088 |
| `w11-actor-b-is-enemy-6player/is_enemy.cpp` | `isEnemy__6PlayerP10GameObject` | `0x001aba90` | 88 | 76 |
| `w11-actor-b-join-commander-3orc/join_commander.cpp` | `joinCommander__3Orc` | `0x0022b880` | 92 | 84 |
| `w11-actor-b-leave-commander-3orc/leave_commander.cpp` | `leaveCommander__3Orc` | `0x0022b8e0` | 80 | 88 |
| `w11-actor-b-lose-body-part-8scorpion/lose_body_part.cpp` | `loseBodyPart__8Scorpioni` | `0x001f2e00` | 260 | 252 |
| `w11-actor-b-collision-6player/msg_collision.cpp` | `msg_collision__6PlayerP10GameObject` | `0x001ab988` | 196 | 192 |
| `w11-actor-b-draw-12recalleffect/msg_draw.cpp` | `msg_draw__12RecallEffect` | `0x001e55c8` | 8 | 8 |
| `w11-actor-b-draw-6phprop/msg_draw.cpp` | `msg_draw__6PhProp` | `0x0014a468` | 420 | 448 |
| `w11-actor-b-draw-6player/msg_draw.cpp` | `msg_draw__6Player` | `0x00196fc8` | 2220 | 2368 |
| `w11-actor-b-get-poke-name-6phprop/msg_get_poke_name.cpp` | `msg_getPokeName__6PhPropPUsi` | `0x00154a10` | 168 | 168 |
| `w11-actor-b-hurt-6player/msg_hurt.cpp` | `msg_hurt__6PlayerfP10GameObjecti` | `0x001a46a8` | 996 | 1204 |
| `w11-actor-b-poke-6phprop/msg_poke.cpp` | `msg_poke__6PhPropi` | `0x00154ab8` | 240 | 236 |
| `w11-actor-b-run-12recalleffect/msg_run.cpp` | `msg_run__12RecallEffect` | `0x001e55c0` | 8 | 8 |
| `w11-actor-b-run-6phprop/msg_run.cpp` | `msg_run__6PhProp` | `0x00149438` | 4140 | 3644 |
| `w11-actor-b-save-7shooter/msg_save.cpp` | `msg_save__7ShooterPc` | `0x001e5d80` | 24 | 16 |
| `w11-actor-b-draw-effects-6player/weapon_effects.cpp` | `playerDrawWeaponEffects__6Playerf` | `0x001aaad8` | 1200 | 940 |
| `w11-actor-b-set-active-chunks-6player/set_active_chunks.cpp` | `setActiveChunks__6Player` | `0x0030e818` | 8 | 8 |
| `w11-actor-b-set-on-fire-6phprop/set_on_fire.cpp` | `setOnFire__6PhPropi` | `0x00154ba8` | 8 | 8 |
| `w11-actor-b-set-state-6player/set_state.cpp` | `setState__6Playeri` | `0x001a4d50` | 296 | 312 |
| `w11-actor-b-set-weapon-3orc/set_weapon.cpp` | `setWeapon__3Orc` | `0x00226fb8` | 256 | 320 |
| `w11-actor-b-stun-6player/stun.cpp` | `stun__6Playeri` | `0x0030e820` | 8 | 8 |

## Imports closed (37)

`_$_10Projectile`, `_$_12RecallEffect`, `_$_6PhProp`, `_$_6Player`, `_$_6Sparks`, `__tf10Projectile`, `__tf12RecallEffect`, `__tf6Player`, `clone__10Projectile`, `clone__12RecallEffect`, `clone__6PhProp`, `clone__6Player`, `drawAttachments__6PlayerP11_drawRecordf`, `findCommander__3Orc`, `getModel__6Player`, `initType__3Orcb`, `init__4Propi`, `isEnemy__6PlayerP10GameObject`, `joinCommander__3Orc`, `leaveCommander__3Orc`, `loseBodyPart__8Scorpioni`, `msg_collision__6PlayerP10GameObject`, `msg_draw__12RecallEffect`, `msg_draw__6PhProp`, `msg_draw__6Player`, `msg_getPokeName__6PhPropPUsi`, `msg_hurt__6PlayerfP10GameObjecti`, `msg_poke__6PhPropi`, `msg_run__12RecallEffect`, `msg_run__6PhProp`, `msg_save__7ShooterPc`, `playerDrawWeaponEffects__6Playerf`, `setActiveChunks__6Player`, `setOnFire__6PhPropi`, `setState__6Playeri`, `setWeapon__3Orc`, `stun__6Playeri`

Not closed: `msg_run__6Player` (0x00197878, 27,496 bytes).

## Imports opened

Symbols my units reference but do not define (from the `U` entries of the 37 objects minus everything they define). I did not run
a new partial build, so some may already be supplied by other packages or by the shared branch:

`DLG_Active__Fv`, `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`, `_$_10GameObject`, `_$_14AnimationState`, `_$_5Light`, `_$_8Creature`, `_3Orc$m_commanderList`, `_4item$globalItem`, `__10GameObjectG6Point3PQ210GameObject11resourceMapPPc`, `__10StaticFireG6Point3fif`, `__14AnimationStateRC14AnimationState`, `__15AnimationState8RC15AnimationState8`, `__8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif`, `__8ItemPropG6Point3PQ24item11descriptionP8Matrix34`, `__8ListHeadRC8ListHead`, `__rtti_si`, `__tf10GameObject`, `__tf11SpellCharge`, `__tf15SpellHolyShield`, `__tf8Creature`, `__ti10GameObject`, `__ti8Creature`, `_vt$10GameObject`, `_vt$10Projectile`, `_vt$12RecallEffect`, `_vt$4Prop`, `_vt$6PhProp`, `_vt$6Player`, `_vt$6Sparks`, `_vt$8Creature`, `animAddOneShot__FP14AnimationStateP15AnimationHeaderffi`, `animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi`, `animGlobalSync__FP14AnimationState`, `blockAlloc__Fi`, `blockFree__FPv`, `bodyAddForce__FP5_bodyP6Point3N31`, `bodyApplyForce__FP5_bodyG6Point3T1f`, `bodyApplyImpulse__FP5_bodyG6Point3T1f`, `bodyGetVelocity__FP5_bodyP6Point3T1`, `cacheEquipment__FPcP10ArmorCachePP12_modelHeaderPP8_texture`, `charAnimTest`, `checkIfSpellIsRunning__8CreatureRC9type_info`, `cutMode`, `draw__8CreatureP12_modelHeaderP8_textureT1`, `eRand__10GameObject`, `engineCurrentLevelName`, `engineSetScreenColor__Fiiii`, `enumerateAlivePlayers__Fv`, `frameNm`, `fxFlyingFire__FP6Point3T0ffb`, `fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34`, `gameControlPaused`, `gameFlags`, `gameHidePlayers`, `gamePause`, `gamePlayersPresent`, `getName__4itemPQ24item11descriptioni`, `getPicName__4itemPciPQ24item11description`, `getProxyMeshId__4itemPQ24item11description`, `haloColor`, `haloTexture1`, `healColor`, `healPulse`, `hitSomething__11SpellChargeP10GameObject`, `hudNetWaiting`, `iatan2__Fff`, `icos__Fi`, `interfaceOn`, `inventoryItemCheckSlot__FiPQ24item11description`, `isin__Fi`, `jumpGateJumping__Fv`, `levelFlags`, `levelName`, `lightAdd__FP5Light`, `lightResetOptionalParams__Fv`, `lightSetAmbientOffset__FPs`, `listDestroy__FP8ListHead`, `listOnewayLink__FP8ListHeadT0Pv`, `lumpFindResource__FPcT0`, `manaAbsorbOnHit__15SpellHolyShield`, `matMakeTransYaw__FP8Matrix34G6Point3s`, `matRot__FP8Matrix34is`, `matScale__FP8Matrix34f`, `matTrans__FP8Matrix34fff`, `matTransform__FP6Point3P8Matrix44T0`, `matUntransform__FP6Point3P8Matrix33T0`, `maybeDrawMultiPlayerMarkers__6Player`, `memset`, `modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii`, `modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi`, `modelGetActiveChunks__FP12_modelHeaderi`, `modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f`, `modelGetCollision__FP12_modelHeaderi`, `modelResetOptionalParams__Fv`, `modelSetAttachmentFlip__Fv`, `modelSetHalo__FP7HaloDefi`, `netGetCameraCustomControl__Fi`, `netNetworkMode`, `netPlayerIsLocal__Fi`, `netPrediction`, `netPredictionFastForward`, `nmPlayers`, `norrath_player_armor_cache_view_7cb770`, `objectAddToDeleteList__FP10GameObject`, `objectAddToRunList__FP10GameObject`, `objectRemoveFromRunList__FP10GameObject`, `objectUpdateInGrid__FP10GameObject`, `player`, `playerBlockSparks__6Playeri`, `playerItemScan__6PlayerP3padi`, `playerMaybeGetItem__FiPQ24item11description`, `recallOn`, `resetRouting__8Creature`, `setParameters__8Creaturefffffi`, `setupHalo__4itemPQ24item11descriptionf`, `shipOn`, `shopActiveFrame`, `skills`, `soundPlaySequence__FP15AnimationHeaderG6Point3f`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`, `spellAlpha`, `sprintf`, `strcmp`, `strcpy`, `strlen`, `strstr`, `syncRandState`, `textGetString__FPc`, `textureDrawColor__FP8_textureiiiUi`, `thereIsACutPlayer`, `ustrcat__FPUsPc`, `ustrcat__FPUsT0`, `world`, `worldCameraMatrix`, `worldCameraPos`, `worldCheckForWater__FP12_worldHeaderG6Point3fPf`, `worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3`, `worldDifficulty`, `worldFindStandHeight__FP12_worldHeaderG6Point3f`, `worldPerturbWater__FP12_worldHeaderG6Point3ff`, `worldViewMatrix`.

Notes on a few: `norrath_player_armor_cache_view_7cb770` is the existing alias for the local image symbol `cache`
(0x007cb770), used by `playerDrawWeaponEffects__6Player` and `drawAttachments__6Player`; `_3Orc$m_commanderList` is
the Orc static member list head; `skills`, `player`, `levelName` etc. are data owned by other packages.

## Build-plan entries needed

Not edited in the shared files. `docs/agents/w11-actor-b-plan-entries.py --toolchain /path/to/ee [--private-root DIR]`
(derived from the w10-actor-c script) regenerates these lines and writes a private plan and snapshot copy outside the
repository; the plan with these entries passes `python3 tools/build_partial.py --check-plan` (4,491 selected inputs,
5,224 source and header files, `source_plan_valid`, no target tools executed). Each needs a fresh `input_order` and a
snapshot input (component = directory name, `private_object_sha256` not claimed), and `default_authored_inputs` raised
by 37. `header_closure` lists `sources/w11-actor-b-common/access.hpp` where it is included.

```
{"emitted_functions": [{"compiled_body_bytes": 252, "name": "clone__10Projectile"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_clone_10projectile.o", "profile": "authored_cpp", "sha256": "e067c14ab5c8dd0dc2dbe3a8d19bab7107fb2eef12ac2f1e781af6589d10e633", "source": "sources/w11-actor-b-clone-10projectile/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 312, "name": "clone__12RecallEffect"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_clone_12recalleffect.o", "profile": "authored_cpp", "sha256": "6c7e43dcef9e7edb72c0fb9981edb9ece240b8d695ef81e585002502fba6312c", "source": "sources/w11-actor-b-clone-12recalleffect/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 400, "name": "clone__6PhProp"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_clone_6phprop.o", "profile": "authored_cpp", "sha256": "0f6f904409d44cdd53e5a79dca30102e3cf53e9699f7097328380400d2ad1da8", "source": "sources/w11-actor-b-clone-6phprop/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1444, "name": "clone__6Player"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_clone_6player.o", "profile": "authored_cpp", "sha256": "e5b7a89f152594b307b17292c4550114298d71891cc7b40b05ae36a312d1f306", "source": "sources/w11-actor-b-clone-6player/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 192, "name": "msg_collision__6PlayerP10GameObject"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_collision_6player.o", "profile": "authored_cpp", "sha256": "6d1e3494c74f8730a8db7a5265d2b2fc54aa79f9f7b3dd0d93cbdb6e6c4db11e", "source": "sources/w11-actor-b-collision-6player/msg_collision.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "msg_draw__12RecallEffect"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_draw_12recalleffect.o", "profile": "authored_cpp", "sha256": "f59ed758e029f426507ff2360fe262f27565092c79d07e45b980ce83c23a27d9", "source": "sources/w11-actor-b-draw-12recalleffect/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 448, "name": "msg_draw__6PhProp"}], "header_closure": ["sources/w11-actor-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w11ab_draw_6phprop.o", "profile": "authored_cpp", "sha256": "5cd3e3f232171773f6755c558e00351053053507ce76c73ff489bc8f0ed00331", "source": "sources/w11-actor-b-draw-6phprop/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 2368, "name": "msg_draw__6Player"}], "header_closure": ["sources/w11-actor-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w11ab_draw_6player.o", "profile": "authored_cpp", "sha256": "d39d67fef96488eac6be541290dd39e41b88fc7ca9eae2ee2629ce6ca7e82523", "source": "sources/w11-actor-b-draw-6player/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 2000, "name": "drawAttachments__6PlayerP11_drawRecordf"}], "header_closure": ["sources/w11-actor-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w11ab_draw_attachments_6player.o", "profile": "authored_cpp", "sha256": "00f6e4402fc25276eecec880f78f1b294f0b26bb7bcebbe55ae19d54166d5f8f", "source": "sources/w11-actor-b-draw-attachments-6player/draw_attachments.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 940, "name": "playerDrawWeaponEffects__6Playerf"}], "header_closure": ["sources/w11-actor-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w11ab_draw_effects_6player.o", "profile": "authored_cpp", "sha256": "6932f90960a0284dda303115579f2a3b28a06713563a02020c428ae91e11453d", "source": "sources/w11-actor-b-draw-effects-6player/weapon_effects.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 112, "name": "_$_10Projectile"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_dtor_10projectile.o", "profile": "authored_cpp", "sha256": "060cca5a2b3cc86f79bd92cbd43d5cccf77bca2ee6a4dd8f2c825c23fa9ec9a3", "source": "sources/w11-actor-b-dtor-10projectile/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "_$_12RecallEffect"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_dtor_12recalleffect.o", "profile": "authored_cpp", "sha256": "ebb4bcbb7d8f81f338191c0526a24bd221a2bc3e6796eb0d3d0bd45a55edec5d", "source": "sources/w11-actor-b-dtor-12recalleffect/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "_$_6PhProp"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_dtor_6phprop.o", "profile": "authored_cpp", "sha256": "829611a7a20109175f7d2e257cc8df7ed7f5c2c25865f30695a4bbfa21aba2b2", "source": "sources/w11-actor-b-dtor-6phprop/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 352, "name": "_$_6Player"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_dtor_6player.o", "profile": "authored_cpp", "sha256": "917a1e3e6124805382e420f47c8640484b4493bf619f57b61b17ea4ac5644c79", "source": "sources/w11-actor-b-dtor-6player/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "_$_6Sparks"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_dtor_6sparks.o", "profile": "authored_cpp", "sha256": "a189ba1053b0a41286029bf6205eebdd50f960eaabf4b8fdc6524c6c304fd601", "source": "sources/w11-actor-b-dtor-6sparks/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 200, "name": "findCommander__3Orc"}], "header_closure": ["sources/w11-actor-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w11ab_find_commander_3orc.o", "profile": "authored_cpp", "sha256": "08ca892cba7130979a8da4874444b129f1b3251f05072d411121c23976543e0e", "source": "sources/w11-actor-b-find-commander-3orc/find_commander.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getModel__6Player"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_get_model_6player.o", "profile": "authored_cpp", "sha256": "995be1a0a3d21d3095f57d199f75c53583c22a1a004690a377958ddd582672b3", "source": "sources/w11-actor-b-get-model-6player/get_model.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 168, "name": "msg_getPokeName__6PhPropPUsi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_get_poke_name_6phprop.o", "profile": "authored_cpp", "sha256": "e8644ad5b5e7d4955d19e07735b849a62a27f36bd03a8c412b967c4dbb0fc029", "source": "sources/w11-actor-b-get-poke-name-6phprop/msg_get_poke_name.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1204, "name": "msg_hurt__6PlayerfP10GameObjecti"}], "header_closure": ["sources/w11-actor-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w11ab_hurt_6player.o", "profile": "authored_cpp", "sha256": "1a41fa3fc4552abf0d7dbd18ec741a23124a07f5499f673b16668de144f933b4", "source": "sources/w11-actor-b-hurt-6player/msg_hurt.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1088, "name": "init__4Propi"}], "header_closure": ["sources/w11-actor-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w11ab_init_4prop.o", "profile": "authored_cpp", "sha256": "ee5df90bf14a7944861ef60011cfef26ed18fea5b914f49b2249db8ccdb22390", "source": "sources/w11-actor-b-init-4prop/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1612, "name": "initType__3Orcb"}], "header_closure": ["sources/w11-actor-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w11ab_init_type_3orc.o", "profile": "authored_cpp", "sha256": "8651acfca7786f9de781f0af35a92a513deea8e8dfd103d131e42bd81faf330b", "source": "sources/w11-actor-b-init-type-3orc/init_type.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 76, "name": "isEnemy__6PlayerP10GameObject"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_is_enemy_6player.o", "profile": "authored_cpp", "sha256": "deecae678315a068899f82ba6d9ecad2397909afe75e613c8c4470b1cfbaa122", "source": "sources/w11-actor-b-is-enemy-6player/is_enemy.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 84, "name": "joinCommander__3Orc"}], "header_closure": ["sources/w11-actor-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w11ab_join_commander_3orc.o", "profile": "authored_cpp", "sha256": "45056f09c60871e2ef742a88c516b4bc3a3bd440b62e88e7e010160c4a3cfc37", "source": "sources/w11-actor-b-join-commander-3orc/join_commander.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "leaveCommander__3Orc"}], "header_closure": ["sources/w11-actor-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w11ab_leave_commander_3orc.o", "profile": "authored_cpp", "sha256": "47424aed7b4132da02479a5939e5a477b02d0066014998a964438b1a36e494d8", "source": "sources/w11-actor-b-leave-commander-3orc/leave_commander.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 252, "name": "loseBodyPart__8Scorpioni"}], "header_closure": ["sources/w11-actor-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w11ab_lose_body_part_8scorpion.o", "profile": "authored_cpp", "sha256": "dccf534637701bd91ad0f292df778744aee083ec8effaaa1b3a82b47aa3dcbd1", "source": "sources/w11-actor-b-lose-body-part-8scorpion/lose_body_part.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 236, "name": "msg_poke__6PhPropi"}], "header_closure": ["sources/w11-actor-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w11ab_poke_6phprop.o", "profile": "authored_cpp", "sha256": "562744d82b2f968b41718cee729ff18898294165829f5559cd8be5350b0f6f5f", "source": "sources/w11-actor-b-poke-6phprop/msg_poke.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "msg_run__12RecallEffect"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_run_12recalleffect.o", "profile": "authored_cpp", "sha256": "dce5dc282317c90e102bf5e7811a76e364008f386d94be5782f0b80ee017768c", "source": "sources/w11-actor-b-run-12recalleffect/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 3644, "name": "msg_run__6PhProp"}], "header_closure": ["sources/w11-actor-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w11ab_run_6phprop.o", "profile": "authored_cpp", "sha256": "22c92f272402f473b3dffe62675c150a3d7b7c4b758cbbe57e68ffd46acbf159", "source": "sources/w11-actor-b-run-6phprop/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "msg_save__7ShooterPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_save_7shooter.o", "profile": "authored_cpp", "sha256": "fa67b4364b6612d51b6658b00d19904f5d4c2f4c59ba7cef7b05530058e98539", "source": "sources/w11-actor-b-save-7shooter/msg_save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "setActiveChunks__6Player"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_set_active_chunks_6player.o", "profile": "authored_cpp", "sha256": "57b5e35d4a2018841c625be6de0a51df8516e1d8438d53c9d9efd52797707b82", "source": "sources/w11-actor-b-set-active-chunks-6player/set_active_chunks.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "setOnFire__6PhPropi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_set_on_fire_6phprop.o", "profile": "authored_cpp", "sha256": "cf97163e5d1efb147c34bd05ef4461cabf3a5fbcfe3063a192609fdb7d468d38", "source": "sources/w11-actor-b-set-on-fire-6phprop/set_on_fire.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 312, "name": "setState__6Playeri"}], "header_closure": ["sources/w11-actor-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w11ab_set_state_6player.o", "profile": "authored_cpp", "sha256": "986218707a347047efc25b4f2baa869d0d323bb5cc123d973374b1f89c9c4b7c", "source": "sources/w11-actor-b-set-state-6player/set_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 320, "name": "setWeapon__3Orc"}], "header_closure": ["sources/w11-actor-b-common/access.hpp"], "kind": "independently_authored_source", "object_name": "w11ab_set_weapon_3orc.o", "profile": "authored_cpp", "sha256": "b7e19d082a07295e8ced8ba7603bb6a0af86441cd44f7b1e718ec09d2e6383cb", "source": "sources/w11-actor-b-set-weapon-3orc/set_weapon.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "stun__6Playeri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_stun_6player.o", "profile": "authored_cpp", "sha256": "337c26e06f4ea718cf1934199efb29471b170e4f699c44579017c748a3e10990", "source": "sources/w11-actor-b-stun-6player/stun.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf10Projectile"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_tf_10projectile.o", "profile": "authored_cpp", "sha256": "c859486575c6f8ed861ad927d2b6998ba6f003eabd8871a61d62a17230f5d29a", "source": "sources/w11-actor-b-tf-10projectile/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf12RecallEffect"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_tf_12recalleffect.o", "profile": "authored_cpp", "sha256": "5b7094c4c503faecb74ebf2cf4195bbb9c946015c43c84e050ca14e6a3773130", "source": "sources/w11-actor-b-tf-12recalleffect/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf6Player"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w11ab_tf_6player.o", "profile": "authored_cpp", "sha256": "92aacdd31176f2dbc5047b4f942e538462c8aa739b92fce393a52a6e616b3bd6", "source": "sources/w11-actor-b-tf-6player/typeinfo.cpp"}
```

## Authored but never executed

All of it ran only in the host contracts and in the private R5900 paired runs; none of it ran on an EE or in the game.
Pairing used stand-in callees, so the real behaviour of the callees, struct layouts and ordering effects across frames
are untested. Particularly assumption-heavy: `msg_run__6PhProp` (physics with 25 callees), `drawAttachments__6Player`
(jump tables, persistent locals) and `msg_hurt__6Player` (random threshold 44,999 on the shared generator).

## Unknown / not done

- `msg_run__6Player` is not authored (27 KB, thousands of instructions, dozens of callees).
- Boundary-only mutants (one ulp on a threshold constant, `<` vs `<=` at exact equality) survive in `msg_run__6PhProp`
  and the commit message lists them.
- Field meanings and state names, as listed above.
