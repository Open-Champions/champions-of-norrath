# Prop run and draw slots

Package `prop-run-draw`, branch `agents/prop-run-draw`, base `20a5902` of the shared reconstruction draft. The 21 names in the brief were all unresolved imports of the partial build at that base (950 unresolved symbols in `unresolved-imports.json`). Addresses and sizes below are symbol values of the private reference `work/private/SLUS_205.65`, taken with the EE `nm`, and the bodies were read with the EE `objdump`. No game bytes, tables or listings are committed; the few constants and strings quoted are the values the code uses.

Each function is its own commit, compiled with the reviewed EE `g++` `authored_cpp` flags, and has a `host_check.cpp` that returns non-zero when a call, argument, order, store or compare is wrong. Host checks define `NORRATH_VSLOTS_B_HOST` where they use `vslots-b-bytes/bytes.hpp` and are not build inputs. Most checks were also run against hand-made mutants (changed offset, constant, compare or order); the mutants that survived are listed below. Nothing here has run on the EE or as gameplay, and compiled bodies are larger than the image extents (these are behaviour reconstructions, not byte matches).

## Functions

| symbol | image | extent | compiled | source |
|---|---|---|---|---|
| `msg_run__4Prop` | `0x147e30` | 780 | 2472 | `prop-run-draw-prop-run/msg_run.cpp` |
| `msg_draw__13UserParamProp` | `0x14a910` | 296 | 384 | `prop-run-draw-userparamprop-draw/msg_draw.cpp` |
| `msg_draw__4Prop` | `0x154828` | 308 | 420 | `prop-run-draw-prop-draw/msg_draw.cpp` |
| `msg_run__13UserParamProp` | `0x154bb0` | 72 | 100 | `prop-run-draw-userparamprop-run/msg_run.cpp` |
| `msg_draw__16CosmeticPropAnim` | `0x155110` | 168 | 208 | `prop-run-draw-cosmeticpropanim-draw/msg_draw.cpp` |
| `msg_run__16CosmeticPropAnim` | `0x1551b8` | 68 | 60 | `prop-run-draw-cosmeticpropanim-run/msg_run.cpp` |
| `msg_run__3Cat` | `0x1ee528` | 880 | 1260 | `prop-run-draw-cat-run/msg_run.cpp` |
| `msg_run__7Mermaid` | `0x1f7d80` | 392 | 568 | `prop-run-draw-mermaid-run/msg_run.cpp` |
| `msg_draw__14GnomeNavigator` | `0x201df8` | 472 | 600 | `prop-run-draw-gnomenavigator-draw/msg_draw.cpp` |
| `msg_run__14GnomeNavigator` | `0x201fd0` | 476 | 704 | `prop-run-draw-gnomenavigator-run/msg_run.cpp` |
| `msg_run__11SmallSpider` | `0x2098f8` | 280 | 376 | `prop-run-draw-smallspider-run/msg_run.cpp` |
| `msg_run__5Lever` | `0x215e28` | 300 | 532 | `prop-run-draw-lever-run/msg_run.cpp` |
| `msg_trigger__7Counteri` | `0x2162a0` | 224 | 280 | `prop-run-draw-counter-trigger/msg_trigger.cpp` |
| `msg_draw__9CdEyeball` | `0x216878` | 244 | 812 | `prop-run-draw-cdeyeball-draw/msg_draw.cpp` |
| `msg_run__4Trap` | `0x21b298` | 408 | 484 | `prop-run-draw-trap-run/msg_run.cpp` |
| `msg_draw__5Lever` | `0x21cad8` | 236 | 340 | `prop-run-draw-lever-draw/msg_draw.cpp` |
| `msg_run__9CdEyeball` | `0x21d1e0` | 192 | 340 | `prop-run-draw-cdeyeball-run/msg_run.cpp` |
| `msg_draw__7Natasla` | `0x233400` | 940 | 540 | `prop-run-draw-natasla-draw/msg_draw.cpp` |
| `msg_trigger__10GameObjecti` | `0x312e50` | 8 | 8 | `prop-run-draw-msg-trigger-gameobject/msg_trigger.cpp` |
| `msg_run__10GameObject` | `0x312e58` | 8 | 8 | `prop-run-draw-msg-run-gameobject/msg_run.cpp` |
| `msg_draw__10GameObject` | `0x312e60` | 8 | 8 | `prop-run-draw-msg-draw-gameobject/msg_draw.cpp` |

## What each body does (observed)

The per-function comment at the top of each source is the full record. In short:

- `msg_run/draw/trigger__10GameObject`: eight-byte `jr ra; nop` weak symbols. They are the same bodies that `gameobject-base` authored as `norrath_vt_msg_*_10GameObject`; the real names are defined here so the imports close, and the old names stay for the vtables that reference them.
- `Prop`: draw builds a yaw matrix when bit 0 of halfword +320 is clear, sets an ambient offset while `frameNm - 3 < word +260`, and draws. Run moves with `worldClipMotion`, applies gravity against `worldFindStandHeight`, and counts down halfword +270 (removing the object from the run list at zero).
- `UserParamProp`: run advances the animation and calls `objectActiveProcessing`. Draw sets a halo (an 8-byte static in `.sdata`, initial bytes `ff ff ff ff 00 80 cc 43`) from `isin(frameNm << 9)`, draws a shadow and the model.
- `CosmeticPropAnim`: the same draw/run shape with a 32-bit flags word.
- `Lever`: run steps a single at +76 by `direction * 1.5 / 60`, clamps to [0, 1], then reads the tags `onOnly` and `offOnly`. Draw rotates through a hermite curve.
- `Counter`: trigger counts at +76 against the `count` tag and then reads `onOnly`/`offOnly`.
- `Trap`: run queries a box around the object, remembers in four bytes (+76..+79) which players were inside last frame, and calls `gameAttackResolver` once for each that just entered.
- `Cat`, `Mermaid`, `SmallSpider`, `GnomeNavigator` run: shared prologue (`newDamageHandler`, `everyFrame`, activation check at 600, animation advance, slide back), then a state switch on halfword +572 (jump tables at `0x46d9f0` for Cat, `0x46e990` for Mermaid, `0x46f440` for GnomeNavigator). State changes go through virtual slot byte 184 (`setState__8Creaturei`, per the vslots-b record for `stun`).
- `CdEyeball`: run calls slots 168 and 176 and limits the facing halfword to within 6000 of halfword +962. Draw builds a scaled yaw matrix and draws two models.
- `Natasla` draw: five attachment-driven part draws, each with a different rotation axis and shift of halfword +808.
- `GnomeNavigator` draw: draws the creature, then an exclamation icon when the camera word allows, a script returns 1 and the `exclamation` tag exists.

## Inferred

- Slot byte 184 is `setState__8Creaturei` (taken from the vslots-b `stun` record, not re-derived). Slot bytes 168 and 176 are called with no argument registers set before them, so they are modelled as `(this)` and `(this, 0, Point3 copy)`; their names are unknown.
- Where the image reads flags with one 64-bit load (`Prop` +232, `UserParamProp` +352, `CdEyeball` +552) the callee parameter is declared `unsigned long long`. `modelDraw`'s mangled name says `Ul` (32 bits on this target), so only the low word is established as used.
- `matTransform` writes its first argument from its third, and `matTransform`'s matrix is `worldViewMatrix` (`0x37ef30`): inferred from which buffer is read afterwards.
- The `Trap` seen-array index (word +960 of the listed object) is not bounds checked in the image; the source mirrors that, so an index above 3 is undefined behaviour on the host.
- Counter `msg_trigger` compares the new value after an increment but the old value after a decrement; that asymmetry is observed in the image and kept.

## Unknown

Names and meanings of every object field used here (+76, +80, +81, +252.., +256, +260, +268, +270, +320, +572, +576, +804, +808, +960, +962, +968..+972 and the rest), of the state numbers, of slots 168 and 176, of the `Lever` hermite limits (-7000/7000) and of all float constants. The unsigned-versus-signed meaning of the `eRand` return (the image uses `divu`, so it is treated as unsigned). The return types of the called functions the image ignores (declared `void` or `int` as used). R5900 NaN/denormal behaviour of the float paths in `Prop` run, `UserParamProp` draw, `Lever` run and `GnomeNavigator` draw: the host checks run IEEE arithmetic.

## Host-check gaps

Mutants that survived, each shown equivalent: `Prop` run `k1 < sumsq` versus `<=` and `fall < K5` versus `<=` (the equal float values are unreachable), `depth > 0` versus `>= 0` (the added term is zero); `CdEyeball` run `d > 6000` versus `>= 6000` and the `-6000` mirror (the stored value equals the current one); `Mermaid` run reading the state as unsigned halfword instead of sign-extended (negative states still land in the default arm). `GnomeNavigator` draw also scales the third transform component in the image; nothing reads it, so the source omits it.

## Imports closed (21)

- `msg_draw__10GameObject`
- `msg_draw__13UserParamProp`
- `msg_draw__14GnomeNavigator`
- `msg_draw__16CosmeticPropAnim`
- `msg_draw__4Prop`
- `msg_draw__5Lever`
- `msg_draw__7Natasla`
- `msg_draw__9CdEyeball`
- `msg_run__10GameObject`
- `msg_run__11SmallSpider`
- `msg_run__13UserParamProp`
- `msg_run__14GnomeNavigator`
- `msg_run__16CosmeticPropAnim`
- `msg_run__3Cat`
- `msg_run__4Prop`
- `msg_run__4Trap`
- `msg_run__5Lever`
- `msg_run__7Mermaid`
- `msg_run__9CdEyeball`
- `msg_trigger__10GameObjecti`
- `msg_trigger__7Counteri`

