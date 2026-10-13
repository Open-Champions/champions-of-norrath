# VI container instances: Pool, Array, Map, List, Vector (A)

Package `agents/w12-vicont-a`. Addresses and sizes are from the private hash-pinned `SLUS_205.65` symbol table (each source file's header comment carries its function's address and size). Nothing here ran on the EE or as gameplay.

## Checks performed

- Each of the 26 functions is its own commit and compiles alone with the reviewed `authored_cpp` flags without warnings or errors. Compiled sizes differ from the image (for example Add 456 vs 188 bytes), so no byte match is claimed.
- Each was compared with the image in a private R5900 interpreter: the original instructions and the freshly built authored object run from identical memory snapshots with identical scripted callees, and the callee call log (argument registers included), every changed byte outside the stack, and the return value must agree. Zero mismatches. Scenario counts: Allocate 37-73 each (capacity zero/nonzero, storage null/non-null, 0-3 live entries, allocator success/failure, free-chain and destroy results of each sign), Add 289, array Clear 12, list Clear 10 (0-4 nodes; a delete callee overwrites the node's next word to prove it is read first), pool Clear 4, DestroyPool 4, Begin 5.
- Deliberate mutations made the comparison fail: the copy start index and range in Allocate, the result mapping, the stored storage word, the destroy-result threshold, the list tail store, the array count store, the Begin sentinel, a pool header store, the doubling and the count increment in Add, and the material member base. Not every function got its own mutation; instances with identical bodies (the three pool Allocates, five array Clears, three list Clears, seven DestroyPools) share one tested template.
- The seven DestroyPool host checks (`host_check.cpp`, not plan inputs) pass natively. The other 19 have no host check: x86_64 test binaries would not start on this machine during the run (even a hello-world hung), and those sources keep stored words as 32-bit addresses, which an arm64 host cannot run. For them the interpreter comparison is the only executed check.
- `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests, OK.

## Observed

- Pool layout shared by the sprite, hsprite-anim, static-lighting, ref-map and coll-buffer instances (and the VIVector instance): word +0 counts live entries, +4 capacity, +8 entry block with 12-byte entries (words a, b, c), +12 free chain head, +16 first live index, +20 last live index; -1 means none. Map: 28-byte entries; slot 0 is not copied on growth and gets words 0-3 zeroed.
- Allocate: a first call stores the new block before testing it for null; growth copies the live entries (map: from slot 1), then runs the type's Begin/Next/End walk (inlined DestroyPool) or calls DestroyPool (sprite pool and vector), frees the old block and stores the new one. The pool and map instances then call `LinkFreeChain(object, count)`; the vector and `VIArray<int>` do not.
- DestroyPool of all seven instances is the same walk (`for (c = Begin; c != End; c = Next(c))`, End re-evaluated each time) with an empty body and result 0.
- `Add` (sprite pool) doubles the capacity (1 if zero) and calls Allocate when the free head is -1, stores the pointer held by its reference argument into entry word a, links the entry as the new last live entry, pops it from the free chain and returns the new last index.
- `Clear` for the five VIArray instances has the same body as the `VIArray<int>` one; the three list instances (`VIPrimBuffer*`, `VIGUIPage*`, `VISurface*`) walk nodes through the word at node+4 and only delete them. The material list reads the link at node+480 and tears down a 3 x 144-byte member array (a VIMatrix33 at +16 in each) before each delete.
- `VIArray<int>::Allocate` ends in a counting loop with an empty body; it is not reproduced.

## Inferred

Field meanings come from how the words are used and are named only in comments. The walk in DestroyPool/Allocate is what remains of a per-entry teardown for pointer/POD element types; template arguments come only from the mangled names. That the material member array is VIMaterial's inlined destructor is a reading of the loop, not a fact.

## Unknown

- What `LinkFreeChain` does exactly (not part of this package), and why the sprite pool and the vector call DestroyPool while the other pools inline the walk.
- Whether the empty `VIArray<int>` loop has any purpose. The `node+48 == 0` test in the material list was not exercised (no real node has that address).
- Pool Allocate leaks the new block when DestroyPool fails (observed); no known instance fails.

## Imports closed (26)

- `Add__t6VIPool1ZP8VISpriteRCP8VISprite`
- `Allocate__t7VIArray1Zi`
- `Allocate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry`
- `Allocate__t6VIPool1ZP13VIHSpriteAnim`
- `Allocate__t6VIPool1ZP8VIRefMap`
- `Allocate__t6VIPool1ZP8VISprite`
- `Allocate__t6VIPool1ZP16VIStaticLighting`
- `Allocate__t8VIVector1Z18VISceneDisplayElem`
- `Begin__Ct6VIPool1ZP12VICollBuffer`
- `Clear__t7VIArray1Z11VIClipEntry`
- `Clear__t7VIArray1Z7VIPlane`
- `Clear__t7VIArray1Z7VIVect3`
- `Clear__t7VIArray1Z13VIWorldRegion`
- `Clear__t7VIArray1Z6VIZone`
- `Clear__t6VIList1ZP9VIGUIPage`
- `Clear__t6VIList1Z10VIMaterial`
- `Clear__t6VIList1ZP12VIPrimBuffer`
- `Clear__t6VIList1ZP9VISurface`
- `Clear__t6VIPool1ZP12VICollBuffer`
- `DestroyPool__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry`
- `DestroyPool__t5VIMap2ZiZi`
- `DestroyPool__t6VIPool1ZP12VICollBuffer`
- `DestroyPool__t6VIPool1ZP13VIColorBuffer`
- `DestroyPool__t6VIPool1ZP13VIHSpriteAnim`
- `DestroyPool__t6VIPool1ZP12VIPrimBuffer`
- `DestroyPool__t6VIPool1Z19VIRasterMaterialLRU`

## Imports opened

Callees the new sources need (42 distinct, from an `nm -u` of every compiled unit), not defined by this package. Names that a source elsewhere in this branch already defines are listed second; the integrator should confirm that those definitions are the ones the plan selects.

Still undefined:

- `Begin__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry`
- `Begin__Ct5VIMap2ZiZi`
- `Begin__Ct6VIPool1Z19VIRasterMaterialLRU`
- `Begin__Ct6VIPool1ZP12VIPrimBuffer`
- `Begin__Ct6VIPool1ZP13VIColorBuffer`
- `DestroyPool__t6VIPool1ZP8VISprite`
- `DestroyPool__t7VIArray1Z11VIClipEntry`
- `DestroyPool__t7VIArray1Z13VIWorldRegion`
- `DestroyPool__t7VIArray1Z6VIZone`
- `End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry`
- `End__Ct5VIMap2ZiZi`
- `End__Ct6VIPool1Z19VIRasterMaterialLRU`
- `End__Ct6VIPool1ZP12VIPrimBuffer`
- `End__Ct6VIPool1ZP13VIColorBuffer`
- `LinkFreeChain__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`
- `LinkFreeChain__t6VIPool1ZP13VIHSpriteAnimi`
- `LinkFreeChain__t6VIPool1ZP16VIStaticLightingi`
- `LinkFreeChain__t6VIPool1ZP8VIRefMapi`
- `LinkFreeChain__t6VIPool1ZP8VISpritei`
- `Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`
- `Next__Ct5VIMap2ZiZii`
- `Next__Ct6VIPool1Z19VIRasterMaterialLRUi`
- `Next__Ct6VIPool1ZP12VICollBufferi`
- `Next__Ct6VIPool1ZP12VIPrimBufferi`
- `Next__Ct6VIPool1ZP13VIColorBufferi`
- `_$_10VIMatrix33`

Defined elsewhere in the branch:

- `Begin__Ct6VIPool1ZP13VIHSpriteAnim` (defined in `w10-vi-b-scene-clear`)
- `Begin__Ct6VIPool1ZP16VIStaticLighting` (defined in `w10-vi-b-scene-clear`)
- `Begin__Ct6VIPool1ZP8VIRefMap` (defined in `w10-vi-b-scene-clear`)
- `DestroyPool__t7VIArray1Z7VIPlane` (defined in `w11-tmpl-c-array-plane-dtor`)
- `DestroyPool__t7VIArray1Z7VIVect3` (defined in `w11-tmpl-c-array-vect3-dtor`)
- `DestroyPool__t8VIVector1Z18VISceneDisplayElem` (defined in `w11-tmpl-c-vector-scene-display-elem-dtor`)
- `End__Ct6VIPool1ZP12VICollBuffer` (defined in `w11-vi-render-a-collide-clear`)
- `End__Ct6VIPool1ZP13VIHSpriteAnim` (defined in `w10-vi-b-scene-clear`)
- `End__Ct6VIPool1ZP16VIStaticLighting` (defined in `w10-vi-b-scene-clear`)
- `End__Ct6VIPool1ZP8VIRefMap` (defined in `w10-vi-b-scene-clear`)
- `Next__Ct6VIPool1ZP13VIHSpriteAnimi` (defined in `w10-vi-b-scene-clear`)
- `Next__Ct6VIPool1ZP16VIStaticLightingi` (defined in `w10-vi-b-scene-clear`)
- `Next__Ct6VIPool1ZP8VIRefMapi` (defined in `w10-vi-b-scene-clear`)
- `__builtin_delete` (defined in `w10-vi-a-esfparse-dtor`)
- `__builtin_vec_delete` (defined in `w11-tmpl-c-array-plane-dtor`)
- `__builtin_vec_new` (defined in `w11-crt-a-builtin-vec-new`)

## Build-plan entries needed

Profile `authored_cpp`, no headers. Do not add `host_check.cpp`. The integrator assigns `input_order`. 26 entries:

```json
{"source":"sources/w12-vicont-a-add-pool-sprite/add.cpp","object_name":"add.o","sha256":"cbab4de9201bd1004de29f0c875179021b9847f614e5496b1cecf9038dc4eb5f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Add__t6VIPool1ZP8VISpriteRCP8VISprite","compiled_body_bytes":456}]}
{"source":"sources/w12-vicont-a-allocate-array-int/allocate.cpp","object_name":"allocate.o","sha256":"fb2f51ccb717e1067f6aa65560db3d3fea224e8b34ed2339709bd438ce6fbac7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Allocate__t7VIArray1Zi","compiled_body_bytes":72}]}
{"source":"sources/w12-vicont-a-allocate-map-dictentry/allocate.cpp","object_name":"allocate.o","sha256":"16c7debe0c4e2577dc3269f46a360fae83b836f75c82b248e6287bbd9db5d6e3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Allocate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry","compiled_body_bytes":408}]}
{"source":"sources/w12-vicont-a-allocate-pool-hspriteanim/allocate.cpp","object_name":"allocate.o","sha256":"9a1c8051251771cb0813de81407d061b0cdc874ced6bba455db32d9ea022c855","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Allocate__t6VIPool1ZP13VIHSpriteAnim","compiled_body_bytes":320}]}
{"source":"sources/w12-vicont-a-allocate-pool-refmap/allocate.cpp","object_name":"allocate.o","sha256":"fc1e0a8f69ccc55755779ea665ee6004cbfc0ca8ac57fb25eae130778f7b6a7c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Allocate__t6VIPool1ZP8VIRefMap","compiled_body_bytes":320}]}
{"source":"sources/w12-vicont-a-allocate-pool-sprite/allocate.cpp","object_name":"allocate.o","sha256":"31c4aac1dd204df78d8b13dcd85cc103f82a59aaaf9578a8125c87225a50316a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Allocate__t6VIPool1ZP8VISprite","compiled_body_bytes":276}]}
{"source":"sources/w12-vicont-a-allocate-pool-staticlighting/allocate.cpp","object_name":"allocate.o","sha256":"4177d2fb19d425b2c9073eec7e31defeeb8b64e7143a57313927019244b5e966","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Allocate__t6VIPool1ZP16VIStaticLighting","compiled_body_bytes":320}]}
{"source":"sources/w12-vicont-a-allocate-vector-scenedisplayelem/allocate.cpp","object_name":"allocate.o","sha256":"4ececae5bdbff54527d06a837c5db1284a42c3e41505d721c24333db9615c686","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Allocate__t8VIVector1Z18VISceneDisplayElem","compiled_body_bytes":280}]}
{"source":"sources/w12-vicont-a-begin-pool-collbuffer/begin.cpp","object_name":"begin.o","sha256":"a34639e7a3ded01689ad15f146ac4a099bd436c80cb0b9204aadcd11a9862830","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Begin__Ct6VIPool1ZP12VICollBuffer","compiled_body_bytes":32}]}
{"source":"sources/w12-vicont-a-clear-array-clipentry/clear.cpp","object_name":"clear.o","sha256":"7bef905a3a91b8720e44991a229127c26ea9289786a50e64f0af2440a43a6c77","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t7VIArray1Z11VIClipEntry","compiled_body_bytes":120}]}
{"source":"sources/w12-vicont-a-clear-array-plane/clear.cpp","object_name":"clear.o","sha256":"1ef4346e7e1a70b17537cda6d4e88032463ff042e46bac6f8ddf7c771871b70b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t7VIArray1Z7VIPlane","compiled_body_bytes":120}]}
{"source":"sources/w12-vicont-a-clear-array-vect3/clear.cpp","object_name":"clear.o","sha256":"8238a543b2a443c54e1f0d0b53e67858bccdd0d2633264e83d72e165b24e0c20","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t7VIArray1Z7VIVect3","compiled_body_bytes":120}]}
{"source":"sources/w12-vicont-a-clear-array-worldregion/clear.cpp","object_name":"clear.o","sha256":"2fdf16ba87d3a5080ca86a74a24e881b936fe4004628b428d816842786d55fc8","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t7VIArray1Z13VIWorldRegion","compiled_body_bytes":120}]}
{"source":"sources/w12-vicont-a-clear-array-zone/clear.cpp","object_name":"clear.o","sha256":"5819bafb4791b007a4ed1285236380cf25a7bf47dd355e17ee02a757fcaeae3e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t7VIArray1Z6VIZone","compiled_body_bytes":120}]}
{"source":"sources/w12-vicont-a-clear-list-guipage/clear.cpp","object_name":"clear.o","sha256":"2c19100474e44e67455b64abcc0eafc1929d0df108a8f8b4f48b4a76bf413a81","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t6VIList1ZP9VIGUIPage","compiled_body_bytes":88}]}
{"source":"sources/w12-vicont-a-clear-list-material/clear.cpp","object_name":"clear.o","sha256":"daaf4b41dd6df553047dd6377af28aaedb829e070b89fd0b78f0131d32c594d4","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t6VIList1Z10VIMaterial","compiled_body_bytes":184}]}
{"source":"sources/w12-vicont-a-clear-list-primbuffer/clear.cpp","object_name":"clear.o","sha256":"610d34f6236a3007c2a3f562d7e79f2c8a1d3c35b9f72f775e59681f53039706","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t6VIList1ZP12VIPrimBuffer","compiled_body_bytes":88}]}
{"source":"sources/w12-vicont-a-clear-list-surface/clear.cpp","object_name":"clear.o","sha256":"560b6ba5d1ca6af4cd559b71158d470f3a0f340d0fb25bdbd78aded366fedd5c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t6VIList1ZP9VISurface","compiled_body_bytes":88}]}
{"source":"sources/w12-vicont-a-clear-pool-collbuffer/clear.cpp","object_name":"clear.o","sha256":"2fcdfd917dab5e01287e5e64eafb671cb640067fd20d94ef1c19408fae956fec","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t6VIPool1ZP12VICollBuffer","compiled_body_bytes":184}]}
{"source":"sources/w12-vicont-a-destroy-map-dictentry/destroy.cpp","object_name":"destroy.o","sha256":"76fbaa2adf6d4796408c7159606d41cbe8c1286480d0cbde70552d364b70a8b1","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry","compiled_body_bytes":88}]}
{"source":"sources/w12-vicont-a-destroy-map-int-int/destroy.cpp","object_name":"destroy.o","sha256":"d4775ec4bdd38d4589467ec7b1a8f806ddc2c655ed691b5725be6b10120046ed","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t5VIMap2ZiZi","compiled_body_bytes":88}]}
{"source":"sources/w12-vicont-a-destroy-pool-collbuffer/destroy.cpp","object_name":"destroy.o","sha256":"3615cc05c6dd8269f2d23084f03045c24abbe6607a12e4d0f73fc9a02bfaa62c","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t6VIPool1ZP12VICollBuffer","compiled_body_bytes":88}]}
{"source":"sources/w12-vicont-a-destroy-pool-colorbuffer/destroy.cpp","object_name":"destroy.o","sha256":"5733e1ec62e95feaa097ee2ef094f771251872171feab903f44b76498b2bfa28","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t6VIPool1ZP13VIColorBuffer","compiled_body_bytes":88}]}
{"source":"sources/w12-vicont-a-destroy-pool-hspriteanim/destroy.cpp","object_name":"destroy.o","sha256":"fbfee6346e47bdea7b10aa027e023f9f2c80f8bf94deba1e982ad814dae354b3","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t6VIPool1ZP13VIHSpriteAnim","compiled_body_bytes":88}]}
{"source":"sources/w12-vicont-a-destroy-pool-primbuffer/destroy.cpp","object_name":"destroy.o","sha256":"9ad1f1bcd0b7fc11de1b5a74cb7dff8b19a81a5a79029ec7458aebd400445561","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t6VIPool1ZP12VIPrimBuffer","compiled_body_bytes":88}]}
{"source":"sources/w12-vicont-a-destroy-pool-rastermaterial-lru/destroy.cpp","object_name":"destroy.o","sha256":"dad91d44dfa5f6bbea31a85d1f0f0bc16140df2f603804c1ea1073db8c3cf28a","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"DestroyPool__t6VIPool1Z19VIRasterMaterialLRU","compiled_body_bytes":88}]}
```

## Authored but never executed

All of it on real hardware or in the game. Every function ran only inside the private comparison interpreter (with scripted callees) and, for the seven DestroyPool functions, a native host check. `LinkFreeChain`, the Begin/Next/End helpers, the allocator and the matrix destructor were never run for real here. Nineteen functions have no host check (see above).
