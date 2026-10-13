# w10-actor-n: spell, skill and actor virtuals (Soul to Torch)

Package `w10-actor-n`, branch `agents/w10-actor-n`. Private reference: the original
executable `SLUS_205.65` (SHA-256 `9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`).
Addresses are that file's symbol values. No game bytes or disassembly listings are committed.
Everything was compiled with the reviewed EE g++ `authored_cpp` flags. Nothing here has run on a
PS2 or in a game; "paired" means the authored object was run next to the original function in a
private R5900 interpreter (kept outside git).

## How each function was checked

- Paired run (private): compile the unit, link it with every undefined symbol bound to the
  original's address, then run the original and the authored function from identical memory
  snapshots with identical stub callees. Compared: the ordered callee log (argument registers,
  float arguments, pointed-to vectors and descriptions, and a hash of the object at every call,
  which catches store-order differences), the return value where the original defines one, and
  every changed byte outside the stack. Scenarios use fixed random seeds. A run that halts the
  interpreter counts as a failure.
- Host contract (`host_check.cpp` in each directory, not a build input): restates the expected
  behaviour as stub-callee sequences and field values. Units that read 32-bit pointer words use
  `W10_ACTOR_N_HOST`, which turns those words into offsets from a host base (shared header
  `w10-actor-n-common/access.hpp`). All were compiled and run on the host (`g++ -std=c++17`).
- The vtable was checked differently: it was linked with every callee bound to its original
  address and the resulting 144 bytes were compared with the image.
- Two traps found while checking: (1) `msg_run__8SuperOrc` keeps the result of
  `creatureAdvanceAnimation` (the image moves it into a saved register in the delay slot of the
  next call), which a first reading attributed to `slideBack`; (2) in `msg_run__10StaticFire` a
  float square of a clamped constant was folded at compile time by the EE compiler to a value
  one unit below the interpreter's nearest rounding (inferred: the compiler's float model
  truncates), so the source keeps it a run-time multiply with an empty `asm` barrier. Neither affects the committed behaviour
  claims; they are recorded because the interpreter rounds to nearest and the hardware does
  not, so float-heavy routines were checked under the interpreter's rounding only.

## Functions

