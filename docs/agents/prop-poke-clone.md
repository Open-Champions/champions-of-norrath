# Prop poke, route, clone, typeinfo, and destructor slots

Package `agents/prop-poke-clone`, taken over after the first agent was cut off with nothing committed. All 37 symbols below are authored under their observed names, one per commit, from `docs/agents/gameobject-base.md`'s base-slot list and the unresolved imports of the shared tip (`20a5902`, 908 authored inputs, 950 unresolved). Addresses and byte counts come from the private hash-pinned `SLUS_205.65` symbol table and the reviewed EE decoder; they are local evidence, not values reproduced by the relocatable build. Nothing here ran on an EE or as gameplay.

The `gameobject-base` units already in the plan define the GameObject slots under authored `norrath_vt_*` names, which no vtable view names. The views name the observed symbols (`clone__10GameObject`, `getModel__10GameObject`, ...), so this package defines those names separately rather than editing the other package's files. It does not alias them to each other.

## Evidence

- **GameObject base slots** (`clone`, `getModel`, `msg_getPokeName`, `msg_moveCollision`, `msg_poke`, `msg_route`): the same behaviour as `gameobject-base`. The five leaves are 8 bytes (`jr ra`; `getModel` and `getPokeName` return zero). Clone allocates 76 bytes, stores the GameObject table at +72 before three ListHead copies, then copies the observed byte ranges.
- **Clones**: all share the shape allocate, null returns null, store the GameObject table at +72, three `__8ListHeadRC8ListHead` calls at +0/+4/+8 from the same source offsets, then field copies at identical offsets, then the class table replaces +72. Sizes: Counter and TriggerParams 80, Lever and Timer 84, PokeReflector and Player1 76, CosmeticPropAnim 152, Prop 328, UserParamProp 368, Creature and SmallSpider 960, CdEyeball 968, Cat 992. Prop adds counted loops of 16, 64 and 64 bytes at +84, +100 and +164. Creature adds counted loops (20, 16, 16, 128 and 64 bytes), one animation-state copy at +288, and list-head copies at +576, +752, +888. SmallSpider, Cat and CdEyeball repeat the Creature body, then write the Creature table at +72 before their own table. UserParamProp (`__15AnimationState8RC15AnimationState8` at +84) and CosmeticPropAnim (`__14AnimationStateRC14AnimationState` at +100) each make one animation-state copy-constructor call. The word stored in that call's delay slot is written before the call and the fields after it follow. The copy order in the sources follows the image, and the host checks assert that order against stubbed calls.
- **Class tables used**: Prop `norrath_player_view_44d018`, Player1 `norrath_player_view_44d2e8`, UserParamProp `norrath_player_view_44c9e8`, CosmeticPropAnim `norrath_factory_animation_vtable` (0x44cb08, already defined by the factory package), Creature `norrath_actor_view_4488f0`, Cat `norrath_actor_view_44c170`, CdEyeball `norrath_actor_view_44b170`, SmallSpider `norrath_actor_view_44b540`, PokeReflector `norrath_next_view_449280`, Lever `norrath_next_view_4491f0`, Timer `norrath_next_view_4490d0`, Counter `norrath_next_view_449160`, TriggerParams `norrath_next_view_44c958`.
- **Typeinfo functions**: 76-byte shape (Prop, Player1, UserParamProp, CosmeticPropAnim): a zero first word of the 12-byte `__ti` object calls the GameObject typeinfo function and `__rtti_si` with the observed class name and `__ti10GameObject` (gp 0x4879f0 - 3112). `__tf8AutoProp` also sets up `__ti10AutoObject` (0x8eb4e8) inline before its own. `__tfP4Prop` and `__tfP6PhProp` call the pointee's `__tf` function and then `__rtti_ptr` with the pointee typeinfo, matching the already-published `__tfP8ItemProp` source on `agents/inventory-items`.
- **Destructors**: store the class table at +72, destroy an embedded animation state when there is one (UserParamProp at +84 with flag 0, CosmeticPropAnim at +100 with flag 2), run the GameObject destructor with flag 0, and free the object when bit 0 of the flags is set (a tail call in the image).
- **UserParamProp poke** runs `objectDoMyActions` with 1, then calls table slot byte 8 with 3 on a non-null object. In the published Vehicle table that slot is the destructor, so this reads as a delete request (inferred from that one table).
- **UserParamProp getPokeName** always stores `frameNm` at +360, returns 0 without a `message` tag, else converts the tag text and returns 1.
- **PokeReflector getPokeName and poke** read the `target` tag, find the named object(s), and call table slot byte 104 (getPokeName) or 112 (poke) on them. In the Vehicle table those slot numbers are the getPokeName and poke slots.
- **Lever poke** clears bit 20 of the flag word at +12, picks one of two `game_leverpull%d.anm` sequences from `eRand`, plays it from the `fx.lmp` lump, adds the object to the run list, and sets the byte at +81.
- **Lever getPokeName** dots the player's offset with the facing vector rotated by -16384 and scales by the signed byte at +80, choosing the text keys `PullLever` or `PushLever`.
- **Creature getPokeName** (980 bytes) is the largest body. See the source header for the decision order: early outs, a MummyKing type test on flagged objects, the `talkCone` test, the shop text, a 180.0-unit box query for blockers, then `CannotTalk` or `TalkTo` plus the name text.

