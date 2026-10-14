# Actor, creature, player, and next data views

Package work is on `agents/data-views`, based on revision `3079057`. The shared reconstruction tip moved while this branch was isolated. The counts below are from this branch's own partial link, not from that later tip.

The partial link at that revision had 1040 unresolved imports. Adding these 98 objects closed 98 view imports and opened 142 function imports. COMMON stayed zero. The absolute totals will differ once other packages are merged. Host checks returned success for every object. The reviewed EE g++ `authored_cpp` profile compiled every object, and a private relocatable link included them. Nothing ran on the EE or as gameplay.

## Evidence

Observed, inferred, and unknown stay separated in the source comments. Short form:

- Tag views are C strings. The object is the characters plus the terminating NUL. Alignment padding after that NUL is a placement gap, not part of the view.
- `.lit4` views are one float each. The source literal is the observed 32-bit pattern.
- Sized ELF objects keep that size and their initial bytes. Zero words and zero bytes are BSS. `nmPlayers` and `gameDifficulty` start at 1. `worldDifficulty` starts at float 1.
- `fsList` and `fsListBack` record symbol size 0. The next symbol starts 4 bytes later, and every caller loads or stores one word, so each view is a 4-byte zero word.
- Resource maps are pointer-plus-zero-word tables ending in a null slot. The names are the strings those pointers select. The second word of every slot is zero.
- `globalItem` is 1696 bytes: 106 records of mesh pointer, texture pointer, and two zero words. Callers index it with a 16-byte stride.
- Virtual tables are 8-byte slots. The first word is the adjustment already loaded by authored virtual calls. The second word is a function pointer, or zero when the image word is zero. The function names are the ELF function symbols at those addresses.

## Imports closed

- `norrath_actor_view_42b118`
- `norrath_actor_view_42b768`
- `norrath_actor_view_42baa8`
- `norrath_actor_view_42c130`
- `norrath_actor_view_42c710`
- `norrath_actor_view_42d0c0`
- `norrath_actor_view_4488f0`
- `norrath_actor_view_44b170`
- `norrath_actor_view_44b540`
- `norrath_actor_view_44c170`
- `norrath_actor_view_44e1c0`
- `norrath_actor_view_44e6a0`
- `norrath_actor_view_44f4c8`
- `norrath_actor_view_460710`
- `norrath_actor_view_460720`
- `norrath_actor_view_460730`
- `norrath_actor_view_460740`
- `norrath_actor_view_47fb54`
- `norrath_actor_view_480eec`
- `norrath_actor_view_480ef0`
- `norrath_actor_view_48113c`
- `norrath_actor_view_481140`
- `norrath_actor_view_4813a4`
- `norrath_actor_view_4813a8`
- `norrath_actor_view_4813ac`
- `norrath_actor_view_4816d8`
- `norrath_actor_view_481a08`
- `norrath_actor_view_481a0c`
- `norrath_actor_view_481a10`
- `norrath_actor_view_4837a8`
- `norrath_actor_view_4837b0`
- `norrath_actor_view_4837b8`
- `norrath_actor_view_4837c0`
- `norrath_actor_view_4837c8`
- `norrath_actor_view_4837d0`
- `norrath_actor_view_4837d8`
- `norrath_actor_view_483ad0`
- `norrath_actor_view_484318`
- `norrath_actor_view_4844a4`
- `norrath_actor_view_484f98`
- `norrath_actor_view_485150`
- `norrath_actor_view_485410`
- `norrath_actor_view_485418`
- `norrath_creature_view_47fb58`
- `norrath_creature_view_47fb5c`
- `norrath_creature_view_47fb60`
- `norrath_creature_view_47fb64`
- `norrath_creature_view_47fb68`
- `norrath_creature_view_4837e0`
- `norrath_creature_view_483829`
- `norrath_creature_view_483e58`
- `norrath_creature_view_484538`
- `norrath_creature_view_484cac`
- `norrath_creature_view_484cd8`
- `norrath_creature_view_48690c`
- `norrath_creature_view_486910`
- `norrath_next_view_42c650`
- `norrath_next_view_4490d0`
- `norrath_next_view_449160`
- `norrath_next_view_4491f0`
- `norrath_next_view_449280`
- `norrath_next_view_44c958`
- `norrath_next_view_44e820`
- `norrath_player_view_31f540`
- `norrath_player_view_429410`
- `norrath_player_view_42a3f8`
- `norrath_player_view_44c9e8`
- `norrath_player_view_44cd48`
- `norrath_player_view_44cef8`
- `norrath_player_view_44d018`
- `norrath_player_view_44d2e8`
- `norrath_player_view_462350`
- `norrath_player_view_462360`
- `norrath_player_view_4623a0`
- `norrath_player_view_4623b0`
- `norrath_player_view_47fed4`
- `norrath_player_view_47fed8`
- `norrath_player_view_47fedc`
- `norrath_player_view_47fee0`
- `norrath_player_view_483ba0`
- `norrath_player_view_483ba8`
- `norrath_player_view_483bb0`
- `norrath_player_view_483bb8`
- `norrath_player_view_483bc0`
- `norrath_player_view_483bc8`
- `norrath_player_view_483bd0`
- `norrath_player_view_483bd8`
- `norrath_player_view_483be0`
- `norrath_player_view_483be8`
- `norrath_player_view_483bf0`
- `norrath_player_view_483c08`
- `norrath_player_view_483c10`
- `norrath_player_view_483c18`
- `norrath_player_view_483c20`
- `norrath_player_view_483c28`
- `norrath_player_view_483f3e`
- `norrath_player_view_484ca8`
- `norrath_player_view_484f38`