| source | symbol | original extent | compiled bytes | check |
|---|---|---|---|---|
| `w10-actor-n-clone-10staticfire/clone.cpp` | `clone__10StaticFire` | 0x00128978, 636 bytes | 476 | 10 paired scenarios incl. null allocation and four light-word combinations, 158 of 159 words covered, host contract |
| `w10-actor-n-clone-11spiderqueen/clone.cpp` | `clone__11SpiderQueen` | 0x0011ec80, 1560 bytes | 824 | 7 paired scenarios incl. null allocation, 389 of 390 words covered, host contract |
| `w10-actor-n-clone-6switch/clone.cpp` | `clone__6Switch` | 0x00127a18, 272 bytes | 272 | 7 paired scenarios incl. null allocation, all 68 original words covered, host contract |
| `w10-actor-n-clone-8superorc/clone.cpp` | `clone__8SuperOrc` | 0x0011f7e0, 1568 bytes | 816 | 7 paired scenarios incl. null allocation, all 392 words covered, host contract |
| `w10-actor-n-dtor-10staticfire/dtor.cpp` | `_$_10StaticFire` | 0x0028f490, 132 bytes | 172 | 32 paired scenarios over twelve flag values (23-33 of 24-34 words covered, rest padding), host contract |
| `w10-actor-n-dtor-11spiderqueen/dtor.cpp` | `_$_11SpiderQueen` | 0x0030f680, 108 bytes | 112 | 32 paired scenarios over twelve flag values (23-33 of 24-34 words covered, rest padding), host contract |
| `w10-actor-n-dtor-5torch/dtor.cpp` | `_$_5Torch` | 0x0030fef0, 116 bytes | 120 | 32 paired scenarios over twelve flag values (23-33 of 24-34 words covered, rest padding), host contract |
| `w10-actor-n-dtor-6switch/dtor.cpp` | `_$_6Switch` | 0x00311b80, 100 bytes | 88 | 32 paired scenarios over twelve flag values (23-33 of 24-34 words covered, rest padding), host contract |
| `w10-actor-n-dtor-8superorc/dtor.cpp` | `_$_8SuperOrc` | 0x0030f740, 100 bytes | 88 | 32 paired scenarios over twelve flag values (23-33 of 24-34 words covered, rest padding), host contract |
| `w10-actor-n-get-poke-name-6switch/get_poke_name.cpp` | `msg_getPokeName__6SwitchPUsi` | 0x0021be40, 60 bytes | 60 | 4 paired scenarios, host contract |
| `w10-actor-n-msg-draw-10staticfire/msg_draw.cpp` | `msg_draw__10StaticFire` | 0x0028eb58, 1180 bytes | 1288 | 250 paired scenarios, 294 of 295 words covered, host contract |
| `w10-actor-n-msg-draw-11spiderqueen/msg_draw.cpp` | `msg_draw__11SpiderQueen` | 0x0020fa28, 76 bytes | 60 | 3 paired scenarios, host contract |
| `w10-actor-n-msg-draw-8superorc/msg_draw.cpp` | `msg_draw__8SuperOrc` | 0x00229f90, 276 bytes | 200 | 40 paired scenarios, all 69 words covered, host contract |
| `w10-actor-n-msg-hurt-11spiderqueen/msg_hurt.cpp` | `msg_hurt__11SpiderQueenfP10GameObjecti` | 0x0020cd60, 464 bytes | 384 | 80 paired scenarios, 115 of 116 words covered, host contract |
| `w10-actor-n-msg-hurt-8superorc/msg_hurt.cpp` | `msg_hurt__8SuperOrcfP10GameObjecti` | 0x0022ba60, 112 bytes | 132 | 18 paired scenarios, host contract |
| `w10-actor-n-msg-run-10staticfire/msg_run.cpp` | `msg_run__10StaticFire` | 0x0028e088, 2768 bytes | 2516 | 400 paired scenarios, 683 of 692 words covered, host contract |
| `w10-actor-n-msg-run-8superorc/msg_run.cpp` | `msg_run__8SuperOrc` | 0x0022a0a8, 3156 bytes | 2628 | 650 paired scenarios (500 mixed, 150 on state 13), 767 of 789 words covered, host contract |
| `w10-actor-n-msg-trigger-11spiderqueen/msg_trigger.cpp` | `msg_trigger__11SpiderQueeni` | 0x0020f9b8, 112 bytes | 132 | 6 paired scenarios, host contract |
| `w10-actor-n-msg-trigger-8superorc/msg_trigger.cpp` | `msg_trigger__8SuperOrci` | 0x0022b9d8, 136 bytes | 136 | 8 paired scenarios, host contract |
| `w10-actor-n-new-wind-10staticfire/new_wind.cpp` | `newWind__10StaticFirei` | 0x0028eff8, 288 bytes | 268 | 120 paired scenarios, all 72 words covered, host contract |
| `w10-actor-n-poke-6switch/poke.cpp` | `msg_poke__6Switchi` | 0x0021be80, 108 bytes | 96 | 6 paired scenarios, host contract |
| `w10-actor-n-set-active-chunks-10teleporter/set_active_chunks.cpp` | `setActiveChunks__10Teleporter` | 0x00312880, 8 bytes | 8 | 4 paired scenarios (random memory), host contract |
| `w10-actor-n-set-state-11spiderqueen/set_state.cpp` | `setState__11SpiderQueeni` | 0x0020cf30, 1516 bytes | 1640 | 1350 paired scenarios, 370 of 379 words covered (rest padding), host contract |
| `w10-actor-n-set-state-4soul/set_state.cpp` | `setState__4Souli` | 0x001f46c8, 1672 bytes | 1664 | 770 paired scenarios (500 mixed, 270 on plain paths); the matrix branch of state 23 is unreachable; host contract |
| `w10-actor-n-set-state-8superorc/set_state.cpp` | `setState__8SuperOrci` | 0x0022ad00, 964 bytes | 1012 | 384 paired scenarios, 235 of 241 words covered (rest padding), host contract |
| `w10-actor-n-tf-10staticfire/typeinfo.cpp` | `__tf10StaticFire` | 0x00312270, 76 bytes | 92 | 9 paired runs (first word 0, 1, 0x1234, -1; base registered or not), host contract |
| `w10-actor-n-tf-11spiderqueen/typeinfo.cpp` | `__tf11SpiderQueen` | 0x0030f6f0, 80 bytes | 92 | 9 paired runs (first word 0, 1, 0x1234, -1; base registered or not), host contract |
| `w10-actor-n-tf-5torch/typeinfo.cpp` | `__tf5Torch` | 0x0030ff68, 76 bytes | 92 | 9 paired runs (first word 0, 1, 0x1234, -1; base registered or not), host contract |
| `w10-actor-n-tf-6switch/typeinfo.cpp` | `__tf6Switch` | 0x00311be8, 132 bytes | 192 | 9 paired runs (first word 0, 1, 0x1234, -1; base registered or not), host contract |
| `w10-actor-n-tf-8superorc/typeinfo.cpp` | `__tf8SuperOrc` | 0x0030f7a8, 80 bytes | 92 | 9 paired runs (first word 0, 1, 0x1234, -1; base registered or not), host contract |
| `w10-actor-n-vt-6sparks/vtable.cpp` | `_vt$6Sparks` | 0x0044a270, 144 bytes | data | linked with callees bound to original addresses: all 144 bytes equal the image, host contract |