## Imports opened (30)

Found by diffing `unresolved-imports.json` of the base build against the build with these sources (950 to 959 unresolved symbols: 21 closed, 30 new). None of these is defined in the plan yet. `worldViewMatrix` is a data word (64 bytes at `0x37ef30`) and needs a definition by its owner.

- `addTurnAnimation__3Cat`
- `animAddOneShot__FP14AnimationStateP15AnimationHeaderffi`
- `animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps`
- `bExitStatePersueIdle__8Creature`
- `bFarFromHome__8Creature`
- `eRand__10GameObject`
- `evalHermite__Ffffff`
- `gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb`
- `lightSetAmbientOffset__FPs`
- `matIdent__FP8Matrix34`
- `matMakeTransYawScale__FP8Matrix34G6Point3sf`
- `matRot__FP8Matrix34is`
- `matTrans__FP8Matrix34fff`
- `matTransform__FP6Point3P8Matrix44T0`
- `modelDrawShadow__FP12_modelHeaderiP8Matrix34P14AnimationStatec`
- `modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f`
- `modelSetHalo__FP7HaloDefi`
- `move2__8CreatureG6Point3`
- `newDamageHandler__8CreatureP12_modelHeader`
- `objectActiveProcessing__FP10GameObjectii`
- `readyToSwitch__3Cat`
- `slideBack__8Creaturef`
- `smartWander__8Creature`
- `steerAndRouteToPosition__8CreatureG6Point3`
- `subTurnAnimation__3Cat`
- `switchToRun__3Cat`
- `switchToWalk__3Cat`
- `worldClipMotion__FP12_worldHeaderG6Point3T1fiP6Point3`
- `worldFindStandHeight__FP12_worldHeaderG6Point3f`
- `worldViewMatrix`

Already defined and reused instead of opened: `frameNm`, `gameCam`, `memset`, `lumpFindResource__FPcT0`, `norrath_actors_b_view_4844a0` (the `world` word at `0x4844a0`), `norrath_actor_find_tag_string`, `norrath_game_object_find_tag_int`, `draw__8CreatureP12_modelHeaderP8_textureT1`, `objectActivationCheck__8Creaturei`, `objectRemoveFromRunList__FP10GameObject`, `objectUpdateInGrid__FP10GameObject`, `objectDoMyActions__FP10GameObjecti`, `matMakeTransYaw__FP8Matrix34G6Point3s`, `matRot__FP8Matrix33is`, `modelDraw__...`, `isin__Fi`, `scriptStart__FPc`, `textureDrawColor__FP8_textureiiiUi`, `modelResetOptionalParams__Fv`, `lightResetOptionalParams__Fv`, `everyFrame__8CreatureP12_modelHeader`, `creatureAdvanceAnimation__8Creaturef`, `objectFindInBox__FG6Point3T0PP10GameObjecti`.

## Build-plan entries needed

`profile` is `authored_cpp` for all. Header closure is `sources/vslots-b-bytes/bytes.hpp` where the source includes it (all except the three GameObject files, which include nothing). Entries go after the current last input (`input_order` 909 at the base). `snapshot.json` needs matching input rows (`emitted_functions` are the symbol and compiled bytes in the table; `cat-run` and `gnomenavigator-run` also emit a local `_ZN12_GLOBAL__N_1L9set_stateEPvi` of 48 bytes). A private plan with these 21 entries built cleanly from the base: 929 source compilations, 788 function bodies, 959 unresolved symbols.

