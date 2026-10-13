# VI container template instances (2 of 3) - package `w11-tmpl-b`

All 27 assigned imports are closed by 27 independently authored units under
`experimental/sources/w11-tmpl-b-*`, one verified function per commit. Addresses, sizes and
instruction order come from the pinned `SLUS_205.65` image (disassembled privately with the
reviewed EE binutils; nothing from it is committed). Each source opens with Observed / Inferred /
Unknown comments.

## Observed, inferred, unknown

- Observed: the shapes of 25 trivial instances. Six list destructors call the instance's `Clear`
  then optionally `__builtin_delete(this)` on flags bit 0. Fourteen pool/map/array destructors test
  the storage word (this+8; this+4 for `VIArray<VIClipEntry>`), call that instance's
  `DestroyPool`, re-test the word, call `__builtin_vec_delete` on it, then optionally
  `__builtin_delete(this)`. Four pool `Next` instances index a 12-byte-element table at this+8, load the
  word at +4 and map -1 to 0x7fffffff; the list `Next` loads node+4 and maps 0 to 0x7fffffff. Two `Init`
  routines (array of int, vector of scene display elements) as described in their source comments.
- Inferred only from the mangled names and the code shape: that these words are storage pointers,
  next links and end markers. The element layouts and the full container structs are unknown.
- Unknown: the `Allocate`/`DestroyPool`/`Clear` result ranges beyond their sign, and the 12-byte element's
  other two words.

## Verification performed

- Every unit compiles with the `authored_cpp` flags of the reviewed EE toolchain (not byte-identical to
  the original, whose compiler scheduled differently; sizes are in the plan entries below).
- Every unit has a `host_check.cpp` (not a plan input) compiled with the host C++ compiler; all print `ok`.
  The container structs use natural-width pointers, so the array destructor's host check locates the
  storage word with `offsetof` (offset 4 only on the 32-bit target).
- Every unit was run against the original in a private R5900 interpreter (outside git): same arguments
  and memory snapshot, callees replaced by identical logging stubs, comparing the call log (callee,
  arguments, the container words seen) and every changed byte outside the stack, plus the returned value
  for `Next` and `Init`. Scenarios: list destructors 10 flag values each; pool/map/array destructors
  storage word null or set x DestroyPool leaving or clearing the word x 7 flag values (28 of 29
  original words reached; the rest is a delay-slot nop); `Next` 24 index/value combinations per pool
  instance and 6 values for the list instance; `Init` array 64 and vector 96 combinations of callee results
  (-3, -1, 0, 2) (22 of 23 and 35 of 35 words). Mutating a branch condition, the end marker or the flags
  mask in a scratch copy made the comparison fail, so the checks are not vacuous. No HALT occurred.
- `python3 -m unittest discover -s tools -p 'test_*.py'` passes (214 tests). No partial build was run.

## Imports closed (27)

`Init__t7VIArray1Zii`, `Init__t8VIVector1Z18VISceneDisplayElemi`, `Next__Ct6VIList1ZP9VIGUIPagei`,
`Next__Ct6VIPool1ZP13VIHSpriteAnimi`, `Next__Ct6VIPool1ZP16VIStaticLightingi`,
`Next__Ct6VIPool1ZP8VIRefMapi`, `Next__Ct6VIPool1ZP8VISpritei`,
`_$_t6VIList1Z10VIMaterial`, `_$_t6VIList1Z7VIActor`, `_$_t6VIList1ZP12VIPrimBuffer`, `_$_t6VIList1ZP9VIGUIPage`, `_$_t6VIList1ZP9VISurface`, `_$_t6VIList1ZQ27VIScene12VISceneOccup`, `_$_t6VIPool1Z19VIRasterMaterialLRU`, `_$_t6VIPool1ZP12VICollBuffer`, `_$_t6VIPool1ZP12VIPrimBuffer`, `_$_t6VIPool1ZP13VIColorBuffer`, `_$_t6VIPool1ZP13VIHSpriteAnim`, `_$_t6VIPool1ZP13VIMaterialPal`, `_$_t6VIPool1ZP16VIStaticLighting`, `_$_t6VIPool1ZP7VIRFont`, `_$_t6VIPool1ZP8VIRefMap`, `_$_t6VIPool1ZP8VISprite`, `_$_t6VIPool1ZP9VISurface`, `_$_t5VIMap2ZUiZQ212VIDictionary11VIDictEntry`, `_$_t5VIMap2ZiZi`, `_$_t7VIArray1Z11VIClipEntry`.

