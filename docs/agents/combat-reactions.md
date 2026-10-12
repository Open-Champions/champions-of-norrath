# Combat reactions

Package `agents/combat-reactions`. Addresses and sizes are from the pinned private `SLUS_205.65` (`.text`). Nothing here has run on the EE or as gameplay. The compiled sizes are the reviewed EE g++ `authored_cpp` objects and are not a claim of byte equality with the image.

## Evidence

Each function has a host check beside it (`host_check.cpp`, not a plan input) that fails on a wrong branch, offset, or call order. The seven non-trivial bodies were also compared against the image: `docs/agents/combat-reactions-compare.py` runs the image's instructions and the freshly linked authored object in a small R5900 interpreter with the same randomized object and the same stubbed callees, and requires equal stub call logs (argument registers and point bytes included) and equal object bytes. The last run was 1500 seeds each for `setState__11SmallSpideri`, `setState__3Cati`, `setState__7Mermaidi`, `setState__14GnomeNavigatori`, `charm__8Creaturei`, `msg_collision__4GoldP10GameObject` and `updateEnemy__8Creature`, with zero mismatches. Changing a flag constant in the Gnome body, or the 480 and 50.0 thresholds in `updateEnemy`, makes the check fail. It needs the private executable and the toolchain through `NORRATH_EXE` and `NORRATH_EE`; floats in its scenarios are small integers because the interpreter rounds to nearest and the EE truncates.

Observed, inferred and unknown are kept in each source's header comment. Short form:

- The three no-op bodies and the Creature `msg_alert` copy the 12-byte Point3 argument to their own frame and return. The authored bodies are empty.
- `charm__8Creaturei` (0x112ba0, 64 bytes): with bit 0x20 of the word at +12 set, stores the low half of the argument at +792, destroys the list at +576 and clears its head word.
- `msg_collision__4GoldP10GameObject` (0x154db0, 184 bytes): four early-out tests, then plays `inv_equip_coin.anm` from `fx.lmp`, gives the amount at +128 to the player index at other+960 with a copy of the point at +16, clears the amount and deletes the object.
- Four `setState` bodies (SmallSpider 0x209a10, Cat 0x1ee8a0, Mermaid 0x1f7f08, GnomeNavigator 0x2021b0) are jump-table switches over the state. Their tables were read from the image: SmallSpider 0x46ff50, Cat 0x46da30 (14 entries), Mermaid 0x46e9c0, GnomeNavigator 0x46f470. SmallSpider and GnomeNavigator first call `setStateProcessing` with the address of a stack copy of the state and store whatever it leaves there. Several states leave the stored halfword unchanged on purpose. The transition rate singles are the .lit4 patterns 0x3e999999 (0.3), 0x3dcccccc (0.1), 0x3f000000 (0.5) and 0x3eaaaaab.
- `updateEnemy__8Creature` (0x10b558, 1212 bytes): target selection in three stages (current list at +576, attacker list at +752, a box search through `objectFindInBox` with half-extent leash times 0.707), then list update. Slot byte 160 of the creature vtable is called as `isEnemy`. The image's two stored copies of 0.707 (0x3f34fdf3) are one value here.
- Data: `norrath_collision_frame_bytes` is the 4-byte word `collisionFrame` at 0x484188, zero in the image. `norrath_collision_grid_scale_bytes` is the .lit4 word at 0x48001c, bits 0x3bcccccc (0.00625). `norrath_spell_type_name_view` is `5Spell` at 0x486318 and `norrath_spell_blessed_type_name_view` is `12SpellBlessed` at 0x4787b8 (the image has several identical `5Spell` strings; 0x486318 is the one the Blessed routine passes).

## Closed by others while this package ran

Six of the 21 names in the original brief were defined elsewhere in the shared draft before this branch finished: `death__8Creature`, `msg_alert__8CreatureifG6Point3P10GameObject`, `msg_collision__3CatP10GameObject`, `msg_collision__11SmallSpiderP10GameObject` (all `agents/vslots-a`), `setState__7Nataslai` and `turnToFace__8CreatureiG6Point3` (`agents/vslots-b`). This package had written its own death, Creature alert, Cat and SmallSpider collision, and Natasla setState bodies (commits `971875f`, `feffcce`, `c2b60c5`, `b17eb67`, `3783b26`). They agreed with the other authors' bodies on every branch, offset and call, and were removed in `e42b55b` so the two definitions do not collide. Do not add them back.

