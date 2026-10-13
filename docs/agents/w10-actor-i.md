# Spell, skill and actor virtuals, Lantern to Minion (w10-actor-i)

Package `w10-actor-i`, branch `agents/w10-actor-i`, based on `cc78a45e`. Addresses and sizes are from the private `SLUS_205.65` (`.text`). Nothing here has run on the EE or as gameplay. Compiled sizes are the reviewed EE g++ `authored_cpp` objects and are not a claim of byte equality with the image.

## What was done

All 32 listed function symbols of Lantern, LavaMonster, LavaVehicle, MaleDarkElf and Minion now have an authored definition under `experimental/sources/w10-actor-i-*`, one commit per function (the brief counted 31 names; its list has 32). Every function is its own source directory with one `.cpp`; there is no shared header. The vtable-adjacent data these bodies use is referred to by the symbol names the earlier view units already export (`norrath_ctors_ant_lantern_view_44a8a0`, `norrath_ctors_lava_wraith_view_44b810`, `norrath_ctors_lava_wraith_view_450440`, `norrath_ctors_lava_wraith_view_44f5b8`, `norrath_game_object_vtable_view`, `norrath_actor_view_4488f0`, `norrath_player_view_31f540`) and, for LavaVehicle, by the `_vt$11LavaVehicle` asm label of `w9-startup-crt-c-vt-lavavehicle`.

## How each body was checked

Each body was run next to the image's own instructions in a private R5900 interpreter (`priv-w10-actor-i/`, outside git, it loads the private executable; see the private-harness note in the project memory). For every scenario both versions start from an identical randomized memory snapshot and identical Python stand-ins for every callee, selected by the original ELF name. The check fails unless the callee logs (argument registers, selected pointed-to bytes, float register bits, stack arguments, and a hash of the object bytes at every call) match, the post-call object, scratch and allocation regions are byte-equal, and, where the function returns a value, the value matches. Instruction coverage of the image's body was recorded for every body except the type-info, destructor and clone checks, whose scenario sets cover each branch by construction (node word zero and nonzero, flag bit 0 set and clear, null and non-null allocation, member words zero and nonzero). For the others every word was executed by at least one scenario except alignment `nop` words and, in two places, code the image cannot reach itself (see Unknown). For each function more than a trivial one I changed a constant, offset, branch direction or call argument in the authored source and required the check to fail; the survivors are listed under Unknown.

| function | scenarios | words covered | mutations that fail the check |
|---|---|---|---|
| `__tf*` (4 classes) | 9 node states each | by construction | none run (node name text and base call are in the log) |
| `_$_*` destructors (5) | 10 flag values each | by construction | the Light flag of Lantern and the list order of Minion were mutated |
| `clone__7Lantern` | 12 | by construction | copy range |
| `clone__11LavaMonster`, `clone__11MaleDarkElf` | 10 | by construction | span, table pointer |
| `msg_run__7Lantern`, `msg_draw__7Lantern` | 200 | all | constants, thresholds, offsets |
| small Minion, LavaMonster and MaleDarkElf bodies | 100 to 300 | all | offsets, constants, order |
| `setState__11LavaMonsteri` | 600 | all but nops | 8 of 8 |
| `setState__11MaleDarkElfi` | 1200 | all but nops and one unreachable group | 13 of 13 |
| `msg_run__11MaleDarkElf` | 6000 | 1036 of 1058 | 27 of 27 |
| `msg_run__6Minion` | 8000 | 1156 of 1175 | about 30; two undetectable (see Unknown) |
| `msg_run__11LavaMonster` | 2500 | 1375 of 1385 | 34 of 37 |

The interpreter's `div` and `divu` by zero were made to follow the hardware (LO = -1 or 1, HI = dividend) during this package; before that the stale HI made an unguarded `% count` with count 0 look like a mismatch.