## Imports closed

- `_$_13UserParamProp`
- `_$_16CosmeticPropAnim`
- `_$_4Prop`
- `_$_7Player1`
- `__tf13UserParamProp`
- `__tf16CosmeticPropAnim`
- `__tf4Prop`
- `__tf7Player1`
- `__tf8AutoProp`
- `__tfP4Prop`
- `__tfP6PhProp`
- `clone__10GameObject`
- `clone__11SmallSpider`
- `clone__13PokeReflector`
- `clone__13TriggerParams`
- `clone__13UserParamProp`
- `clone__16CosmeticPropAnim`
- `clone__3Cat`
- `clone__4Prop`
- `clone__5Lever`
- `clone__5Timer`
- `clone__7Counter`
- `clone__7Player1`
- `clone__8Creature`
- `clone__9CdEyeball`
- `getModel__10GameObject`
- `msg_getPokeName__10GameObjectPUsi`
- `msg_getPokeName__13PokeReflectorPUsi`
- `msg_getPokeName__13UserParamPropPUsi`
- `msg_getPokeName__5LeverPUsi`
- `msg_getPokeName__8CreaturePUsi`
- `msg_moveCollision__10GameObjectPP10GameObject`
- `msg_poke__10GameObjecti`
- `msg_poke__13PokeReflectori`
- `msg_poke__13UserParamPropi`
- `msg_poke__5Leveri`
- `msg_route__10GameObjecti`

Defined data, not an import: `norrath_ti_4Prop`, `norrath_ti_7Player1`, `norrath_ti_13UserParamProp`, `norrath_ti_16CosmeticPropAnim`, `norrath_ti_8AutoProp`, `norrath_ti_10AutoObject`, `norrath_ti_P4Prop`, `norrath_ti_P6PhProp` (12-byte zero-initialized typeinfo objects). `agents/inventory-items` already imports `norrath_ti_4Prop`, so this package supplies it.

## Imports opened

- `_$_14AnimationState`
- `__14AnimationStateRC14AnimationState`
- `__15AnimationState8RC15AnimationState8`
- `__tf6PhProp`
- `__tf9MummyKing`
- `__throw_bad_typeid`
- `eRand__10GameObject`
- `findTagFloat__10GameObjectPci`
- `norrath_rtti_ptr`
- `norrath_ti_6PhProp`
- `usprintf__FPUsT0ffffffff`
- `ustrcat__FPUsPc`
- `ustrcat__FPUsT0`
- `ustrcpy__FPUsT0`