## Build-plan entries needed

Append these 31 authored inputs with fresh `input_order` values after the current tip; `default_authored_inputs` grows by 31. Profile `authored_cpp`. `host_check.cpp` files are not inputs (each includes host headers). Header closure is `sources/w10-actor-n-common/access.hpp` where listed. Body sizes come from the reviewed EE g++ build of exactly these files, not from the original executable. No private object hash is claimed. Units that define their own typeinfo objects (`__ti*`) or the vtable are data definitions listed in the same entry.

```json
{"emitted_functions":[{"compiled_body_bytes":476,"name":"clone__10StaticFire"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"b695668c809df96456290c1e4fd94fdf25f6c93fda173912f20587e1dd7844c2","source":"sources/w10-actor-n-clone-10staticfire/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":824,"name":"clone__11SpiderQueen"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"97c6dbe83283fb14418e7d396810b0ad7c7184a0faef39a5b5ad807cac1d3558","source":"sources/w10-actor-n-clone-11spiderqueen/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":272,"name":"clone__6Switch"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"7da98022553b33c00b616b79d05974a18934cc413589eef10817f76bb87079f5","source":"sources/w10-actor-n-clone-6switch/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":816,"name":"clone__8SuperOrc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"clone.o","profile":"authored_cpp","sha256":"45d16383e64c930f0f363a800f6a8c84cd4ccf7cf0006b7d9feeaf1dc436d598","source":"sources/w10-actor-n-clone-8superorc/clone.cpp"}
{"emitted_functions":[{"compiled_body_bytes":172,"name":"_$_10StaticFire"}],"header_closure":["sources/w10-actor-n-common/access.hpp"],"kind":"independently_authored_source","object_name":"dtor.o","profile":"authored_cpp","sha256":"9c06709cbea83cda356537813e8091c88df591dd5e0fa4446e667d27e5e6fd0c","source":"sources/w10-actor-n-dtor-10staticfire/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":112,"name":"_$_11SpiderQueen"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor.o","profile":"authored_cpp","sha256":"8bc195beaac28df67fafda9bd0592000c64484ca654d34297a1d930a12c1dc51","source":"sources/w10-actor-n-dtor-11spiderqueen/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":120,"name":"_$_5Torch"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor.o","profile":"authored_cpp","sha256":"3ca21327622b0341745eddd47f92f7e9c9851447260956f43ff17d2a179aff22","source":"sources/w10-actor-n-dtor-5torch/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":88,"name":"_$_6Switch"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor.o","profile":"authored_cpp","sha256":"6affd90cfc7a04e417de98d8a8719de5771a7fa53f2a8c774fdb75d84ce0d8b0","source":"sources/w10-actor-n-dtor-6switch/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":88,"name":"_$_8SuperOrc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"dtor.o","profile":"authored_cpp","sha256":"bb8a9cc3aed92924f8e56ac22eeb031756da8ebe6f2c966311a66efa5537c65c","source":"sources/w10-actor-n-dtor-8superorc/dtor.cpp"}
{"emitted_functions":[{"compiled_body_bytes":60,"name":"msg_getPokeName__6SwitchPUsi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"get_poke_name.o","profile":"authored_cpp","sha256":"7feaaf5d5d5761a8e3b0c83f2340b132165817131266b2eaf3f7e990a02dabe3","source":"sources/w10-actor-n-get-poke-name-6switch/get_poke_name.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1288,"name":"msg_draw__10StaticFire"}],"header_closure":["sources/w10-actor-n-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_draw.o","profile":"authored_cpp","sha256":"a3902b2e192fe08ceca37bcd50dbd0174caa55e78754cec565739fe9ea04e1b6","source":"sources/w10-actor-n-msg-draw-10staticfire/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":60,"name":"msg_draw__11SpiderQueen"}],"header_closure":["sources/w10-actor-n-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_draw.o","profile":"authored_cpp","sha256":"2785033e74875828faafd2dc7cfc8e521d888aa2b27c35cf3a759e36fce0ca89","source":"sources/w10-actor-n-msg-draw-11spiderqueen/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":200,"name":"msg_draw__8SuperOrc"}],"header_closure":["sources/w10-actor-n-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_draw.o","profile":"authored_cpp","sha256":"074aa6fd8e152d552560e8490e698eac88390de3ee0d6d3a925a82e958565a8c","source":"sources/w10-actor-n-msg-draw-8superorc/msg_draw.cpp"}
{"emitted_functions":[{"compiled_body_bytes":384,"name":"msg_hurt__11SpiderQueenfP10GameObjecti"}],"header_closure":["sources/w10-actor-n-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_hurt.o","profile":"authored_cpp","sha256":"2b1090248239a6c0fc176e6b2d29238c1d4b23215f7991be285501790d8e53d5","source":"sources/w10-actor-n-msg-hurt-11spiderqueen/msg_hurt.cpp"}
{"emitted_functions":[{"compiled_body_bytes":132,"name":"msg_hurt__8SuperOrcfP10GameObjecti"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_hurt.o","profile":"authored_cpp","sha256":"b076d1a02eabc567bb12512702de15b7d62e1e9c3efa4bd6ee860d58f3ecf542","source":"sources/w10-actor-n-msg-hurt-8superorc/msg_hurt.cpp"}
{"emitted_functions":[{"compiled_body_bytes":2516,"name":"msg_run__10StaticFire"}],"header_closure":["sources/w10-actor-n-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_run.o","profile":"authored_cpp","sha256":"9f0d2fc87955c1fb84f3cd5032fa7e2a97a599de30eb8aafb5eccddf2d94a0bf","source":"sources/w10-actor-n-msg-run-10staticfire/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":200,"name":"decide"},{"compiled_body_bytes":128,"name":"face_and_leave"},{"compiled_body_bytes":268,"name":"melee_round"},{"compiled_body_bytes":2628,"name":"msg_run__8SuperOrc"}],"header_closure":["sources/w10-actor-n-common/access.hpp"],"kind":"independently_authored_source","object_name":"msg_run.o","profile":"authored_cpp","sha256":"873dbd96748b36e41d024a5a61f5c3a19ac858795f94a7ec783ef20a9e91763b","source":"sources/w10-actor-n-msg-run-8superorc/msg_run.cpp"}
{"emitted_functions":[{"compiled_body_bytes":132,"name":"msg_trigger__11SpiderQueeni"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_trigger.o","profile":"authored_cpp","sha256":"e6a0d74d58537071f80d65e9e3f25f8c8fc155404b6a04d6cd6d35385b7890e2","source":"sources/w10-actor-n-msg-trigger-11spiderqueen/msg_trigger.cpp"}
{"emitted_functions":[{"compiled_body_bytes":136,"name":"msg_trigger__8SuperOrci"}],"header_closure":[],"kind":"independently_authored_source","object_name":"msg_trigger.o","profile":"authored_cpp","sha256":"a994bd96c9fcaaffe6c3847011a829350016d0b8ed0eff0da8b95fc233ec1352","source":"sources/w10-actor-n-msg-trigger-8superorc/msg_trigger.cpp"}
{"emitted_functions":[{"compiled_body_bytes":268,"name":"newWind__10StaticFirei"}],"header_closure":["sources/w10-actor-n-common/access.hpp"],"kind":"independently_authored_source","object_name":"new_wind.o","profile":"authored_cpp","sha256":"82f0f6e63b3d75222fa3b5b41f379c748382e1621463a22b28f6c7ca66046920","source":"sources/w10-actor-n-new-wind-10staticfire/new_wind.cpp"}
{"emitted_functions":[{"compiled_body_bytes":96,"name":"msg_poke__6Switchi"}],"header_closure":[],"kind":"independently_authored_source","object_name":"poke.o","profile":"authored_cpp","sha256":"4e906f888076fb7a9c239c028612c89af539bf4177ed872a24734b9fa6648437","source":"sources/w10-actor-n-poke-6switch/poke.cpp"}
{"emitted_functions":[{"compiled_body_bytes":8,"name":"setActiveChunks__10Teleporter"}],"header_closure":[],"kind":"independently_authored_source","object_name":"set_active_chunks.o","profile":"authored_cpp","sha256":"57dcdc1854164220c66c2bb43e8f5a2ad71c7410b02ce65ab93e140760dd36c8","source":"sources/w10-actor-n-set-active-chunks-10teleporter/set_active_chunks.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1640,"name":"setState__11SpiderQueeni"}],"header_closure":["sources/w10-actor-n-common/access.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"cecdfb7d66d299c034fa8611880a92d3c8dc4926ef6a792d37828e8e3c105798","source":"sources/w10-actor-n-set-state-11spiderqueen/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1664,"name":"setState__4Souli"}],"header_closure":["sources/w10-actor-n-common/access.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"9bc638a9edc6fbafde975acfe06d958eaba58280fe332b9c52cfec7866b703d5","source":"sources/w10-actor-n-set-state-4soul/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":1012,"name":"setState__8SuperOrci"}],"header_closure":["sources/w10-actor-n-common/access.hpp"],"kind":"independently_authored_source","object_name":"set_state.o","profile":"authored_cpp","sha256":"b819f721d0f6679d75cdc4f8a341d83cd671b5f0aab60d974215a14e5c8213db","source":"sources/w10-actor-n-set-state-8superorc/set_state.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"__tf10StaticFire"}],"header_closure":[],"kind":"independently_authored_source","object_name":"typeinfo.o","profile":"authored_cpp","sha256":"6d8e7ecf77dfe2ff17d050e1fadfcaa2c43fbee1ed009c4178438cd39ff35a44","source":"sources/w10-actor-n-tf-10staticfire/typeinfo.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"__tf11SpiderQueen"}],"header_closure":[],"kind":"independently_authored_source","object_name":"typeinfo.o","profile":"authored_cpp","sha256":"372e57323020a995725c2f1426fa451aeea9f78b443080ef2ef07a5dc78191a5","source":"sources/w10-actor-n-tf-11spiderqueen/typeinfo.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"__tf5Torch"}],"header_closure":[],"kind":"independently_authored_source","object_name":"typeinfo.o","profile":"authored_cpp","sha256":"67e1ca10a1c545deb5e741d6e91a334f2c5c489457db8690da0dcb9cf83bd4cf","source":"sources/w10-actor-n-tf-5torch/typeinfo.cpp"}
{"emitted_functions":[{"compiled_body_bytes":192,"name":"__tf6Switch"}],"header_closure":[],"kind":"independently_authored_source","object_name":"typeinfo.o","profile":"authored_cpp","sha256":"01d2fa6a81b0d80e095429c93a02707c75c4acc923536500e85fee95ec84e011","source":"sources/w10-actor-n-tf-6switch/typeinfo.cpp"}
{"emitted_functions":[{"compiled_body_bytes":92,"name":"__tf8SuperOrc"}],"header_closure":[],"kind":"independently_authored_source","object_name":"typeinfo.o","profile":"authored_cpp","sha256":"f1c028af8c78c70210b34a465a58b8a1085fb6aa21f610a3bab7673eee150976","source":"sources/w10-actor-n-tf-8superorc/typeinfo.cpp"}
{"emitted_functions":[],"header_closure":[],"kind":"independently_authored_source","object_name":"vtable.o","profile":"authored_cpp","sha256":"ba24fe050150dc824e6026303ffd8e7f8179bc0d8417a366bb8d6ac5a2884d9e","source":"sources/w10-actor-n-vt-6sparks/vtable.cpp"}
```

