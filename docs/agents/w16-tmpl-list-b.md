# Template containers, part 2 (w16-tmpl-list-b)

Package work is on `agents/w16-tmpl-list-b`. Source: `experimental/sources/w16-tmpl-list-b-*`, one directory per function, each with the
authored unit and a `host_check.cpp` (host-only, not a plan input). The private comparison harness, generator and plan copy stay outside git.
Nothing in a shared file was edited; the integrator merges the plan entries below (printed by `docs/agents/w16-tmpl-list-b-plan-entries.py`).

## Observed

All 29 functions are in the reference image as weak symbols between 0x011f3bf0 and 0x01225d08. Common shapes (offsets in bytes, `n` is the
word at +0):

- `VIList<T*>::Clear` (3): walks the chain through the word at node+4, `__builtin_delete` of each node, storing the next node into list+4 after
  each delete; then zeroes +8, +0, +4. No destructor, count not read.
- `VIPool<T>::Clear` (4): when +8 is nonzero calls `DestroyPool` (result ignored), re-reads +8 and `__builtin_vec_delete`s it; always stores zero at
  +0, +4, +8 and -1 at +20, +12, +16.
- `VIArray<T>::Clear` (7) and `VIVector<T>::Clear` (2): same, but a negative `DestroyPool` result returns -1 at once; storage word is +4 (array)
  or +8 (vector); the stores are +4/+0 (array) and +8/+0/+4 (vector).
- `VIPool<T>::DestroyPool` (3 + 1): start cursor is the word at +16 (-1 becomes 0x7fffffff); entries at word +8; element size 104/16/12 with the
  next-index word at +96/+8/+4 for `VIHSpritePlay`/`VIHSpriteAttachment`/`P15VIParticleMotif`; a chain read with -1 mapped to the end marker; no
  call, no write. `VIHSpritePlayNode` instead calls `Begin__Ct6VIPool1Z17VIHSpritePlayNode` and `End__...` (End once per test and once more on a -1 link),
  element size 52, next-index word at +44.
- `VIArray<T>::DestroyPool` for `VIZoneLeaf`, `VIZoneNode`, `VIHSpriteTrigger`, `char` (4): counted loop with an empty body, no memory access beyond +0.
- `DestroyPool` with element destructors (3): `VICSpritePartEmitter` array (element 1056, destructor `_$_17VIParticleEmitter` at element+16),
  `VIMatrix44` vector (element 64, `_$_10VIMatrix44` at +0) and `VIHSpriteNode` vector (element 256, `_$_10VIMatrix44` at +160 then +48), all with flags 2;
  the count is re-read after each pass, the compare against n-1 uses the count read before the calls.
- `End__Ct6VIList1Z7VIActor`, `End__Ct6VIList1ZP10VIFlatFile`: return 0x7fffffff, argument unused.

## Inferred

- Template argument types and the container field names (`head`, `storage`, `count`) come only from the mangled names and the access pattern; the
  authored code uses plain word indexes, not guessed field names.
- 0x7fffffff is the iterator end marker of this family; -1 in a pool index word means "no link".
- The four empty-loop array `DestroyPool` instances had a per-element teardown that the original compiler reduced to nothing; the loop is not reproduced
  (the authored function returns 0 after a volatile read of +0). Likewise a negative count (billions of passes in the image) is not reproduced.

## Checks performed

- Host check per unit: arena of word offsets, scripted callees recording calls, argument addresses and (for lists) the head word at each delete.
  Pool-walk checks run inside an unreadable 4 GiB reservation so a wrong element size, link offset or start word faults instead of passing.
- Mutation testing of every host check (removed stores, flipped branches, changed strides/offsets/flags, swapped destructor order, count re-read removed);
  every mutant was killed except the equivalent ones listed under Unknown.
- Private paired run against the original function in an R5900 interpreter (identical memory snapshots, scripted callees, call logs and every changed byte compared;
  grep for HALT clean): every instruction of the original was reached in all 29 units except one alignment-padding word in 12 of them (the array/vector `Clear` units, and the `VICSpritePartEmitter` and `VIMatrix44` `DestroyPool` units). The harness lives outside git.
- EE compile with the reviewed profile for every unit; private full partial build (shared plan + these 29 units, outside git): 7356 source compilations, one link,
  5863 function bodies, zero COMMON, 814 unresolved imports (841 at the shared tip before; 29 closed, 2 opened). `python3 -m unittest discover -s tools -p 'test_*.py'`: 214 tests pass.

## Imports closed (29)

