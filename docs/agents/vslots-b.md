# Virtual-slot methods, second half

Package `agents/vslots-b`. The 95 names opened by the data-views merge were sorted in plain byte order. This package is numbers 49 through 95. The partial link of the shared tip (`999b562`, 803 authored inputs) still lists all 95, so none of this half was already defined. Direct `jal` counts are from the pinned `SLUS_205.65` `.text` and `.srs` sections. Pointer counts are absolute words in allocated data. Nothing here has run on the EE or as gameplay.

Work is ordered by how often other code names the symbol, not by the byte-order split. Creature methods sit in dozens of vtables. The single-slot actor methods follow them.

`sources/vslots-b-bytes/bytes.hpp` (`c5fea9b60c5c91b0e84169c526dbf41d4cb6dd4b2206f0daa586a117fc2e5578`) is the field helper. It is a header closure, not its own plan input. Host checks define `NORRATH_VSLOTS_B_HOST` and are not plan inputs.

## msg_route__8Creaturei

Observed at `0x113488`, 44 bytes. 53 data pointers, no direct `jal`. The reviewed compile emits 140 bytes. A nonzero argument clears the words at object+708 and object+716. A zero argument clears object+708, stores -1 at object+580 and object+584, and adds one to object+716, wrapping in 32 bits.

## stun__8Creaturei

Observed at `0x112be0`, 80 bytes. 52 data pointers, no direct `jal`. The reviewed compile emits 132 bytes. Signed halfword 10 at object+572 returns without a write. Any other halfword stores object+808 plus the argument at object+804, then calls virtual slot byte 184 with integer 9. That creature slot is `setState__8Creaturei`, loaded through the object's vtable, so this unit does not import it.

## setActiveChunks__8Creature

Observed at `0x113828`, 104 bytes. 51 data pointers, no direct `jal` of this symbol. The reviewed compile emits 164 bytes. Virtual slot byte 128 (`getModel__8Creature` on the creature vtable) is called once. A null return stops and leaves object+552 unchanged. A non-null return calls that slot again. `modelGetActiveChunks__FP12_modelHeaderi` receives the second return and the word at object+244. The eight-byte return register is stored at object+552. The callee is not defined in this package.

## msg_hurt__8CreaturefP10GameObjecti

Observed at `0x113618`, 240 bytes. 50 data pointers and 14 direct calls. The reviewed compile emits 868 bytes. The reviewed EE body keeps the amount in `f12`, the other object in `a1`, and the integer in `a2`. A nonzero byte at object+570 returns immediately. Otherwise `frameNm` is copied to object+812, the integer is ORed into object+744, and the singles at object+560 and object+192 move by the amount after any list work. A non-null other object and a positive single at object+560 also call `iatan2__Fff`, `norrath_list_destroy`, and maybe `norrath_list_oneway_link`. Those two list names are already defined. `frameNm` is already defined.

## Later bodies

Observed address, image size, and reviewed compile size:

| Symbol | Address | Image | Compiled |
|---|---|---|---|
| `msg_moveCollision__8CreaturePP10GameObject` | `0x113738` | 52 | 56 |
| `msg_trigger__8Creaturei` | `0x1137b0` | 116 | 144 |
| `setState__8Creaturei` | `0x30d560` | 8 | 16 |
| `msg_draw__4Trap` | `0x21def0` | 8 | 8 |
| `msg_save__7CounterPc` | `0x21cd68` | 24 | 44 |
| `msg_load__7CounterPc` | `0x21cd80` | 24 | 44 |
| `msg_save__5LeverPc` | `0x21c9a0` | 60 | 96 |
| `msg_trigger__13TriggerParamsi` | `0x1d1a60` | 44 | 60 |
| `msg_run__5Timer` | `0x21ccc8` | 64 | 92 |
| `msg_trigger__5Timeri` | `0x21cc70` | 88 | 136 |
| `msg_draw__7Mermaid` | `0x1f8060` | 40 | 32 |
| `msg_draw__11SmallSpider` | `0x20f8d0` | 72 | 68 |
| `msg_draw__3Cat` | `0x1eed90` | 96 | 164 |
| `msg_run__7Natasla` | `0x2337b0` | 80 | 96 |
| `runSpells__8CreatureG6Point3` | `0x113978` | 180 | 212 |
| `msg_poke__8Creaturei` | `0x111df0` | 372 | 572 |
| `turnToFace__8CreatureiG6Point3` | `0x10a2a8` | 380 | 1800 |
| `setState__7Nataslai` | `0x233800` | 116 | 172 |
| `msg_load__5LeverPc` | `0x21c9e0` | 112 | 136 |