## Imports opened

These are the ELF function symbols referenced by the virtual tables. They were not unresolved imports before this package. Their bodies are not defined here.

- `_$_11SmallSpider`
- `_$_13PokeReflector`
- `_$_13TriggerParams`
- `_$_13UserParamProp`
- `_$_14GnomeNavigator`
- `_$_3Cat`
- `_$_4Gold`
- `_$_4Prop`
- `_$_4Trap`
- `_$_5Lever`
- `_$_5Timer`
- `_$_7Counter`
- `_$_7Mermaid`
- `_$_7Natasla`
- `_$_7Player1`
- `_$_8Creature`
- `_$_8ItemProp`
- `_$_9CdEyeball`
- `__tf11SmallSpider`
- `__tf13PokeReflector`
- `__tf13TriggerParams`
- `__tf13UserParamProp`
- `__tf14GnomeNavigator`
- `__tf3Cat`
- `__tf4Gold`
- `__tf4Prop`
- `__tf4Trap`
- `__tf5Lever`
- `__tf5Timer`
- `__tf7Counter`
- `__tf7Mermaid`
- `__tf7Natasla`
- `__tf7Player1`
- `__tf8Creature`
- `__tf8ItemProp`
- `__tf9CdEyeball`
- `clone__10GameObject`
- `clone__11SmallSpider`
- `clone__13PokeReflector`
- `clone__13TriggerParams`
- `clone__13UserParamProp`
- `clone__3Cat`
- `clone__4Gold`
- `clone__4Prop`
- `clone__5Lever`
- `clone__5Timer`
- `clone__7Counter`
- `clone__7Player1`
- `clone__8Creature`
- `clone__8ItemProp`
- `clone__9CdEyeball`
- `confuse__8Creaturei`
- `death__8Creature`
- `getMinibossName__8Creature`
- `getModel__10GameObject`
- `getModel__8Creature`
- `getSkillRank__8Creaturei`
- `initMiniboss__8Creature`
- `isEnemy__8CreatureP10GameObject`
- `msg_alert__10GameObjectifG6Point3P10GameObject`
- `msg_alert__8CreatureifG6Point3P10GameObject`
- `msg_collisionWorld__10GameObjectG6Point3`
- `msg_collisionWorld__8CreatureG6Point3`
- `msg_collision__10GameObjectP10GameObject`
- `msg_collision__11SmallSpiderP10GameObject`
- `msg_collision__3CatP10GameObject`
- `msg_collision__4GoldP10GameObject`
- `msg_collision__8CreatureP10GameObject`
- `msg_draw__10GameObject`
- `msg_draw__11SmallSpider`
- `msg_draw__13UserParamProp`
- `msg_draw__14GnomeNavigator`
- `msg_draw__3Cat`
- `msg_draw__4Gold`
- `msg_draw__4Prop`
- `msg_draw__4Trap`
- `msg_draw__5Lever`
- `msg_draw__7Mermaid`
- `msg_draw__7Natasla`
- `msg_draw__8ItemProp`
- `msg_draw__9CdEyeball`
- `msg_getPokeName__10GameObjectPUsi`
- `msg_getPokeName__13PokeReflectorPUsi`
- `msg_getPokeName__13UserParamPropPUsi`
- `msg_getPokeName__5LeverPUsi`
- `msg_getPokeName__8CreaturePUsi`
- `msg_getPokeName__8ItemPropPUsi`
- `msg_hurt__10GameObjectfP10GameObjecti`
- `msg_hurt__8CreaturefP10GameObjecti`
- `msg_load__10GameObjectPc`
- `msg_load__4GoldPc`
- `msg_load__4PropPc`
- `msg_load__5LeverPc`
- `msg_load__7CounterPc`
- `msg_load__8CreaturePc`
- `msg_load__8ItemPropPc`
- `msg_moveCollision__10GameObjectPP10GameObject`
- `msg_moveCollision__8CreaturePP10GameObject`
- `msg_poke__10GameObjecti`
- `msg_poke__13PokeReflectori`
- `msg_poke__13UserParamPropi`
- `msg_poke__5Leveri`
- `msg_poke__8Creaturei`
- `msg_poke__8ItemPropi`
- `msg_route__10GameObjecti`
- `msg_route__8Creaturei`
- `msg_run__10GameObject`
- `msg_run__11SmallSpider`
- `msg_run__13UserParamProp`
- `msg_run__14GnomeNavigator`
- `msg_run__3Cat`
- `msg_run__4Gold`
- `msg_run__4Prop`
- `msg_run__4Trap`
- `msg_run__5Lever`
- `msg_run__5Timer`
- `msg_run__7Mermaid`
- `msg_run__7Natasla`
- `msg_run__9CdEyeball`
- `msg_save__10GameObjectPc`
- `msg_save__4GoldPc`
- `msg_save__4PropPc`
- `msg_save__5LeverPc`
- `msg_save__7CounterPc`
- `msg_save__8CreaturePc`
- `msg_save__8ItemPropPc`
- `msg_trigger__10GameObjecti`
- `msg_trigger__13TriggerParamsi`
- `msg_trigger__5Timeri`
- `msg_trigger__7Counteri`
- `msg_trigger__8Creaturei`
- `runSpells__8CreatureG6Point3`
- `setActiveChunks__8Creature`
- `setState__11SmallSpideri`
- `setState__14GnomeNavigatori`
- `setState__3Cati`
- `setState__7Mermaidi`
- `setState__7Nataslai`
- `setState__8Creaturei`
- `stun__8Creaturei`
- `turnToFace__8CreatureiG6Point3`
- `updateEnemy__8Creature`