## Imports closed

- `charm__8Creaturei`
- `msg_alert__10GameObjectifG6Point3P10GameObject`
- `msg_collisionWorld__10GameObjectG6Point3`
- `msg_collision__10GameObjectP10GameObject`
- `msg_collision__4GoldP10GameObject`
- `msg_hurt__10GameObjectfP10GameObjecti`
- `norrath_collision_frame_bytes`
- `norrath_collision_grid_scale_bytes`
- `norrath_spell_blessed_type_name_view`
- `norrath_spell_type_name_view`
- `setState__11SmallSpideri`
- `setState__14GnomeNavigatori`
- `setState__3Cati`
- `setState__7Mermaidi`
- `updateEnemy__8Creature`

Measured on the shared tip `ca8050c` (1142 unresolved) with these 15 inputs added through a private copy of the plan: 1133 unresolved, 15 closed, 6 opened, COMMON zero.

## Imports opened

- `animAddOneShot__FP14AnimationStateP15AnimationHeaderffi` (0x160010)
- `animGlobalSync__FP14AnimationState` (0x1639e8)
- `getSmartRandomDestination__8Creaturei` (0x10d250; the Point3 result comes back through a hidden first pointer)
- `hudGivePlayerGold__FiG6Point3i` (0x1c4190; Point3 passed by address)
- `resetRouting__8Creature` (0x113460)
- `setStateProcessing__8CreaturePi` (0x1129f0; second argument is the address of the state word)

Already-unresolved names this package also calls: `frameNm`, `objectFindInBox__FG6Point3T0PP10GameObjecti` (original name, Point3 by address, not the by-value adapter `norrath_object_find_in_box`) and `floorAdd__FG6Point3sfii` (original name). Already-defined authored names it calls: `norrath_actor_random` (eRand), `norrath_animation_add_transition` (animAddTransitionTo), `norrath_animation_set_to_end`, `norrath_list_destroy`, `norrath_list_oneway_link`, `norrath_lump_find_resource`, `norrath_sound_play_sequence`, and `norrath_object_add_to_delete_list` (objectAddToDeleteList; this one is itself still unresolved on the tip).

## Build-plan entries needed

Profile `authored_cpp`. Do not add `host_check.cpp`. The integrator assigns `input_order` and adds `sources/combat-reactions-bytes/bytes.hpp` to the file list; `host_vcall.hpp` in that directory is only for host checks. `update_enemy.cpp` inlines its distance helper so `updateEnemy__8Creature` is its only emitted function.