Move-collision reads 4-byte list words until zero and stores nothing. Creature trigger ignores its integer, requires a nonzero word at object+892, and passes the bytes `dialogScript` through the existing `norrath_actor_find_tag_string`. `setState__8Creaturei` was weak in the image; this definition stores the low 16 bits at object+572. Trap draw returns without a write. Counter save and load move one word at object+76 and return 4. Lever save copies 10 bytes: the bits at object+76, the word at object+12, and bytes 80 and 81. TriggerParams stores 0 or -1 at the target object's +100. Timer run wraps the word at object+76 and, unless the signed result is positive, calls out. Timer trigger multiplies the `time` tag by 60. Mermaid, spider, and cat draw pass a model and a texture into `draw__8CreatureP12_modelHeaderP8_textureT1`. Natasla run stops when `everyFrame` returns nonzero. `runSpells` copies 12 point bytes, walks the list at object+888 with the existing safe iterator at mask 0, calls table slot byte 16 on each element's word at +20, and destroys the iterator with flag 2. Creature poke returns on a zero word at object+892 or a zero entity word at `player` byte 21872 of record `index * 23984`. Tag `shop` with someone in the shop starts the shop for a local player. Otherwise `dx * icos + dy * isin` must be greater than zero, `DLG_SetMutex` must be zero, and the argument is stored in `activePlayer` before `dialogScript` is passed to `scriptStart`. The image also subtracts the singles at byte 24 and does not read that difference. `turnToFace` calls `iatan2` with the point difference, adds 32768 when the flag is 1, and writes the clamped half at object+284 back into object+56. Byte object+800 selects the 1..60 scale. Natasla state 0 calls `animAddTransitionTo` before storing the argument at object+572. States 2, 4, 7, and every unsigned value at or above 11 also store that half. The other states below 11 leave it. Lever load restores the same 10 bytes lever save writes, and a nonzero last byte calls the existing run-list add.

`norrath_actor_add_run_list`, `norrath_game_object_find_tag_int`, `norrath_actor_find_tag_string`, and the three `norrath_list_safe_iterator_*` names are already defined. `scriptStart__FPc` was already unresolved.

## Imports closed

- `msg_route__8Creaturei`
- `stun__8Creaturei`
- `setActiveChunks__8Creature`
- `msg_hurt__8CreaturefP10GameObjecti`
- `msg_moveCollision__8CreaturePP10GameObject`
- `msg_trigger__8Creaturei`
- `setState__8Creaturei`
- `msg_draw__4Trap`
- `msg_save__7CounterPc`
- `msg_load__7CounterPc`
- `msg_save__5LeverPc`
- `msg_trigger__13TriggerParamsi`
- `msg_run__5Timer`
- `msg_trigger__5Timeri`
- `msg_draw__7Mermaid`
- `msg_draw__11SmallSpider`
- `msg_draw__3Cat`
- `msg_run__7Natasla`
- `runSpells__8CreatureG6Point3`
- `msg_poke__8Creaturei`
- `turnToFace__8CreatureiG6Point3`
- `setState__7Nataslai`
- `msg_load__5LeverPc`

## Imports opened

- `modelGetActiveChunks__FP12_modelHeaderi`
- `iatan2__Fff`
- `objectDoMyActions__FP10GameObjecti`
- `objectRemoveFromRunList__FP10GameObject` — `norrath_actor_remove_from_run_list` is a different authored name and was not called
- `draw__8CreatureP12_modelHeaderP8_textureT1`
- `modelSetShadowBrightness__Ff`
- `modelResetOptionalParams__Fv`
- `everyFrame__8CreatureP12_modelHeader`
- `objectActivationCheck__8Creaturei`
- `creatureAdvanceAnimation__8Creaturef`
- `DLG_SetMutex__Fv`
- `gameSomeoneInShop__Fv`

`player`, `activePlayer`, `icos__Fi`, `isin__Fi`, `shopStart__Fiii`, `scriptStart__FPc`, and `animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi` were already unresolved. `netPlayerIsLocal__Fi` is already defined.

## Build-plan entries needed

