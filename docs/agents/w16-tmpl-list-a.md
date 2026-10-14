# List, pool, array and vector templates (part 1)

Package work is on `agents/w16-tmpl-list-a`. New source is under `experimental/sources/w16-tmpl-list-a-*`, one function per directory. Paired scripts are in `docs/agents/w16-tmpl-list-a-compare/`. They need a private R5900 interpreter and the private image, so they cannot run from a clean checkout. The reviewed EE profile `authored_cpp` compiled every unit. `python3 -m unittest discover -s tools -p 'test_*.py'` was run at the end: 214 tests, OK.

## Method

Each function was compiled with the reviewed toolchain and run next to the original image function. Both runs started from identical memory, with identical stand-in callees that log their arguments. The comparison covered return value, call log (including the memory a callee saw), and every changed byte outside the stack. Hooked callees were `__builtin_new`, `__builtin_vec_new`, `__builtin_vec_delete`, the element constructors, `DestroyPool`, `LinkFreeChain`, `Begin`, `End` and `Next`. Each source was then mutated (changed constants, offsets, branch senses, swapped fields, dropped copies) and the same run was required to fail. Every mutant tried was caught (about 60 in total, across all new units). Two mutants of the `VIHSpritePlayNode` walk made the run loop until the step limit, which also counts as caught.

## Observed

All offsets are bytes. Symbol-table sizes are short for some of these routines, so extents were taken to the next symbol.

Pool object: `+0` count, `+4` capacity, `+8` element block, `+12` free-chain head, `+16` first live index, `+20` last live index. `+16`/`+20` and `+12` start at -1 after `Clear`. Pointer-element pools use 12-byte entries (value, link, back link). The other pools use 104-byte (`VIHSpritePlay`), 52-byte (`VIHSpritePlayNode`, live chain through `+44`) and 16-byte (`VIHSpriteAttachment`) elements.

Allocate for a pool is a grow step. Zero capacity returns 0. With no block it allocates `capacity * size`, stores the pointer before testing it, and goes straight to `LinkFreeChain`. With a block it allocates a fresh one (null returns -1 without storing), copies the first `count` elements (count re-read after each copy), then either calls `DestroyPool` (negative returns -1, fresh block leaked) or walks the live chain with `Begin`/`End`/`Next` without doing anything per entry, deletes the old block and stores the fresh one. The result is `LinkFreeChain(pool, count) < 0 ? -1 : 0`.

Vector object: `+0` count, `+4` capacity, `+8` element block. `Allocate` for `VIMatrix44` (64-byte elements) copy-constructs each element with `__10VIMatrix44RC10VIMatrix44` and then destroys the old ones inline with `_$_10VIMatrix44(element, 2)` (no `DestroyPool` call). For `VIHSpriteNode` (256-byte elements) the copy is member by member as words with two matrix copy constructors at `+48` and `+160`, bytes `+156..+159` are not copied, and `DestroyPool` is called afterwards.

Array object: `+0` count, `+4` element block (not a capacity word). `Allocate` returns 0 for a zero count, otherwise allocates `count * size`, stores the pointer before testing it, returns -1 for null, and constructs each element: 16 zero bytes for `VIHSpriteTrigger`; for `VICSpritePartEmitter` (1056 bytes) a call to `__17VIParticleEmitter(element + 16)` followed by nine field stores (`+0` = -1, `+1016` = -1, `+1040` as a doubleword, the rest zero).

List object: `+0` count, `+4` head, `+8` tail. `Add` allocates a node, constructs it (`VIMaterial`, 496 bytes, links at `+480`/`+484`; `VIActor`, 176 bytes, links at `+160`/`+164`) or stores the pointer value (`VIPrimBuffer*`, 12-byte node, links at `+4`/`+8`), appends it at the tail, increments the count and returns the node, or -1 when allocation fails. Pool `Add` takes the free-chain head, doubling the capacity and calling `Allocate` when it is -1, stores the pointer into the entry, links it at the live tail and returns the new tail index.