`norrath_rtti_ptr` and `norrath_ti_6PhProp` use the names that `agents/inventory-items` and the other typeinfo packages already chose, so they should match once both branches are merged. Other callees were already defined or already unresolved and are used by the names other packages use: `norrath_block_alloc`, `norrath_block_free`, `norrath_list_head_copy_construct`, `norrath_vt_dtor_10GameObject`, `norrath_vt_tf_10GameObject`, `norrath_rtti_si`, `norrath_ti_10GameObject`, `norrath_game_object_vtable_view`, `norrath_actor_find_tag_string`, `norrath_game_object_find_tag_int`, `norrath_actor_add_run_list`, `lumpFindResource__FPcT0`, `soundPlaySequence__FP15AnimationHeaderP10GameObjectf`, `sprintf`, `frameNm`, `player`, `icos__Fi`, `isin__Fi`, `textGetString__FPc`, `gameSomeoneInShop__Fv`, `objectDoMyActions__FP10GameObjecti`, `objectFindObject__FPc`, `objectFindObject__FPcPP10GameObjecti`, `objectFindInBox__FG6Point3T0PP10GameObjecti`, `__eq__C9type_infoRC9type_info`.

A private copy of the plan and snapshot (kept outside git, with these 37 entries appended) built with the reviewed EE toolchain: 945 compilations, one link, 802 function bodies, 927 unresolved, 0 COMMON. Against the same tip without these entries (908 inputs, 950 unresolved) that is 37 closed, 14 opened, and none of the 37 left.

## Build-plan entries needed

Profile `authored_cpp`. The integrator assigns `input_order`. Do not add any `host_check.cpp`. `bytes.hpp` is the existing `sources/vslots-b-bytes/bytes.hpp` (this package reuses it as a header closure and does not edit it). The 37 objects get object names beginning `ppc_` in the private plan only to keep them unique; the integrator may keep or change them.