## Build-plan entries needed

Append these authored inputs and assign fresh `input_order` values. Do not reuse orders from revision `3079057`. Do not add anything under `data-views-host-checks`: those files are host `main` programs. `header_closure` is empty. `profile` is `authored_cpp`. No `private_object_sha256` is claimed. These units emit no functions.

`default_authored_inputs` increases by 98.

```json
{"source":"sources/data-views-symbols/norrath_actor_view_42b118.cpp","object_name":"norrath_actor_view_42b118.o","sha256":"fc4f0fbf6a17b470fcaaa73eb56e30bb87ba5cbdb796dc9bfa9cd8a32bdaaa08","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_42b768.cpp","object_name":"norrath_actor_view_42b768.o","sha256":"4186e315568174b63c28472e6b0b3f63470d689b3a30cdbb5b1be4a4bb73b470","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_42baa8.cpp","object_name":"norrath_actor_view_42baa8.o","sha256":"35df379600a367754daa7796dc4a8648b22545f761eabe18190cfb2834b84bbb","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_42c130.cpp","object_name":"norrath_actor_view_42c130.o","sha256":"2e101aa16fcce46aa994ffe6bcfdca0f971b5355c2c9dae0be9e70081b789ee6","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_42c710.cpp","object_name":"norrath_actor_view_42c710.o","sha256":"e8523052dd806a2e9ef106539b5125b71bc86ade1451c326ced95bf049597d1f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_42d0c0.cpp","object_name":"norrath_actor_view_42d0c0.o","sha256":"7a649c0fa83300a0c4311c96dfd07e209f1f294cdbe46bb40ded9b49bde23730","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_4488f0.cpp","object_name":"norrath_actor_view_4488f0.o","sha256":"faa425dc16d00534c32cf2d90afff8105facbc2db793074a19145531e9800dcb","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_44b170.cpp","object_name":"norrath_actor_view_44b170.o","sha256":"0b7228c0efb0ecd608d2daa25fc357695e2383a0825c1169ad2ec92b919e89c1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_44b540.cpp","object_name":"norrath_actor_view_44b540.o","sha256":"dbf455c68d55beb61ebdbf8e699031503e500fbdad7e1f8026bcd604eb3ceded","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_44c170.cpp","object_name":"norrath_actor_view_44c170.o","sha256":"06d78d2c8d4baefa46218d2a95dfffebf3dd0a3d15b8ac2709947a9f240145f3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_44e1c0.cpp","object_name":"norrath_actor_view_44e1c0.o","sha256":"ec8c4b1250650fc101c38bef9b6c60406e534eed174b4c30b8fa997a0f9521b9","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_44e6a0.cpp","object_name":"norrath_actor_view_44e6a0.o","sha256":"2bd0550fcfb77312e92596660ba3a6a6e8afb0297fd5ea5fd62758c2512dc34c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_44f4c8.cpp","object_name":"norrath_actor_view_44f4c8.o","sha256":"1a0ca54f9b8e8750f5481913a05b446297f1fcc9402e555f9f62ca43103dfbc7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_460710.cpp","object_name":"norrath_actor_view_460710.o","sha256":"6aea23ff551f8fe0c13604e01619bd7edd6dec73e410d484972385bb88c56c41","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_460720.cpp","object_name":"norrath_actor_view_460720.o","sha256":"bc8feecaf2b7d6e534eb73dd3bb10ebe297b7236d17039529d8080c92d63bde7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_460730.cpp","object_name":"norrath_actor_view_460730.o","sha256":"3f1f96f21ad5ddf02f9471c7509df23a5e0805d7891ffc809d9886c13ca991f6","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_460740.cpp","object_name":"norrath_actor_view_460740.o","sha256":"37523836f3032ae7e91af40e19d16f12ea183afaa3a94d619bc902678c3e5fd3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_47fb54.cpp","object_name":"norrath_actor_view_47fb54.o","sha256":"455938307ce5c59485a621bff70064e346c8364f26a1181c0af49c2e77a9d26e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_480eec.cpp","object_name":"norrath_actor_view_480eec.o","sha256":"2010e221e7ae841a00a22442c255f9897e52a1c4710abfbe5bb1e53bd0ef4f87","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_480ef0.cpp","object_name":"norrath_actor_view_480ef0.o","sha256":"cf71799c7899002ca60e67bc354e689dc0494bd50faa2b4042f199993ab23816","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_48113c.cpp","object_name":"norrath_actor_view_48113c.o","sha256":"3f50b9ca61d0a51af97bffb515fff777810510e4185f05c1f2d61dcbc46e9396","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_481140.cpp","object_name":"norrath_actor_view_481140.o","sha256":"8e171aa17711df3166eed55208a28bd4bd8dd8e333e5b2f268f2cfd454fec1f7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_4813a4.cpp","object_name":"norrath_actor_view_4813a4.o","sha256":"75ced2d1444e7b274e703f2feae56b82bdb2d7ffee0239e411bbce6c9586c346","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_4813a8.cpp","object_name":"norrath_actor_view_4813a8.o","sha256":"aff5ab875fa7c433312b8c669a9082e1299b343b1b6e32cc8a17d77eb7d51e09","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_4813ac.cpp","object_name":"norrath_actor_view_4813ac.o","sha256":"1b0fff5e3b2c74a5155d29d9edf68fd98475e31232e46c6422a3bb2d58925f27","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_4816d8.cpp","object_name":"norrath_actor_view_4816d8.o","sha256":"10dd8a2050bfd5543591c92aff5924dcd1be7ed1b07e12b20a13fc79fafcd44d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_481a08.cpp","object_name":"norrath_actor_view_481a08.o","sha256":"10ce2932da8a26784da60d74e0394f5360db883fae65762d1db9b392da543f71","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_481a0c.cpp","object_name":"norrath_actor_view_481a0c.o","sha256":"6e30a2f9d881c5703fe45a21fba0083208b89d2b9e95f575b1a2ff7646062548","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_481a10.cpp","object_name":"norrath_actor_view_481a10.o","sha256":"304d9d46cd91c3865a47e68f29b8bd09fa33921b508af60249478412cffc9830","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_4837a8.cpp","object_name":"norrath_actor_view_4837a8.o","sha256":"a67dd81dc3a43d718705beecec4ac65a42d27be6bdc688442d83e51a9680852f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_4837b0.cpp","object_name":"norrath_actor_view_4837b0.o","sha256":"0156057b06465add75cf558eb340bac8827d763ab47ff1645d4fc7ac925b1436","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_4837b8.cpp","object_name":"norrath_actor_view_4837b8.o","sha256":"69a8c03b9861eba6502ba1f1f7e7ff6ecb8c2803859914c28aaaba7a8729173f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_4837c0.cpp","object_name":"norrath_actor_view_4837c0.o","sha256":"cebfe9117f780564abb6330bda1fd9b526a3f149b65fb7c6ceb6d8e13e0320c7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_4837c8.cpp","object_name":"norrath_actor_view_4837c8.o","sha256":"482457de3e88a8f13427736c8d744ff7d682f83b3771919b2215382a5a4aeb56","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_4837d0.cpp","object_name":"norrath_actor_view_4837d0.o","sha256":"7f442608724c5897a9f489033f928677f237dcdc0171f49619765ef138af4f45","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_4837d8.cpp","object_name":"norrath_actor_view_4837d8.o","sha256":"984e503de8de107b03b9c37455b83912cf459e3eea8fcdccc3e24f6e2a35fea8","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_483ad0.cpp","object_name":"norrath_actor_view_483ad0.o","sha256":"a8e6d20e0bde8a3ca2a32899c2937c3a4c915d5ca0091f79470017f5f231f251","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_484318.cpp","object_name":"norrath_actor_view_484318.o","sha256":"4a7b2e1fd3dad34044206e6e5b777f352f9e04441db701a5900558e9f5edb40d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_4844a4.cpp","object_name":"norrath_actor_view_4844a4.o","sha256":"e9ccbc5e21cfbd7bfe1b49d50f2b39e68b9e1b344cf456e65dc9e8e56ed01bdf","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_484f98.cpp","object_name":"norrath_actor_view_484f98.o","sha256":"51eafe40ccd093a713b55842a28b2ab294ef38d3f2c05966079bd6eac4b771ab","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_485150.cpp","object_name":"norrath_actor_view_485150.o","sha256":"f8b6b3d5bff265a0f37a4815e210d5de1f1563f9d671e112fa47b0e6a3abe7cc","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_485410.cpp","object_name":"norrath_actor_view_485410.o","sha256":"7ba9bf6176b5a9ef5996e79b080bdec210185092208021003c7e8b98dfcfde46","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_actor_view_485418.cpp","object_name":"norrath_actor_view_485418.o","sha256":"d84beec250d1428c8373e24075842f716525469c648c8d18e50628db95950521","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_creature_view_47fb58.cpp","object_name":"norrath_creature_view_47fb58.o","sha256":"dec31e045956cf4604dbf2893eb238676bc752976898d364c54bbd6cbe0405b0","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_creature_view_47fb5c.cpp","object_name":"norrath_creature_view_47fb5c.o","sha256":"58610fca38edbe6f20729422996546ab5334729c94b40e4ad989d9ad768e32d2","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_creature_view_47fb60.cpp","object_name":"norrath_creature_view_47fb60.o","sha256":"cb31983be75246b523e3128b7ae7910b6f1a0196e58c26f6e2304a6042607c5a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_creature_view_47fb64.cpp","object_name":"norrath_creature_view_47fb64.o","sha256":"bd5a9c045ffdbccce56df7738ed2667612e51a78bbf55b527dff3e6bf2a6632d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_creature_view_47fb68.cpp","object_name":"norrath_creature_view_47fb68.o","sha256":"ca007e5ba0687a46da5e6f98ba6f326279f1d8a4be6db042dc4ee3aae63b4ea1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_creature_view_4837e0.cpp","object_name":"norrath_creature_view_4837e0.o","sha256":"1c58c2d3e98fe0cadf4c45decbdca69abd5d59038a039d266b384f1f3e72c134","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_creature_view_483829.cpp","object_name":"norrath_creature_view_483829.o","sha256":"999ad4de8db8a293fb02ba5133252d546e9575a32af11ab28eae9fcb982fe318","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_creature_view_483e58.cpp","object_name":"norrath_creature_view_483e58.o","sha256":"e074726c16be0bae417a6c656d3093c6b3b0af1d4252fb84b1d61ec16ebaa804","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_creature_view_484538.cpp","object_name":"norrath_creature_view_484538.o","sha256":"ad8c023e1991a8699264fdb3e68d679910dbb0b18c961fa83da714888c8721d0","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_creature_view_484cac.cpp","object_name":"norrath_creature_view_484cac.o","sha256":"5044060e1e49dfc1884e92bd7929deb2fe771df2933fdeaeb3f6910d0483fc1f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_creature_view_484cd8.cpp","object_name":"norrath_creature_view_484cd8.o","sha256":"c2cabb285032b452a27673b1888a81ef8987777835bc4877e3ae66c7ad33582e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_creature_view_48690c.cpp","object_name":"norrath_creature_view_48690c.o","sha256":"2ab5fdec7f338e35b841724d4cc5cc788cb3fb2ad972c5a5c396b077dd84496e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_creature_view_486910.cpp","object_name":"norrath_creature_view_486910.o","sha256":"ffd401bd98dc4fba32d7cecec01a4cb94e1cd94119a114565254fcdc6429b160","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_next_view_42c650.cpp","object_name":"norrath_next_view_42c650.o","sha256":"61a5616d8147d0649988d491e55ad1f380bc2d9f3af2bf3b89979b5d15f33f3b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_next_view_4490d0.cpp","object_name":"norrath_next_view_4490d0.o","sha256":"1df2b0ccc9c852d203d5a20aa56486bf59af195574ba8f27fd594d3adc72a977","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_next_view_449160.cpp","object_name":"norrath_next_view_449160.o","sha256":"38db056c4d07680218c7589ebe9ff2c43d5282eb7eca4e1bcfc4e7d95d3e7617","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_next_view_4491f0.cpp","object_name":"norrath_next_view_4491f0.o","sha256":"387cf965e68d02d23001474d6953263836955c9b1453494326a43b3db28b58e0","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_next_view_449280.cpp","object_name":"norrath_next_view_449280.o","sha256":"79573fa555a5b5ebb1dc92c94d9e6386518d82e7bbbdaec3d917f5347e884bc7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_next_view_44c958.cpp","object_name":"norrath_next_view_44c958.o","sha256":"82c8e3dd77183ffc963c9d23d444ed83b80dd348710298da19bad8ccde82ce8b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_next_view_44e820.cpp","object_name":"norrath_next_view_44e820.o","sha256":"293bc0e9b2526498752fa8df556e34aa1c411d2b04de2a78cdfc554101a33a97","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_31f540.cpp","object_name":"norrath_player_view_31f540.o","sha256":"87ad47c7fc7f88ad53d15c9ad8be4456c109434d32c1a25027e9b3eeb15c61a3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_429410.cpp","object_name":"norrath_player_view_429410.o","sha256":"b1efbc47a835a073915662c2b94657423f75ef4c21c7c2baeabee508978f49ad","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_42a3f8.cpp","object_name":"norrath_player_view_42a3f8.o","sha256":"9ba3c55d5a91160c4a9326e0a4fcba714d335dadf161c61d7fc0cdd8e145ce63","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_44c9e8.cpp","object_name":"norrath_player_view_44c9e8.o","sha256":"09f1888f2b7f017ab43473ed7bdae0aa67ac411089de69eb5bea98b7a9d5fc01","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_44cd48.cpp","object_name":"norrath_player_view_44cd48.o","sha256":"570b5a420599a8bc8bf5ff6477e1e5cb6c63ddfaf9ed29ad2e60e2c36822834d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_44cef8.cpp","object_name":"norrath_player_view_44cef8.o","sha256":"c233dc566a453ec57fc3b4c26b7b5b06a837260ad5a6763b4e4400148717ad42","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_44d018.cpp","object_name":"norrath_player_view_44d018.o","sha256":"7a64cadc78b0018b5bde1aab847d5fa9c8ccab1ce0ff535a6e2db278280c0567","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_44d2e8.cpp","object_name":"norrath_player_view_44d2e8.o","sha256":"a34d465d4862d4d947652c96cdc5505f9c95cd30339f68b0a0f1a9216f914d41","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_462350.cpp","object_name":"norrath_player_view_462350.o","sha256":"62a11808eca0f9639136b7bd7304bb156164ecf623d40696da52856b91a200ec","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_462360.cpp","object_name":"norrath_player_view_462360.o","sha256":"5931ef9d16f94c1b07c68f35ad3a6629e84c86d2a40ec6e5fba1e3b5ef3e99f4","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_4623a0.cpp","object_name":"norrath_player_view_4623a0.o","sha256":"f2b4f9f4a9d56171068998bb5e997a2bbbc54fe782ba09d79a3c41e0d9ae5d52","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_4623b0.cpp","object_name":"norrath_player_view_4623b0.o","sha256":"ff0978f59851b8a5c3b18b6c128ce8e493cac4b2f03cf5dee65581752abfd90d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_47fed4.cpp","object_name":"norrath_player_view_47fed4.o","sha256":"a7b4202ed4edfdb81cf38700dc699383b450d86f9e6ab2cca9ab5dd64f37e559","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_47fed8.cpp","object_name":"norrath_player_view_47fed8.o","sha256":"d391e6955bb9493ce5b999ad0f4061e67d25de5bd043237b4c696c58899ac99d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_47fedc.cpp","object_name":"norrath_player_view_47fedc.o","sha256":"6fa1070a942ad16d15237b3668aada800484a795eed62a0f6658a7270d42834e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_47fee0.cpp","object_name":"norrath_player_view_47fee0.o","sha256":"996eff204809f83d79751eee147a4400bef5b867b03a4d9f3297d415bd0ae81e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483ba0.cpp","object_name":"norrath_player_view_483ba0.o","sha256":"a82f19378a8f990b8b9b42165d0a41a386415aea19c4090257a643ad6d5bf5d0","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483ba8.cpp","object_name":"norrath_player_view_483ba8.o","sha256":"b8cbb362bc01fed9553f7d10ba8e057061ea3f7aa9792f768c58c801da08dc48","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483bb0.cpp","object_name":"norrath_player_view_483bb0.o","sha256":"6cf7669abdccd1959c93625171a07ed76a1d9c88864b55f04a6887db17bde713","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483bb8.cpp","object_name":"norrath_player_view_483bb8.o","sha256":"fd4658e13f3b93f42f734f43a37d55e96ba1b18712f68de07df91c474f1c31c0","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483bc0.cpp","object_name":"norrath_player_view_483bc0.o","sha256":"34c99dc343f0930b56941a7b999facf06dcef310021c6349347b25ea16faa025","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483bc8.cpp","object_name":"norrath_player_view_483bc8.o","sha256":"d3eba7ac38111d7d84e68a4602247094f828ad84179f2a09a2b7762c6782bd8d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483bd0.cpp","object_name":"norrath_player_view_483bd0.o","sha256":"a6772b22be2e07cd2b85e814c7a30af852e0abe48471cce083b90c6c4fe80a06","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483bd8.cpp","object_name":"norrath_player_view_483bd8.o","sha256":"accf8ad158702185d8304a8462d5392b8891e736a828a41ab17ba08689a62ff5","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483be0.cpp","object_name":"norrath_player_view_483be0.o","sha256":"96b85179374d3fdf26f009e25918b15cff64fd676198205a23646d4092ad5464","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483be8.cpp","object_name":"norrath_player_view_483be8.o","sha256":"44a8e1b7b7a2a3c814ceff24da3558c5ae85a2065d6db20a997e23a82dd52fde","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483bf0.cpp","object_name":"norrath_player_view_483bf0.o","sha256":"ec446218e88b41d1ee31e08230d1769082a191f94d9225e36fd390ddabd198c8","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483c08.cpp","object_name":"norrath_player_view_483c08.o","sha256":"e99adb9d81d56763398c54da9a15c8879b23067743ec6fbb2a1a97944284d017","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483c10.cpp","object_name":"norrath_player_view_483c10.o","sha256":"6b70dfc965b0c3e7cfd844edcb9f3febe2a49f66de6c55f52bae20597ac73ad3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483c18.cpp","object_name":"norrath_player_view_483c18.o","sha256":"749a39983c9278883df45fa82b48a6a91da322297630f7870c516ae3e579f6bf","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483c20.cpp","object_name":"norrath_player_view_483c20.o","sha256":"867e96ba0a9202b8492d8d65403e3dc730fcc14fc88f59ed5c197160f3e83f2f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483c28.cpp","object_name":"norrath_player_view_483c28.o","sha256":"fcfee2d39ecee7498af6b26c00d32dd980bbbe54c8a5ea307da0acb85ea976b3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_483f3e.cpp","object_name":"norrath_player_view_483f3e.o","sha256":"288639fd122f6f8dd4a82b9c20fd14089135c2499d4b91908d0e0b1c03a92c27","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_484ca8.cpp","object_name":"norrath_player_view_484ca8.o","sha256":"acac2881b6f250efba0fb15106c47ec4009f5f7e87193efe27eb78330747997c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/data-views-symbols/norrath_player_view_484f38.cpp","object_name":"norrath_player_view_484f38.o","sha256":"78e7c2ccc21c04c725009b7b9bb28f28d06c31ce552ff5c291c62747a128afe2","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
```