## Imports closed

31 of the 32 names in the task list are now defined by my objects (checked with `ld -r` over all of them): `setState__4Souli`, `_vt$6Sparks`, `_$_11SpiderQueen`, `__tf11SpiderQueen`, `clone__11SpiderQueen`, `msg_draw__11SpiderQueen`, `msg_hurt__11SpiderQueenfP10GameObjecti`, `msg_trigger__11SpiderQueeni`, `setState__11SpiderQueeni`, `_$_10StaticFire`, `__tf10StaticFire`, `clone__10StaticFire`, `msg_draw__10StaticFire`, `msg_run__10StaticFire`, `newWind__10StaticFirei`, `_$_8SuperOrc`, `__tf8SuperOrc`, `clone__8SuperOrc`, `msg_draw__8SuperOrc`, `msg_hurt__8SuperOrcfP10GameObjecti`, `msg_run__8SuperOrc`, `msg_trigger__8SuperOrci`, `setState__8SuperOrci`, `_$_6Switch`, `__tf6Switch`, `clone__6Switch`, `msg_getPokeName__6SwitchPUsi`, `msg_poke__6Switchi`, `setActiveChunks__10Teleporter`, `_$_5Torch`, `__tf5Torch`.

Still open from the task list: `msg_run__11SpiderQueen`.