| symbol | image address | image bytes | compiled bytes | source directory |
|---|---|---|---|---|
| `_$_11LavaMonster` | 0x30f2e0 | 100 | 88 | `w10-actor-i-dtor-11lavamonster` |
| `_$_11LavaVehicle` | 0x316760 | 100 | 88 | `w10-actor-i-dtor-11lavavehicle` |
| `_$_11MaleDarkElf` | 0x3168d0 | 100 | 88 | `w10-actor-i-dtor-11maledarkelf` |
| `_$_6Minion` | 0x315af0 | 116 | 120 | `w10-actor-i-dtor-6minion` |
| `_$_7Lantern` | 0x3102a8 | 116 | 120 | `w10-actor-i-dtor-7lantern` |
| `__tf11LavaMonster` | 0x30f348 | 80 | 92 | `w10-actor-i-tf-11lavamonster` |
| `__tf11MaleDarkElf` | 0x316938 | 80 | 92 | `w10-actor-i-tf-11maledarkelf` |
| `__tf6Minion` | 0x315b68 | 80 | 92 | `w10-actor-i-tf-6minion` |
| `__tf7Lantern` | 0x310320 | 76 | 92 | `w10-actor-i-tf-7lantern` |
| `clone__11LavaMonster` | 0x11c310 | 1460 | 540 | `w10-actor-i-clone-11lavamonster` |
| `clone__11MaleDarkElf` | 0x11c8c8 | 1580 | 548 | `w10-actor-i-clone-11maledarkelf` |
| `clone__7Lantern` | 0x123b68 | 456 | 360 | `w10-actor-i-clone-7lantern` |
| `getMinibossName__11MaleDarkElf` | 0x289870 | 60 | 56 | `w10-actor-i-get-miniboss-name-male-dark-elf` |
| `getModel__11LavaMonster` | 0x1ee280 | 32 | 32 | `w10-actor-i-get-model-lava-monster` |
| `getSoulAttachMatrix__6MinionP8Matrix34` | 0x235420 | 88 | 76 | `w10-actor-i-get-soul-attach-matrix` |
| `initMiniboss__11MaleDarkElf` | 0x289840 | 48 | 44 | `w10-actor-i-init-miniboss-male-dark-elf` |
| `msg_alert__11MaleDarkElfifG6Point3P10GameObject` | 0x289788 | 180 | 188 | `w10-actor-i-msg-alert-male-dark-elf` |
| `msg_draw__11LavaMonster` | 0x1eba20 | 212 | 192 | `w10-actor-i-msg-draw-lava-monster` |
| `msg_draw__11MaleDarkElf` | 0x287610 | 380 | 304 | `w10-actor-i-msg-draw-male-dark-elf` |
| `msg_draw__6Minion` | 0x2352d0 | 100 | 112 | `w10-actor-i-msg-draw-6minion` |
| `msg_draw__7Lantern` | 0x231240 | 212 | 232 | `w10-actor-i-msg-draw-7lantern` |
| `msg_hurt__6MinionfP10GameObjecti` | 0x2353b0 | 112 | 100 | `w10-actor-i-msg-hurt-6minion` |
| `msg_load__11LavaMonsterPc` | 0x1ed0a0 | 348 | 324 | `w10-actor-i-msg-load-lava-monster` |
| `msg_load__11MaleDarkElfPc` | 0x2896f8 | 140 | 112 | `w10-actor-i-msg-load-male-dark-elf` |
| `msg_run__11LavaMonster` | 0x1ebaf8 | 5540 | 3492 | `w10-actor-i-msg-run-lava-monster` |
| `msg_run__11MaleDarkElf` | 0x287790 | 4232 | 4212 | `w10-actor-i-msg-run-male-dark-elf` |
| `msg_run__6Minion` | 0x233bd0 | 4700 | 2740 | `w10-actor-i-msg-run-minion` |
| `msg_run__7Lantern` | 0x231160 | 220 | 240 | `w10-actor-i-msg-run-7lantern` |
| `msg_save__11LavaMonsterPc` | 0x1ee1e8 | 144 | 200 | `w10-actor-i-msg-save-lava-monster` |
| `msg_save__11MaleDarkElfPc` | 0x289680 | 120 | 92 | `w10-actor-i-msg-save-male-dark-elf` |
| `setState__11LavaMonsteri` | 0x1ed200 | 992 | 1152 | `w10-actor-i-set-state-lava-monster` |
| `setState__11MaleDarkElfi` | 0x288818 | 3264 | 2444 | `w10-actor-i-set-state-male-dark-elf` |