## Authored but never executed

All 98 objects. Host comparisons ran on this Mac and failed the process when a string, float bit pattern, initial scalar, map name, proxy name, adjustment, or virtual-slot function disagreed. The reviewed EE compiler produced relocatable objects, and those objects were compared with the pinned executable for the same bytes and relocation targets. No object was executed on the EE, under the original process, or against gameplay. Virtual calls through the new tables were not executed. The private link only checked symbol closure.

## Unknown

- `norrath_player_view_34b752` sits 242 bytes inside ELF symbol `levelName` (`0x34b660`, 322 bytes). The whole parent is zero, and the caller only compares it as a C string. There is no separate symbol size for the field, so it stays unresolved.
- `norrath_actor_view_486878` is ELF symbol `runList` with recorded size 0. The next symbol starts 8 bytes later. Callers pass the address to unresolved `norrath_list_oneway_link`, which does not show how many bytes the head occupies.
- `norrath_actor_view_50bfd0` has no symbol size. One caller forms `base + (index << 2)` and wraps that index at 60, while the same source says the signed compare has no lower bound. The next named symbol is 49904 bytes later, so the array extent is not established.
- `norrath_player_view_600d20` is ELF symbol `goldAnm` with recorded size 0. The next symbol starts 16 bytes later. Callers pass it to unresolved `norrath_player_resource_map_get`.
- The virtual-table function bodies, overrides, and dispatch are not part of this package. A zero function word is a null slot. It is not one of the file symbols that also have address 0.
- These definitions close imports. They do not place the objects at the original addresses, and they do not claim section identity with `.lit4`, `.sdata`, or `.rodata`. Resource-name bytes are duplicated per table; pointer identity across tables is not preserved.
- `norrath_factory_text_483ba8` is the same original address as `norrath_player_view_483ba8` under another import name. This package defines only the player-view name.