`Begin` for a pool returns the first-live word at `+16`, or `End(pool)` (tail call) when it is -1; `VIHSpritePlayNode` returns the constant 0x7fffffff instead. The list `Begin` returns the head, or `End(list)` when it is zero.

## Inferred

- The helper names `Begin`, `End`, `Next`, `DestroyPool`, `LinkFreeChain` come from the symbol table; their roles are inferred from the calls.
- Element sizes come from the multiplications in the allocation sizes and the copy loops.
- The 52-byte `VIHSpritePlayNode` walk and the `VIMatrix44` destroy loop are inlined destructor loops of trivially destructible or already-handled elements.
- The seven pointer-element pool `Add` instances are one template instance with different `Allocate` callees (their instruction streams are identical once call targets are removed).

## Commits

One function per commit, in this order: `Begin` for the VIPrimBuffer, VICollBuffer, VIColorBuffer and VIHSpritePlayNode pools and the VIFlatFile list; `Allocate` for the HSpriteTrigger and CSpritePartEmitter arrays; `Allocate` for the Matrix44 and HSpriteNode vectors; `Allocate` for the HSpritePlay, HSpriteAttachment and HSpritePlayNode pools; `Add` for the HSpriteAnim, MaterialPal, SpellEffect, ParticleDefinition, RFont and Surface pools; `Add` for the Material, Actor and PrimBuffer* lists.

## Already authored by earlier waves, now compared against the original

Nine more of the requested symbols already had sources from earlier waves (`w12-vicont-a-*`) that are in the snapshot but not in the build plan. I did not write second copies. I compared each unchanged source with the original using the same paired method. All matched with zero differences:

- `Add__t6VIPool1ZP8VISpriteRCP8VISprite` from `w12-vicont-a-add-pool-sprite`: 50 cases
- `Allocate__t7VIArray1Zi` from `w12-vicont-a-allocate-array-int`: 7 cases
- `Allocate__t6VIPool1ZP13VIHSpriteAnim` from `w12-vicont-a-allocate-pool-hspriteanim`: 144 cases
- `Allocate__t6VIPool1ZP8VIRefMap` from `w12-vicont-a-allocate-pool-refmap`: 144 cases
- `Allocate__t6VIPool1ZP8VISprite` from `w12-vicont-a-allocate-pool-sprite`: 84 cases
- `Allocate__t6VIPool1ZP16VIStaticLighting` from `w12-vicont-a-allocate-pool-staticlighting`: 144 cases
- `Allocate__t8VIVector1Z18VISceneDisplayElem` from `w12-vicont-a-allocate-vector-scenedisplayelem`: 60 cases
- `Begin__Ct6VIPool1ZP12VICollBuffer` from `w12-vicont-a-begin-pool-collbuffer`: 7 cases (my own copy `w16-tmpl-list-a-pool-collbuffer-begin` is also committed and also matches; keep one)
- `Clear__t6VIList1Z10VIMaterial` from `w12-vicont-a-clear-list-material`: 4 cases

I did not mutate these nine, so their sensitivity rests on the same scripts that caught the mutants of the sibling routines. The comparison is a check of existing text; no commit of mine changes them.

## Build-plan entries needed

The plan needs one `inputs` item and one `files` hash per directory below, appended in any order after the current last input, with the snapshot extended to match. Each entry below gives the exact fields; `input_order` is for the integrator. Sizes are compiled body bytes from a standalone compile with the reviewed profile.

My sources:

```
{"emitted_functions": [{"compiled_body_bytes": 228, "name": "Allocate__t7VIArray1Z20VICSpritePartEmitter"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_array_cspritepartemitter_allocate.o", "profile": "authored_cpp", "sha256": "99b18cbb09976e2210a76598edbe7458ce4ea3147a41dfcd3158ad7eeeed3e70", "source": "sources/w16-tmpl-list-a-array-cspritepartemitter-allocate/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "Allocate__t7VIArray1Z16VIHSpriteTrigger"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_array_hspritetrigger_allocate.o", "profile": "authored_cpp", "sha256": "d17a431108aa9c2ee16b99c4d4964494288191a76fbb3ed435651e890b9eb6c5", "source": "sources/w16-tmpl-list-a-array-hspritetrigger-allocate/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 140, "name": "Add__t6VIList1Z7VIActor"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_list_actor_add.o", "profile": "authored_cpp", "sha256": "3c05263c88ef3ce1ae46400fa86d20f56e482adfa7ca835c36ec498bfa8aa914", "source": "sources/w16-tmpl-list-a-list-actor-add/add.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "Begin__Ct6VIList1ZP10VIFlatFile"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_list_flatfile_begin.o", "profile": "authored_cpp", "sha256": "6eb1bf514f444ed5a0351fe03511d1df1c9c00a3e1d23fe76125c4c1d345bd98", "source": "sources/w16-tmpl-list-a-list-flatfile-begin/begin.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 140, "name": "Add__t6VIList1Z10VIMaterial"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_list_material_add.o", "profile": "authored_cpp", "sha256": "cfefad05a21350bfb151941392eacfee4cd1bb841f20d64c4ae62738d3d4f355", "source": "sources/w16-tmpl-list-a-list-material-add/add.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 140, "name": "Add__t6VIList1ZP12VIPrimBufferRCP12VIPrimBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_list_primbuffer_add.o", "profile": "authored_cpp", "sha256": "68db3f6d2a65d2e5859247717832bcf11de954db5380332c367bfed64a65231e", "source": "sources/w16-tmpl-list-a-list-primbuffer-add/add.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "Begin__Ct6VIPool1ZP12VICollBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_pool_collbuffer_begin.o", "profile": "authored_cpp", "sha256": "3753ade49a661cf192f9c7ac32b60a46532b2628f7af52c0f8844ba888bef27e", "source": "sources/w16-tmpl-list-a-pool-collbuffer-begin/begin.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "Begin__Ct6VIPool1ZP13VIColorBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_pool_colorbuffer_begin.o", "profile": "authored_cpp", "sha256": "c90b343b7055209e80c3e34a72072b4918861af3f1985727b5a3396b09039145", "source": "sources/w16-tmpl-list-a-pool-colorbuffer-begin/begin.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 456, "name": "Add__t6VIPool1ZP13VIHSpriteAnimRCP13VIHSpriteAnim"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_pool_hspriteanim_add.o", "profile": "authored_cpp", "sha256": "4ff639869e067384d98ade2dd21cc3818ea2b6137fa57561512306e8d4456865", "source": "sources/w16-tmpl-list-a-pool-hspriteanim-add/add.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 268, "name": "Allocate__t6VIPool1Z19VIHSpriteAttachment"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_pool_hspriteattachment_allocate.o", "profile": "authored_cpp", "sha256": "9e97312335fe0b0b4a9cd877edaf9938ea16bce089cf3bcf471d722cf191f5a3", "source": "sources/w16-tmpl-list-a-pool-hspriteattachment-allocate/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 272, "name": "Allocate__t6VIPool1Z13VIHSpritePlay"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_pool_hspriteplay_allocate.o", "profile": "authored_cpp", "sha256": "66c52b026f9f1f239015fd72cecd9fd9fa9ef80e28829ee4de0423dc3410c593", "source": "sources/w16-tmpl-list-a-pool-hspriteplay-allocate/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 348, "name": "Allocate__t6VIPool1Z17VIHSpritePlayNode"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_pool_hspriteplaynode_allocate.o", "profile": "authored_cpp", "sha256": "a912abe9db804361aa9337b1d65ec9507660dd1d00c78f17b41a5b0fb9cb4d66", "source": "sources/w16-tmpl-list-a-pool-hspriteplaynode-allocate/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 456, "name": "Add__t6VIPool1ZP13VIMaterialPalRCP13VIMaterialPal"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_pool_materialpal_add.o", "profile": "authored_cpp", "sha256": "0446e5d70520be429508f8aba10c2309dea324cc37c9f3f7860698939ed528cc", "source": "sources/w16-tmpl-list-a-pool-materialpal-add/add.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 456, "name": "Add__t6VIPool1ZP20VIParticleDefinitionRCP20VIParticleDefinition"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_pool_particledefinition_add.o", "profile": "authored_cpp", "sha256": "3f0bf62e6ddc517cc28054524200429221f809b7880c02b1c49cfc5386e92812", "source": "sources/w16-tmpl-list-a-pool-particledefinition-add/add.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 36, "name": "Begin__Ct6VIPool1Z17VIHSpritePlayNode"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_pool_playnode_begin.o", "profile": "authored_cpp", "sha256": "a17301ba3c519db208f501ee4efc5d742e8238ebaa5cac8fbabfd0f70e537a2d", "source": "sources/w16-tmpl-list-a-pool-playnode-begin/begin.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "Begin__Ct6VIPool1ZP12VIPrimBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_pool_primbuffer_begin.o", "profile": "authored_cpp", "sha256": "0fced3a1b9adf08136bf0b1723b20add1d78bc2b03580fc9d13291c3277d5478", "source": "sources/w16-tmpl-list-a-pool-primbuffer-begin/begin.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 456, "name": "Add__t6VIPool1ZP7VIRFontRCP7VIRFont"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_pool_rfont_add.o", "profile": "authored_cpp", "sha256": "e782f63efa8415ad17f6388d2a15515307c0e3bfab54140ff37aa56a920a4d09", "source": "sources/w16-tmpl-list-a-pool-rfont-add/add.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 456, "name": "Add__t6VIPool1ZP13VISpellEffectRCP13VISpellEffect"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_pool_spelleffect_add.o", "profile": "authored_cpp", "sha256": "376c76539a3fb52a8fb6504738e038c75e9da7ada90a847e8b5106975af7312f", "source": "sources/w16-tmpl-list-a-pool-spelleffect-add/add.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 456, "name": "Add__t6VIPool1ZP9VISurfaceRCP9VISurface"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_pool_surface_add.o", "profile": "authored_cpp", "sha256": "cf5233bab69c65d34d37ca38d6df1c69393d8fc53678baca24b378fcc4269975", "source": "sources/w16-tmpl-list-a-pool-surface-add/add.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 600, "name": "Allocate__t8VIVector1Z13VIHSpriteNode"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_vector_hspritenode_allocate.o", "profile": "authored_cpp", "sha256": "0b63aaa1f5bc4ec0caad02ad2c2b54250b392a66bd587e8c263d99e043f7a977", "source": "sources/w16-tmpl-list-a-vector-hspritenode-allocate/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 352, "name": "Allocate__t8VIVector1Z10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16_tmpl_list_a_vector_matrix44_allocate.o", "profile": "authored_cpp", "sha256": "97d97d4680a859fe4cfab33074c972ed9a71fd8f5cf1ef6b17f5ff24d2788d34", "source": "sources/w16-tmpl-list-a-vector-matrix44-allocate/allocate.cpp"}
```