`msg_run__11MaleDarkElf`, `msg_run__6Minion`, `msg_run__11LavaMonster`, `setState__11MaleDarkElfi` and the Minion/LavaMonster helpers emit extra file-local functions (anonymous-namespace helpers); they appear in the entries below as `_ZN12_GLOBAL__N_1...` names, as in earlier packages.

## Observed

- The five destructors store the class's table pointer at +72, call the base destructor (`_$_10GameObject` for Lantern with the Light member at +76 destroyed first with flag 2; Creature for LavaMonster, MaleDarkElf and Minion with the lists at +968 and +960 destroyed first; `_$_7Vehicle` for LavaVehicle) with flag 0 and free the block when bit 0 of the incoming flag is set. The four type-info functions register the class with `__rtti_si` through the 12-byte `__ti` node (`__tf8Creature` or `__tf10GameObject` as base).
- Lantern flickers: `msg_run` sets the Light member's size terms from a noise of the object address and frame number and drops the +112 word when the level name contains `mine`; `msg_draw` draws one camera-facing sprite from the same noise value.
- `getModel__11LavaMonster` picks resource-block word +12 or +28 by the word at +1048. `getMinibossName__11MaleDarkElf` formats `MBDarkElf%d` into a local static buffer from the variant byte at +76, which `initMiniboss` sets to 1..16.
- The Minion and LavaMonster `msg_save`/`msg_load` pair and the MaleDarkElf pair write and read the fields listed in each source header.
- `setState` and `msg_run` of LavaMonster, MaleDarkElf and Minion are state machines over the halfword at +572; the per-state behaviour, thresholds, constants and call order are recorded in each source's header comment. LavaMonster `setState` and MaleDarkElf `setState` use jump tables of 16 and 17 entries (0x46d810, 0x4776b0); the three `msg_run` bodies use tables at 0x46d7d0 (16), 0x477640 (17) and 0x4724f0 (20).
- All singles are the image's bit patterns (for example 0.1 is 0x3dcccccc, not the compiler's 0x3dcccccd).

## Inferred

- Names for callees come from the executable's symbol names. Meanings such as "variant", "soul", "enemy list", "lava bomb" are only suggested by those names and by use; none is established.
- The Light member at +76 of Lantern is a light from the parameter type of `lightAdd__FP5Light` (as for Lamp and Candle).

## Imports closed

- `_$_7Lantern`
- `__tf7Lantern`
- `clone__7Lantern`
- `msg_draw__7Lantern`
- `msg_run__7Lantern`
- `_$_11LavaMonster`
- `__tf11LavaMonster`
- `clone__11LavaMonster`
- `getModel__11LavaMonster`
- `msg_draw__11LavaMonster`
- `msg_load__11LavaMonsterPc`
- `msg_run__11LavaMonster`
- `msg_save__11LavaMonsterPc`
- `setState__11LavaMonsteri`
- `_$_11LavaVehicle`
- `_$_11MaleDarkElf`
- `__tf11MaleDarkElf`
- `clone__11MaleDarkElf`
- `getMinibossName__11MaleDarkElf`
- `initMiniboss__11MaleDarkElf`
- `msg_alert__11MaleDarkElfifG6Point3P10GameObject`
- `msg_draw__11MaleDarkElf`
- `msg_load__11MaleDarkElfPc`
- `msg_run__11MaleDarkElf`
- `msg_save__11MaleDarkElfPc`
- `setState__11MaleDarkElfi`
- `_$_6Minion`
- `__tf6Minion`
- `getSoulAttachMatrix__6MinionP8Matrix34`
- `msg_draw__6Minion`
- `msg_hurt__6MinionfP10GameObjecti`
- `msg_run__6Minion`