Also defined, not in the task list: `__ti10StaticFire`, `__ti11SpiderQueen`, `__ti5Torch`, `__ti6Switch`, `__ti8SuperOrc` (the five `__ti*` type-info objects are defined by their `__tf*` units).

## Imports opened

Symbols my objects reference but do not define, from an `ld -r` link of only my objects (all of them are defined by other packages, by the original executable, or are the file-local data noted below):

`P_AddParticle__FP11ParticleDefP6Point3T1f`, `P_FaceSprite__FG6Point3ffiiiiP8_texturesi`, `_$_10GameObject`, `_$_5Light`, `_$_6Sparks`, `_$_8Creature`, `_$_9WorldPart`, `__15AnimationState8RC15AnimationState8`, `__8ListHeadRC8ListHead`, `__rtti_si`, `__tf10GameObject`, `__tf6Sparks`, `__tf8Creature`, `__ti10GameObject`, `__ti8Creature`, `__ti9WorldPart`, `_vt$10GameObject`, `_vt$10StaticFire`, `_vt$11SpiderQueen`, `_vt$5Torch`, `_vt$6Switch`, `_vt$8Creature`, `_vt$8SuperOrc`, `addEffect__FiG6Point3`, `addSmokelet__10StaticFire`, `animAddOneShot__FP14AnimationStateP15AnimationHeaderffi`, `animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi`, `animAdvanceAnimationTime__FP14AnimationStatefP6Point3P6Point2Ps`, `animGlobalSync__FP14AnimationState`, `animSetToEnd__FP14AnimationState`, `bExitStatePersueIdle__8Creature`, `blockAlloc__Fi`, `blockFree__FPv`, `bossMeterCurrent`, `bossMeterName`, `clone__6Sparks`, `creatureAdvanceAnimation__8Creaturef`, `cutMode`, `draw__8CreatureP12_modelHeaderP8_textureT1`, `dropTreasure__8CreatureG6Point3iffffffPc`, `eRand__10GameObject`, `enemyInZone__8Creaturefs`, `engineCurrentLevelName`, `everyFrame__8CreatureP12_modelHeader`, `frameNm`, `fxRunWeapon__FPQ24item11descriptionP12_modelHeaderP13fxWeaponStateG6Point3sT1P14AnimationStatefiifP8Matrix34`, `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb`, `gamePositionVisible__FP6Point3f`, `getModel__10GameObject`, `getSmartRandomDestination__8Creaturei`, `getSoulAttachMatrix__6MinionP8Matrix34`, `haloTexture2`, `haloTexture7`, `haloTexture8`, `iatan2__Fff`, `icos__Fi`, `isin__Fi`, `levelName`, `lightAdd__FP5Light`, `lightDeactivate__FP5Light`, `lightResetOptionalParams__Fv`, `lightSetAmbientOffset__FPs`, `listDestroy__FP8ListHead`, `matInvert__FP8Matrix34T0`, `matMul__FP8Matrix34N20`, `matTransform__FP6Point3P8Matrix34T0`, `memFree__FPv`, `modelDrawAttachment__FP12_modelHeaderP8_textureiT0P11_drawRecordUlii`, `modelGetActiveChunks__FP12_modelHeaderi`, `modelGetAttachmentMatrix__FP12_modelHeaderP14AnimationStateiP8Matrix34f`, `modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f`, `modelGetNmShadowPoints__FP12_modelHeader`, `modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f`, `modelResetOptionalParams__Fv`, `modelSetRelightAttachment__FG6Point3`, `modelSetShadowScale__Ff`, `move2__8CreatureG6Point3`, `msg_alert__10GameObjectifG6Point3P10GameObject`, `msg_collisionWorld__10GameObjectG6Point3`, `msg_collision__6SparksP10GameObject`, `msg_draw__6Sparks`, `msg_getPokeName__10GameObjectPUsi`, `msg_hurt__10GameObjectfP10GameObjecti`, `msg_hurt__8CreaturefP10GameObjecti`, `msg_load__10GameObjectPc`, `msg_moveCollision__10GameObjectPP10GameObject`, `msg_poke__10GameObjecti`, `msg_route__10GameObjecti`, `msg_run__6Sparks`, `msg_save__10GameObjectPc`, `msg_trigger__10GameObjecti`, `newDamageHandler__8CreatureP12_modelHeader`, `norrath_w10_actor_n_view_7d6d80`, `norrath_w10_actor_n_view_8d63c0`, `norrath_w10_actor_n_view_8d6460`, `objectActivationCheck__8Creaturei`, `objectAddToDeleteList__FP10GameObject`, `objectDoMyActions__FP10GameObjecti`, `objectFindInBox__FG6Point3T0PP10GameObjecti`, `objectRemoveFromRunList__FP10GameObject`, `produceMeleeDamage__8CreaturefiiiiPQ24item11description`, `resetRouting__8Creature`, `reset__3Web`, `setStateProcessing__8CreaturePi`, `slideBack__8Creaturef`, `smartWander__8Creature`, `soundFadeStream__Fi`, `soundResetStream__Fi`, `soundStartMusic__FPci`, `sprintf`, `steerAndRouteToEnemy__8Creature`, `strcasecmp`, `strcmp`, `strstr`, `superorcSlamEffect__8SuperOrcP8Creaturei`, `textGetString__FPc`, `turnToFaceEnemy__8Creature`, `updateSmokelets__10StaticFire`, `ustrcpy__FPUsT0`, `world`, `worldMoveDoor__FP12_worldHeaderiG6Point3s`

