# w10-actor-j: spell, skill and actor virtuals (Minion to OniBall)

Package `w10-actor-j`, branch `agents/w10-actor-j`. Private reference: the original
executable `SLUS_205.65` (SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`).
Addresses are that file's symbol values. No game bytes or disassembly listings are committed.
Everything below was compiled with the reviewed EE g++ `authored_cpp` flags. Nothing here has run
on a PS2 or in a game; "paired" means the authored object was run next to the original function
in a private R5900 interpreter (kept outside git).

## How each function was checked

- Paired run (private): compile the unit, link it with every undefined symbol bound to the
  original's address, then run the original and the authored function from identical memory
  snapshots with identical stub callees. Compared: the ordered callee log (argument registers,
  float arguments, stack arguments where the call has more than eight, pointed-to vectors and
  descriptions, and a hash of the object at every call, which catches store-order
  differences), the return value where the original defines one, and every changed byte outside
  the stack. Scenarios use fixed random seeds plus targeted cases for branches the random runs
  did not reach. Instruction coverage of the original was measured; the uncovered words are
  listed per function below as "padding" when they are nops or annulled delay slots.
- Host contract (`host_check.cpp` in each directory, not a build input): restates the expected
  behaviour as stub-callee sequences and field values. Units that read 32-bit pointer words use
  `W10_ACTOR_J_HOST`, which turns those words into offsets from a host base and routes table
  calls through a logging stub. All host contracts compile with the host compiler and return 0.
- Data tables (`_vt$3NPC`, `_vt$13MissileWeapon`) were linked with every slot function bound
  to its image address; the linked bytes equal the image bytes. A host contract checks the slot
  order with stubs.
- Private tooling used: a copy of the R5900 interpreter and paired-run harness described in the
  team notes (outside git). Two harness limits mattered: the image's `strlen`, `strcmp` and
  `memset` use MMI instructions the interpreter does not decode and were hooked in Python where a
  scenario reaches them; and a hook cannot see incidental argument registers, so calls whose
  callee ignores a register (the table call after `notify`, `setState` third argument) log only
  the arguments the callee reads.

## Functions

| directory | symbol | original (address, bytes) | compiled bytes | check |
|---|---|---|---|---|
| `w10-actor-j-get-model-3npc` | `getModel__3NPC` | 0x30fba0, 8 | 8 | 6 paired scenarios; host contract |
| `w10-actor-j-msg-hurt-3npc` | `msg_hurt__3NPCfP10GameObjecti` | 0x233230, 8 | 8 | 6 paired scenarios (no memory change); host contract |
| `w10-actor-j-msg-trigger-6minion` | `msg_trigger__6Minioni` | 0x235338, 120 | 136 | 8 paired scenarios incl. float edge cases; host contract |
| `w10-actor-j-msg-trigger-3npc` | `msg_trigger__3NPCi` | 0x2325b0, 264 | 256 | 135 paired scenarios, 65 of 66 words covered (padding); host contract |
| `w10-actor-j-set-state-3npc` | `setState__3NPCi` | 0x2326b8, 352 | 320 | 300 paired scenarios, 87 of 88 words covered; host contract |
| `w10-actor-j-msg-save-3npc` | `msg_save__3NPCPc` | 0x2331a0, 144 | 108 | 12 paired scenarios; host contract |
| `w10-actor-j-msg-load-3npc` | `msg_load__3NPCPc` | 0x231978, 324 | 276 | 16 paired scenarios; host contract |
| `w10-actor-j-msg-run-3npc` | `msg_run__3NPC` | 0x2320f0, 1212 | 1040 | 250 paired scenarios, 302 of 303 words covered; host contract |
| `w10-actor-j-msg-draw-3npc` | `msg_draw__3NPC` | 0x231ac0, 1584 | 1436 (plus 2 file-local helpers) | 260 paired scenarios, all 396 words covered; host contract |
| `w10-actor-j-clone-3npc` | `clone__3NPC` | 0x122068, 1588 | 816 | 8 paired scenarios incl. null allocation (destination hash at each call); copy ranges measured on the original; host contract |
| `w10-actor-j-dtor-3npc` | `_$_3NPC` | 0x30fae8, 100 | 88 | 24 paired scenarios; host contract |
| `w10-actor-j-vt-3npc` | `_vt$3NPC (data)` | 0x44adb0, 240 | data | linked against image addresses the table equals the image bytes; host slot-order contract |
| `w10-actor-j-init-miniboss-5mummy` | `initMiniboss__5Mummy` | 0x23be40, 56 | 108 | 30 paired scenarios; host contract |
| `w10-actor-j-get-miniboss-name-5mummy` | `getMinibossName__5Mummy` | 0x23be78, 60 | 56 | 10 paired scenarios (image sprintf); host contract |
| `w10-actor-j-msg-save-5mummy` | `msg_save__5MummyPc` | 0x23bd58, 96 | 76 | 40 paired scenarios; host contract |
| `w10-actor-j-msg-load-5mummy` | `msg_load__5MummyPc` | 0x23bdb8, 132 | 124 | 40 paired scenarios; host contract |
| `w10-actor-j-run-spells-5mummy` | `runSpells__5MummyG6Point3` | 0x239988, 256 | 204 | 40 paired scenarios, all 64 words covered; host contract |
| `w10-actor-j-drop-weapon-5mummy` | `dropWeapon__5Mummy` | 0x2398a0, 228 | 172 | 30 paired scenarios, all 57 words covered; host contract |
| `w10-actor-j-msg-draw-5mummy` | `msg_draw__5Mummy` | 0x237658, 304 | 260 | 40 paired scenarios, all 76 words covered; host contract |
| `w10-actor-j-set-state-5mummy` | `setState__5Mummyi` | 0x239008, 2196 | 2188 | 380+ paired scenarios (every state, kind word 0..6 and 9, handled path); 531+16 of 549 words covered (padding); host contract |
| `w10-actor-j-msg-run-5mummy` | `msg_run__5Mummy` | 0x237788, 6268 | 4944 (plus 4 file-local helpers) | 1,346 paired scenarios (random per state plus targeted: kind word 0..6, spell rolls, spawn results, home/teleport); 1,541 of 1,567 words covered, the rest 14 nops and six unreachable dispatch fall-throughs; host contract |
| `w10-actor-j-dtor-5mummy` | `_$_5Mummy` | 0x315c70, 100 | 88 | 24 paired scenarios; host contract |
| `w10-actor-j-tf-5mummy` | `__tf5Mummy` | 0x315cd8, 80 | 92 | 6 paired scenarios (typeinfo node bound to the image object); host contract |
| `w10-actor-j-msg-draw-9mummyking` | `msg_draw__9MummyKing` | 0x23beb8, 108 | 128 | 40 paired scenarios, all 27 words covered; host contract |
| `w10-actor-j-msg-hurt-9mummyking` | `msg_hurt__9MummyKingfP10GameObjecti` | 0x23bf28, 120 | 124 | 40 paired scenarios, all 30 words covered; host contract |
| `w10-actor-j-msg-trigger-9mummyking` | `msg_trigger__9MummyKingi` | 0x23ad30, 348 | 276 | 48 paired scenarios, 86 of 87 words covered; host contract |
| `w10-actor-j-set-state-9mummyking` | `setState__9MummyKingi` | 0x23ae90, 1112 | 1208 | 300 paired scenarios over states -7..20, 274 of 278 words covered (padding); host contract |
| `w10-actor-j-msg-run-9mummyking` | `msg_run__9MummyKing` | 0x239d68, 4040 | 3132 (plus 2 file-local helpers) | 1,028 paired scenarios (random plus targeted); 974+ of 1010 words covered, the rest padding and unreachable loop iterations; host contract |
| `w10-actor-j-dtor-9mummyking` | `_$_9MummyKing` | 0x315f58, 228 | 192 | 24 paired scenarios; host contract (the image's null-object address guards are not reproduced) |
| `w10-actor-j-ctor-7oniball` | `__7OniBallG6Point3iP10GameObjectP10_vagHeader` | 0x21a420, 444 | 684 | 120 paired scenarios, all 111 words covered; host contract |
| `w10-actor-j-vt-13missileweapon` | `_vt$13MissileWeapon (data)` | 0x449e78, 152 | data | linked against image addresses the table equals the image bytes; host slot-order contract |
| `w10-actor-j-set-state-6minion` | `setState__6Minioni` | 0x234e30, 1180 | 1316 | 88 paired scenarios over states -7..23 plus targeted state-3 cases; 289 of 295 words covered (padding); host contract |

`w10-actor-j-common/access.hpp` is a shared header (field accessors, table-entry call helpers,
host switch); it is listed in the header closure of the units that include it.

## How the units differ from a literal reading of the image

- Calls to the soft-float double helpers use the image's own `fptodp`, `dpcmp` and `dpsub`
  (declared with C linkage), not the compiler's libgcc names, so no new libgcc import appears.
  `sqrt.s` is emitted with inline assembly as in the sibling sources.
- Arguments the image passes as full integers but the mangled name types as `short` (the
  heading of `DiseaseBolt` and `ShockOfFrost` constructors in `msg_run__5Mummy`) are declared
  `int` in the authored prototype so the register content matches; the symbol name is kept with an
  assembler label.
- Several compiled bodies are smaller than the image extents because the image was built with an
  older compiler; the sources are behaviour reconstructions, not byte matches.
- The `_$_9MummyKing` image code compares the object address against `-1136` and `-1108` before
  the two array loops (a null-object guard); that comparison is not reproduced.

## Observed, inferred, unknown

Observed: every offset, constant, call order and branch used in the sources comes from the
original function bodies; the jump tables of `setState__5Mummyi` (18 entries plus a 6-entry
sub-table), `setState__9MummyKingi` (18), `setState__6Minioni` (20), `msg_run__5Mummy` (18) and
`msg_run__9MummyKing` (18) were read from the image; the copy ranges of `clone__3NPC` were
measured by running the original on a pattern-filled source; float constants taken from the
global-pointer pool are reproduced by bit pattern where the decimal differs (for example
0x3e4ccccc for 0.2, 0x423fffff just under 48.0, 0x3d4ccccc for the OniBall hit-point factor).

Inferred (from symbol names and shapes only): that `_$_5Mummy`, `_$_3NPC` and `_$_9MummyKing`
are destructors whose flag bit 0 means "free"; that `getMinibossName` formats a name from the
byte at +76; that +960 on Mummy and MummyKing selects a variant ("kind"), with the other fields
used as counters and timers (+256, +804, +808, +1104..+1128); that +576 holds the current
target node and its word +12 the target object; that the skill table entries called through
(word +96)+96 / +104 are roll functions.

Unknown: what the state numbers stand for; what almost every object field means beyond the
offsets; why the Mummy spell choices depend on the kind word; what the flag bits 26 / 27 / 32 /
41 of the 64-bit word at +160 stand for; the real names of the file-local data views
(`mummy_miniboss_name_buffer_7d7e20`, `npc_msg_run_particle_7d7dd0`).

## Imports

Closed (the 32 names of the task): `msg_trigger__6Minioni`, `setState__6Minioni`,
`_vt$13MissileWeapon`, `_$_5Mummy`, `__tf5Mummy`, `dropWeapon__5Mummy`,
`getMinibossName__5Mummy`, `initMiniboss__5Mummy`, `msg_draw__5Mummy`, `msg_load__5MummyPc`,
`msg_run__5Mummy`, `msg_save__5MummyPc`, `runSpells__5MummyG6Point3`, `setState__5Mummyi`,
`_$_9MummyKing`, `msg_draw__9MummyKing`, `msg_hurt__9MummyKingfP10GameObjecti`,
`msg_run__9MummyKing`, `msg_trigger__9MummyKingi`, `setState__9MummyKingi`, `_$_3NPC`,
`_vt$3NPC`, `clone__3NPC`, `getModel__3NPC`, `msg_draw__3NPC`, `msg_hurt__3NPCfP10GameObjecti`,
`msg_load__3NPCPc`, `msg_run__3NPC`, `msg_save__3NPCPc`, `msg_trigger__3NPCi`, `setState__3NPCi`,
`__7OniBallG6Point3iP10GameObjectP10_vagHeader`.

Duplicate to resolve: `_vt$3NPC` is also authored as `norrath_ctors_lava_wraith_view_44adb0`
(package ctors-lava-wraith, selected earlier), whose NPC constructor stores that view's address.
The table in this package uses the real symbol name and was compared with the image the same
way. A build must keep only one of the two (or alias them); otherwise the NPC constructor and the
NPC destructor/clone store different table addresses.

Opened by this package (names the units reference that are neither defined nor already
unresolved at the shared tip `cc78a45e`; computed from the baseline link of that tip and the
undefined symbols of the compiled units, 51 names; the two file-local views are my own
invention of a name for an unnamed bss block):

`ISeeDeadPeople__8Creaturef`, `RandomTeleportSpell__FP10GameObjectP6Point3f`, `_$_13MissileWeapon`, `__10MummySnakeG6Point3sP10GameObjectT3P15AnimationHeader`, `__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader`, `__10SpawnMummyG6Point3PcP10GameObject`, `__11DiseaseBoltsiG6Point3P10GameObjectP5Skill`, `__12ShockOfFrostG6Point3siP10GameObjectP5Skill`, `__15FrostStormSpellG6Point3ifiP10GameObjectT1`, `__15SpellConeOfFireiP8CreatureP5Skill`, `__18SpellDiseaseShieldiP8CreatureP5Skill`, `__8ListHeadRC8ListHead`, `__9DustDevilG6Point3`, `__9FireStormiP10GameObjectG6Point3P5Skill`, `__tf13MissileWeapon`, `_vt$10GameObject`, `_vt$5Mummy`, `_vt$7OniBall`, `_vt$8Creature`, `_vt$9MummyKing`, `alternateEnemyInZone__8Creatureffss`, `animRandomizeTime__FP14AnimationState`, `bSpawnMummy__9MummyKing`, `bSpawnSnake__9MummyKing`, `clone__13MissileWeapon`, `creatureFindLeadYaw__8CreatureP10GameObjectf`, `creatureSlamEffect__8CreatureP8Creaturei`, `enemyInZone2__8Creatureffssfi`, `enemyInZone__8Creaturefs`, `findClosestShadowBall__13MissileWeaponP10GameObject`, `findEnemyLeadPosition__8CreaturefP6Point3`, `fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34`, `fxWeaponToDust__FP12_modelHeaderT0P14AnimationStateG6Point3sffi`, `fxWeaponToSparkles__FP12_modelHeaderT0P14AnimationStateG6Point3sffi`, `initType__5Mummyb`, `modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii`, `modelSetRelightAttachment__FG6Point3`, `msg_collision__13MissileWeaponP10GameObject`, `msg_draw__13MissileWeapon`, `msg_run__13MissileWeapon`, `mummy_miniboss_name_buffer_7d7e20`, `npc_msg_run_particle_7d7dd0`, `numInRegion__8creatureG6Point3f`, `objectIsCrowded__8CreatureP10GameObjectffi`, `playerSelectionGetNPCActiveChunks__FP3NPC`, `playerSelectionNPCDraw__FP3NPCP12_modelHeaderP11_drawRecord`, `playerSelectionNPCVisible__FP3NPC`, `produceGeneralDamage__8CreatureffssiiiiPQ24item11description`, `produceMeleeDamage__8CreaturefiiiiPQ24item11description`, `steerAndRouteToEnemy__8Creature`, `turnToFaceEnemy__8Creature`.

Already unresolved at the tip and still imported here (12 names): `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`, `__10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap`, `__15AnimationState8RC15AnimationState8`, `__tf15SpellConeOfFire`, `blockFree__FPv`, `bossMeterName`, `dropTreasure__8CreatureG6Point3iffffffPc`, `ef1Noise__Fi`, `fxCreatureToDust__FP12_modelHeaderP14AnimationStateG6Point3sff`, `playedStinger`, `skills`, `terminateSpell__8CreatureRC9type_info`.

The other external names the units reference (111) are defined by the shared tip. The units do not
redefine any symbol that the tip defines.

Link check (private, not a full rebuild): the shared tip's partial object from a clean build of
`cc78a45e` (1,332 unresolved names) was relocatably linked with the 32 compiled units using
the driver's own link flags. The link reports no multiple definitions, and the unresolved count
becomes 1,351 (the 32 scope names close, 51 names open). `build_partial.py --check-plan` accepts a
private plan and snapshot copy that contain the entries below. `tools/check_source_snapshot.py`
rejects the new directories until the integrator records them, as with the other packages.

Notes on the opened names: `_vt$5Mummy`, `_vt$9MummyKing`, `_vt$7OniBall`, `_vt$8Creature` and
`_vt$10GameObject` are table data referenced by the destructors, the clone and the OniBall
constructor; the spell, projectile and effect constructors (`__9FireStormi...`, `__6Sparks...`,
`__15SpellConeOfFirei...`, `__10MummySnake...`, `__10SpawnMummy...`, `__9DustDevil...`) and the
Creature/Mummy helpers (`enemyInZone`, `produceGeneralDamage`, `initType__5Mummyb`, `bSpawnMummy`,
...) belong to the spell, Creature and MummyKing packages; the MissileWeapon methods
(`msg_run__13MissileWeapon`, `clone__13MissileWeapon`, ...) appear only as slots of
`_vt$13MissileWeapon`.

## Build-plan entries needed

Append these authored inputs (one per directory, 32 entries) with fresh `input_order` values
after the current tip; `default_authored_inputs` grows by 32. `host_check.cpp` files are not
inputs. Entries carry the `access.hpp` header where the source includes it. The two data units
emit no functions. Body sizes are from the reviewed EE g++ build of these exact files and were
not compared with the original executable. Regenerate with
`python3 docs/agents/w10-actor-j-plan-entries.py --toolchain /path/to/ee` (add
`--private-root DIR` to write a private plan and snapshot copy that contain them).

```json
{"emitted_functions": [{"compiled_body_bytes": 816, "name": "clone__3NPC"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "clone.o", "profile": "authored_cpp", "sha256": "a3a6b99b74ae2a28b4baec87b4ae74ff1ecc0f8cfa5df995a2de3b808944d3be", "source": "sources/w10-actor-j-clone-3npc/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 684, "name": "__7OniBallG6Point3iP10GameObjectP10_vagHeader"}], "header_closure": ["sources/w10-actor-j-common/access.hpp"], "kind": "independently_authored_source", "object_name": "oniball.o", "profile": "authored_cpp", "sha256": "4277ccc0a3a150cf5152a9acd8863b1e0bf2e1a17daf93cb0387ce7875972dcf", "source": "sources/w10-actor-j-ctor-7oniball/oniball.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 172, "name": "dropWeapon__5Mummy"}], "header_closure": ["sources/w10-actor-j-common/access.hpp"], "kind": "independently_authored_source", "object_name": "drop_weapon.o", "profile": "authored_cpp", "sha256": "06a5980eaa5f93b57aca3f650930d27452b5bf839ce03f790006871b01f51c6d", "source": "sources/w10-actor-j-drop-weapon-5mummy/drop_weapon.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "_$_3NPC"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "dtor.o", "profile": "authored_cpp", "sha256": "9d35293298aeedf2554b2ce62e180c271fdd5a3e69fef63f8ef4b4221b083660", "source": "sources/w10-actor-j-dtor-3npc/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "_$_5Mummy"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "dtor.o", "profile": "authored_cpp", "sha256": "a43066e2e3a7fbb6d9c12bbaba0c06162bbaaa4f7243b069ea623569301fa1d4", "source": "sources/w10-actor-j-dtor-5mummy/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 192, "name": "_$_9MummyKing"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "dtor.o", "profile": "authored_cpp", "sha256": "ac26123788eb1dbb36a783f84688f283f8fc5f91e6a72cafd78241410526dec1", "source": "sources/w10-actor-j-dtor-9mummyking/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 56, "name": "getMinibossName__5Mummy"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_miniboss_name.o", "profile": "authored_cpp", "sha256": "eed99db0d69724d640cc6a73d65694312b1426c3c738fd68370600da5d646d65", "source": "sources/w10-actor-j-get-miniboss-name-5mummy/get_miniboss_name.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "getModel__3NPC"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "get_model.o", "profile": "authored_cpp", "sha256": "9d79c16b6f269f4f8797bf33f55a320f9b7e099ae097f0e1f26461334c5a2ade", "source": "sources/w10-actor-j-get-model-3npc/get_model.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 108, "name": "initMiniboss__5Mummy"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "init_miniboss.o", "profile": "authored_cpp", "sha256": "68d8c0a52c1b9be02b2e0c0fc29ed448dfd3ce68a098996809ff151c2dfb3b18", "source": "sources/w10-actor-j-init-miniboss-5mummy/init_miniboss.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 156, "name": "_ZN12_GLOBAL__N_1L6reloadEPhRPvS2_"}, {"compiled_body_bytes": 136, "name": "_ZN12_GLOBAL__N_1L15draw_attachmentEPhPKcS2_PvS3_"}, {"compiled_body_bytes": 1436, "name": "msg_draw__3NPC"}], "header_closure": ["sources/w10-actor-j-common/access.hpp"], "kind": "independently_authored_source", "object_name": "msg_draw.o", "profile": "authored_cpp", "sha256": "42393d66e296875b0cbdd420263923f8557181ba34e1b1c37da29c51f40e58bf", "source": "sources/w10-actor-j-msg-draw-3npc/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 260, "name": "msg_draw__5Mummy"}], "header_closure": ["sources/w10-actor-j-common/access.hpp"], "kind": "independently_authored_source", "object_name": "msg_draw.o", "profile": "authored_cpp", "sha256": "75bfebd9b7bf742e3bfa3d196cf1d0d32c4422d5e7a35eb57f9c5aa586a3e182", "source": "sources/w10-actor-j-msg-draw-5mummy/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 128, "name": "msg_draw__9MummyKing"}], "header_closure": ["sources/w10-actor-j-common/access.hpp"], "kind": "independently_authored_source", "object_name": "msg_draw.o", "profile": "authored_cpp", "sha256": "2fdfde6c9deb412e78e81c89b56779c57b04e4f642a6a4f46b2a923ae58d8409", "source": "sources/w10-actor-j-msg-draw-9mummyking/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "msg_hurt__3NPCfP10GameObjecti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "msg_hurt.o", "profile": "authored_cpp", "sha256": "f32cbb71ab23f2b8de38b3daede34ab4a7f0677b1d342c2978967c021f059b61", "source": "sources/w10-actor-j-msg-hurt-3npc/msg_hurt.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 124, "name": "msg_hurt__9MummyKingfP10GameObjecti"}], "header_closure": ["sources/w10-actor-j-common/access.hpp"], "kind": "independently_authored_source", "object_name": "msg_hurt.o", "profile": "authored_cpp", "sha256": "3b0c5ccca1574df613b85c603ff007f2bb1c7b4effb09f21bd371f82f4f1f907", "source": "sources/w10-actor-j-msg-hurt-9mummyking/msg_hurt.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 276, "name": "msg_load__3NPCPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "msg_load.o", "profile": "authored_cpp", "sha256": "69f2674791adeff7793ca000c3c88be76c32998a7934be1feda1a4fef2c12ab7", "source": "sources/w10-actor-j-msg-load-3npc/msg_load.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 124, "name": "msg_load__5MummyPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "msg_load.o", "profile": "authored_cpp", "sha256": "1fe4edc91fe8b555818018d3990a944fac2eba233d6e5039e75bb1deb9bc8b3d", "source": "sources/w10-actor-j-msg-load-5mummy/msg_load.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1040, "name": "msg_run__3NPC"}], "header_closure": ["sources/w10-actor-j-common/access.hpp"], "kind": "independently_authored_source", "object_name": "msg_run.o", "profile": "authored_cpp", "sha256": "9bee74c035117bda4398ac647b360af9f06cb1f2b3f7d1e89c115fef084afd17", "source": "sources/w10-actor-j-msg-run-3npc/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 64, "name": "_ZN12_GLOBAL__N_1L11skill_valueEPhji"}, {"compiled_body_bytes": 144, "name": "_ZN12_GLOBAL__N_1L15cast_fire_stormEPhRK6Point3"}, {"compiled_body_bytes": 28, "name": "_ZN4w10j10call_entryEPhjm.isra.0"}, {"compiled_body_bytes": 564, "name": "_ZN12_GLOBAL__N_1L11cast_sparksEPh"}, {"compiled_body_bytes": 4944, "name": "msg_run__5Mummy"}], "header_closure": ["sources/w10-actor-j-common/access.hpp"], "kind": "independently_authored_source", "object_name": "msg_run.o", "profile": "authored_cpp", "sha256": "204995804852169d1a27b4e0e20053ccd94ca0868c97c6b3c8bab630c8eb7c49", "source": "sources/w10-actor-j-msg-run-5mummy/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 332, "name": "_ZN12_GLOBAL__N_1L16roll_descriptionEPhRNS_11DescriptionEjs"}, {"compiled_body_bytes": 272, "name": "_ZN12_GLOBAL__N_1L11spawn_mummyEPhjPKc"}, {"compiled_body_bytes": 3132, "name": "msg_run__9MummyKing"}], "header_closure": ["sources/w10-actor-j-common/access.hpp"], "kind": "independently_authored_source", "object_name": "msg_run.o", "profile": "authored_cpp", "sha256": "4603d025aa3eb950b08c0a53bbbcd9326fc6991413c63e239a8a9d0738cf86b3", "source": "sources/w10-actor-j-msg-run-9mummyking/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 108, "name": "msg_save__3NPCPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "msg_save.o", "profile": "authored_cpp", "sha256": "1103898984b4e3edc382dc02b3dc0a778c2928136ace251452458f65c5b115f0", "source": "sources/w10-actor-j-msg-save-3npc/msg_save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 76, "name": "msg_save__5MummyPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "msg_save.o", "profile": "authored_cpp", "sha256": "d0ca9469c03b2817803be8d0731cb715d0a289ee863920179a27e75752a294e6", "source": "sources/w10-actor-j-msg-save-5mummy/msg_save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 256, "name": "msg_trigger__3NPCi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "msg_trigger.o", "profile": "authored_cpp", "sha256": "bfa90960a38e38a04a8282bdb77e806750a2dfccc2bdcaff48dbc278adeca189", "source": "sources/w10-actor-j-msg-trigger-3npc/msg_trigger.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "msg_trigger__6Minioni"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "msg_trigger.o", "profile": "authored_cpp", "sha256": "8cb6cf6b9872cfdae460e8ed20d2f745300afe247ad42a1d30ee3a00af16f4d3", "source": "sources/w10-actor-j-msg-trigger-6minion/msg_trigger.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 276, "name": "msg_trigger__9MummyKingi"}], "header_closure": ["sources/w10-actor-j-common/access.hpp"], "kind": "independently_authored_source", "object_name": "msg_trigger.o", "profile": "authored_cpp", "sha256": "4fc9d192f8272c7e6429f5c02ad836d412d6498b54ad7e457a1009e1bbc8d2ea", "source": "sources/w10-actor-j-msg-trigger-9mummyking/msg_trigger.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 204, "name": "runSpells__5MummyG6Point3"}], "header_closure": ["sources/w10-actor-j-common/access.hpp"], "kind": "independently_authored_source", "object_name": "run_spells.o", "profile": "authored_cpp", "sha256": "863d0a0fad0724a5041b4a3334535dd59598031d40c5512b713be6921b49933b", "source": "sources/w10-actor-j-run-spells-5mummy/run_spells.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 320, "name": "setState__3NPCi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "set_state.o", "profile": "authored_cpp", "sha256": "155fa1b5c3114d23f61a9fb10ce0246fa85b40086ffbee021dff01de4a5f0e11", "source": "sources/w10-actor-j-set-state-3npc/set_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 2188, "name": "setState__5Mummyi"}], "header_closure": ["sources/w10-actor-j-common/access.hpp"], "kind": "independently_authored_source", "object_name": "set_state.o", "profile": "authored_cpp", "sha256": "b2f2bd78ac07e02f45e956e65ee452a263a2b50d2f2e06aa9b503c2f14c53c93", "source": "sources/w10-actor-j-set-state-5mummy/set_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1316, "name": "setState__6Minioni"}], "header_closure": ["sources/w10-actor-j-common/access.hpp"], "kind": "independently_authored_source", "object_name": "set_state.o", "profile": "authored_cpp", "sha256": "de29c3f0d3372fce986cf0a7931d881160deff2b676004c498f44b62abfe2bf5", "source": "sources/w10-actor-j-set-state-6minion/set_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1208, "name": "setState__9MummyKingi"}], "header_closure": ["sources/w10-actor-j-common/access.hpp"], "kind": "independently_authored_source", "object_name": "set_state.o", "profile": "authored_cpp", "sha256": "a9177a403fbd0ece3c4961787fe009a3da311bed90843441ce7fa681b50159c5", "source": "sources/w10-actor-j-set-state-9mummyking/set_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf5Mummy"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "typeinfo.o", "profile": "authored_cpp", "sha256": "edcb26cb7eec4bdf5c552963976cd791f625fca12dba480601546bec01c3acd1", "source": "sources/w10-actor-j-tf-5mummy/typeinfo.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "vt_13missileweapon.o", "profile": "authored_cpp", "sha256": "c0e6858b969279b1ce369fc40d4de6ac019aa0b252013267ff8980575134045c", "source": "sources/w10-actor-j-vt-13missileweapon/vt_13missileweapon.cpp"}
{"emitted_functions": [], "header_closure": [], "kind": "independently_authored_source", "object_name": "vt_3npc.o", "profile": "authored_cpp", "sha256": "adee682d0aa9ab1f6d3d372e7da8de0acc5aefb887e6044b3ebb6fc80b008823", "source": "sources/w10-actor-j-vt-3npc/vt_3npc.cpp"}
```

## Authored but never executed

Nothing here has run on an EE, in the game, or against a real object. Everything in the paired
runs used stub callees; the real callees (`setStateProcessing__8Creature`, `enemyInZone`, the
spell constructors, `blockAlloc__Fi`, the animation and sound routines) were never run with
these bodies. The table calls (`(word +72)+168/176/184/192`) were exercised only against fake
tables. The two data tables were only compared as linked bytes.

## What remains unknown or blocked

See "Observed, inferred, unknown" above. Blockers for the lead integrator: the duplicate
`_vt$3NPC` view; the module-local data names `mummy_miniboss_name_buffer_7d7e20` (the
`buffer.1071` bss block) and `npc_msg_run_particle_7d7dd0` (the bss block named `kingParticle`
in the image) need a storage owner and a symbol mapping, because the image gives them local
names; the `fptodp`/`dpcmp`/`dpsub` imports need the soft-float package to define them.
