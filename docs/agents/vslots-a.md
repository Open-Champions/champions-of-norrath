# Virtual-slot methods, first half

Package notes for `agents/vslots-a`. The 95 virtual-slot names still unresolved
at `a25bb86` were sorted in plain byte order. This package is names 1 through
48. Work is on `agents/vslots-a` only. `experimental/build-plan.json`,
`experimental/snapshot.json`, and `tools/` were not edited. Host checks sit
beside each source and are not plan inputs.

Each unit was compiled with the reviewed EE GCC 15.2.0 `authored_cpp` profile.
Each host check was run with the system `c++ -std=c++17 -Wall -Wextra -Werror`.
The compiled body sizes below are those objects. They are not a claim that the
body matches the image.

## Imports closed

- `_$_11SmallSpider`
- `_$_13PokeReflector`
- `_$_13TriggerParams`
- `_$_14GnomeNavigator`
- `_$_3Cat`
- `_$_4Trap`
- `_$_5Lever`
- `_$_5Timer`
- `_$_7Counter`
- `_$_7Mermaid`
- `_$_7Natasla`
- `_$_8Creature`
- `_$_9CdEyeball`
- `__tf11SmallSpider`
- `__tf13PokeReflector`
- `__tf13TriggerParams`
- `__tf14GnomeNavigator`
- `__tf3Cat`
- `__tf4Trap`
- `__tf5Lever`
- `__tf5Timer`
- `__tf7Counter`
- `__tf7Mermaid`
- `__tf7Natasla`
- `__tf8Creature`
- `__tf9CdEyeball`
- `clone__10GameObject`
- `clone__11SmallSpider`
- `clone__13PokeReflector`
- `clone__13TriggerParams`
- `clone__3Cat`
- `clone__5Lever`
- `clone__5Timer`
- `clone__7Counter`
- `clone__8Creature`
- `clone__9CdEyeball`
- `confuse__8Creaturei`
- `death__8Creature`
- `getMinibossName__8Creature`
- `getModel__8Creature`
- `getSkillRank__8Creaturei`
- `initMiniboss__8Creature`
- `isEnemy__8CreatureP10GameObject`
- `msg_alert__8CreatureifG6Point3P10GameObject`
- `msg_collisionWorld__8CreatureG6Point3`
- `msg_collision__11SmallSpiderP10GameObject`
- `msg_collision__3CatP10GameObject`
- `msg_collision__8CreatureP10GameObject`

`_$_8Creature`, `__tf8Creature`, `__ti8Creature`, and
`msg_collision__8CreatureP10GameObject` are also referenced inside this
package and resolve here. Each `__tf` unit also defines a 12-byte BSS
`__ti` object of the same class.

## Imports opened

These names are referenced and are not defined by this package:

- `__rtti_si`
- `_$_14AnimationState`
- `_$_15FxCreatureState`
- `__15AnimationState8RC15AnimationState8`
- `animSetToEnd__FP14AnimationState`
- `dropItem__4itemG6Point3Pc`
- `findTagString__10GameObjectPci`
- `objectAddToDeleteList__FP10GameObject`
- `readyToSwitch__3Cat`
- `scriptStart__FPc`
- `switchToRun__3Cat`

The units also call existing authored symbols: `norrath_block_alloc`,
`norrath_block_free`, `norrath_list_destroy`, `norrath_list_head_copy_construct`,
`norrath_list_safe_iterator_construct`, `norrath_list_safe_iterator_next`,
`norrath_list_safe_iterator_destroy`, `norrath_vt_dtor_10GameObject`,
`norrath_vt_tf_10GameObject`, `norrath_ti_10GameObject`,
`norrath_game_object_vtable_view`, and the data-views vtables
`norrath_actor_view_4488f0`, `norrath_actor_view_44b170`,
`norrath_actor_view_44b540`, `norrath_actor_view_44c170`,
`norrath_actor_view_44e1c0`, `norrath_actor_view_44e6a0`,
`norrath_actor_view_44f4c8`, `norrath_next_view_4490d0`,
`norrath_next_view_449160`, `norrath_next_view_4491f0`,
`norrath_next_view_449280`, `norrath_next_view_44c958`, and
`norrath_next_view_44e820`.

## Build-plan entries needed