## Imports opened

`Clear__t6VIList1Z10VIMaterial`, `Clear__t6VIList1Z7VIActor`, `Clear__t6VIList1ZP12VIPrimBuffer`,
`Clear__t6VIList1ZP9VIGUIPage`, `Clear__t6VIList1ZP9VISurface`, `Clear__t6VIList1ZQ27VIScene12VISceneOccup`,
`DestroyPool__<instance>` for each of the 14 pool, map and array instances (`t6VIPool1...`,
`t5VIMap2ZUiZQ212VIDictionary11VIDictEntry`, `t5VIMap2ZiZi`, `t7VIArray1Z11VIClipEntry`),
`Clear__t7VIArray1Zi`, `Allocate__t7VIArray1Zi`, `DestroyPool__t8VIVector1Z18VISceneDisplayElem`,
`Allocate__t8VIVector1Z18VISceneDisplayElem`, `__builtin_delete`, `__builtin_vec_delete`. Several belong
to w11-tmpl-a, w11-tmpl-c or the runtime packages; compare with `unresolved-imports.json` after integration.

## Build-plan entries needed

Entries below (profile `authored_cpp`, empty header closure, kind `independently_authored_source`) go at the
end of the plan, in this order; `snapshot.json` must record the sources too. `host_check.cpp` files are not
inputs. Neither shared file was edited here. The `input_order` and the shared snapshot digest are for the
integrator to set.