```json
{"source":"sources/combat-reactions-alert-game-object/alert.cpp","object_name":"alert.o","sha256":"36b3711989e35426f0dd44f7fb6f7664e43b1ad89838df94bbce4046f641eca1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_alert__10GameObjectifG6Point3P10GameObject","compiled_body_bytes":8}]}
{"source":"sources/combat-reactions-charm/charm.cpp","object_name":"charm.o","sha256":"c432d5eba53b932d57dccf724cf350988a6ec001e200ae4039cba2bd137fb723","header_closure":["sources/combat-reactions-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"charm__8Creaturei","compiled_body_bytes":108}]}
{"source":"sources/combat-reactions-collision-frame/frame.cpp","object_name":"frame.o","sha256":"bcce16b5e61c37bd93e2c09f9fa2a54caedac09c29ab13c5ccfd8577cda670e7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/combat-reactions-collision-game-object/collision.cpp","object_name":"collision.o","sha256":"ef1b1d52ffe2752793c962f5d7a0a063d20ad163fb1e3dd778f5088c409c28db","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_collision__10GameObjectP10GameObject","compiled_body_bytes":8}]}
{"source":"sources/combat-reactions-collision-gold/collision_gold.cpp","object_name":"collision_gold.o","sha256":"2f97d81c085ffd3ac818107ef322b01a5bdfbdeedaa6063ca8a674f1e2f26952","header_closure":["sources/combat-reactions-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_collision__4GoldP10GameObject","compiled_body_bytes":248}]}
{"source":"sources/combat-reactions-collision-grid-scale/grid_scale.cpp","object_name":"grid_scale.o","sha256":"b31d610f8413b091d291e0602c1eedae9776c76a84a72114bc84067c724582c5","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/combat-reactions-collision-world/collision_world.cpp","object_name":"collision_world.o","sha256":"cf967f67e9d5283182dce7fc51f5e233b0db0026e0b30385ca1dcc4665d35a7b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_collisionWorld__10GameObjectG6Point3","compiled_body_bytes":8}]}
{"source":"sources/combat-reactions-hurt-game-object/hurt.cpp","object_name":"hurt.o","sha256":"58d06ce15d89de4f59c6fc98ff84e310cf0e8e050e8d97c161009ddb969db2a3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_hurt__10GameObjectfP10GameObjecti","compiled_body_bytes":8}]}
{"source":"sources/combat-reactions-set-state-cat/set_state_cat.cpp","object_name":"set_state_cat.o","sha256":"8978e6804ab7619b75417c8f0b85263a1f3d0cf494397a6d1f163425cbeab430","header_closure":["sources/combat-reactions-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"setState__3Cati","compiled_body_bytes":1028}]}
{"source":"sources/combat-reactions-set-state-gnome/set_state_gnome.cpp","object_name":"set_state_gnome.o","sha256":"4649660c800fbb00050257ef6700aadee20464353db5e4b768b555813028393c","header_closure":["sources/combat-reactions-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"setState__14GnomeNavigatori","compiled_body_bytes":836}]}
{"source":"sources/combat-reactions-set-state-mermaid/set_state_mermaid.cpp","object_name":"set_state_mermaid.o","sha256":"51e395cf6b2b0d10fd88c03c9168d8a2aee5ad0b94be0be9947408a682c11517","header_closure":["sources/combat-reactions-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"setState__7Mermaidi","compiled_body_bytes":644}]}
{"source":"sources/combat-reactions-set-state-spider/set_state_spider.cpp","object_name":"set_state_spider.o","sha256":"6a011b8704dbdf8dcbf7142d9e3141c47cc2bef15e0471bc869e4f5f59b6744d","header_closure":["sources/combat-reactions-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"setState__11SmallSpideri","compiled_body_bytes":832}]}
{"source":"sources/combat-reactions-spell-blessed-type-name/type_name.cpp","object_name":"type_name.o","sha256":"ae7d85f982d8dd83664d5ce5599a4cd7a418692a52e70909e6e8a056f4704baa","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/combat-reactions-spell-type-name/type_name.cpp","object_name":"type_name.o","sha256":"70a119afaeb20b1b1d38b471d2a86d7fe7c7ea5750b18a5ddfb6ab7a91b477c1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/combat-reactions-update-enemy/update_enemy.cpp","object_name":"update_enemy.o","sha256":"a6d6a26d48ffcd2fbe431e323c9d2681c9946de0e1ecfb79adf179502ef45dc0","header_closure":["sources/combat-reactions-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"updateEnemy__8Creature","compiled_body_bytes":3568}]}
```

## Authored but never executed

All of it on real hardware or in the game. The seven functions listed under Evidence have been run only inside the comparison interpreter. The four no-op bodies, the data objects and the type-name views have host checks only. `setState` bodies that reach the vtable call (SmallSpider and GnomeNavigator state 10) were compared with a stub in place of the called slot.

## Unknown

- What the states of Cat, Mermaid, SmallSpider and GnomeNavigator mean, and what the fields at +256, +272, +808, +968 and +972 hold.
- Which creature field is the leash used by `updateEnemy` (the single at +788 is used that way), and what the byte at +569, the word at +812 and bit 0x4000000 at +160 mean.
- Whether `getSmartRandomDestination`, `setStateProcessing`, `hudGivePlayerGold` and `animGlobalSync` return values the callers here ignore; none is read.
- EE FPU behavior on NaN or denormal inputs to the distance tests.
- Whether the other identical `5Spell` strings in the image are separate objects or a name-pool artifact.