## Observed, inferred, unknown

Observed: every offset, constant, call order, branch, jump table and table slot used in the
sources comes from the original function bodies and was confirmed by the paired runs listed
above (call logs with a hash of the object at each call, float registers, and all changed
bytes outside the stack). The `setState` jump tables have 17 (SuperOrc), 21 (SpiderQueen) and
25 (Soul) entries; `msg_run__8SuperOrc` has a 17-entry table. `_vt$6Sparks` holds 18 slots.
`__tf5Torch` and `__tf10StaticFire` register one base (`__ti10GameObject`); `__tf6Switch`
registers `__ti9WorldPart` inline (it never calls `__tf9WorldPart`) and then itself.

Inferred (from symbol names and call shapes only): which class each type-info function and
destructor belongs to; that the float at +560 of the boss-like creatures is a health-like value
(it feeds the boss meter); that the pair at +180 of SuperOrc is a planar velocity; that the
member at +128 of StaticFire and +76 of Torch is a Light (from its destructor); that the ring
of 10-byte records of StaticFire is a set of smoke or flame particles; that Teleporter's
`setActiveChunks` is a deliberate no-op.

Unknown: the meaning of nearly every object field and every state number; what the
`P_FaceSprite` and `modelDrawAttachment` numeric arguments stand for; what the end-marker slot
of the vtable means to the dispatcher; the layout of the `item::description` stack record
(only the bytes the function touches are reproduced); whether the Soul matrix-carrying branch
of state 23 is ever reachable (its guard makes it unreachable from `setState__4Souli` itself).