Append these authored inputs and assign fresh `input_order` values. Increase
`default_authored_inputs` by 48. Add each `sha256` to the plan `files` map
under the same `source` key. Do not add any `host_check.cpp`. `header_closure`
is empty. `profile` is `authored_cpp`. No `private_object_sha256` is claimed.
The four large clone objects also contain one local 88-byte rodata span table.
That object is not a function and is not listed in `emitted_functions`.

```json
{"source":"sources/vslots-a-clone-cat/clone.cpp","object_name":"vslots-a-clone-cat.o","sha256":"12a6231f3c82c40620cf812b96a5e9e265461f2a0bb776c70f8b3ab8b6deba72","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__3Cat","compiled_body_bytes":572}]}
{"source":"sources/vslots-a-clone-counter/clone.cpp","object_name":"vslots-a-clone-counter.o","sha256":"231c884a3194a5cbd6e0a00403afaed7ad246acef048da45b4fe0daa8eddaa68","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__7Counter","compiled_body_bytes":588}]}
{"source":"sources/vslots-a-clone-creature/clone.cpp","object_name":"vslots-a-clone-creature.o","sha256":"370b3d209ceef56267eebd330d11f080c1f3b74a266284c30cd3f6617953ac7c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__8Creature","compiled_body_bytes":536}]}
{"source":"sources/vslots-a-clone-eyeball/clone.cpp","object_name":"vslots-a-clone-eyeball.o","sha256":"ec2f118a34cc2778a2769cd327b6f19474316cc9dca4f059f545caa790d1883d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__9CdEyeball","compiled_body_bytes":572}]}
{"source":"sources/vslots-a-clone-gameobject/clone.cpp","object_name":"vslots-a-clone-gameobject.o","sha256":"db1a6cee6768fd38653c4ba8c4b41d0156fa425604e1a1870b9e189c3d65237a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__10GameObject","compiled_body_bytes":512}]}
{"source":"sources/vslots-a-clone-lever/clone.cpp","object_name":"vslots-a-clone-lever.o","sha256":"c5f7b36a2f60a6b88526151ae3fab92e71dad832fa0c7208b6e72a919b3e0a8a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__5Lever","compiled_body_bytes":604}]}
{"source":"sources/vslots-a-clone-pokereflector/clone.cpp","object_name":"vslots-a-clone-pokereflector.o","sha256":"90a403ba260f3a967a8ca42c63a7b864ab37c05d8e6afcd5017f3dd464316954","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__13PokeReflector","compiled_body_bytes":548}]}
{"source":"sources/vslots-a-clone-spider/clone.cpp","object_name":"vslots-a-clone-spider.o","sha256":"c2f42200febe5e844cdc12c380da14a647898f74b427966539e92b4e7db2047b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__11SmallSpider","compiled_body_bytes":572}]}
{"source":"sources/vslots-a-clone-timer/clone.cpp","object_name":"vslots-a-clone-timer.o","sha256":"7017a362dd06a27e4d28199f78a4bd14ba48c0f618d6b8cd570f6e8c668e98c1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__5Timer","compiled_body_bytes":588}]}
{"source":"sources/vslots-a-clone-triggerparams/clone.cpp","object_name":"vslots-a-clone-triggerparams.o","sha256":"0c7f1ff804bc529f4a13b24457f84efc4a373c63ee1f3e0801abd33860fea09c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"clone__13TriggerParams","compiled_body_bytes":588}]}
{"source":"sources/vslots-a-confuse/confuse.cpp","object_name":"vslots-a-confuse.o","sha256":"87ee011c4ec8007dd90930066487bfe0daa6aa407cc3db0f143096fc91c6b7e2","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"confuse__8Creaturei","compiled_body_bytes":68}]}
{"source":"sources/vslots-a-death/death.cpp","object_name":"vslots-a-death.o","sha256":"4b5e22d3315ba467c5c93681ade349da2ab16a37c39447d0d1b4b2ce0e973dd0","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"death__8Creature","compiled_body_bytes":272}]}
{"source":"sources/vslots-a-dtor-11smallspider/dtor.cpp","object_name":"vslots-a-dtor-11smallspider.o","sha256":"871e764e0fcc3d1e0602374c9102e7080cbb71d78e1e445326ecb1d9b01ad88c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_11SmallSpider","compiled_body_bytes":112}]}
{"source":"sources/vslots-a-dtor-13pokereflector/dtor.cpp","object_name":"vslots-a-dtor-13pokereflector.o","sha256":"e0af119eb860f192a9038ad2e0faa276da7b546b0c17621d2e83c56fd2f83264","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_13PokeReflector","compiled_body_bytes":112}]}
{"source":"sources/vslots-a-dtor-13triggerparams/dtor.cpp","object_name":"vslots-a-dtor-13triggerparams.o","sha256":"64c579510dd8fb62aa55d2b8cbb2b9bcba8cddbb0780c0fe11c8fc459e5a3005","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_13TriggerParams","compiled_body_bytes":112}]}
{"source":"sources/vslots-a-dtor-14gnomenavigator/dtor.cpp","object_name":"vslots-a-dtor-14gnomenavigator.o","sha256":"49a698c170e28fbfdb0af09687e75b4c27971087dca536ce7ed7a9e52dbf0522","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_14GnomeNavigator","compiled_body_bytes":112}]}
{"source":"sources/vslots-a-dtor-3cat/dtor.cpp","object_name":"vslots-a-dtor-3cat.o","sha256":"485b1ce7ef3716817b2c46019c41dda4b1818b87a41ff1c6a78051c1b82a7cbf","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_3Cat","compiled_body_bytes":112}]}
{"source":"sources/vslots-a-dtor-4trap/dtor.cpp","object_name":"vslots-a-dtor-4trap.o","sha256":"4887685d22c902e4c103088e917995c40c353e5456a8f311f075ad10f5682850","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_4Trap","compiled_body_bytes":112}]}
{"source":"sources/vslots-a-dtor-5lever/dtor.cpp","object_name":"vslots-a-dtor-5lever.o","sha256":"9b90df9e5a4a27f70c9e7ed36996d0fa79de7b6b226c5002fe4c68b6e75d581c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_5Lever","compiled_body_bytes":112}]}
{"source":"sources/vslots-a-dtor-5timer/dtor.cpp","object_name":"vslots-a-dtor-5timer.o","sha256":"44cb0c31b46302f0c403932e12c93c3eb13be6933ca30b73999426562826430c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_5Timer","compiled_body_bytes":112}]}
{"source":"sources/vslots-a-dtor-7counter/dtor.cpp","object_name":"vslots-a-dtor-7counter.o","sha256":"65bc7a908bdcfbfc71d6842bf523812552baa937ed4e83d71690b6d2658008b2","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_7Counter","compiled_body_bytes":112}]}
{"source":"sources/vslots-a-dtor-7mermaid/dtor.cpp","object_name":"vslots-a-dtor-7mermaid.o","sha256":"f7426f9446cde8a8acc87471ce7f58649b38e511889de93563941d1cdcad9f25","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_7Mermaid","compiled_body_bytes":112}]}
{"source":"sources/vslots-a-dtor-7natasla/dtor.cpp","object_name":"vslots-a-dtor-7natasla.o","sha256":"39d46b72c8b9ab03b8707548b8c5841421f71ed050a518d1fa3ba6d0790c4761","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_7Natasla","compiled_body_bytes":112}]}
{"source":"sources/vslots-a-dtor-9cdeyeball/dtor.cpp","object_name":"vslots-a-dtor-9cdeyeball.o","sha256":"aec064d7f52bc8feef44e6f8d9a3b1c44a454ed9fc11ffe6c6279122dde0d574","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_9CdEyeball","compiled_body_bytes":112}]}
{"source":"sources/vslots-a-dtor-creature/dtor.cpp","object_name":"vslots-a-dtor-creature.o","sha256":"095e8aec3bfbe940024d5fd67a468f7139970e775fbc94af8fe10b6367a2c1f8","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"_$_8Creature","compiled_body_bytes":360}]}
{"source":"sources/vslots-a-get-miniboss-name/get_miniboss_name.cpp","object_name":"vslots-a-get-miniboss-name.o","sha256":"1648439a8eacc1f657f08645b0a74cb9901db646fe894b7a0871f23cc5c42ce1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"getMinibossName__8Creature","compiled_body_bytes":12}]}
{"source":"sources/vslots-a-get-model/get_model.cpp","object_name":"vslots-a-get-model.o","sha256":"8e45c9084345339b28f9c1469c7ba4bdb69054a36cb660b1746d0d21d27c359c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"getModel__8Creature","compiled_body_bytes":20}]}
{"source":"sources/vslots-a-get-skill-rank/get_skill_rank.cpp","object_name":"vslots-a-get-skill-rank.o","sha256":"13b5fafeefb3b10c65bac759c8afd77cec908ebc7918a9a6f6074c65d44a8ed3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"getSkillRank__8Creaturei","compiled_body_bytes":8}]}
{"source":"sources/vslots-a-init-miniboss/init_miniboss.cpp","object_name":"vslots-a-init-miniboss.o","sha256":"e5b373fb7e33c343f9e25be39f56a1ce0c4d1809623774b3dd51758e0731ba81","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"initMiniboss__8Creature","compiled_body_bytes":16}]}
{"source":"sources/vslots-a-is-enemy/is_enemy.cpp","object_name":"vslots-a-is-enemy.o","sha256":"7ac33e5530bb3eef2bdea303b81e9b71892f4ea6bd0205fa8119a82764a68f71","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"isEnemy__8CreatureP10GameObject","compiled_body_bytes":284}]}
{"source":"sources/vslots-a-msg-alert/msg_alert.cpp","object_name":"vslots-a-msg-alert.o","sha256":"76b91eab68d2164a712256acee9ccf7725536c4171ee3248340c18742ca98d89","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_alert__8CreatureifG6Point3P10GameObject","compiled_body_bytes":52}]}
{"source":"sources/vslots-a-msg-collision-cat/collision.cpp","object_name":"vslots-a-msg-collision-cat.o","sha256":"6544c6aa2f724e1e539b3158730e4185e8ef3e6dadf56cd86b969f86074c0678","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_collision__3CatP10GameObject","compiled_body_bytes":220}]}
{"source":"sources/vslots-a-msg-collision-creature/msg_collision.cpp","object_name":"vslots-a-msg-collision-creature.o","sha256":"9f905361333163ac9da6709eef80cc2c66e89db9c315687b2d6751510311fd62","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_collision__8CreatureP10GameObject","compiled_body_bytes":8}]}
{"source":"sources/vslots-a-msg-collision-spider/collision.cpp","object_name":"vslots-a-msg-collision-spider.o","sha256":"a9b0b95743bcdc6639d7c2f40a1fd175620e4988adda9cd9d6c6f88874c58ecb","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_collision__11SmallSpiderP10GameObject","compiled_body_bytes":180}]}
{"source":"sources/vslots-a-msg-collision-world/msg_collision_world.cpp","object_name":"vslots-a-msg-collision-world.o","sha256":"5975d591661339b6a3e9bc911ceb45a8da7365118c9f16868ca1a1b80e5cb6d3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"msg_collisionWorld__8CreatureG6Point3","compiled_body_bytes":52}]}
{"source":"sources/vslots-a-tf-11smallspider/typeinfo.cpp","object_name":"vslots-a-tf-11smallspider.o","sha256":"2bd7d3c59d3c343cb088ed982f0351930a720020578463c42726ad1e2331df82","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf11SmallSpider","compiled_body_bytes":92}]}
{"source":"sources/vslots-a-tf-13pokereflector/typeinfo.cpp","object_name":"vslots-a-tf-13pokereflector.o","sha256":"3322121fe1a67a64ea30836ed4356ae751c1cb9feaa4ba86794df125fda2211f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf13PokeReflector","compiled_body_bytes":92}]}
{"source":"sources/vslots-a-tf-13triggerparams/typeinfo.cpp","object_name":"vslots-a-tf-13triggerparams.o","sha256":"edc812995a6477b041316f3c5d70382a5c723aeff29c971aeccec2d7cda50377","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf13TriggerParams","compiled_body_bytes":92}]}
{"source":"sources/vslots-a-tf-14gnomenavigator/typeinfo.cpp","object_name":"vslots-a-tf-14gnomenavigator.o","sha256":"65e40bda161078d80db716730d53071f762a5b35a88ebb2ea6cc62918af7d60a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf14GnomeNavigator","compiled_body_bytes":92}]}
{"source":"sources/vslots-a-tf-3cat/typeinfo.cpp","object_name":"vslots-a-tf-3cat.o","sha256":"fc0aa0f2a3522937de768c9c5ffc2fe3098e6f8c65d29dc02eabff8dd66d5c1d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf3Cat","compiled_body_bytes":92}]}
{"source":"sources/vslots-a-tf-4trap/typeinfo.cpp","object_name":"vslots-a-tf-4trap.o","sha256":"febf72a79fb30d4b5acae8f9b437e81d9a213bfa4cc902068baf8bdc42ac028e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf4Trap","compiled_body_bytes":92}]}
{"source":"sources/vslots-a-tf-5lever/typeinfo.cpp","object_name":"vslots-a-tf-5lever.o","sha256":"ae64e3f9f7587511a7a95387e310e1e44431e983d93a66955d7e61da2413a409","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf5Lever","compiled_body_bytes":92}]}
{"source":"sources/vslots-a-tf-5timer/typeinfo.cpp","object_name":"vslots-a-tf-5timer.o","sha256":"e9dedd72e7b7360895c742db50f919be4e4794b61d1107bc3587eee16336a8d9","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf5Timer","compiled_body_bytes":92}]}
{"source":"sources/vslots-a-tf-7counter/typeinfo.cpp","object_name":"vslots-a-tf-7counter.o","sha256":"8db2051790391b852c5bcf6a8e66afd5a8b0a68425caed2f52b24ffc8a238de1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf7Counter","compiled_body_bytes":92}]}
{"source":"sources/vslots-a-tf-7mermaid/typeinfo.cpp","object_name":"vslots-a-tf-7mermaid.o","sha256":"2bfa7a81bff2fbbbb3975e6d6a665a54ca400148d3ac26ac2963c811df623c6c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf7Mermaid","compiled_body_bytes":92}]}
{"source":"sources/vslots-a-tf-7natasla/typeinfo.cpp","object_name":"vslots-a-tf-7natasla.o","sha256":"af265c1d22fd4318f40fea628ddba69a54b47a38cea0ab17b21abeaa29a9365c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf7Natasla","compiled_body_bytes":92}]}
{"source":"sources/vslots-a-tf-9cdeyeball/typeinfo.cpp","object_name":"vslots-a-tf-9cdeyeball.o","sha256":"4b41a9a4c5730c0cca9ff49fcc61d5e2b6812c09b107158bf2d5ec77ea95c7df","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf9CdEyeball","compiled_body_bytes":92}]}
{"source":"sources/vslots-a-tf-creature/typeinfo.cpp","object_name":"vslots-a-tf-creature.o","sha256":"52512e0cce974bea3d03bc61b72040ad4ffef93fe4fab99fea9c47f585d202e6","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"__tf8Creature","compiled_body_bytes":92}]}
```