```json
{"source":"sources/prop-poke-clone-clone-cat/clone.cpp","object_name":"ppc_clone-cat.o","sha256":"0aea9bf392122baaee29e51238be53f559e24e9fe4865ee726b07811afd2b97d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__3Cat","compiled_body_bytes":3328}]}
{"source":"sources/prop-poke-clone-clone-cd-eyeball/clone.cpp","object_name":"ppc_clone-cd-eyeball.o","sha256":"98adbd7b731a5a3d27bec696c5fc665dc88f225f14e9aa45d4eb051d15401c3c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__9CdEyeball","compiled_body_bytes":3124}]}
{"source":"sources/prop-poke-clone-clone-cosmetic-prop-anim/clone.cpp","object_name":"ppc_clone-cosmetic-prop-anim.o","sha256":"f17dbbeb11354b983ef06581019ff3129b9c8f608ec2411f36918286a269fa44","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__16CosmeticPropAnim","compiled_body_bytes":920}]}
{"source":"sources/prop-poke-clone-clone-counter/clone.cpp","object_name":"ppc_clone-counter.o","sha256":"fb998dcdc4548e30ce3f3bc61c72ff773bc8f5a85e0f23966b539cdf222d5578","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__7Counter","compiled_body_bytes":664}]}
{"source":"sources/prop-poke-clone-clone-creature/clone.cpp","object_name":"ppc_clone-creature.o","sha256":"f5dd1c245480a1453829a537a6ff59e501984d7794297e257e32622d8863e979","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__8Creature","compiled_body_bytes":3056}]}
{"source":"sources/prop-poke-clone-clone-game-object/clone.cpp","object_name":"ppc_clone-game-object.o","sha256":"f0aef69d2a771695bec2299d88aa4214cd14c1e6d2707a1b3203c9b398521a9e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__10GameObject","compiled_body_bytes":512}]}
{"source":"sources/prop-poke-clone-clone-lever/clone.cpp","object_name":"ppc_clone-lever.o","sha256":"ae6e33c90f7d52050e3c27694a6ce615609ae3ce7a8afdc25570c6cd758f6349","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__5Lever","compiled_body_bytes":672}]}
{"source":"sources/prop-poke-clone-clone-player1/clone.cpp","object_name":"ppc_clone-player1.o","sha256":"97bf8c64ac5d7d87d675c150e0d6f10c7c51fdcbf12b2a3d5040688bd73520f9","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__7Player1","compiled_body_bytes":616}]}
{"source":"sources/prop-poke-clone-clone-poke-reflector/clone.cpp","object_name":"ppc_clone-poke-reflector.o","sha256":"5a3545e43b70d24534ec7336fc8b6e019060d2222cfd3bd77d3b283b455c24fd","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__13PokeReflector","compiled_body_bytes":616}]}
{"source":"sources/prop-poke-clone-clone-prop/clone.cpp","object_name":"ppc_clone-prop.o","sha256":"e93caa2726505dc57417962bfcb4eef8d5b9b713bdab5a2e1416fe261856a7bf","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__4Prop","compiled_body_bytes":1232}]}
{"source":"sources/prop-poke-clone-clone-small-spider/clone.cpp","object_name":"ppc_clone-small-spider.o","sha256":"2cd6afa9fdd28171117d6926896f5d345ad9fdefda11b37ea4f452bb3b165270","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__11SmallSpider","compiled_body_bytes":3092}]}
{"source":"sources/prop-poke-clone-clone-timer/clone.cpp","object_name":"ppc_clone-timer.o","sha256":"e902c69322ca6f10973e81a88aade80de439c64194c979316ca39ad52d2639b5","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__5Timer","compiled_body_bytes":696}]}
{"source":"sources/prop-poke-clone-clone-trigger-params/clone.cpp","object_name":"ppc_clone-trigger-params.o","sha256":"8f7bb149ad3aa88da0749a1e37c14a0bbc95132ac55f3759b6eb805ec4fb4363","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__13TriggerParams","compiled_body_bytes":656}]}
{"source":"sources/prop-poke-clone-clone-user-param-prop/clone.cpp","object_name":"ppc_clone-user-param-prop.o","sha256":"e9bc8daafc45ed29050e271c3cb30efaaf0547429eec4acd5ed5fe567ebe4fb2","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__13UserParamProp","compiled_body_bytes":800}]}
{"source":"sources/prop-poke-clone-dtor-cosmetic-prop-anim/dtor.cpp","object_name":"ppc_dtor-cosmetic-prop-anim.o","sha256":"6ff1e4e8a7229a89688357939c83446a1c1fd8a019f2b6e73b30e7d14c86af77","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_16CosmeticPropAnim","compiled_body_bytes":144}]}
{"source":"sources/prop-poke-clone-dtor-player1/dtor.cpp","object_name":"ppc_dtor-player1.o","sha256":"106457dfa56c26f8d5264d0319231ec55ed22725ccd1f621813ee60cd21ccaef","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_7Player1","compiled_body_bytes":112}]}
{"source":"sources/prop-poke-clone-dtor-prop/dtor.cpp","object_name":"ppc_dtor-prop.o","sha256":"4c67b82ad8887e7979b6f89a4bbaf626d7d3a9734ff99eac9231d354dbd8ffaa","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_4Prop","compiled_body_bytes":112}]}
{"source":"sources/prop-poke-clone-dtor-user-param-prop/dtor.cpp","object_name":"ppc_dtor-user-param-prop.o","sha256":"be577a8252c63fdb117aeb46d1ea5dd91fa7ec233a1c8c7995f7f8d8c7f6c6c2","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_13UserParamProp","compiled_body_bytes":144}]}
{"source":"sources/prop-poke-clone-get-model/get_model.cpp","object_name":"ppc_get-model.o","sha256":"2461c190e69598d40e6e8e5f82b54a975b1229ab27d200e21e2ed5398b4aee5d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"getModel__10GameObject","compiled_body_bytes":8}]}
{"source":"sources/prop-poke-clone-get-poke-name-creature/msg_get_poke_name.cpp","object_name":"ppc_get-poke-name-creature.o","sha256":"eb11250cd6e9c024b627f377198abf825cc89769be0473e0b0ec1ff524cdcfc0","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_getPokeName__8CreaturePUsi","compiled_body_bytes":1324}]}
{"source":"sources/prop-poke-clone-get-poke-name-lever/msg_get_poke_name.cpp","object_name":"ppc_get-poke-name-lever.o","sha256":"52e077d610a482ee83815b33e1364f935a2384b9ef30035e80f62073b662e141","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_getPokeName__5LeverPUsi","compiled_body_bytes":460}]}
{"source":"sources/prop-poke-clone-get-poke-name-poke-reflector/msg_get_poke_name.cpp","object_name":"ppc_get-poke-name-poke-reflector.o","sha256":"dd358ac56200a1417da991b392ac947ac439ab05e386cdc6ea248b436719efde","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_getPokeName__13PokeReflectorPUsi","compiled_body_bytes":200}]}
{"source":"sources/prop-poke-clone-get-poke-name-user-param-prop/msg_get_poke_name.cpp","object_name":"ppc_get-poke-name-user-param-prop.o","sha256":"04d918c969335038609c1d6eced6bf97310fce5b0d06d86ec82e46b3c0a6ec8b","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_getPokeName__13UserParamPropPUsi","compiled_body_bytes":144}]}
{"source":"sources/prop-poke-clone-msg-get-poke-name/msg_get_poke_name.cpp","object_name":"ppc_msg-get-poke-name.o","sha256":"36b637cef13df4b9412040dab13971ef91d83ddc85ea79f1e05870690c3d0eb1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_getPokeName__10GameObjectPUsi","compiled_body_bytes":8}]}
{"source":"sources/prop-poke-clone-msg-move-collision/msg_move_collision.cpp","object_name":"ppc_msg-move-collision.o","sha256":"0c1ad2a732b62a8a9a9766bcd620605e958764f6c6e7e230a772c4616f750ff1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_moveCollision__10GameObjectPP10GameObject","compiled_body_bytes":8}]}
{"source":"sources/prop-poke-clone-msg-poke/msg_poke.cpp","object_name":"ppc_msg-poke.o","sha256":"7129641c76e99abe1491824eae8b9c794c734b7d86ee30d8c758c49a6180bde7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_poke__10GameObjecti","compiled_body_bytes":8}]}
{"source":"sources/prop-poke-clone-msg-poke-lever/msg_poke.cpp","object_name":"ppc_msg-poke-lever.o","sha256":"5f58ac3a02d9385d9fe65e6e2fffdd71f2c80e2986b72d7f64392c06c724cb68","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_poke__5Leveri","compiled_body_bytes":160}]}
{"source":"sources/prop-poke-clone-msg-poke-poke-reflector/msg_poke.cpp","object_name":"ppc_msg-poke-poke-reflector.o","sha256":"8b352e16c6482f902886ba06d5fed687ecc9ab70f947afe1390451e53883ac26","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_poke__13PokeReflectori","compiled_body_bytes":204}]}
{"source":"sources/prop-poke-clone-msg-poke-user-param-prop/msg_poke.cpp","object_name":"ppc_msg-poke-user-param-prop.o","sha256":"1a38b472fd9e1f4d804c5480d2d805738d6029ccf8565c1b3ebd7374017c0f6d","header_closure":["sources/vslots-b-bytes/bytes.hpp"],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_poke__13UserParamPropi","compiled_body_bytes":108}]}
{"source":"sources/prop-poke-clone-msg-route/msg_route.cpp","object_name":"ppc_msg-route.o","sha256":"8c3209cb809a906740a70d0b35d906a8d63489a0a5504966b04b0cc6807cadf0","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_route__10GameObjecti","compiled_body_bytes":8}]}
{"source":"sources/prop-poke-clone-typeinfo-auto-prop/typeinfo.cpp","object_name":"ppc_typeinfo-auto-prop.o","sha256":"8c5de9f967da3bc49fed9f14c3dd7439db6b0493ea0286c754bdab54020e297f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf8AutoProp","compiled_body_bytes":184}]}
{"source":"sources/prop-poke-clone-typeinfo-cosmetic-prop-anim/typeinfo.cpp","object_name":"ppc_typeinfo-cosmetic-prop-anim.o","sha256":"2cb96296ede9ef58a789054840f7b70d5a8fa6d74b2d545b7d7a42f7a1d434e8","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf16CosmeticPropAnim","compiled_body_bytes":92}]}
{"source":"sources/prop-poke-clone-typeinfo-player1/typeinfo.cpp","object_name":"ppc_typeinfo-player1.o","sha256":"d4f377019b16c03c0946a9fadeafa6a5350cec5bf480cd33e75f1475138cee74","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf7Player1","compiled_body_bytes":92}]}
{"source":"sources/prop-poke-clone-typeinfo-prop/typeinfo.cpp","object_name":"ppc_typeinfo-prop.o","sha256":"58173c07e4defc9c959f2be274523080f7815a8f0f1c748b353d08b0d6ba1d88","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf4Prop","compiled_body_bytes":92}]}
{"source":"sources/prop-poke-clone-typeinfo-ptr-ph-prop/typeinfo.cpp","object_name":"ppc_typeinfo-ptr-ph-prop.o","sha256":"2ef2bc600751bf0dee72fc30192d77fb50b7bde9192eab8b8638e811f0bd743d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tfP6PhProp","compiled_body_bytes":92}]}
{"source":"sources/prop-poke-clone-typeinfo-ptr-prop/typeinfo.cpp","object_name":"ppc_typeinfo-ptr-prop.o","sha256":"67c20865802399203810a8c3d60f54d16c3cc257dbe1576c92a768ac620ef9ce","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tfP4Prop","compiled_body_bytes":92}]}
{"source":"sources/prop-poke-clone-typeinfo-user-param-prop/typeinfo.cpp","object_name":"ppc_typeinfo-user-param-prop.o","sha256":"be5a1f194294dac35df780eacbd7a90786e127e8136a38c23e61716c63d1cebf","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf13UserParamProp","compiled_body_bytes":92}]}
```