Earlier-wave sources that now have a matching comparison (use these or skip them, but not both for `Begin__Ct6VIPool1ZP12VICollBuffer`):

```
{"emitted_functions": [{"compiled_body_bytes": 456, "name": "Add__t6VIPool1ZP8VISpriteRCP8VISprite"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_vicont_a_add_pool_sprite.o", "profile": "authored_cpp", "sha256": "cbab4de9201bd1004de29f0c875179021b9847f614e5496b1cecf9038dc4eb5f", "source": "sources/w12-vicont-a-add-pool-sprite/add.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "Allocate__t7VIArray1Zi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_vicont_a_allocate_array_int.o", "profile": "authored_cpp", "sha256": "fb2f51ccb717e1067f6aa65560db3d3fea224e8b34ed2339709bd438ce6fbac7", "source": "sources/w12-vicont-a-allocate-array-int/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 320, "name": "Allocate__t6VIPool1ZP13VIHSpriteAnim"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_vicont_a_allocate_pool_hspriteanim.o", "profile": "authored_cpp", "sha256": "9a1c8051251771cb0813de81407d061b0cdc874ced6bba455db32d9ea022c855", "source": "sources/w12-vicont-a-allocate-pool-hspriteanim/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 320, "name": "Allocate__t6VIPool1ZP8VIRefMap"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_vicont_a_allocate_pool_refmap.o", "profile": "authored_cpp", "sha256": "fc1e0a8f69ccc55755779ea665ee6004cbfc0ca8ac57fb25eae130778f7b6a7c", "source": "sources/w12-vicont-a-allocate-pool-refmap/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 276, "name": "Allocate__t6VIPool1ZP8VISprite"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_vicont_a_allocate_pool_sprite.o", "profile": "authored_cpp", "sha256": "31c4aac1dd204df78d8b13dcd85cc103f82a59aaaf9578a8125c87225a50316a", "source": "sources/w12-vicont-a-allocate-pool-sprite/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 320, "name": "Allocate__t6VIPool1ZP16VIStaticLighting"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_vicont_a_allocate_pool_staticlighting.o", "profile": "authored_cpp", "sha256": "4177d2fb19d425b2c9073eec7e31defeeb8b64e7143a57313927019244b5e966", "source": "sources/w12-vicont-a-allocate-pool-staticlighting/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 280, "name": "Allocate__t8VIVector1Z18VISceneDisplayElem"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_vicont_a_allocate_vector_scenedisplayelem.o", "profile": "authored_cpp", "sha256": "4ececae5bdbff54527d06a837c5db1284a42c3e41505d721c24333db9615c686", "source": "sources/w12-vicont-a-allocate-vector-scenedisplayelem/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "Begin__Ct6VIPool1ZP12VICollBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_vicont_a_begin_pool_collbuffer.o", "profile": "authored_cpp", "sha256": "a34639e7a3ded01689ad15f146ac4a099bd436c80cb0b9204aadcd11a9862830", "source": "sources/w12-vicont-a-begin-pool-collbuffer/begin.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "Clear__t6VIList1Z10VIMaterial"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w12_vicont_a_clear_list_material.o", "profile": "authored_cpp", "sha256": "daaf4b41dd6df553047dd6377af28aaedb829e070b89fd0b78f0131d32c594d4", "source": "sources/w12-vicont-a-clear-list-material/clear.cpp"}
```