```json
{"object_name": "w11_tmpl_b_list_dtor_10vimaterial.o", "source": "sources/w11-tmpl-b-list-dtor-10vimaterial/dtor.cpp", "sha256": "ab070a220b9d91f4067d8957eeb0e945d56f2b9d727795b3b5b93d2422521f87", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIList1Z10VIMaterial", "compiled_body_bytes": 76}]}
{"object_name": "w11_tmpl_b_list_dtor_7viactor.o", "source": "sources/w11-tmpl-b-list-dtor-7viactor/dtor.cpp", "sha256": "b9414913342598a893bb5bd83587458f0457dfc5fbb1c0c080f880793407c46f", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIList1Z7VIActor", "compiled_body_bytes": 76}]}
{"object_name": "w11_tmpl_b_list_dtor_p12viprimbuffer.o", "source": "sources/w11-tmpl-b-list-dtor-p12viprimbuffer/dtor.cpp", "sha256": "cd98ec2302a447341228da18e0c7b610c1b486f2147f1e51fe94eba849a20c1d", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIList1ZP12VIPrimBuffer", "compiled_body_bytes": 76}]}
{"object_name": "w11_tmpl_b_list_dtor_p9viguipage.o", "source": "sources/w11-tmpl-b-list-dtor-p9viguipage/dtor.cpp", "sha256": "7783e7bff463896be3fbc5e20d3065fa128eb55218a97f59163115fda40860f6", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIList1ZP9VIGUIPage", "compiled_body_bytes": 76}]}
{"object_name": "w11_tmpl_b_list_dtor_p9visurface.o", "source": "sources/w11-tmpl-b-list-dtor-p9visurface/dtor.cpp", "sha256": "df357de26d517e9bd5cba15ebf8849d6fdd67319d2e8e7cbe9e5f8f6753af1c7", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIList1ZP9VISurface", "compiled_body_bytes": 76}]}
{"object_name": "w11_tmpl_b_list_dtor_q27viscene12visceneoccup.o", "source": "sources/w11-tmpl-b-list-dtor-q27viscene12visceneoccup/dtor.cpp", "sha256": "b4a5a0be9ec57f783ea12a80c815e4ac3c48fb01341141bcb6620bbc3b85c1e3", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIList1ZQ27VIScene12VISceneOccup", "compiled_body_bytes": 76}]}
{"object_name": "w11_tmpl_b_pool_dtor_19virastermateriallru.o", "source": "sources/w11-tmpl-b-pool-dtor-19virastermateriallru/dtor.cpp", "sha256": "32af896feb27064da8d27c665a91815a06b2433901d4f0f16303571e28695be2", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIPool1Z19VIRasterMaterialLRU", "compiled_body_bytes": 144}]}
{"object_name": "w11_tmpl_b_pool_dtor_p12vicollbuffer.o", "source": "sources/w11-tmpl-b-pool-dtor-p12vicollbuffer/dtor.cpp", "sha256": "aa709d16401f89267d84bc54fa6f16751f16fa3d968a1282d2b71573a99f29c6", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIPool1ZP12VICollBuffer", "compiled_body_bytes": 144}]}
{"object_name": "w11_tmpl_b_pool_dtor_p12viprimbuffer.o", "source": "sources/w11-tmpl-b-pool-dtor-p12viprimbuffer/dtor.cpp", "sha256": "a3d0e39a22eebff747853b7afc4eace86e6f7739395c511fb79c6d55931e6193", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIPool1ZP12VIPrimBuffer", "compiled_body_bytes": 144}]}
{"object_name": "w11_tmpl_b_pool_dtor_p13vicolorbuffer.o", "source": "sources/w11-tmpl-b-pool-dtor-p13vicolorbuffer/dtor.cpp", "sha256": "306b213ff60927fdd4d2f1655ad05c6c896d5c3f58e6512e05d45b0241022322", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIPool1ZP13VIColorBuffer", "compiled_body_bytes": 144}]}
{"object_name": "w11_tmpl_b_pool_dtor_p13vihspriteanim.o", "source": "sources/w11-tmpl-b-pool-dtor-p13vihspriteanim/dtor.cpp", "sha256": "a3ec4eca2b4f39e4e3d28663708969bbc00494a377bc18641cce154ba7769b6c", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIPool1ZP13VIHSpriteAnim", "compiled_body_bytes": 144}]}
{"object_name": "w11_tmpl_b_pool_dtor_p13vimaterialpal.o", "source": "sources/w11-tmpl-b-pool-dtor-p13vimaterialpal/dtor.cpp", "sha256": "aba056ec176fce203b59c602d2a06d7d62840e3a54c4c186790b62c59d03d3b3", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIPool1ZP13VIMaterialPal", "compiled_body_bytes": 144}]}
{"object_name": "w11_tmpl_b_pool_dtor_p16vistaticlighting.o", "source": "sources/w11-tmpl-b-pool-dtor-p16vistaticlighting/dtor.cpp", "sha256": "6207636038754d25cc578adf4d5669863cb64fd0067f3dd1a02238598a1c2f33", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIPool1ZP16VIStaticLighting", "compiled_body_bytes": 144}]}
{"object_name": "w11_tmpl_b_pool_dtor_p7virfont.o", "source": "sources/w11-tmpl-b-pool-dtor-p7virfont/dtor.cpp", "sha256": "ae2349d3e4609f3759ef05f2dd007c16c8272efdfbdf37aafd0002926d3e7659", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIPool1ZP7VIRFont", "compiled_body_bytes": 144}]}
{"object_name": "w11_tmpl_b_pool_dtor_p8virefmap.o", "source": "sources/w11-tmpl-b-pool-dtor-p8virefmap/dtor.cpp", "sha256": "4215883e0ea840b16ce20c191df88ca113ea5d4fbe7fc9212f9d28eed585b18e", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIPool1ZP8VIRefMap", "compiled_body_bytes": 144}]}
{"object_name": "w11_tmpl_b_pool_dtor_p8visprite.o", "source": "sources/w11-tmpl-b-pool-dtor-p8visprite/dtor.cpp", "sha256": "58e11a107175f841ea0bbadc58a5e9d42eb5510ab102609ba8584d5a9cc1b7cc", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIPool1ZP8VISprite", "compiled_body_bytes": 144}]}
{"object_name": "w11_tmpl_b_pool_dtor_p9visurface.o", "source": "sources/w11-tmpl-b-pool-dtor-p9visurface/dtor.cpp", "sha256": "6d11adc76b01d3b3ab4d26a9db0da0d8e5f1bbee4a911e831d68f57fb57d306d", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t6VIPool1ZP9VISurface", "compiled_body_bytes": 144}]}
{"object_name": "w11_tmpl_b_map_dtor_uizq212vidictionary11vidictentry.o", "source": "sources/w11-tmpl-b-map-dtor-uizq212vidictionary11vidictentry/dtor.cpp", "sha256": "28b6840dc0e23d59121ef3d5590c1f7487964d4e76324799e19bd66020694760", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t5VIMap2ZUiZQ212VIDictionary11VIDictEntry", "compiled_body_bytes": 144}]}
{"object_name": "w11_tmpl_b_map_dtor_izi.o", "source": "sources/w11-tmpl-b-map-dtor-izi/dtor.cpp", "sha256": "b38a06a32853850b5da13d56e147e9a073b8b10de815ebd83071d0041a588352", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t5VIMap2ZiZi", "compiled_body_bytes": 144}]}
{"object_name": "w11_tmpl_b_array_dtor_11viclipentry.o", "source": "sources/w11-tmpl-b-array-dtor-11viclipentry/dtor.cpp", "sha256": "287c5730cb0bd7f77ca1546c6be0ef0d6e40a23d51b2f8b56c4df30b2e663349", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "_$_t7VIArray1Z11VIClipEntry", "compiled_body_bytes": 144}]}
{"object_name": "w11_tmpl_b_next_p9viguipage.o", "source": "sources/w11-tmpl-b-next-p9viguipage/next.cpp", "sha256": "d6856da17d868c590955eb07d10233908b79b2bc64031ed083029ab45eafd4e3", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "Next__Ct6VIList1ZP9VIGUIPagei", "compiled_body_bytes": 36}]}
{"object_name": "w11_tmpl_b_next_p13vihspriteanim.o", "source": "sources/w11-tmpl-b-next-p13vihspriteanim/next.cpp", "sha256": "206f0cbc2ff6bf74e6809d627ed4951bfd0dfad0e521dcbd570ef9ec1664fc09", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "Next__Ct6VIPool1ZP13VIHSpriteAnimi", "compiled_body_bytes": 60}]}
{"object_name": "w11_tmpl_b_next_p16vistaticlighting.o", "source": "sources/w11-tmpl-b-next-p16vistaticlighting/next.cpp", "sha256": "15a6415a5486d91acf97baa4fe906856046ddd96f769704c5ece5854b6e31800", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "Next__Ct6VIPool1ZP16VIStaticLightingi", "compiled_body_bytes": 60}]}
{"object_name": "w11_tmpl_b_next_p8virefmap.o", "source": "sources/w11-tmpl-b-next-p8virefmap/next.cpp", "sha256": "2a9b0a79bf496954dfc3a6e3b4652b0c36561c4214980a9a6c8bf19132e77e88", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "Next__Ct6VIPool1ZP8VIRefMapi", "compiled_body_bytes": 60}]}
{"object_name": "w11_tmpl_b_next_p8visprite.o", "source": "sources/w11-tmpl-b-next-p8visprite/next.cpp", "sha256": "3f64210710cee6c8a8d2c03d01d521317df336943940fea1e0e0246ee1979222", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "Next__Ct6VIPool1ZP8VISpritei", "compiled_body_bytes": 60}]}
{"object_name": "w11_tmpl_b_array_init_int.o", "source": "sources/w11-tmpl-b-array-init-int/init.cpp", "sha256": "029b76e9dd216d124e02ecbe89b797280ae3382c8bb4b49aef1ecc997acb59a6", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "Init__t7VIArray1Zii", "compiled_body_bytes": 72}]}
{"object_name": "w11_tmpl_b_vector_init_scenedisplayelem.o", "source": "sources/w11-tmpl-b-vector-init-scenedisplayelem/init.cpp", "sha256": "97a8d5534749e324c076993967744202ee4eab15e5fd15f61a50ad08dc4f0064", "profile": "authored_cpp", "kind": "independently_authored_source", "header_closure": [], "emitted_functions": [{"name": "Init__t8VIVector1Z18VISceneDisplayElemi", "compiled_body_bytes": 112}]}
```

## Authored but never executed

All 27 units were executed only in the private interpreter against the original function with stubbed
callees, and on the host with recorder callees. None has run inside a linked game, with real
`DestroyPool`/`Allocate`/`Clear` instances, or under real heap allocation.

## Unknown

Full struct layouts of every container instance, the semantics of the 12-byte pool element, whether
the end marker 0x7fffffff is shared by other iterator routines of these templates, and the owners
of the remaining non-assigned instances (`Begin`, `End`, `Add`, `Erase`, `Allocate`, `DestroyPool`).