## Authored but never executed

All 37 functions compile with the reviewed EE `g++` `authored_cpp` profile and link into the private partial object. Each has a host check built with the host C++ compiler against stubs for every callee, run on this machine. None ran on the EE or in the original process. The clone checks assert every copied range, every gap byte, the list-head zeroing, the call order against the image, and the null-allocation path. They would fail if an offset or size were wrong (a one-off change to the final Creature loop was tried and caught). The typeinfo checks assert the first-word gate and the base/pointee order. Compiled body sizes differ from the image sizes; no byte match to the original is claimed. The two floating-point bodies (Lever and Creature getPokeName) compile to the same operation order but their R5900 NaN and denormal behaviour is not established; Creature uses an inline `rsqrt.s`, as the lighting package does.

## Unknown

- Meaning of the flag bits (+12 bits 3, 5, 6, 20, 24; the 64-bit word at +160) and of the words at +80, +81, +360, +792 and +892 in the poke bodies.
- What the 4 bytes at 228..231 of Prop, and the other gap bytes between copied ranges in each clone, are for; the allocator's bytes are left as found.
- The size and layout of the two animation-state copy-constructors' destinations; the clone calls them and their bytes are not modelled here.
- Whether the table slot byte 8 called by UserParamProp poke is the destructor in the UserParamProp table itself; only the Vehicle table was read.
- Whether another unit also sets up `norrath_ti_10AutoObject`; the AutoProp typeinfo function does it inline here.
- Stack lists of 16 and 64 object words: the image does not clear them, and the callee's termination behaviour at the limit is not established. The PokeReflector poke loop can read one word past 16 entries in the image if the callee fills all of them.
- `getModel__10GameObject` and the other four leaf symbols are weak in the image; these definitions are strong.