## Imports closed (29)

`Add__t6VIList1Z10VIMaterial`, `Add__t6VIList1Z7VIActor`, `Add__t6VIList1ZP12VIPrimBufferRCP12VIPrimBuffer`, `Add__t6VIPool1ZP13VIHSpriteAnimRCP13VIHSpriteAnim`, `Add__t6VIPool1ZP13VIMaterialPalRCP13VIMaterialPal`, `Add__t6VIPool1ZP13VISpellEffectRCP13VISpellEffect`, `Add__t6VIPool1ZP20VIParticleDefinitionRCP20VIParticleDefinition`, `Add__t6VIPool1ZP7VIRFontRCP7VIRFont`, `Add__t6VIPool1ZP8VISpriteRCP8VISprite`, `Add__t6VIPool1ZP9VISurfaceRCP9VISurface`, `Allocate__t6VIPool1Z13VIHSpritePlay`, `Allocate__t6VIPool1Z17VIHSpritePlayNode`, `Allocate__t6VIPool1Z19VIHSpriteAttachment`, `Allocate__t6VIPool1ZP13VIHSpriteAnim`, `Allocate__t6VIPool1ZP16VIStaticLighting`, `Allocate__t6VIPool1ZP8VIRefMap`, `Allocate__t6VIPool1ZP8VISprite`, `Allocate__t7VIArray1Z16VIHSpriteTrigger`, `Allocate__t7VIArray1Z20VICSpritePartEmitter`, `Allocate__t7VIArray1Zi`, `Allocate__t8VIVector1Z10VIMatrix44`, `Allocate__t8VIVector1Z13VIHSpriteNode`, `Allocate__t8VIVector1Z18VISceneDisplayElem`, `Begin__Ct6VIList1ZP10VIFlatFile`, `Begin__Ct6VIPool1Z17VIHSpritePlayNode`, `Begin__Ct6VIPool1ZP12VICollBuffer`, `Begin__Ct6VIPool1ZP12VIPrimBuffer`, `Begin__Ct6VIPool1ZP13VIColorBuffer`, `Clear__t6VIList1Z10VIMaterial`.

