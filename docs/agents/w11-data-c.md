# w11-data-c: particle, resource-map and item data (3 of 3)

Branch `agents/w11-data-c`. New source is under `experimental/sources/w11-data-c-*`. Host checks sit beside each source and are not EE plan inputs. Every unit was compiled with the reviewed `authored_cpp` flags using the reviewed EE g++ (direct compile outside the official driver, one unit at a time; no partial build was run). Fresh EE objects are not original-byte matches. Nothing here ran on the EE.

## Evidence

- Observed: the pinned symbol table lists each particle-definition object below as a size-0 local BSS symbol followed by the next symbol exactly 0x50 bytes later, so each object is 80 bytes. Eight local symbols are named `smokeParticle`, four `trailParticleSpark` and two `trailParticle`.
- Observed: `udkResourceMap` is a 216-byte `.data` symbol at `0x0042d578`: 27 slots of a name pointer plus a zero word, the last slot null. All 26 names, read from the image through the pointers, equal the strings in the source. It duplicates the earlier address-named view `norrath_ctors_lava_wraith_view_42d578` under the original name.
- Observed: the words at `0x00482430`, `0x00482434` and `0x00482438` are each `0x3e4ccccc`. The bytes at `0x00483b70` and `0x00485380` both begin `fx.lmp` and a NUL.
- Observed: the 24 bytes at `0x0020fac8` (`__3Webii`) are five word stores and a return; the fresh object of `web_construct_name.cpp` has the same six instruction words.
- Inferred: the particle objects are filled by the particle-definition constructor during static initialisation, as the sibling static-init units show. Fields unknown.
- Unknown: which of several same-named originals a caller means for `smokeParticle`, `trailParticle` and `trailParticleSpark`; the definition is one global 80-byte object and does not choose. These definitions are not placed at the original addresses, so the static-init units (which define file-local objects of the same names) fill different objects than the ones callers read. That mismatch is why they stay "second link objects", as for the `particles` package.

## Units

| Unit | Defines | Check |
|---|---|---|
| `w11-data-c-web-construct-name` | function `norrath_web_construct` (24 bytes) | host store/no-other-byte test; same instruction words as the original |
| nine `w11-data-c-*-particle*` units | `smokeParticle`, `soulParticle`, `trailParticle`, `trailParticleSpark`, `vaporParticle`, `waveMainParticle`, `shockwaveInnoruukParticle`, `shockwaveInnoruukParticle2`, `summonhammerParticle` | host: size 80, all zero; EE `nm` size 0x50 in BSS |
| thirteen caller-named units | `norrath_w10_actor_d_view_7d6c40` (breathParticle1), `..._7d7720` (fuseParticle), `..._7d7770` (smokeParticle), `norrath_w10_actor_n_view_7d6d80` (soulParticle), `..._8d63c0` (fireParticle), `..._8d6460` (sparkParticle), `npc_msg_run_particle_7d7dd0` (kingParticle), `torch_msg_run_particle_7d7ba0/7c40/7c90` (torchParticle, heatParticle, fireSparkParticle), `undead_knight_msg_run_particle_7d7e30`, `undead_knight_clone_msg_run_particle_7d7e80/7ed0` (udkParticle2/3/4) | same |
| `w11-data-c-udk-resource-map` | `udkResourceMap` | host: size, null last slot, all 26 names, zero words |
| `w11-data-c-rtti-literals-482430` | three `0x3e4ccccc` words | host: value and about 0.2 as float |
| `w11-data-c-crt-b-view-483b70`, `-485380` | the `fx.lmp` strings | host: string and NUL |

`norrath_web_construct` is a second copy of the body of `__3Webii`. `w9-camera-map-web-construct` defines both names and was left out of the plan because `w9-startup-crt-a` already defines `__3Webii`; an alias-only unit does not work (a `.set` to an undefined symbol stays undefined in the EE object), so this unit carries the body. The integrator can drop it once callers use `__3Webii`.

`w11-data-c-rtti-literals-482430` overlaps the unselected `w9-rtti-vtables-entities-a-view-482424`, which also defines these three names (plus three other literals). Select only one.

## Imports closed