`Clear__t6VIList1ZP12VIPrimBuffer`
`Clear__t6VIList1ZP9VIGUIPage`
`Clear__t6VIList1ZP9VISurface`
`Clear__t6VIPool1Z13VIHSpritePlay`
`Clear__t6VIPool1Z17VIHSpritePlayNode`
`Clear__t6VIPool1Z19VIHSpriteAttachment`
`Clear__t6VIPool1ZP12VICollBuffer`
`Clear__t7VIArray1Z11VIClipEntry`
`Clear__t7VIArray1Z13VIWorldRegion`
`Clear__t7VIArray1Z16VIHSpriteTrigger`
`Clear__t7VIArray1Z20VICSpritePartEmitter`
`Clear__t7VIArray1Z6VIZone`
`Clear__t7VIArray1Z7VIPlane`
`Clear__t7VIArray1Z7VIVect3`
`Clear__t8VIVector1Z10VIMatrix44`
`Clear__t8VIVector1Z13VIHSpriteNode`
`DestroyPool__t6VIPool1Z13VIHSpritePlay`
`DestroyPool__t6VIPool1Z17VIHSpritePlayNode`
`DestroyPool__t6VIPool1Z19VIHSpriteAttachment`
`DestroyPool__t6VIPool1ZP15VIParticleMotif`
`DestroyPool__t7VIArray1Z10VIZoneLeaf`
`DestroyPool__t7VIArray1Z10VIZoneNode`
`DestroyPool__t7VIArray1Z16VIHSpriteTrigger`
`DestroyPool__t7VIArray1Z20VICSpritePartEmitter`
`DestroyPool__t7VIArray1Zc`
`DestroyPool__t8VIVector1Z10VIMatrix44`
`DestroyPool__t8VIVector1Z13VIHSpriteNode`
`End__Ct6VIList1Z7VIActor`
`End__Ct6VIList1ZP10VIFlatFile`

## Imports opened (2)

`Begin__Ct6VIPool1Z17VIHSpritePlayNode`, `End__Ct6VIPool1Z17VIHSpritePlayNode` (called by `DestroyPool__t6VIPool1Z17VIHSpritePlayNode`; 0x01203a78 and 0x012039f0 in the image,
44 and 12 bytes). Other callees (`__builtin_delete`, `__builtin_vec_delete`, `_$_17VIParticleEmitter`, `_$_10VIMatrix44`) are already defined in the plan.

## Build-plan entries needed

Profile `authored_cpp`, no headers, one entry per directory (`input_order` assigned by the integrator; regenerate with
`python3 docs/agents/w16-tmpl-list-b-plan-entries.py --toolchain <ee>`):