`Begin__Ct6VIPool1ZP12VICollBuffer` is defined by both my unit and the earlier-wave unit; select one.

## Imports opened

Against the unresolved list of the shared baseline build, these names are newly referenced by the sources above and defined nowhere yet:

`Allocate__t6VIPool1ZP13VIMaterialPal`, `Allocate__t6VIPool1ZP13VISpellEffect`, `Allocate__t6VIPool1ZP20VIParticleDefinition`, `Allocate__t6VIPool1ZP7VIRFont`, `Allocate__t6VIPool1ZP9VISurface`, `LinkFreeChain__t6VIPool1Z13VIHSpritePlayi`, `LinkFreeChain__t6VIPool1Z17VIHSpritePlayNodei`, `LinkFreeChain__t6VIPool1Z19VIHSpriteAttachmenti`, `LinkFreeChain__t6VIPool1ZP13VIHSpriteAnimi`, `LinkFreeChain__t6VIPool1ZP16VIStaticLightingi`, `LinkFreeChain__t6VIPool1ZP8VIRefMapi`, `LinkFreeChain__t6VIPool1ZP8VISpritei`, `__10VIMaterial`, `__7VIActor`.

Names already on the baseline list that these sources also call, owned by other packages: `DestroyPool__t6VIPool1Z13VIHSpritePlay`, `DestroyPool__t6VIPool1Z19VIHSpriteAttachment`, `DestroyPool__t6VIPool1ZP8VISprite`, `DestroyPool__t8VIVector1Z13VIHSpriteNode`, `DestroyPool__t8VIVector1Z18VISceneDisplayElem`, `End__Ct6VIList1ZP10VIFlatFile`, `End__Ct6VIPool1Z17VIHSpritePlayNode`, `End__Ct6VIPool1ZP12VICollBuffer`, `End__Ct6VIPool1ZP12VIPrimBuffer`, `End__Ct6VIPool1ZP13VIColorBuffer` and `__10VIMatrix44RC10VIMatrix44`. Calls to `__builtin_new`, `__builtin_vec_new`, `__builtin_vec_delete`, `__builtin_delete`, `_$_10VIMatrix33`, `_$_10VIMatrix44`, `__17VIParticleEmitter` and the `Begin`/`End`/`Next` of the HSpriteAnim, RefMap and StaticLighting pools resolve to functions already in the plan. The opened list was computed from per-unit symbol tables against the shared baseline and the current plan, not from a fresh full link.

## Authored but never executed

Everything here ran only inside the private R5900 interpreter with stand-in callees, never on an EE, in the game or with real element constructors. The nodes, entries and blocks were synthetic. Allocation hooks returned fixed arena blocks, so no real heap behaviour was exercised. Non-wrapping pointer arithmetic is assumed: the per-slot null tests in the original (which can only fire on a 32-bit wrap) are not reproduced where the compiler removes them.

## Unknown

- The element types' fields and the meaning of most words, apart from the links and counts named above.
- Template parameters beyond what the mangled names give, and whether `Add` for the pointer pools truly stores a pointer or an index-typed value.
- Whether the original's doubleword copies (`ldl`/`ldr`, `sdl`/`sdr`) rely on unaligned access; the authored copies use words.
- The compiled authored bodies are larger than the originals (for example the pool `Add` is 456 bytes against 264). No byte match is claimed or attempted.
- The role of pool word `+20` beyond "last live index", and the exact types of the Begin/End results (treated as `int`).
- Whether the fresh block leak on a negative `DestroyPool` result is intended.