`norrath_rtti_entities_a_view_482430`, `norrath_rtti_entities_a_view_482434`, `norrath_rtti_entities_a_view_482438`, `norrath_w10_actor_d_view_7d6c40`, `norrath_w10_actor_d_view_7d7720`, `norrath_w10_actor_d_view_7d7770`, `norrath_w10_actor_n_view_7d6d80`, `norrath_w10_actor_n_view_8d63c0`, `norrath_w10_actor_n_view_8d6460`, `norrath_w9_startup_crt_b_view_483b70`, `norrath_w9_startup_crt_b_view_485380`, `norrath_web_construct`, `npc_msg_run_particle_7d7dd0`, `shockwaveInnoruukParticle`, `shockwaveInnoruukParticle2`, `smokeParticle`, `soulParticle`, `summonhammerParticle`, `torch_msg_run_particle_7d7ba0`, `torch_msg_run_particle_7d7c40`, `torch_msg_run_particle_7d7c90`, `trailParticle`, `trailParticleSpark`, `udkResourceMap`, `undead_knight_clone_msg_run_particle_7d7e80`, `undead_knight_clone_msg_run_particle_7d7ed0`, `undead_knight_msg_run_particle_7d7e30`, `vaporParticle`, `waveMainParticle`. All 29 of the assigned names. The official partial build was not run; the closure rests on the names defined by each unit's EE object (`nm`), checked against the assigned list.

## Imports opened

None. Every unit has an empty undefined-symbol list.

## Build-plan entries needed

Append to `inputs` with fresh `input_order` values (the entries below omit `input_order`); the hashes are the SHA-256 of each source file:

```jsonl
{"source":"sources/w11-data-c-actor-d-view-7d6c40/norrath_w10_actor_d_view_7d6c40.cpp","object_name":"norrath_w10_actor_d_view_7d6c40.o","sha256":"9a44362d46ed88934bfb9fb3cf4c9dbd622f6cde48e3dd2e30425d779369e47c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-actor-d-view-7d7720/norrath_w10_actor_d_view_7d7720.cpp","object_name":"norrath_w10_actor_d_view_7d7720.o","sha256":"fcd5c05d6cfa1974fdfd5365519fafcc22691335c8ad6bdb0f4d4b24194ea2b8","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-actor-d-view-7d7770/norrath_w10_actor_d_view_7d7770.cpp","object_name":"norrath_w10_actor_d_view_7d7770.o","sha256":"f1bb69efdc1d88c9a8a1d1ad47542f7e33ac5cef01b1a40fcee277645a991e8a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-actor-n-view-7d6d80/norrath_w10_actor_n_view_7d6d80.cpp","object_name":"norrath_w10_actor_n_view_7d6d80.o","sha256":"825f8852d0682cb0c8901ba850df797b656fe667c7da972651e5b6fc9c7119af","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-actor-n-view-8d63c0/norrath_w10_actor_n_view_8d63c0.cpp","object_name":"norrath_w10_actor_n_view_8d63c0.o","sha256":"3a77b0d4b25d942f12dc36593f522093788a257c1a0a0dfbc0c8ec62fa5c86fd","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-actor-n-view-8d6460/norrath_w10_actor_n_view_8d6460.cpp","object_name":"norrath_w10_actor_n_view_8d6460.o","sha256":"1ed82ee9990fafa00475a4c86b135b0afb16933618e3b0f430d860875fac543b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-crt-b-view-483b70/norrath_w9_startup_crt_b_view_483b70.cpp","object_name":"norrath_w9_startup_crt_b_view_483b70.o","sha256":"cc3ee5a6cede99f99c470f9a692b7777c7072f3658646031061d09a247000e1b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-crt-b-view-485380/norrath_w9_startup_crt_b_view_485380.cpp","object_name":"norrath_w9_startup_crt_b_view_485380.o","sha256":"f8d336a2270666b4d0de4e92474d03ef0cecdbf7629d869f6f3453bdb435ef3e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-npc-msg-run-particle-7d7dd0/npc_msg_run_particle_7d7dd0.cpp","object_name":"npc_msg_run_particle_7d7dd0.o","sha256":"e7cc40b9d2eb0a232a5389678dd8e1893337ff891dd96aab705cad3d402e0c17","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-rtti-literals-482430/norrath_rtti_entities_a_view_482430.cpp","object_name":"norrath_rtti_entities_a_view_482430.o","sha256":"ba3bcb3ae17bcec54152e8d7ec7dfbbb99fadf2de25a95c9a42be62eb1b1f174","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-shockwave-innoruuk-particle/shockwaveInnoruukParticle.cpp","object_name":"shockwaveInnoruukParticle.o","sha256":"a61bcf20dc7629f354e252f444ef25283defd6fd1747fa961aa814b5c2a82144","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-shockwave-innoruuk-particle-2/shockwaveInnoruukParticle2.cpp","object_name":"shockwaveInnoruukParticle2.o","sha256":"c2285cf1ed9cacbaab41bd7105510dfa9d07f5d6984f2868a6e9ea7d1aec7b7e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-smoke-particle/smokeParticle.cpp","object_name":"smokeParticle.o","sha256":"7bce9ea2e3f04994f6ec311dcb4bfa6ec1e11c56525412db37920fa934759d33","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-soul-particle/soulParticle.cpp","object_name":"soulParticle.o","sha256":"0fa71f04e553503ab2a3fa0e0f7587546eacf96683458a9e17459de0d7bc76f1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-summonhammer-particle/summonhammerParticle.cpp","object_name":"summonhammerParticle.o","sha256":"cdbcfae8637b7fbfcbba757824ae7bdba9e2b8e5a02962096b9179f8d38b9999","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-torch-msg-run-particle-7d7ba0/torch_msg_run_particle_7d7ba0.cpp","object_name":"torch_msg_run_particle_7d7ba0.o","sha256":"72a8903b0b4e51487d5d27db030ffbbaeaf7ba681194c03ed555187858429c2c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-torch-msg-run-particle-7d7c40/torch_msg_run_particle_7d7c40.cpp","object_name":"torch_msg_run_particle_7d7c40.o","sha256":"fb60b35383887d50aadd18c4096d78c11396c617118431470bd4ae262a783b5b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-torch-msg-run-particle-7d7c90/torch_msg_run_particle_7d7c90.cpp","object_name":"torch_msg_run_particle_7d7c90.o","sha256":"1b27338cbc73051c3b1956ce0b6f1761bb6d2dab14c61578a9311dbb2621fe29","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-trail-particle/trailParticle.cpp","object_name":"trailParticle.o","sha256":"1313900f2ebd21cca094a3a0d02f3899e7e301b4b1e11e650a87486e95db1ffd","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-trail-particle-spark/trailParticleSpark.cpp","object_name":"trailParticleSpark.o","sha256":"6134b229c9e1a6888ad4038a80f5b8762dbab8e91264a6f5063b1f4434943608","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-udk-resource-map/udkResourceMap.cpp","object_name":"udkResourceMap.o","sha256":"1a6d3b73c8ebf1305b3bfcbfb258d02b6dfd287d1e951a77bc12d6aa646e2fe7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-undead-knight-clone-msg-run-particle-7d7e80/undead_knight_clone_msg_run_particle_7d7e80.cpp","object_name":"undead_knight_clone_msg_run_particle_7d7e80.o","sha256":"7c9202e9636ef32df40e2f692ee82bfd49ed1dbbc93f90496a236e27a8475728","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-undead-knight-clone-msg-run-particle-7d7ed0/undead_knight_clone_msg_run_particle_7d7ed0.cpp","object_name":"undead_knight_clone_msg_run_particle_7d7ed0.o","sha256":"ca1103384c1adc0cfd71ec534b131ac2eeb838e37c6b1d7061d3b340c1a4e4f7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-undead-knight-msg-run-particle-7d7e30/undead_knight_msg_run_particle_7d7e30.cpp","object_name":"undead_knight_msg_run_particle_7d7e30.o","sha256":"2e676df1370b5e072d10ded5dc4d5e2c7c2002f9266a7b6cee51769c956d7a11","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-vapor-particle/vaporParticle.cpp","object_name":"vaporParticle.o","sha256":"cfee7866e0681c3caa5d6ab684d5aa49335e90a9efd63468d0fbe02dd0813cdb","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-wave-main-particle/waveMainParticle.cpp","object_name":"waveMainParticle.o","sha256":"a8150e3ee428cb8bd72f0539d920dee5e525a14a5058639a6ec2786f243ace0f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[]}
{"source":"sources/w11-data-c-web-construct-name/web_construct_name.cpp","object_name":"web_construct_name.o","sha256":"d36820047451ada75df3266a4420b50b773fa7fa69651460e384a7b187accaf3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"norrath_web_construct","compiled_body_bytes":24}]}
```

## Authored but never executed

All of it on the EE. Only host checks ran; the web constructor's instruction words were compared with the image by inspection.

## Unknown

- The meaning of any particle-definition field, and the original placement of every object here.
- Which same-named original object a caller of `smokeParticle`, `trailParticle` or `trailParticleSpark` means.
- The meaning of the `0x3e4ccccc` literals and of the `udk` slot indexes.
- Whether the string views at `0x00483b70` and `0x00485380` are one pooled string or two; the image holds them at two addresses.
- The item-data part of the package title: no assigned import is item data.
