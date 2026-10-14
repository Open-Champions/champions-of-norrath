# VI container template instances (1 of 3)

Package `agents/w11-tmpl-a`. Addresses are from the private hash-pinned `SLUS_205.65` symbol table. Nothing here has run on the EE or as gameplay. Sizes below are of the reviewed EE g++ `authored_cpp` objects and are not a claim of byte equality with the image.

## Evidence

Every function is one commit and has a `host_check.cpp` beside it (not a plan input; build with `c++ -arch x86_64 -Wl,-no_pie -Wl,-pagezero_size,0x1000` so statics sit below 4 GiB, because the sources treat stored words as 32-bit addresses). Each was also compared with the image in a private R5900 interpreter: the original instructions and the freshly linked authored object run from identical memory snapshots with identical stub callees, and the stub call logs (argument registers included), changed memory and return values must agree. Scenarios cover every branch of the original (empty and non-empty, callee results zero/positive/negative as sign-extended 64-bit values, a callee that clears the array word, list lengths 0-4, erase of first/middle/last/sole node). Zero mismatches. Deliberate mutations (a wrong stored word, a missing store, a wrong link word) made the comparison fail. The comparison harness is private and outside git.

## Observed

- Pool (`VIPool<T*>`, four instances: `VISprite`, `VIHSpriteAnim`, `VIRefMap`, `VIStaticLighting`) Begin/End/Clear/Init bodies are instruction-identical across the four instances apart from callee symbols. End returns `0x7fffffff`. Begin returns the word at +16 unless it is -1, then tail-calls End. Clear: if the word at +8 is nonzero it calls `DestroyPool__t6VIPool1Z<T>` with the pool, re-reads +8 and passes it to `__builtin_vec_delete` if still nonzero; then stores 0 at +0/+4/+8 and -1 at +20/+12/+16; returns 0. Init: Clear, a negative result returns -1, stores the argument at +4, then `Allocate__t6VIPool1Z<T>`; result -1 when negative else 0.
- `VIList<VIGUIPage*>`: Begin returns list+4 or End's value; End returns `0x7fffffff`; Erase unlinks one node (next at node+4, prev at node+8, head list+4, tail list+8), deletes it, decrements list+0 and returns the next node or `0x7fffffff`.
- `VIList<VIActor>`/`VIList<VIScene::VISceneOccup>` Clear: intrusive walk (next word at node+160 and node+24), actor nodes get `_$_7VIActor(node, 2)` then `__builtin_delete`; occupancy nodes only `__builtin_delete`; then zero at +8, +0, +4. Init calls Clear and maps negative to -1, else 0.
- `VIArray<int>`, `VIVector<VISceneDisplayElem>`, `VIMap<unsigned, VIDictionary::VIDictEntry>` Clear: DestroyPool, a negative result returns -1 with nothing stored, otherwise delete the storage word and zero the header words (the map also stores -1 at +16). Map Init stores argument+1 at +4 then calls Allocate.

## Inferred

Field meanings (capacity, first index, head/tail/count, element storage) come from how the words are used and from the scene-clear caller in the earlier VI package. They are named only in comments, never in code. Template argument types come only from the mangled names. That the argument of the map Init is a count that gets one extra slot is a guess from `+1`.

## Imports closed