## Authored but never executed

All of it ran only in the private interpreter and, for the host contracts, on the host.
Nothing has run on a PS2 or inside the game. Specifically not exercised by any run:

- the entry-search loop of `msg_run__10StaticFire` beyond the first entry (the record is built
  fresh, so only entry 0 is ever chosen); the loop is kept for fidelity;
- the Soul matrix branch of state 23 (see above) and the SuperOrc state-13 slam with a zero
  word at +964 in the paired runs (it is covered by the host contract only);
- everything float-related is checked under the interpreter's round-to-nearest, not the
  hardware's behaviour.

## Not closed (blocked) / needing another owner

- `fireParticle` (0x008d63c0), `sparkParticle` (0x008d6460) and `soulParticle` (0x007d6d80) are
  file-local objects of the original and live in the static-initialiser translation units of
  other packages. My sources reference them as `norrath_w10_actor_n_view_8d63c0`,
  `norrath_w10_actor_n_view_8d6460` and `norrath_w10_actor_n_view_7d6d80`. The integrator needs
  either offset aliases or a change to export those objects from the initialiser units; not
  guessed here.
- `levelName`, `cutMode`, `frameNm`, `bossMeterName`, `bossMeterCurrent`, `engineCurrentLevelName`,
  `haloTexture2/7/8` and the callee names listed under "Imports opened" are owned by other
  packages; I left them unresolved.
- `__ti5Torch`, `__ti10StaticFire`, `__ti11SpiderQueen`, `__ti8SuperOrc` and `__ti6Switch`
  (12-byte zero-initialised objects) are defined by my type-info units. If another package also
  defines any of them the link will report a duplicate.
- The Soul/SpiderQueen `msg_*` functions not in my scope (for example `msg_run__11SpiderQueen`
  is covered here only if it appears in the table above) stay with whoever owns them.