All 32 were confirmed closed by a private partial build of the shared selection plus these units (unresolved list afterwards: 1329 names, none of these).

## Imports opened

These names are referenced by these units and by no other unit of the build. They are not defined by this package.

- `__15RainOfFireSpellG6Point3ifiP10GameObjectT1P15AnimationHeader`
- `__19ColdArrowProjectileG6Point3sP10GameObjectiP5Skill`
- `__19FireArrowProjectileG6Point3sP10GameObjectiP5Skill`
- `__19MultiFireProjectileG6Point3sP10GameObjectiP5Skill`
- `__21PoisonArrowProjectileG6Point3sP10GameObjectiP5Skill`
- `__22ExplodeArrowProjectileG6Point3sP10GameObjectiP5Skill`
- `__4SoulG6Point3sPPcP6Minion`
- `__6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf`
- `__8ListHeadRC8ListHead`
- `_vt$8LavaBomb`
- `alternateEnemyInZone__8Creatureffss`
- `creatureSlamEffect__8CreatureP8Creaturei`
- `enemyInZone2__8Creatureffssfi`
- `enemyInZone__8Creaturefs`
- `findEnemyLeadPosition__8CreaturefP6Point3`
- `flee2__8CreatureP10GameObject`
- `fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34`
- `getTexture__11LavaMonster`
- `initType__11MaleDarkElfb`
- `lavaSparkParticle`
- `lavamonsterParticle1`
- `lavamonsterParticle2`
- `modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii`
- `modelGetCharAttachmentMatrix__FP12_modelHeaderG6Point3sP14AnimationStateiP8Matrix34f`
- `objectIsCrowded__8CreatureP10GameObjectffi`
- `produceGeneralDamage__8CreatureffssiiiiPQ24item11description`
- `produceMeleeDamage__8CreaturefiiiiPQ24item11description`
- `steerAndRouteToEnemy__8Creature`
- `turnToFaceEnemy__8Creature`

`lavaSparkParticle`, `lavamonsterParticle1` and `lavamonsterParticle2` are the file-local particle definitions of `w8-fx-particles-init-lava-monster` (static, 80 bytes at 0x7d6d30, 0x7d6c90 and 0x7d6ce0); that unit has to export them (or an alias) for these references to resolve. `_vt$8LavaBomb` (0x44de58) has no authored definition. The twelve other unresolved names this package calls are also called by other units (for example `blockFree__FPv`, `floorAdd__FG6Point3sfii`, `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`, `ef1Noise__Fi`, `lightAdd__FP5Light`).

## Build-plan entries needed

`python3 docs/agents/w10-actor-i-plan-entries.py --toolchain /Users/case/norrath-agents/repo/work/toolchains/ee [--private-root DIR]` prints them (and can write a private plan copy). Profile `authored_cpp`, no headers, no `host_check.cpp`. The integrator assigns `input_order`. The printed entries at this revision:

```json
{"emitted_functions": [{"compiled_body_bytes": 540, "name": "clone__11LavaMonster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_clone_11lavamonster.o", "profile": "authored_cpp", "sha256": "9b1e2eb74efed2232f48f0bcb29a9693b4b35b99a45e83677539311fae5c305f", "source": "sources/w10-actor-i-clone-11lavamonster/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 548, "name": "clone__11MaleDarkElf"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_clone_11maledarkelf.o", "profile": "authored_cpp", "sha256": "169962683db1937e54c304e8330c4e065329a727d56c6fab55778d55b6031183", "source": "sources/w10-actor-i-clone-11maledarkelf/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 360, "name": "clone__7Lantern"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_clone_7lantern.o", "profile": "authored_cpp", "sha256": "2e41bbeb7773d7a6723f966b5697dcad69c29cdf067f2eafaa72a7388e776864", "source": "sources/w10-actor-i-clone-7lantern/clone.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "_$_11LavaMonster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_dtor_11lavamonster.o", "profile": "authored_cpp", "sha256": "91e2c2eea6dccf5bf5d0a6e3b1e15c62ad6e04ddf5b8626f7c88190e27e24946", "source": "sources/w10-actor-i-dtor-11lavamonster/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "_$_11LavaVehicle"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_dtor_11lavavehicle.o", "profile": "authored_cpp", "sha256": "055e239a91803243e2de139185250d2df4402b1feb88a462f4d6c00bca053c5e", "source": "sources/w10-actor-i-dtor-11lavavehicle/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "_$_11MaleDarkElf"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_dtor_11maledarkelf.o", "profile": "authored_cpp", "sha256": "138f7aa1d38eb346074ed372ecb91eef81e29f81cb28a3b31f386a3c2f46a6b0", "source": "sources/w10-actor-i-dtor-11maledarkelf/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "_$_6Minion"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_dtor_6minion.o", "profile": "authored_cpp", "sha256": "0d3537169de741c7391b72de964e59ed73fbcd9748b039d5898938b602a9ee95", "source": "sources/w10-actor-i-dtor-6minion/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "_$_7Lantern"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_dtor_7lantern.o", "profile": "authored_cpp", "sha256": "e55f42102572e7c815b4e97b1f3ef36981327e6bc90c70753b70f4a6fdd9e5cf", "source": "sources/w10-actor-i-dtor-7lantern/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 56, "name": "getMinibossName__11MaleDarkElf"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_get_miniboss_name_male_dark_elf.o", "profile": "authored_cpp", "sha256": "b863581d66b3018b2bdf9709d12ed4e560f08a54bb13b8ca169611d48ecb8afa", "source": "sources/w10-actor-i-get-miniboss-name-male-dark-elf/get_miniboss_name.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "getModel__11LavaMonster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_get_model_lava_monster.o", "profile": "authored_cpp", "sha256": "fbaf2115d85b3693c5d57e265d56d85a302f6a0b68b83ac0827e1e6124793d30", "source": "sources/w10-actor-i-get-model-lava-monster/get_model.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 76, "name": "getSoulAttachMatrix__6MinionP8Matrix34"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_get_soul_attach_matrix.o", "profile": "authored_cpp", "sha256": "8819c19ff3ff1c09202e3dca018a2e1ec86d48c05200b4380c9c942f667f16cd", "source": "sources/w10-actor-i-get-soul-attach-matrix/get_soul_attach_matrix.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "initMiniboss__11MaleDarkElf"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_init_miniboss_male_dark_elf.o", "profile": "authored_cpp", "sha256": "ab151aec3f0315ed726984ffb40ab7dd37bec587214c2db9ea2df32647ffebbe", "source": "sources/w10-actor-i-init-miniboss-male-dark-elf/init_miniboss.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 188, "name": "msg_alert__11MaleDarkElfifG6Point3P10GameObject"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_msg_alert_male_dark_elf.o", "profile": "authored_cpp", "sha256": "f7190629aa708bd3c390273cc035110ba496071fea376cf4f94aab53c0522857", "source": "sources/w10-actor-i-msg-alert-male-dark-elf/msg_alert.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 112, "name": "msg_draw__6Minion"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_msg_draw_6minion.o", "profile": "authored_cpp", "sha256": "b0bbeb1d260e12a93b2905df7fcb9c54712f4aa97616ea7b703764d3adb379e3", "source": "sources/w10-actor-i-msg-draw-6minion/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 232, "name": "msg_draw__7Lantern"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_msg_draw_7lantern.o", "profile": "authored_cpp", "sha256": "6535f4cada66f78afedc4a4aa2dad4026104f2af90354f42e77cb3c537a3442a", "source": "sources/w10-actor-i-msg-draw-7lantern/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 192, "name": "msg_draw__11LavaMonster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_msg_draw_lava_monster.o", "profile": "authored_cpp", "sha256": "9737f9ae8919fc1e5c1daca4b07495753c635d415668b67519d4099d7bb5cd35", "source": "sources/w10-actor-i-msg-draw-lava-monster/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 304, "name": "msg_draw__11MaleDarkElf"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_msg_draw_male_dark_elf.o", "profile": "authored_cpp", "sha256": "6f7e82af7064d1053a4b0569e0206ef83e7d700a4245ca195ffd584feda72482", "source": "sources/w10-actor-i-msg-draw-male-dark-elf/msg_draw.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 100, "name": "msg_hurt__6MinionfP10GameObjecti"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_msg_hurt_6minion.o", "profile": "authored_cpp", "sha256": "5917e7a23ee256929c7c9e185e4c55524e454d08a4569fee8dc12af57b06db47", "source": "sources/w10-actor-i-msg-hurt-6minion/msg_hurt.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 324, "name": "msg_load__11LavaMonsterPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_msg_load_lava_monster.o", "profile": "authored_cpp", "sha256": "c4aa021a03761b0d3659af33015afc6d2e19b0f1d5d9739fa773da45cc735004", "source": "sources/w10-actor-i-msg-load-lava-monster/msg_load.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 112, "name": "msg_load__11MaleDarkElfPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_msg_load_male_dark_elf.o", "profile": "authored_cpp", "sha256": "45043e197eb1d0f9a57ccf85fa247d20a794b3faab84197aad6a564d53a07205", "source": "sources/w10-actor-i-msg-load-male-dark-elf/msg_load.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 240, "name": "msg_run__7Lantern"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_msg_run_7lantern.o", "profile": "authored_cpp", "sha256": "ed90a99c72cadeef3844daed47110f180520ade2a47432fe02966875c699af6d", "source": "sources/w10-actor-i-msg-run-7lantern/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 224, "name": "_ZN12_GLOBAL__N_1L11unit_randomEPhPf"}, {"compiled_body_bytes": 140, "name": "_ZN12_GLOBAL__N_1L19random_shadow_pointEPhiPNS_6Point3E"}, {"compiled_body_bytes": 116, "name": "_ZN12_GLOBAL__N_1L15shadow_particleEPhiPv.constprop.0"}, {"compiled_body_bytes": 3492, "name": "msg_run__11LavaMonster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_msg_run_lava_monster.o", "profile": "authored_cpp", "sha256": "5fcb03ebd26a5c75ad2ee13b8f4cb88ee24c8205a9a52faf40571625b92edb5d", "source": "sources/w10-actor-i-msg-run-lava-monster/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 56, "name": "_ZN12_GLOBAL__N_1L8in_zone2EPh"}, {"compiled_body_bytes": 260, "name": "_ZN12_GLOBAL__N_1L14tail_after_bitEPh"}, {"compiled_body_bytes": 4212, "name": "msg_run__11MaleDarkElf"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_msg_run_male_dark_elf.o", "profile": "authored_cpp", "sha256": "b46fefde65f5de91fbca131ca92ab94be4844c6c62a921acfe1ec12406acad88", "source": "sources/w10-actor-i-msg-run-male-dark-elf/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 200, "name": "_ZN12_GLOBAL__N_1L4pickEPhffi"}, {"compiled_body_bytes": 844, "name": "_ZN12_GLOBAL__N_1L5chaseEPhb"}, {"compiled_body_bytes": 2740, "name": "msg_run__6Minion"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_msg_run_minion.o", "profile": "authored_cpp", "sha256": "33b1764231bc84b0941f76edbb5ad663def923501db723ff6a5900efb0b0bd59", "source": "sources/w10-actor-i-msg-run-minion/msg_run.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 200, "name": "msg_save__11LavaMonsterPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_msg_save_lava_monster.o", "profile": "authored_cpp", "sha256": "9af01b626690442301d76db177d6d3d93b2b4b445e823c2231048cc69ae43c15", "source": "sources/w10-actor-i-msg-save-lava-monster/msg_save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "msg_save__11MaleDarkElfPc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_msg_save_male_dark_elf.o", "profile": "authored_cpp", "sha256": "8291b3cffefddda7a0fd5737f582c3b688b7ffd3948ec4a07d6bca9a4fa253a5", "source": "sources/w10-actor-i-msg-save-male-dark-elf/msg_save.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1152, "name": "setState__11LavaMonsteri"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_set_state_lava_monster.o", "profile": "authored_cpp", "sha256": "3485cc1638c7511b19ed8b242d202d8238b2a6eb39c7fef154903382387d5893", "source": "sources/w10-actor-i-set-state-lava-monster/set_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 320, "name": "_ZN12_GLOBAL__N_1L13release_pieceERKNS_7ContextEjii.constprop.0"}, {"compiled_body_bytes": 2444, "name": "setState__11MaleDarkElfi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_set_state_male_dark_elf.o", "profile": "authored_cpp", "sha256": "29234de27e39a25e854daff9ade1d1df0f3cf8541c5503ea96b5d7ec3ceb4ab4", "source": "sources/w10-actor-i-set-state-male-dark-elf/set_state.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf11LavaMonster"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_tf_11lavamonster.o", "profile": "authored_cpp", "sha256": "edbf235b4cab4365143c752400bb6f2f95758d0c019302adb6c25692a6c7b174", "source": "sources/w10-actor-i-tf-11lavamonster/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf11MaleDarkElf"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_tf_11maledarkelf.o", "profile": "authored_cpp", "sha256": "e954d52dcdd9157335aa29ff72efa9464a8529f8457e8d44a961af3374dda404", "source": "sources/w10-actor-i-tf-11maledarkelf/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf6Minion"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_tf_6minion.o", "profile": "authored_cpp", "sha256": "a092d63f874b442def983e7c619b75f09c75fab995413a372d9f661572c12ae3", "source": "sources/w10-actor-i-tf-6minion/typeinfo.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 92, "name": "__tf7Lantern"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w10i_tf_7lantern.o", "profile": "authored_cpp", "sha256": "b5fd0bd311a8fda079463ebc45af57293f88c7771dbedcedbb3bc868d8dd7219", "source": "sources/w10-actor-i-tf-7lantern/typeinfo.cpp"}
```