Profile `authored_cpp`. Do not add `host_check.cpp`. The integrator assigns `input_order`.

```json
{"source":"sources/vslots-b-msg-route/msg_route.cpp","object_name":"msg_route.o","sha256":"3de93b207493a65b0f548bdc3e960c3efceee0af8e80f72e08b0d4de49d2519b","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_route__8Creaturei","compiled_body_bytes":140}]}
{"source":"sources/vslots-b-stun/stun.cpp","object_name":"stun.o","sha256":"15d2d41f26653e5343a7ea235d8b2e2446bd750435f7878dde69fca87853d612","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"stun__8Creaturei","compiled_body_bytes":132}]}
{"source":"sources/vslots-b-set-active-chunks/set_active_chunks.cpp","object_name":"set_active_chunks.o","sha256":"864b2a370c54e0b2a715bc4f0822f6b96d27f0fa2886480e8c63a519e23f40c4","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"setActiveChunks__8Creature","compiled_body_bytes":164}]}
{"source":"sources/vslots-b-msg-hurt/msg_hurt.cpp","object_name":"msg_hurt.o","sha256":"27d1e7b429b1b74e7448ec6fc87716d101a81511de91f0fad440ff1a7a52e2f8","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_hurt__8CreaturefP10GameObjecti","compiled_body_bytes":868}]}
{"source":"sources/vslots-b-msg-move-collision/msg_move_collision.cpp","object_name":"msg_move_collision.o","sha256":"97bd556689f3c5319955b1761a84af52749982a748478d518d6ceabc526e16a8","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_moveCollision__8CreaturePP10GameObject","compiled_body_bytes":56}]}
{"source":"sources/vslots-b-msg-trigger/msg_trigger.cpp","object_name":"msg_trigger.o","sha256":"adb9fa7b14ad67602fe247e4bc0694d87fb75264dabe1e6c191f0f51ccf80b1d","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_trigger__8Creaturei","compiled_body_bytes":144}]}
{"source":"sources/vslots-b-set-state/set_state.cpp","object_name":"set_state.o","sha256":"927b7781dee9c1bc95404d9f718a6b9e7157764a079bdaba30820a302c4aafc8","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"setState__8Creaturei","compiled_body_bytes":16}]}
{"source":"sources/vslots-b-draw-trap/draw_trap.cpp","object_name":"draw_trap.o","sha256":"1bb3261aab0ea9d25082123ef55e5fcf5d16758c78f6ccad6ede1cd664d56b00","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_draw__4Trap","compiled_body_bytes":8}]}
{"source":"sources/vslots-b-counter-save/counter_save.cpp","object_name":"counter_save.o","sha256":"743a3988832f58fc9f1525bc90755532429153f0a25339e8e30e76e2cc218745","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_save__7CounterPc","compiled_body_bytes":44}]}
{"source":"sources/vslots-b-counter-load/counter_load.cpp","object_name":"counter_load.o","sha256":"09716749e7a7e3657faa2dd1b985915e2b0302df004e7d3da9558b0b56070300","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_load__7CounterPc","compiled_body_bytes":44}]}
{"source":"sources/vslots-b-lever-save/lever_save.cpp","object_name":"lever_save.o","sha256":"9cd23d239aa3500d6c37db6d57ed612e61d386f652fda03ceef15ca31476e8c6","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_save__5LeverPc","compiled_body_bytes":96}]}
{"source":"sources/vslots-b-trigger-params/trigger_params.cpp","object_name":"trigger_params.o","sha256":"f22de0042b8aff85dd238a02d2436ed17ff89dcc41c85be40b40f82438483158","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_trigger__13TriggerParamsi","compiled_body_bytes":60}]}
{"source":"sources/vslots-b-run-timer/run_timer.cpp","object_name":"run_timer.o","sha256":"37786ab6237593847f205bd8168650243bedeedd0ce388661a0352156da43d59","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_run__5Timer","compiled_body_bytes":92}]}
{"source":"sources/vslots-b-trigger-timer/trigger_timer.cpp","object_name":"trigger_timer.o","sha256":"37ea6fd29dfc763602f041428537e2f21c6d771578c796472adf29c7230178cc","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_trigger__5Timeri","compiled_body_bytes":136}]}
{"source":"sources/vslots-b-draw-mermaid/draw_mermaid.cpp","object_name":"draw_mermaid.o","sha256":"2ace9a8ee79d59fea17066a6f97b73970895aabb03120ea296035275f4eb1f97","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_draw__7Mermaid","compiled_body_bytes":32}]}
{"source":"sources/vslots-b-draw-spider/draw_spider.cpp","object_name":"draw_spider.o","sha256":"b61c3f3bae2a5a0d0b6c8b417a7b49bc49bd17ad2c4aa57e27dc773711ee30e7","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_draw__11SmallSpider","compiled_body_bytes":68}]}
{"source":"sources/vslots-b-draw-cat/draw_cat.cpp","object_name":"draw_cat.o","sha256":"7ffce26c2ed0207d064678de5d8c8daedca75aa53e11d54f44cfe3143cad7246","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_draw__3Cat","compiled_body_bytes":164}]}
{"source":"sources/vslots-b-run-natasla/run_natasla.cpp","object_name":"run_natasla.o","sha256":"c23b2bf8a2c2ca39c46c2ce31f0c3c1a8411b0ef7b77f928660a01a28c9ea1b0","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_run__7Natasla","compiled_body_bytes":96}]}
{"source":"sources/vslots-b-run-spells/run_spells.cpp","object_name":"run_spells.o","sha256":"f1e9856cf96df91665a611966ece846bf528a8ee57ed0310e3ea5b5d5412a836","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"runSpells__8CreatureG6Point3","compiled_body_bytes":212}]}
{"source":"sources/vslots-b-msg-poke/msg_poke.cpp","object_name":"msg_poke.o","sha256":"e393ec9e2fbe267f5c2081e4d5edc93b674b0015578eff6fe1fda4e43ff14449","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_poke__8Creaturei","compiled_body_bytes":572}]}
{"source":"sources/vslots-b-turn-to-face/turn_to_face.cpp","object_name":"turn_to_face.o","sha256":"0d8ac14855922ff22dfad939b4b0413537fc20148477590ee6b2ad4af16bb351","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"turnToFace__8CreatureiG6Point3","compiled_body_bytes":1800}]}
{"source":"sources/vslots-b-set-state-natasla/set_state_natasla.cpp","object_name":"set_state_natasla.o","sha256":"b29e430490b46876839b504dda496ffee8d46574ca4502f9c4851bb738655aed","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"setState__7Nataslai","compiled_body_bytes":172}]}
{"source":"sources/vslots-b-lever-load/lever_load.cpp","object_name":"lever_load.o","sha256":"d13b04112e241d6ddcafd5d5f248aed5f3128e2625f65574c4a9d03db401a8fc","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_load__5LeverPc","compiled_body_bytes":136}]}
```