- `Begin__Ct6VIList1ZP9VIGUIPage`
- `Begin__Ct6VIPool1ZP13VIHSpriteAnim`
- `Begin__Ct6VIPool1ZP16VIStaticLighting`
- `Begin__Ct6VIPool1ZP8VIRefMap`
- `Begin__Ct6VIPool1ZP8VISprite`
- `Clear__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry`
- `Clear__t6VIList1Z7VIActor`
- `Clear__t6VIList1ZQ27VIScene12VISceneOccup`
- `Clear__t6VIPool1ZP13VIHSpriteAnim`
- `Clear__t6VIPool1ZP16VIStaticLighting`
- `Clear__t6VIPool1ZP8VIRefMap`
- `Clear__t6VIPool1ZP8VISprite`
- `Clear__t7VIArray1Zi`
- `Clear__t8VIVector1Z18VISceneDisplayElem`
- `End__Ct6VIList1ZP9VIGUIPage`
- `End__Ct6VIPool1ZP13VIHSpriteAnim`
- `End__Ct6VIPool1ZP16VIStaticLighting`
- `End__Ct6VIPool1ZP8VIRefMap`
- `End__Ct6VIPool1ZP8VISprite`
- `Erase__t6VIList1ZP9VIGUIPagei`
- `Init__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`
- `Init__t6VIList1Z7VIActor`
- `Init__t6VIList1ZQ27VIScene12VISceneOccup`
- `Init__t6VIPool1ZP13VIHSpriteAnimi`
- `Init__t6VIPool1ZP16VIStaticLightingi`
- `Init__t6VIPool1ZP8VIRefMapi`
- `Init__t6VIPool1ZP8VISpritei`

## Imports opened

Callees these functions need that are not defined here (other packages or later work):

- `Allocate__t6VIPool1ZP8VISprite`, `Allocate__t6VIPool1ZP13VIHSpriteAnim`, `Allocate__t6VIPool1ZP8VIRefMap`, `Allocate__t6VIPool1ZP16VIStaticLighting`, `Allocate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry`
- `DestroyPool__t6VIPool1ZP8VISprite`, `DestroyPool__t6VIPool1ZP13VIHSpriteAnim`, `DestroyPool__t6VIPool1ZP8VIRefMap`, `DestroyPool__t6VIPool1ZP16VIStaticLighting`, `DestroyPool__t7VIArray1Zi`, `DestroyPool__t8VIVector1Z18VISceneDisplayElem`, `DestroyPool__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry`
- `_$_7VIActor` (flag 2), `__builtin_delete`, `__builtin_vec_delete` (these two may already be defined by the memory package)

## Build-plan entries needed

Profile `authored_cpp`, no headers. Do not add `host_check.cpp`. The integrator assigns `input_order`. 27 entries:

```json
{"source":"sources/w11-tmpl-a-array-int-clear/clear.cpp","object_name":"clear.o","sha256":"c9d6bc5a746a7230319c9f1a12e33270688011f693699e07f2c718fe8a7e7083","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t7VIArray1Zi","compiled_body_bytes":120}]}
{"source":"sources/w11-tmpl-a-list-actor-clear/clear.cpp","object_name":"clear.o","sha256":"5d7291d0c62863bcf735b76fc30d795c3d133edcde10f603256f1389b917c500","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t6VIList1Z7VIActor","compiled_body_bytes":108}]}
{"source":"sources/w11-tmpl-a-list-actor-init/init.cpp","object_name":"init.o","sha256":"159bfc5e5d1bdc163dd270f84b8fcd714e7c16ecd578b6b5acfefef34550397d","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Init__t6VIList1Z7VIActor","compiled_body_bytes":32}]}
{"source":"sources/w11-tmpl-a-list-guipage-begin/begin.cpp","object_name":"begin.o","sha256":"25d7df622a0c661a3aed51c3422c1eb845da9d561372b024a51fa9f2e7d98adf","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Begin__Ct6VIList1ZP9VIGUIPage","compiled_body_bytes":32}]}
{"source":"sources/w11-tmpl-a-list-guipage-end/end.cpp","object_name":"end.o","sha256":"beee680dacb85beb3f14d85250b176089ce89c62098845c057afbb658f7f26df","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"End__Ct6VIList1ZP9VIGUIPage","compiled_body_bytes":12}]}
{"source":"sources/w11-tmpl-a-list-guipage-erase/erase.cpp","object_name":"erase.o","sha256":"c30a689ebf7038c30f6910e216eaffea8b39397ec3b09b0e69c4a4918f5a9e34","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Erase__t6VIList1ZP9VIGUIPagei","compiled_body_bytes":120}]}
{"source":"sources/w11-tmpl-a-list-sceneoccup-clear/clear.cpp","object_name":"clear.o","sha256":"3e81dfde40d18eae3e65a0d398cb7d4bf16e4f478d94d709506e652ca57090c7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t6VIList1ZQ27VIScene12VISceneOccup","compiled_body_bytes":88}]}
{"source":"sources/w11-tmpl-a-list-sceneoccup-init/init.cpp","object_name":"init.o","sha256":"857f441c822ebcd0af31191b69a54a8b18dbf4a626d15f4ae27ebff7c89254fe","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Init__t6VIList1ZQ27VIScene12VISceneOccup","compiled_body_bytes":32}]}
{"source":"sources/w11-tmpl-a-map-dictentry-clear/clear.cpp","object_name":"clear.o","sha256":"4c6fd5983213f1015a5ca868eb1a963a6cefd832a1c7d43834c247dbc36ac63e","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry","compiled_body_bytes":152}]}
{"source":"sources/w11-tmpl-a-map-dictentry-init/init.cpp","object_name":"init.o","sha256":"8a040ce54df8364ae10ac2705a3c070e4b71ece8b612f2e853069aa2d28aaea4","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Init__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi","compiled_body_bytes":72}]}
{"source":"sources/w11-tmpl-a-pool-hspriteanim-begin/begin.cpp","object_name":"begin.o","sha256":"40218170b5f34db6433794e0726c23c4f6d97412ed54f0c58b7d4225669fe102","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Begin__Ct6VIPool1ZP13VIHSpriteAnim","compiled_body_bytes":32}]}
{"source":"sources/w11-tmpl-a-pool-hspriteanim-clear/clear.cpp","object_name":"clear.o","sha256":"7f21749fa3cb35f5e7e8dcb166da4b69e9adcdcb19119b1fce63dbd2c0e2b2d9","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t6VIPool1ZP13VIHSpriteAnim","compiled_body_bytes":184}]}
{"source":"sources/w11-tmpl-a-pool-hspriteanim-end/end.cpp","object_name":"end.o","sha256":"022c45fe1401212ff124aac56b853a40b202cb97c23c8c754b6762314d3490f2","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"End__Ct6VIPool1ZP13VIHSpriteAnim","compiled_body_bytes":12}]}
{"source":"sources/w11-tmpl-a-pool-hspriteanim-init/init.cpp","object_name":"init.o","sha256":"f2a9d7a120cbe7b978cd0a66bb3253ef82b37887cea3e5be06bf6bfefcd8e6a7","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Init__t6VIPool1ZP13VIHSpriteAnimi","compiled_body_bytes":72}]}
{"source":"sources/w11-tmpl-a-pool-refmap-begin/begin.cpp","object_name":"begin.o","sha256":"ed3ad3329b1ea07d021556b25669eec15a25735a010f9f349c9cc1077f96cd85","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Begin__Ct6VIPool1ZP8VIRefMap","compiled_body_bytes":32}]}
{"source":"sources/w11-tmpl-a-pool-refmap-clear/clear.cpp","object_name":"clear.o","sha256":"7d077a22e4ac3fccce22a03f7671c9f09cf8fc0d72637ffd4c9494a0a93872f6","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t6VIPool1ZP8VIRefMap","compiled_body_bytes":184}]}
{"source":"sources/w11-tmpl-a-pool-refmap-end/end.cpp","object_name":"end.o","sha256":"d70668e45c25ece55e63832bc355641e7270a92d8ff99d3053a43f2046c70b2f","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"End__Ct6VIPool1ZP8VIRefMap","compiled_body_bytes":12}]}
{"source":"sources/w11-tmpl-a-pool-refmap-init/init.cpp","object_name":"init.o","sha256":"787bb4e77605bcb7186ee345da9c9021c03bf1fa0c3f55aef40e10b8f7d67365","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Init__t6VIPool1ZP8VIRefMapi","compiled_body_bytes":72}]}
{"source":"sources/w11-tmpl-a-pool-sprite-begin/begin.cpp","object_name":"begin.o","sha256":"018a34d87738d154eb14efe531927b0eae53c5ff285a47e6eba63c93ed1ddbb2","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Begin__Ct6VIPool1ZP8VISprite","compiled_body_bytes":32}]}
{"source":"sources/w11-tmpl-a-pool-sprite-clear/clear.cpp","object_name":"clear.o","sha256":"6eec57f7226f7636c7f9b31c2cff86767dcc3815fa2964235eb05d1b6528da30","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t6VIPool1ZP8VISprite","compiled_body_bytes":184}]}
{"source":"sources/w11-tmpl-a-pool-sprite-end/end.cpp","object_name":"end.o","sha256":"9fc0b558a653d8a5f700fadd547ba485cace92a965cd8205ef8157f52376c2e9","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"End__Ct6VIPool1ZP8VISprite","compiled_body_bytes":12}]}
{"source":"sources/w11-tmpl-a-pool-sprite-init/init.cpp","object_name":"init.o","sha256":"cb058616082b388faf53aeec604b5079924d3dd8603934390a3af7d6d4fb797b","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Init__t6VIPool1ZP8VISpritei","compiled_body_bytes":72}]}
{"source":"sources/w11-tmpl-a-pool-staticlighting-begin/begin.cpp","object_name":"begin.o","sha256":"642d3b7128d9ab0a3077e0b5f002f1762edeab588659ca726314e80aea8416b2","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Begin__Ct6VIPool1ZP16VIStaticLighting","compiled_body_bytes":32}]}
{"source":"sources/w11-tmpl-a-pool-staticlighting-clear/clear.cpp","object_name":"clear.o","sha256":"2338237f78760a14ed74dc606d98125779fcb2930006a9dbfe75267e340dd192","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t6VIPool1ZP16VIStaticLighting","compiled_body_bytes":184}]}
{"source":"sources/w11-tmpl-a-pool-staticlighting-end/end.cpp","object_name":"end.o","sha256":"a3983c2b2489e1952a01ca57ec5c58aee5325f3b0633465faa0b283fd7b4e436","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"End__Ct6VIPool1ZP16VIStaticLighting","compiled_body_bytes":12}]}
{"source":"sources/w11-tmpl-a-pool-staticlighting-init/init.cpp","object_name":"init.o","sha256":"4d8302079a67012b1300348f4f1a19623b254c9aad56f063991133c27ca959fd","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Init__t6VIPool1ZP16VIStaticLightingi","compiled_body_bytes":72}]}
{"source":"sources/w11-tmpl-a-vector-scenedisplayelem-clear/clear.cpp","object_name":"clear.o","sha256":"5a8d90b216ef2050b957bbccd3f005468cbfb6ff3d7bba4b31bab8e323b459dc","header_closure":[],"profile":"authored_cpp","kind":"independently_authored_source","emitted_functions":[{"name":"Clear__t8VIVector1Z18VISceneDisplayElem","compiled_body_bytes":128}]}
```

## Authored but never executed

All of it on real hardware or in the game. Every function ran only inside the private comparison interpreter and the host checks, with stub callees. `DestroyPool`, `Allocate`, the actor destructor and the allocator were never run for real.

## Unknown

- What the pool words at +0, +12 and +20 hold (Clear writes them; nothing here reads them), and the entry layout beyond the first word (12 bytes per entry per the scene-clear package).
- Whether `VIList<VIGUIPage*>` nodes hold the page pointer at node+0; Erase does not touch it.
- The meaning of the map's +12 and +16 words (zero and -1 after Clear).
- Compiled sizes differ from the image (for example pool Clear 184 vs 104 bytes), so the original was built with different options or source structure; no byte match is claimed.
