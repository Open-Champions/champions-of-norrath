# w10-actor-k: spell, skill and actor virtuals (Orc to RollDoor)

Branch `agents/w10-actor-k`. Private reference: `work/private/SLUS_205.65`, SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0` (checked with `shasum -a 256` for
this run), global pointer `0x004879f0`. Addresses and sizes below are the image's own symbol table.
No game bytes, disassembly or decompiler output are committed; strings quoted in sources are asset
and resource names the code passes to other routines.

## Method and checks performed

- Each function was read privately from a disassembly, rewritten as readable C++ and compiled alone
  with the reviewed `authored_cpp` flags (EE GCC 15.2.0, `-G0 -O2 ... -march=r5900 -std=c++17`). All 32
  units compile.
- Every unit has a `host_check.cpp` built with the host C++ compiler (exit status 0). Units that keep
  addresses in game words use `W10_ACTOR_K_HOST` (see `w10-actor-k-common/access.hpp`), so a stored address
  is an offset in a test arena, and collaborators are recorded stubs. The vtable units compare every
  slot against a stub of the observed name. Static storage and call-order contracts are checked too.
- Private paired runs (outside git): a small R5900 interpreter loaded the original function and my freshly
  compiled one from the same memory snapshot with identical Python stand-ins for every callee, then
  compared the call log (argument registers, float registers, stack argument words, the bytes behind
  pointer arguments, memory digests), the return value where it matters, and every changed byte outside
  the stack. Scenario counts and instruction coverage are in the table. The few original words never
  executed are nops in alignment slots (checked by disassembly), except where stated.
  Findings from these runs that changed the source: the Orc clone's fifth loop iteration (bytes 104 to
  107), the Orc msg_run floor effect using the first roll for the heading, the hidden result pointer of
  getSmartRandomDestination, the end-of-state copy in Orc msg_run state 3, and the float constant 0xbdaaaaaa
  in dismount.
- The vtable units were compared by linking my object with every named slot bound to the original's
  address and comparing all words of the table with the original image bytes.
- Reviewed partial build: I built only my 32 units with `tools/build_partial.py` using a private plan and
  snapshot kept outside git (`/Users/case/norrath-agents/work-w10k-plan`), the reviewed toolchain and the
  driver's flags. Result: `partial_build_complete`, 32 source compilations, one relocatable link, 31 function
  bodies, 166 unresolved symbols (all other packages' or the lead's). I did not run the full shared
  plan (it is not mine to edit).
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.

Observed (from the image): addresses, sizes, instruction-level behaviour and the constants and strings the
code uses. Inferred: class names and the roles of callee names come from the executable's own symbol names;
field offsets are byte offsets whose meaning is not established. Unknown: what the fields mean, and the
bodies of every callee named below.

## Functions and tables

Compiled sizes differ from image extents (different compiler); these are behaviour reconstructions, not
byte matches. No claim of an original-byte match is made.

| source | symbol | image | compiled bytes | sha256 | checks |
|---|---|---|---|---|---|
| `w10-actor-k-clone-11pushtrigger/clone.cpp` | `clone__11PushTrigger` | `0x00116288`, 256 bytes | 232 | `564a0bf1d22044aa67f03fa622c13a067ed8cd346f73fc4b1f6405fa66a550ee` | 10 scenarios |
| `w10-actor-k-clone-3orc/clone.cpp` | `clone__3Orc` | `0x0011cef8`, 1644 bytes | 868 | `7d8000cc04e4af993561979479325bc0aade3f97ebf7ac2361d5faaaaa030d7f` | 10 scenarios |
| `w10-actor-k-clone-8rolldoor/clone.cpp` | `clone__8RollDoor` | `0x001277b8`, 304 bytes | 272 | `ab54fe531f9addb4e8cd26af0e837cdb32337e239f30cf460dcd478047e5c4b3` | 10 scenarios |
| `w10-actor-k-ctor-10projectile/ctor.cpp` | `__10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap` | `0x00153fd0`, 156 bytes | 152 | `8038363cee4e2dd80ad58d74c9428a74f425a9ec87fac31fce89bce86ccef4b3` | 12 scenarios |
| `w10-actor-k-ctor-4prop-three-names/ctor.cpp` | `__4PropG6Point3PcN22li` | `0x001476b8`, 504 bytes | 524 | `2986b91aaac01ff6f4136f3aabd942997dc8b809691a11fcd80837135fac6624` | 60 scenarios, 126/126 words |
| `w10-actor-k-dismount-6player/dismount.cpp` | `dismount__6Player` | `0x001aba50`, 64 bytes | 72 | `e7458874fe690e446698a3be1087a6ec5127b0fd129833d766ab99bab6797eb0` | 11 scenarios |
| `w10-actor-k-dtor-11pushtrigger/dtor.cpp` | `_$_11PushTrigger` | `0x0030e710`, 100 bytes | 88 | `1a08ad503e238c64fc6765617833b84dd874c5d00629bf3c240fdb525c6661bc` | 8 flag values |
| `w10-actor-k-dtor-19particlemorpheffect/dtor.cpp` | `_$_19ParticleMorphEffect` | `0x003130e8`, 100 bytes | 88 | `e858897b39bb6aa1aec0cfe210b10c084e45dab582b960733c024a40e91f38a9` | 8 flag values |
| `w10-actor-k-dtor-3orc/dtor.cpp` | `_$_3Orc` | `0x0030f398`, 108 bytes | 112 | `73456e7c4a84998cf2b16876fd69110f679348148a0c01754f38452d7f0441ad` | 8 flag values |
| `w10-actor-k-dtor-8rolldoor/dtor.cpp` | `_$_8RollDoor` | `0x003119a0`, 100 bytes | 88 | `2bd257114c3b6e4908af88a1fb2f4c9a27eb0b0a7f02846a2ca65b1d3368c561` | 8 flag values |
| `w10-actor-k-get-miniboss-name-3orc/get_miniboss_name.cpp` | `getMinibossName__3Orc` | `0x0022b998`, 60 bytes | 56 | `92ff0efe4ac1b884683ccb6b2c893c4ab5d76153a2f35d96bb0333c4c0a13180` | 8 scenarios |
| `w10-actor-k-global-init-move-10projectile/static_init.cpp` | `_GLOBAL_$I$move__10ProjectileG6Point3` | `0x00155fa0`, 32 bytes | 1448 | `fbf175a8473b144dc4cfa2fd992adbda674816ae4dec8e52bdb35b5cf146e0fd` | 1 run, all 19 construction calls compared |
| `w10-actor-k-init-miniboss-3orc/init_miniboss.cpp` | `initMiniboss__3Orc` | `0x0022b960`, 56 bytes | 108 | `9f7353c1dbe07f365e10587b4c295f3e8a069190a12c5bd9fc2630d4108c6910` | 8 scenarios (edge rolls) |
| `w10-actor-k-msg-alert-3orc/msg_alert.cpp` | `msg_alert__3OrcifG6Point3P10GameObject` | `0x0022b570`, 328 bytes | 352 | `51afe613f74e4728ccc8d019d5610135c0d63c4c20779dab7744d4bb12f9b984` | 600 scenarios, 82/82 words |
| `w10-actor-k-msg-draw-3orc/msg_draw.cpp` | `msg_draw__3Orc` | `0x00227638`, 416 bytes | 464 | `6fe8e6384a6eff7ecec636439bb185ccc542c04d347a2ec1e3c8cc6f2b19b588` | 200 scenarios, 104/104 words |
| `w10-actor-k-msg-draw-8rolldoor/msg_draw.cpp` | `msg_draw__8RollDoor` | `0x0021c690`, 212 bytes | 148 | `8c5e14c9c03302af37a2c726bc4a7d5caf0ff5ec74f86a2f9dd896459975a44c` | 60 scenarios, 53/53 words |
| `w10-actor-k-msg-get-poke-name-11pushtrigger/msg_get_poke_name.cpp` | `msg_getPokeName__11PushTriggerPUsi` | `0x001d1850`, 268 bytes | 340 | `84c731dc92c501836253b2a1363d23176fdde3315a2baae106a035c2fa1fc040` | 300 scenarios, 64/67 words (rest are nops) |
| `w10-actor-k-msg-get-poke-name-8rolldoor/msg_get_poke_name.cpp` | `msg_getPokeName__8RollDoorPUsi` | `0x0021c4a0`, 60 bytes | 60 | `3c5ad1f01ba023b83a0eab84371b86e47170efeaf2dc7e45fd2a732491a7244c` | 2 scenarios |
| `w10-actor-k-msg-hurt-3orc/msg_hurt.cpp` | `msg_hurt__3OrcfP10GameObjecti` | `0x0022b550`, 28 bytes | 8 | `e76efac3cb91e188911069571ff9f4989e87cfe63f0342d3b6d5ae7ee5b43786` | 4 scenarios |
| `w10-actor-k-msg-load-3orc/msg_load.cpp` | `msg_load__3OrcPc` | `0x0022b728`, 144 bytes | 132 | `942e12bcd9c1bfb9e9a3f58e276cb918971daea4c25e730d085ff307a651cb44` | 8 scenarios |
| `w10-actor-k-msg-load-8rolldoor/msg_load.cpp` | `msg_load__8RollDoorPc` | `0x0021c5a0`, 180 bytes | 172 | `8c3ace750ebb859558d3a38d814e9f11d5bf78f79dd4c007ce7ed683955b334c` | 90 scenarios, 45/45 words |
| `w10-actor-k-msg-poke-11pushtrigger/msg_poke.cpp` | `msg_poke__11PushTriggeri` | `0x001d1960`, 148 bytes | 160 | `8cde6d640e9ebaff8e6b763521fe3f5ccb786b7cab739432c2aea49580a1cff7` | 80 scenarios, 37/37 words |
| `w10-actor-k-msg-run-19particlemorpheffect/msg_run.cpp` | `msg_run__19ParticleMorphEffect` | `0x00152e30`, 1952 bytes | 1544 | `24214311197213a336a39763a3a713ea8f506ff668df7119e3c4bb1be6f694ca` | 400 scenarios, 487/488 words (the other is a nop) |
| `w10-actor-k-msg-run-3orc/msg_run.cpp` | `msg_run__3Orc` | `0x002277d8`, 5068 bytes | 4860 | `36c3100dbbdca7ee25757be99fa9492477dd93e9a9d470f3dc9c1489c78505e8` | 3,000 scenarios, 1,246/1,267 words (rest are nops) |
| `w10-actor-k-msg-save-3orc/msg_save.cpp` | `msg_save__3OrcPc` | `0x0022b6b8`, 108 bytes | 84 | `452dfc9b06a2a591843b6696d50315f305310733e48d05f65e818d366abb9d16` | 8 scenarios |
| `w10-actor-k-set-state-3orc/set_state.cpp` | `setState__3Orci` | `0x00228ba8`, 3244 bytes | 3904 | `404f80a3d576e903e7fddc29e652bb5fd7074e924b767204e15543670b56124f` | 2,200 scenarios, 794/811 words (rest are nops) |
| `w10-actor-k-tf-19particlemorpheffect/typeinfo.cpp` | `__tf19ParticleMorphEffect` | `0x00313150`, 76 bytes | 92 | `c2b935e1413de69182f87226f4ddc140ace85e1ec89440a63162e7a8e01966f7` | 2 node states |
| `w10-actor-k-tf-8rolldoor/typeinfo.cpp` | `__tf8RollDoor` | `0x00311a08`, 132 bytes | 184 | `889b231fb8158edef1741cbaf9149e56df515cdac33bf3656f17bfa274724247` | 4 node states |
| `w10-actor-k-vtable-10projectile/vtable.cpp` | (data only) | | | `79ba9c2563caffa37a1632f49db0e54c128acf40f518c12f3048faacf5f32476` | word-for-word compare of the 144 bytes with the original |
| `w10-actor-k-vtable-12recalleffect/vtable.cpp` | (data only) | | | `5c018d2efa51708e52e504897329c1758557bb0832df9aae4d84909206d5a982` | word-for-word compare of the 144 bytes |
| `w10-actor-k-vtable-6phprop/vtable.cpp` | (data only) | | | `a54ad79ba8ca4d92c8412c170ccba29d3e671f5b0bc09face7eaabd2e9871981` | word-for-word compare of the 144 bytes |
| `w10-actor-k-vtable-6player/vtable.cpp` | (data only) | | | `7ad11dc7f04682096acc94209f6ef4ba9a9c6381b48548bd0311ae30a552970d` | word-for-word compare of the 240 bytes |

Notes on specific units:

- `w10-actor-k-common/access.hpp` is a header (not an EE input by itself). It holds the byte-offset
  accessors and the table-call helper (entry = halfword adjustment, padding, function word at the word at
  object+72 plus an offset). Units that include it list it in `header_closure`.
- The Orc `setState`, `msg_run`, `clone`, `msg_draw`, `msg_alert`, `msg_save`, `msg_load` and
  `msg_hurt` reference `initType__3Orcb`, `setWeapon__3Orc`, commander helpers, the Creature methods and
  animation calls by their observed names; none is defined here.
- `w10-actor-k-global-init-move-10projectile` holds the Projectile source unit's static initialiser
  (a file-local function in the image, inlined into the `_GLOBAL_$I$` wrapper) and the storage of the
  19 objects it constructs: fifteen 80-byte particle definitions and four 16-byte resource maps.
  `morpheffectParticle1` and `morpheffectParticle2` are global here because `msg_run__19ParticleMorphEffect`
  names them; the other particle objects are file-local like the original; `goldAnm`, `goldModel` and
  `goldTex` are defined by other units and only referenced; `goldRestModel` is file-local here.
- `__ti8RollDoor` and `__ti19ParticleMorphEffect` (12-byte type nodes, zero-initialised storage) are
  defined by the two type-info units, following the existing DoorSwing unit.
- The Prop constructor is the overload with the three text arguments; its sixth argument is a 64-bit
  value (inferred from the 64-bit store and whole-register pass in the original; the authored build declares it `long long`).

## Imports closed (names)

All 32 names in the package scope are defined, each by the unit named in the table:

`_$_3Orc`, `clone__3Orc`, `getMinibossName__3Orc`, `initMiniboss__3Orc`, `msg_alert__3OrcifG6Point3P10GameObject`, `msg_draw__3Orc`, `msg_hurt__3OrcfP10GameObjecti`, `msg_load__3OrcPc`, `msg_run__3Orc`, `msg_save__3OrcPc`, `setState__3Orci`, `_$_19ParticleMorphEffect`, `__tf19ParticleMorphEffect`, `msg_run__19ParticleMorphEffect`, `_vt$6PhProp`, `_vt$6Player`, `dismount__6Player`, `_GLOBAL_$I$move__10ProjectileG6Point3`, `__10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap`, `_vt$10Projectile`, `__4PropG6Point3PcN22li`, `_$_11PushTrigger`, `clone__11PushTrigger`, `msg_getPokeName__11PushTriggerPUsi`, `msg_poke__11PushTriggeri`, `_vt$12RecallEffect`, `_$_8RollDoor`, `__tf8RollDoor`, `clone__8RollDoor`, `msg_draw__8RollDoor`, `msg_getPokeName__8RollDoorPUsi`, `msg_load__8RollDoorPc`.

Also defined (not in the scope list): `__ti19ParticleMorphEffect`, `__ti8RollDoor`, `morpheffectParticle1`, `morpheffectParticle2`. Of these, none was an unresolved import of the baseline build.

## Imports opened

These come from the 166 unresolved symbols of the link of only my 32 objects, i.e. names my sources reference that none of my units defines. Ten were already unresolved in the lead's baseline build and simply stay open: `__10GameObjectG6Point3PQ210GameObject11resourceMapPPc`, `__15AnimationState8RC15AnimationState8`, `__ti10GameObject`, `blockFree__FPv`, `dropTreasure__8CreatureG6Point3iffffffPc`, `findTagInt__10GameObjectPci`, `floorAdd__FG6Point3sfii`, `goldAnm`, `modelGetBoundingBox__FP12_modelHeaderlPiN52`, `skills`.

The following 69 are referenced by my sources and appear neither in the baseline unresolved list nor as a function defined in the baseline inventories (the baseline is the lead's earlier integration, so other wave-10 packages or data views may already define some of them; I did not check): `ISeeDeadPeople__8Creaturef`, `_$_10Projectile`, `_$_12RecallEffect`, `_$_6PhProp`, `_$_6Player`, `_4item$globalItem`, `__10PoisonRainiP10GameObjectG6Point3P5SkillP15AnimationHeader`, `__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill`, `__19FireArrowProjectileG6Point3sP10GameObjectiP5Skill`, `__22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill`, `__8ListHeadRC8ListHead`, `__tf10Projectile`, `__tf12RecallEffect`, `__tf6Player`, `__ti9WorldPart`, `_vt$10GameObject`, `_vt$11PushTrigger`, `_vt$19ParticleMorphEffect`, `_vt$3Orc`, `_vt$4Prop`, `_vt$8Creature`, `_vt$8RollDoor`, `clone__10Projectile`, `clone__12RecallEffect`, `clone__6PhProp`, `clone__6Player`, `commanderValid__3Orc`, `deleteCommander__3Orc`, `dropItem__3Orci`, `enemyInZone2__8Creatureffssfi`, `enemyInZone__8Creaturefs`, `findCommander__3Orc`, `findEnemyLeadPosition__8CreaturefP6Point3`, `flee2__8CreatureP10GameObject`, `frameNm`, `fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34`, `g_BloodLevel`, `getModel__6Player`, `getSkillRank__6Playeri`, `goldModel`, `goldTex`, `initType__3Orcb`, `init__4Propi`, `isEnemy__6PlayerP10GameObject`, `joinCommander__3Orc`, `leaveCommander__3Orc`, `modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii`, `msg_collision__6PlayerP10GameObject`, `msg_draw__12RecallEffect`, `msg_draw__6PhProp`, `msg_draw__6Player`, `msg_getPokeName__6PhPropPUsi`, `msg_hurt__6PlayerfP10GameObjecti`, `msg_poke__6PhPropi`, `msg_run__12RecallEffect`, `msg_run__6PhProp`, `msg_run__6Player`, `netNetworkMode`, `numInRegion__8creatureG6Point3f`, `objectIsCrowded__8CreatureP10GameObjectffi`, `produceMeleeDamage__8CreaturefiiiiPQ24item11description`, `setActiveChunks__6Player`, `setState__6Playeri`, `setWeapon__3Orc`, `steerAndRouteToEnemy__8Creature`, `stun__6Playeri`, `turnToFaceEnemy__8Creature`, `world`, `worldGetDoorHeight__FP12_worldHeaderi`.

## Build-plan entries needed

Profile `authored_cpp`; no `host_check.cpp` is an input. `emitted_functions` and `compiled_body_bytes` come from the reviewed-flag compile. Add `sources/w10-actor-k-common/access.hpp` to the plan `files` list (sha256 `08c977e5fd1a32a7359c95e14f6aee0ab56f4a208d3cb8802d5c754f25ed0bc6`). `input_order` values are for the lead to assign; `object_name`s are unique.

```json
{"emitted_functions":[{"compiled_body_bytes":232,"name":"clone__11PushTrigger"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone_11pushtrigger.o","profile":"authored_cpp","sha256":"564a0bf1d22044aa67f03fa622c13a067ed8cd346f73fc4b1f6405fa66a550ee","source":"sources/w10-actor-k-clone-11pushtrigger/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":868,"name":"clone__3Orc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone_3orc.o","profile":"authored_cpp","sha256":"7d8000cc04e4af993561979479325bc0aade3f97ebf7ac2361d5faaaaa030d7f","source":"sources/w10-actor-k-clone-3orc/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":272,"name":"clone__8RollDoor"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone_8rolldoor.o","profile":"authored_cpp","sha256":"ab54fe531f9addb4e8cd26af0e837cdb32337e239f30cf460dcd478047e5c4b3","source":"sources/w10-actor-k-clone-8rolldoor/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":152,"name":"__10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap"}],"header_closure":[],"kind":"independently_authored_source","object_name":"ctor_10projectile.o","profile":"authored_cpp","sha256":"8038363cee4e2dd80ad58d74c9428a74f425a9ec87fac31fce89bce86ccef4b3","source":"sources/w10-actor-k-ctor-10projectile/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":524,"name":"__4PropG6Point3PcN22li"}],"header_closure":[],"kind":"independently_authored_source","object_name":"ctor_4prop_three_names.o","profile":"authored_cpp","sha256":"2986b91aaac01ff6f4136f3aabd942997dc8b809691a11fcd80837135fac6624","source":"sources/w10-actor-k-ctor-4prop-three-names/ctor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":72,"name":"dismount__6Player"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dismount_6player.o","profile":"authored_cpp","sha256":"e7458874fe690e446698a3be1087a6ec5127b0fd129833d766ab99bab6797eb0","source":"sources/w10-actor-k-dismount-6player/dismount.cpp"}
{"emitted_functions":[{"compiled_body_bytes":88,"name":"_$_11PushTrigger"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor_11pushtrigger.o","profile":"authored_cpp","sha256":"1a08ad503e238c64fc6765617833b84dd874c5d00629bf3c240fdb525c6661bc","source":"sources/w10-actor-k-dtor-11pushtrigger/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":88,"name":"_$_19ParticleMorphEffect"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor_19particlemorpheffect.o","profile":"authored_cpp","sha256":"e858897b39bb6aa1aec0cfe210b10c084e45dab582b960733c024a40e91f38a9","source":"sources/w10-actor-k-dtor-19particlemorpheffect/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"_$_3Orc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor_3orc.o","profile":"authored_cpp","sha256":"73456e7c4a84998cf2b16876fd69110f679348148a0c01754f38452d7f0441ad","source":"sources/w10-actor-k-dtor-3orc/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":88,"name":"_$_8RollDoor"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor_8rolldoor.o","profile":"authored_cpp","sha256":"2bd257114c3b6e4908af88a1fb2f4c9a27eb0b0a7f02846a2ca65b1d3368c561","source":"sources/w10-actor-k-dtor-8rolldoor/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":56,"name":"getMinibossName__3Orc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"get_miniboss_name_3orc.o","profile":"authored_cpp","sha256":"92ff0efe4ac1b884683ccb6b2c893c4ab5d76153a2f35d96bb0333c4c0a13180","source":"sources/w10-actor-k-get-miniboss-name-3orc/get_miniboss_name.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1448,"name":"_GLOBAL_$I$move__10ProjectileG6Point3"}],"header_closure":[],"kind":"independently_authored_source","object_name":"global_init_move_10projectile.o","profile":"authored_cpp","sha256":"fbf175a8473b144dc4cfa2fd992adbda674816ae4dec8e52bdb35b5cf146e0fd","source":"sources/w10-actor-k-global-init-move-10projectile/static_init.cpp"}
{"emitted_functions":[{"compiled_body_bytes":108,"name":"initMiniboss__3Orc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"init_miniboss_3orc.o","profile":"authored_cpp","sha256":"9f7353c1dbe07f365e10587b4c295f3e8a069190a12c5bd9fc2630d4108c6910","source":"sources/w10-actor-k-init-miniboss-3orc/init_miniboss.cpp"}
{"emitted_functions":[{"compiled_body_bytes":352,"name":"msg_alert__3OrcifG6Point3P10GameObject"}],"header_closure":["sources/w10-actor-k-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_alert_3orc.o","profile":"authored_cpp","sha256":"51afe613f74e4728ccc8d019d5610135c0d63c4c20779dab7744d4bb12f9b984","source":"sources/w10-actor-k-msg-alert-3orc/msg_alert.cpp"}
{"emitted_functions":[{"compiled_body_bytes":464,"name":"msg_draw__3Orc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_draw_3orc.o","profile":"authored_cpp","sha256":"6fe8e6384a6eff7ecec636439bb185ccc542c04d347a2ec1e3c8cc6f2b19b588","source":"sources/w10-actor-k-msg-draw-3orc/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":148,"name":"msg_draw__8RollDoor"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_draw_8rolldoor.o","profile":"authored_cpp","sha256":"8c5e14c9c03302af37a2c726bc4a7d5caf0ff5ec74f86a2f9dd896459975a44c","source":"sources/w10-actor-k-msg-draw-8rolldoor/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":340,"name":"msg_getPokeName__11PushTriggerPUsi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_get_poke_name_11pushtrigger.o","profile":"authored_cpp","sha256":"84c731dc92c501836253b2a1363d23176fdde3315a2baae106a035c2fa1fc040","source":"sources/w10-actor-k-msg-get-poke-name-11pushtrigger/msg_get_poke_name.cpp"}
{"emitted_functions":[{"compiled_body_bytes":60,"name":"msg_getPokeName__8RollDoorPUsi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_get_poke_name_8rolldoor.o","profile":"authored_cpp","sha256":"3c5ad1f01ba023b83a0eab84371b86e47170efeaf2dc7e45fd2a732491a7244c","source":"sources/w10-actor-k-msg-get-poke-name-8rolldoor/msg_get_poke_name.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"msg_hurt__3OrcfP10GameObjecti"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_hurt_3orc.o","profile":"authored_cpp","sha256":"e76efac3cb91e188911069571ff9f4989e87cfe63f0342d3b6d5ae7ee5b43786","source":"sources/w10-actor-k-msg-hurt-3orc/msg_hurt.cpp"}
{"emitted_functions":[{"compiled_body_bytes":132,"name":"msg_load__3OrcPc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_load_3orc.o","profile":"authored_cpp","sha256":"942e12bcd9c1bfb9e9a3f58e276cb918971daea4c25e730d085ff307a651cb44","source":"sources/w10-actor-k-msg-load-3orc/msg_load.cpp"}
{"emitted_functions":[{"compiled_body_bytes":172,"name":"msg_load__8RollDoorPc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_load_8rolldoor.o","profile":"authored_cpp","sha256":"8c3ace750ebb859558d3a38d814e9f11d5bf78f79dd4c007ce7ed683955b334c","source":"sources/w10-actor-k-msg-load-8rolldoor/msg_load.cpp"}
{"emitted_functions":[{"compiled_body_bytes":160,"name":"msg_poke__11PushTriggeri"}],"header_closure":["sources/w10-actor-k-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_poke_11pushtrigger.o","profile":"authored_cpp","sha256":"8cde6d640e9ebaff8e6b763521fe3f5ccb786b7cab739432c2aea49580a1cff7","source":"sources/w10-actor-k-msg-poke-11pushtrigger/msg_poke.cpp"}
{"emitted_functions":[{"compiled_body_bytes":116,"name":"_ZN12_GLOBAL__N_1L10shadow_posEPhjiP6Point3"},{"compiled_body_bytes":1544,"name":"msg_run__19ParticleMorphEffect"}],"header_closure":["sources/w10-actor-k-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_run_19particlemorpheffect.o","profile":"authored_cpp","sha256":"24214311197213a336a39763a3a713ea8f506ff668df7119e3c4bb1be6f694ca","source":"sources/w10-actor-k-msg-run-19particlemorpheffect/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":152,"name":"_ZN12_GLOBAL__N_1L11type4_zonesEPhj.constprop.0"},{"compiled_body_bytes":28,"name":"_ZN4w10k10call_entryEPhjm.isra.0"},{"compiled_body_bytes":4860,"name":"msg_run__3Orc"}],"header_closure":["sources/w10-actor-k-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_run_3orc.o","profile":"authored_cpp","sha256":"36c3100dbbdca7ee25757be99fa9492477dd93e9a9d470f3dc9c1489c78505e8","source":"sources/w10-actor-k-msg-run-3orc/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":84,"name":"msg_save__3OrcPc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_save_3orc.o","profile":"authored_cpp","sha256":"452dfc9b06a2a591843b6696d50315f305310733e48d05f65e818d366abb9d16","source":"sources/w10-actor-k-msg-save-3orc/msg_save.cpp"}
{"emitted_functions":[{"compiled_body_bytes":3904,"name":"setState__3Orci"}],"header_closure":["sources/w10-actor-k-common/access.hpp"],"kind":"independently_authored_source","object_name":"set_state_3orc.o","profile":"authored_cpp","sha256":"404f80a3d576e903e7fddc29e652bb5fd7074e924b767204e15543670b56124f","source":"sources/w10-actor-k-set-state-3orc/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"__tf19ParticleMorphEffect"}],"header_closure":[],"kind":"independently_authored_source","object_name":"tf_19particlemorpheffect.o","profile":"authored_cpp","sha256":"c2b935e1413de69182f87226f4ddc140ace85e1ec89440a63162e7a8e01966f7","source":"sources/w10-actor-k-tf-19particlemorpheffect/typeinfo.cpp"}
{"emitted_functions":[{"compiled_body_bytes":184,"name":"__tf8RollDoor"}],"header_closure":[],"kind":"independently_authored_source","object_name":"tf_8rolldoor.o","profile":"authored_cpp","sha256":"889b231fb8158edef1741cbaf9149e56df515cdac33bf3656f17bfa274724247","source":"sources/w10-actor-k-tf-8rolldoor/typeinfo.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"vtable_10projectile.o","profile":"authored_cpp","sha256":"79ba9c2563caffa37a1632f49db0e54c128acf40f518c12f3048faacf5f32476","source":"sources/w10-actor-k-vtable-10projectile/vtable.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"vtable_12recalleffect.o","profile":"authored_cpp","sha256":"5c018d2efa51708e52e504897329c1758557bb0832df9aae4d84909206d5a982","source":"sources/w10-actor-k-vtable-12recalleffect/vtable.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"vtable_6phprop.o","profile":"authored_cpp","sha256":"a54ad79ba8ca4d92c8412c170ccba29d3e671f5b0bc09face7eaabd2e9871981","source":"sources/w10-actor-k-vtable-6phprop/vtable.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"vtable_6player.o","profile":"authored_cpp","sha256":"7ad11dc7f04682096acc94209f6ef4ba9a9c6381b48548bd0311ae30a552970d","source":"sources/w10-actor-k-vtable-6player/vtable.cpp"}
```

## Authored but never executed

Every function and table here: host contracts with stubbed callees and in-memory objects, EE compilation with the
reviewed flags, a private emulated paired run (R5900 interpreter, not hardware) with stand-in callees, and the reviewed partial
link. None ran on the EE, inside the game or against real callee bodies. The paired scenarios use random object memory and
scripted callee results, so they establish agreement of control flow, arguments and stores under those conditions, not
gameplay. Nothing is an original-byte match.

## Unknown

- What the object fields mean (state names, the words at +1116 and +972 of Orc, the table at +36, the 8-byte records read as
  t + 8*i + k). Offsets and masks are stated as observed.
- The bodies of every callee named in the sources, so several stores here may be overwritten or reinterpreted by them.
- The meaning of `PropSlot` adjustments (all zero in the four tables) and whether the 18th slot of each table is a terminator.
- Whether the file-local particle and resource objects need the same names the original used; the names are the image's symbol names.
- Float behaviour: the authored build uses IEEE single-precision with `rsqrt.s` by inline assembly in Orc `setState`; the host checks use host
  floats, so they do not prove R5900 rounding. The paired runs used the interpreter's float model.
- The private harness has no MMI opcodes, so the image's `strcpy`, `memset` and `memcpy` were replaced by Python equivalents in the runs that reach them.