## Authored but never executed

All 23 functions listed under imports closed. Host checks ran on this machine. The reviewed EE compiler produced relocatable objects. No EE, original-process, or gameplay run. `simpleDrawLoop` and the earlier startup chain are outside this package and were not executed here.

## Unknown

Field words and state numbers above are not named. The upper half of the `modelGetActiveChunks` return is stored because the caller writes eight bytes; what that callee puts in the upper half is not established. R5900 NaN and denormal results of the hurt compare and its two single updates are not established. `turnToFace` uses round-to-nearest, ties to even, for `cvt.w.s`. Whether R5900 `sqrt.s` matches the IEEE square root used here is not established. These 24 symbols in this half are not reconstructed: `msg_draw__14GnomeNavigator`, `msg_draw__5Lever`, `msg_draw__7Natasla`, `msg_draw__9CdEyeball`, `msg_getPokeName__13PokeReflectorPUsi`, `msg_getPokeName__5LeverPUsi`, `msg_getPokeName__8CreaturePUsi`, `msg_load__8CreaturePc`, `msg_poke__13PokeReflectori`, `msg_poke__5Leveri`, `msg_run__11SmallSpider`, `msg_run__14GnomeNavigator`, `msg_run__3Cat`, `msg_run__4Trap`, `msg_run__5Lever`, `msg_run__7Mermaid`, `msg_run__9CdEyeball`, `msg_save__8CreaturePc`, `msg_trigger__7Counteri`, `setState__11SmallSpideri`, `setState__14GnomeNavigatori`, `setState__3Cati`, `setState__7Mermaidi`, `updateEnemy__8Creature`. The largest of those are `updateEnemy__8Creature` (1212 bytes) and `msg_getPokeName__8CreaturePUsi` (980 bytes). Their callees and object fields were not established in this pass.