## Authored but never executed

All of it on the EE or in the game. Every body was run only inside the comparison interpreter against stand-in callees; the `msg_run`, `setState` and `clone` bodies call objects and virtual slots that are fakes there. No host test files are included because the comparison needs the private image.

## Unknown

- What the states of LavaMonster, MaleDarkElf and Minion mean, and what most object fields hold (for example +960/+964/+968/+972/+976/+980/+984 of Minion, +960/+964/+968/+1112 of MaleDarkElf, +960/+968/+1048/+1052 of LavaMonster).
- Whether `getMinibossName`'s static buffer has the size used here (64 bytes; the image's local symbol size was not read).
- Cases where the scenarios cannot tell the authored body from a slightly different one: the exact 4001 boundary of the heading clamp in LavaMonster `msg_run`, one-ulp changes of the single 0.005 and 6.0 in LavaMonster `msg_run`, one-ulp change of the single 1.5 in Minion `msg_run` (the products are integers in the scenarios), and the equal-value `<`/`<=` swap in Minion state 9 below a few thousand scenarios.
- Code in the image that cannot run: the bit 0x200 branch group in `setState__11MaleDarkElfi` state 8 (the bit was just cleared), the type-21 record test in `msg_load__11LavaMonsterPc` (every type is reset to -1 first). They are reproduced as the same tests.
- EE FPU behaviour on NaN or denormal inputs; the interpreter rounds to nearest and the scenario floats are small integers or exact fractions.
- The `teq` zero checks the reviewed compiler emits on unsigned `%` by constants do not appear in the image; they never trigger.
- Whether `lavamonsterParticle*` and `lavaSparkParticle` should be exported from the static-init unit or defined here.