```json
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "Clear__t7VIArray1Z11VIClipEntry"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_clear_array_clipentry.o", "profile": "authored_cpp", "sha256": "b4f22db2d40888d9692aef49018b7144558286a1796a9bc98dca8e152d7583a5", "source": "sources/w16-tmpl-list-b-clear-array-clipentry/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "Clear__t7VIArray1Z16VIHSpriteTrigger"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_clear_array_hspritetrigger.o", "profile": "authored_cpp", "sha256": "47e5098aab6e56e4ec9ddffc646f6c10f7cd48ebd29ac9b1e5c789dd3eaf2f43", "source": "sources/w16-tmpl-list-b-clear-array-hspritetrigger/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "Clear__t7VIArray1Z20VICSpritePartEmitter"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_clear_array_partemitter.o", "profile": "authored_cpp", "sha256": "150a77829a7bc79ba6852867025a9a2ba69902e2c107d7dbd5b15c52df6e401e", "source": "sources/w16-tmpl-list-b-clear-array-partemitter/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "Clear__t7VIArray1Z7VIPlane"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_clear_array_plane.o", "profile": "authored_cpp", "sha256": "2b998f9d937387b11aa67a2235cd8bdc794c99cd90c6c47a83019c1beb3607ad", "source": "sources/w16-tmpl-list-b-clear-array-plane/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "Clear__t7VIArray1Z7VIVect3"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_clear_array_vect3.o", "profile": "authored_cpp", "sha256": "8045edf90b9580b1cc01457a5b93318b342437589d47f3bc6812da863eb248bc", "source": "sources/w16-tmpl-list-b-clear-array-vect3/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "Clear__t7VIArray1Z13VIWorldRegion"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_clear_array_worldregion.o", "profile": "authored_cpp", "sha256": "8bffe9d010cb895afed749f6a6e6a112a263ca7d7c0ab75066ddd8f8e1ee3ab1", "source": "sources/w16-tmpl-list-b-clear-array-worldregion/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "Clear__t7VIArray1Z6VIZone"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_clear_array_zone.o", "profile": "authored_cpp", "sha256": "e776880cb284bf3dfde93c3b69f052344ffc664cd1c704ee0c1285ed9a2e0853", "source": "sources/w16-tmpl-list-b-clear-array-zone/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "Clear__t6VIList1ZP9VIGUIPage"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_clear_list_guipage.o", "profile": "authored_cpp", "sha256": "02f0a1c1024734134324a9d0c58dda25d6a95779a7c9283158372f6ba3d615c9", "source": "sources/w16-tmpl-list-b-clear-list-guipage/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "Clear__t6VIList1ZP12VIPrimBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_clear_list_primbuffer.o", "profile": "authored_cpp", "sha256": "898b5098d6261df909d89d70f87d6e3a26e33a032ac84b843a1fc39a8e2bafbc", "source": "sources/w16-tmpl-list-b-clear-list-primbuffer/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "Clear__t6VIList1ZP9VISurface"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_clear_list_surface.o", "profile": "authored_cpp", "sha256": "6f8a1c9314b18383f1490879268918b9cbd704374c09b63b524d55ce81e07f17", "source": "sources/w16-tmpl-list-b-clear-list-surface/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "Clear__t6VIPool1ZP12VICollBuffer"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_clear_pool_collbuffer.o", "profile": "authored_cpp", "sha256": "a5a760bc7607ad34639c9db94707374e53f5897a9c5c8078b7fafb93a7df9ce3", "source": "sources/w16-tmpl-list-b-clear-pool-collbuffer/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "Clear__t6VIPool1Z19VIHSpriteAttachment"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_clear_pool_hspriteattachment.o", "profile": "authored_cpp", "sha256": "e2bd5d93c4f0b01f962dc43b8c53dbbbac4d6fee84317b45a7c9f3dadb7ad697", "source": "sources/w16-tmpl-list-b-clear-pool-hspriteattachment/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "Clear__t6VIPool1Z13VIHSpritePlay"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_clear_pool_hspriteplay.o", "profile": "authored_cpp", "sha256": "25160561bd3b79e0f6713a6290fe366bbccd98be09e4e9da788e514fc7e99702", "source": "sources/w16-tmpl-list-b-clear-pool-hspriteplay/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "Clear__t6VIPool1Z17VIHSpritePlayNode"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_clear_pool_hspriteplaynode.o", "profile": "authored_cpp", "sha256": "b99fc76bcf6613cd955eda02c3b4a39f0c92a4bd99356666594614497e551907", "source": "sources/w16-tmpl-list-b-clear-pool-hspriteplaynode/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 128, "name": "Clear__t8VIVector1Z13VIHSpriteNode"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_clear_vector_hspritenode.o", "profile": "authored_cpp", "sha256": "afae8039724f69c2cf45ed48c27792ddfeea6b1919459f09cd704a8856967ee0", "source": "sources/w16-tmpl-list-b-clear-vector-hspritenode/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 128, "name": "Clear__t8VIVector1Z10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_clear_vector_matrix44.o", "profile": "authored_cpp", "sha256": "57a32bfae01d390b2c244e0d69eb9b4b4001daddcd03d84a9ddfbd285e0742d9", "source": "sources/w16-tmpl-list-b-clear-vector-matrix44/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "DestroyPool__t7VIArray1Zc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_destroypool_array_char.o", "profile": "authored_cpp", "sha256": "0ae5dc15c7f875fe4687bbe9050b50c564e5dd056d80b627c28426053a9e7bc5", "source": "sources/w16-tmpl-list-b-destroypool-array-char/destroypool.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "DestroyPool__t7VIArray1Z16VIHSpriteTrigger"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_destroypool_array_hspritetrigger.o", "profile": "authored_cpp", "sha256": "6a1a2a491e0a95710ce8040d4c88e697e153bbfd41b8e260d6dc174511c2fe0d", "source": "sources/w16-tmpl-list-b-destroypool-array-hspritetrigger/destroypool.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 116, "name": "DestroyPool__t7VIArray1Z20VICSpritePartEmitter"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_destroypool_array_partemitter.o", "profile": "authored_cpp", "sha256": "4e3df81a3b6cb488f59317eeb6fa4c954ba2b7308196ae748b757675db05b4ce", "source": "sources/w16-tmpl-list-b-destroypool-array-partemitter/destroypool.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "DestroyPool__t7VIArray1Z10VIZoneLeaf"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_destroypool_array_zoneleaf.o", "profile": "authored_cpp", "sha256": "2fb8952a0165e9140ff3f6d899ffcc18c44e1bc13f0eb710ab84d56660a366ff", "source": "sources/w16-tmpl-list-b-destroypool-array-zoneleaf/destroypool.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "DestroyPool__t7VIArray1Z10VIZoneNode"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_destroypool_array_zonenode.o", "profile": "authored_cpp", "sha256": "e6036342874dcc95811674d7b6d28af0e133daad3bcb993d1f8e6badea7a6206", "source": "sources/w16-tmpl-list-b-destroypool-array-zonenode/destroypool.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 80, "name": "DestroyPool__t6VIPool1Z19VIHSpriteAttachment"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_destroypool_pool_hspriteattachment.o", "profile": "authored_cpp", "sha256": "76ecae638cfab9f00c52b26a21a51e1dd11abfa3079496281721a82306093f1d", "source": "sources/w16-tmpl-list-b-destroypool-pool-hspriteattachment/destroypool.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 80, "name": "DestroyPool__t6VIPool1Z13VIHSpritePlay"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_destroypool_pool_hspriteplay.o", "profile": "authored_cpp", "sha256": "f4366440262572dbc7a2712e28fbab4b0d5c4ef3fc3309ef6668ab38c750e676", "source": "sources/w16-tmpl-list-b-destroypool-pool-hspriteplay/destroypool.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 128, "name": "DestroyPool__t6VIPool1Z17VIHSpritePlayNode"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_destroypool_pool_hspriteplaynode.o", "profile": "authored_cpp", "sha256": "574169b37409e5e63a67c2fb2575382700be62f4f88a18a4ca6883251724db59", "source": "sources/w16-tmpl-list-b-destroypool-pool-hspriteplaynode/destroypool.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "DestroyPool__t6VIPool1ZP15VIParticleMotif"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_destroypool_pool_pparticlemotif.o", "profile": "authored_cpp", "sha256": "6be64043f01eb889ad91f71feb491275f64e1b2c2e402ea84de20de8583faa7c", "source": "sources/w16-tmpl-list-b-destroypool-pool-pparticlemotif/destroypool.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "DestroyPool__t8VIVector1Z13VIHSpriteNode"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_destroypool_vector_hspritenode.o", "profile": "authored_cpp", "sha256": "5f3d2295bbc1c59479fa99d46a17b120dc488394974000f6164141c30a4f616e", "source": "sources/w16-tmpl-list-b-destroypool-vector-hspritenode/destroypool.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 108, "name": "DestroyPool__t8VIVector1Z10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_destroypool_vector_matrix44.o", "profile": "authored_cpp", "sha256": "499baca03939952fc03508d24a2065492ceb86edb960619cdf16250f2edb2a48", "source": "sources/w16-tmpl-list-b-destroypool-vector-matrix44/destroypool.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "End__Ct6VIList1Z7VIActor"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_end_list_actor.o", "profile": "authored_cpp", "sha256": "3e6c688b852fe8d8877a08ec751dba0b846d43386ab6d1b0ef50ecda089bf9a8", "source": "sources/w16-tmpl-list-b-end-list-actor/end.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "End__Ct6VIList1ZP10VIFlatFile"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tlb_end_list_flatfile.o", "profile": "authored_cpp", "sha256": "02cd179cb3e031478cb5c2a96a2663bf9e0c7db207c8ab35ff38be8974261634", "source": "sources/w16-tmpl-list-b-end-list-flatfile/end.cpp"}
```

The nine `Clear` units duplicate the bodies of the unselected `w12-vicont-a-clear-*` sources, which had no host check; select only one of each pair (these have checks).
Check for a clash: `w15-vi-sprites-a-*` and `w15-vi-containers-a-*` reference these symbols as imports only; no other source defines them.

## Authored but never executed

All 29 on the console or in the game. They ran only in the private R5900 interpreter with scripted callees and in host checks on the build machine's CPU. The four
empty-loop array `DestroyPool` functions and the two `End` functions are weakly tested: the host check pins only the result and the absence of writes.

## Unknown

- Element layouts of every `T`, and what the pool header words +0, +4, +12, +20 and the array/vector word at +4 (vector) or +8 (array) beyond the reset mean.
- Whether the empty-loop `DestroyPool` instances once held a destructor call, and how the original behaves for a negative count (not reproduced).
- Equivalent mutant: the final `l[1] = 0` of the list `Clear` units (already zero when the chain ends) is indistinguishable from the original in every observable state.