## Authored but never executed

All 48 functions are authored and covered by a host check on this Mac. Their
EE objects were compiled with the reviewed toolchain. None of them is on the
startup path that reaches `engineFrameEnd`, and none was executed on the EE
or as gameplay.

## Unknown

The compiled bodies are not byte-identical to the image. `__rtti_si`,
`__15AnimationState8RC15AnimationState8`, `_$_14AnimationState`, and
`_$_15FxCreatureState` are unresolved, so their stores are not reproduced
here. `readyToSwitch__3Cat`, `switchToRun__3Cat`,
`findTagString__10GameObjectPci`, `scriptStart__FPc`,
`dropItem__4itemG6Point3Pc`, `animSetToEnd__FP14AnimationState`, and
`objectAddToDeleteList__FP10GameObject` are unresolved as well.

Creature-sized clones copy the bytes the body stores. Bytes 286 through 551
are left for the AnimationState copy constructor at object+288, and the
ListHead words at 0, 4, 8, 576, 752, and 888 are left for that constructor.
The GameObject vtable is what the first three ListHead calls observe. The
Creature vtable is what the animation copy observes. SmallSpider, Cat, and
CdEyeball replace that with their own vtable after the last field copy.
The original interleaving of those stores is not reproduced. The host checks
cover the final bytes and the values those calls observe.

`snapshot.json` still needs these 48 sources. This package does not edit it.