| object_name | source | sha256 |
|---|---|---|
| `msg_run_cat_run.o` | `sources/prop-run-draw-cat-run/msg_run.cpp` | `d96169c1e5b4f67dbde5c25cc2069f6626808cbeb73c769abf88b6d5b2f5c2ba` |
| `msg_draw_cdeyeball_draw.o` | `sources/prop-run-draw-cdeyeball-draw/msg_draw.cpp` | `8ace606b5bd2135c0819434908d4ca683d4b8a3ac0d55b051802496384da89c4` |
| `msg_run_cdeyeball_run.o` | `sources/prop-run-draw-cdeyeball-run/msg_run.cpp` | `9bc368c2222ccd5e9ee6183f6ac4f55879a1efe7e2a8ab4517b7b6668f69a884` |
| `msg_draw_cosmeticpropanim_draw.o` | `sources/prop-run-draw-cosmeticpropanim-draw/msg_draw.cpp` | `4653103a81127bc6cd4a825c2823db389ce2075337a549a0ee16939ee05c2d1b` |
| `msg_run_cosmeticpropanim_run.o` | `sources/prop-run-draw-cosmeticpropanim-run/msg_run.cpp` | `145155757118fc73f506933914a834ad4a7ea6d157d42b98fa19bf1832b9650c` |
| `msg_trigger_counter_trigger.o` | `sources/prop-run-draw-counter-trigger/msg_trigger.cpp` | `40aad4239644c3ee94f1e80ebb290c69e597e939e0060ff1ae6d59d073d282ac` |
| `msg_draw_gnomenavigator_draw.o` | `sources/prop-run-draw-gnomenavigator-draw/msg_draw.cpp` | `8e0d44eb79559cacccf0fe1f3faeae9236b534d26188c8e5780ca8dcde8c6e93` |
| `msg_run_gnomenavigator_run.o` | `sources/prop-run-draw-gnomenavigator-run/msg_run.cpp` | `195850c2f5a5c0504a0d04ab194ee1f41814d36c7afed62dfc287abe1b11d500` |
| `msg_draw_lever_draw.o` | `sources/prop-run-draw-lever-draw/msg_draw.cpp` | `de1d6e34ba5e830eecdbf9a0b6b6dd2a41dd938652efc32032a5bcfd907e798e` |
| `msg_run_lever_run.o` | `sources/prop-run-draw-lever-run/msg_run.cpp` | `2df9ebdb1856127b04502e631208d3c9821bc11ffca5d6c1e9e48518b4b3ac64` |
| `msg_run_mermaid_run.o` | `sources/prop-run-draw-mermaid-run/msg_run.cpp` | `083bcce6d3481daa331ff7d91c3e4bdb82ce2ea6260d5f7827f0e3d07d41d5f6` |
| `msg_draw_msg_draw_gameobject.o` | `sources/prop-run-draw-msg-draw-gameobject/msg_draw.cpp` | `d975601a004b72d781e7c44a4539510872c2596f315ee73a0404ff5ef8a513a5` |
| `msg_run_msg_run_gameobject.o` | `sources/prop-run-draw-msg-run-gameobject/msg_run.cpp` | `2303affabb20c04035bbff7a63fccad6240dcfb817753a10951ffc358df66b9b` |
| `msg_trigger_msg_trigger_gameobject.o` | `sources/prop-run-draw-msg-trigger-gameobject/msg_trigger.cpp` | `a07d590c1a00df0c0d3b84cd600127e6d47c0eb412d8eb97e9dec55a7e7d92b1` |
| `msg_draw_natasla_draw.o` | `sources/prop-run-draw-natasla-draw/msg_draw.cpp` | `05c236ac28bf15b25677427f53963386159e7bd02cf2fca691f5196d16e52d44` |
| `msg_draw_prop_draw.o` | `sources/prop-run-draw-prop-draw/msg_draw.cpp` | `b7d7e4c504cfd3c28b8e8dd1c64e328fb80c06bc2352f2d20e26b2d493e15a49` |
| `msg_run_prop_run.o` | `sources/prop-run-draw-prop-run/msg_run.cpp` | `3d81796eeddb1bed90d285f28b720d03161389c0360c998e8cc3f682f7cad9c2` |
| `msg_run_smallspider_run.o` | `sources/prop-run-draw-smallspider-run/msg_run.cpp` | `50cd7ec832ec9a9464c85aad61211c6f2ae51139155e3ad6588492d8d7e0da9d` |
| `msg_run_trap_run.o` | `sources/prop-run-draw-trap-run/msg_run.cpp` | `e87cb9b7bf6feef6cc28fe2d08d585cc452409b693c2d42dda717e5f0ab73fb3` |
| `msg_draw_userparamprop_draw.o` | `sources/prop-run-draw-userparamprop-draw/msg_draw.cpp` | `1a63baeba128dd788e98f448e4dd19c6952a5a1293a03dc44d6f77b3ea60b51c` |
| `msg_run_userparamprop_run.o` | `sources/prop-run-draw-userparamprop-run/msg_run.cpp` | `8be57ecc67cd94a149d4e93199f2418f316477ca04b92674d2811bd878b68e78` |

Note: `msg_run.o` and `msg_draw.o` style names repeat across packages; the private plan used `<stem>_<component>.o` to keep object names unique. The lead integrator should choose names that do not collide with existing `msg_run.o`/`msg_draw.o` entries.

## Authored but never executed

All 21 functions. They compiled and linked in a private relocatable build and passed host checks on the build machine, with every callee replaced by a recording stub. None has run on the EE, against the real callees, or under gameplay. The `Prop` halo static and the `Trap` seen bytes have never been read back by anything but their own host check.

## Tests

`python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests). `snapshot.json` and `build-plan.json` were not edited.
