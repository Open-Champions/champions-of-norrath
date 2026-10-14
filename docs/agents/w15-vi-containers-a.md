# w15-vi-containers-a: VI pool, array, map, list and vector instances and destructors

Branch `agents/w15-vi-containers-a`. Reference image: the private `SLUS_205.65` (SHA-256
`9a60f13c5c316d619a288821cc9496cc61864106b7872b52ee141caed3756ff0`). Addresses and sizes are that image's own symbol values;
nothing from it (bytes, disassembly, decompiler output) is committed. Nothing here has run on the EE or in the game.

All 30 assigned imports are authored, plus two routines the package opened and then closed itself (`LeftRotate` and `RightRotate` of the
dictionary map): 32 units, 32 commits, one function each. Source is in `experimental/sources/w15-vi-containers-a-*`.

## Method and checks

- Each function was read privately, rewritten as C++ in my own words and compiled with the reviewed `authored_cpp` flags (EE GCC 15.2.0).
  The compiled size differs from the image in most cases; no byte match is claimed.
- **Paired runs** (private, outside git): the authored unit was linked at a fixed address with every undefined symbol bound to the
  original's address, then run next to the original function in an R5900 interpreter from identical memory snapshots with identical
  scripted callee hooks. A run passes only when return value (unless the function is `void`), callee call logs (arguments and the
  relevant memory at each call) and every changed byte outside the stack agree. `HALT` is counted as a failure. Scenario counts are in each
  commit message. Mutations of constants, offsets, sides, flags, test directions, call order and re-reads were run against the paired
  scenarios for every unit; survivors are listed under "Unknown".
- **Host checks**: every unit has `host_check.cpp` (not a build input; it includes host headers) that builds with the host compiler and fails
  when an offset, branch, call order or argument is wrong. The EE compile of each unit was also done on its own.
- `python3 -m unittest discover -s tools -p 'test_*.py'` passes.
- A private partial build (plan copy outside git, `docs/agents/w15-vi-containers-a-plan-entries.py --private-root`) compiled and linked
  the shared selection plus these 32 units; results are under "Build".

## Observed container layouts (all from these routines only)

Word offsets are bytes from the object. "Inferred" labels follow the repo rule: names come from the mangled symbol, not from the image.

| Template | +0 | +4 | +8 | +12 | +16 | Evidence |
|---|---|---|---|---|---|---|
| `VIArray<T>` | count | storage pointer | | | | ctor (`__t7VIArray1Zc`), dtor, Clear, Allocate, DestroyPool |
| `VIVector<T>` | count | capacity | storage pointer | | | dtors, Init (copy) |
| `VIPool<T>` | count | capacity | storage pointer | free-chain head (-1 none) | live-chain head (-1 none) | dtors, Begin |
| `VIList<T>` | count | head | tail | | | ctor, Erase |
| `VIMap<unsigned, VIDictionary::VIDictEntry>` | live count | | node array base | root index | free-chain head | Next, FreeNode, EraseFixup, rotations (+ published Find, Erase) |

Dictionary-map nodes are 28 bytes at `base + 28 * index`: +0 colour (0 black, 1 red), +4 parent, +8 left, +12 right, +16 key; index 0 means
"none" and is a real record that gets written. Free nodes are chained through +4 and marked -2 at +8. The destructors share one shape: when
the storage word is nonzero call `DestroyPool`, re-read the word and `__builtin_vec_delete` it; flag bit 0 then deletes the object.

## Units

| Symbol | Image address | Size (image) | Directory suffix |
|---|---|---|---|
| `__t7VIArray1Zc` | `0x011f61b8` | 12 | `array-ctor-char` |
| `__t6VIList1ZP10VIFlatFile` | `0x011f3b60` | 16 | `list-ctor-p-flat-file` |
| `End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry` | `0x0120a628` | 8 | `map-end-dict-entry` |
| `Begin__Ct6VIPool1Z19VIRasterMaterialLRU` | `0x01211da0` | 44 | `pool-begin-raster-material-lru` |
| `DestroyPool__t7VIArray1Z14VIResourceElem` | `0x01206ef0` | 76 | `array-destroypool-resource-elem` |
| `DestroyPool__t6VIPool1ZP13VISpellEffect` | `0x011f3938` | 88 | `pool-destroypool-p-spell-effect` |
| `DestroyPool__t7VIArray1Z10VIZoneRoom` | `0x01213db0` | 160 | `array-destroypool-zone-room` |
| `Allocate__t7VIArray1Z10VIZoneLeaf` | `0x012150e0` | 120 | `array-allocate-zone-leaf` |
| `Allocate__t7VIArray1Z10VIZoneNode` | `0x01214fb8` | 136 | `array-allocate-zone-node` |
| `Clear__t7VIArray1Z10VIZoneLeaf` | `0x01214f50` | 100 | `array-clear-zone-leaf` |
| `Clear__t7VIArray1Z10VIZoneNode` | `0x01214ee8` | 100 | `array-clear-zone-node` |
| `Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44` | `0x01201fd8` | 224 (symbol size field says 168) | `vector-copy-init-matrix44` |
| `Erase__t6VIList1Z7VIActori` | `0x01225120` | 188 | `list-erase-actor` |
| `Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi` | `0x0120a630` | 220 | `map-next-dict-entry` |
| `FreeNode__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi` | `0x0120a710` | 68 | `map-freenode-dict-entry` |
| `EraseFixup__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi` | `0x0120a020` | 1020 | `map-erasefixup-dict-entry` |
| `LeftRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi` | `0x01209e80` | 204 | `map-leftrotate-dict-entry` |
| `RightRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi` | `0x01209f50` | 208 | `map-rightrotate-dict-entry` |
| `_$_t7VIArray1Zc` | `0x011f61c8` | 116 | `array-dtor-char` |
| `_$_t7VIArray1Z16VIHSpriteTrigger` | `0x01203f48` | 116 | `array-dtor-hsprite-trigger` |
| `_$_t7VIArray1Z20VICSpritePartEmitter` | `0x01208400` | 116 | `array-dtor-csprite-part-emitter` |
| `_$_t8VIVector1Z13VIHSpriteNode` | `0x01203e58` | 116 | `vector-dtor-hsprite-node` |
| `_$_t8VIVector1Z10VIMatrix44` | `0x01203ed0` | 116 | `vector-dtor-matrix44` |
| `_$_t6VIPool1Z13VIHSpritePlay` | `0x01204038` | 116 | `pool-dtor-hsprite-play` |
| `_$_t6VIPool1Z17VIHSpritePlayNode` | `0x012040b0` | 116 | `pool-dtor-hsprite-play-node` |
| `_$_t6VIPool1Z19VIHSpriteAttachment` | `0x01203fc0` | 116 | `pool-dtor-hsprite-attachment` |
| `_$_t6VIPool1ZP15VIParticleMotif` | `0x01208a00` | 116 | `pool-dtor-p-particle-motif` |
| `_$_14VIEffectVolume` | `0x0114a120` | 44 | `effect-volume-dtor` |
| `_$_8VISprite` | `0x010080e8` | 52 | `sprite-dtor` |
| `_$_11VIUDPSocket` | `0x0102a1c8` | 124 | `udp-socket-dtor` |
| `_$_Q212RealmService15RealmGatewayAPI` | `0x01124250` | 124 | `realm-gateway-api-dtor` |
| `_$_19VIFlatFileDirectory` | `0x010046a8` | 288 | `flat-file-directory-dtor` |

Per-unit comments in the source headers state observed, inferred and unknown parts separately. Points worth repeating here:

- **Weak template sizes can be wrong.** The symbol size field of `Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44` is 168 but the
  code runs to `0x012020b7` (224 bytes). Check the next symbol before trusting a weak symbol's extent.
- **`RightRotate` re-reads the node base right after its first link store; `LeftRotate` does not.** An aliased-map paired scenario found this;
  the first draft copied the left routine's pattern and mismatched.
- `Erase__t6VIList1Z7VIActori` tests its node for null after having read the node's link words. The authored source addresses the link
  words from `node + 160` so the reviewed compiler does not remove the null test (a pointer-typed `node` makes GCC 15 delete it).
- The empty-loop `DestroyPool` routines (`ResourceElem`, `ZoneRoom` for its index loop) run an index loop over `count` elements; for a negative
  count the image's loop runs for billions of passes. The authored versions do not reproduce that case.
- Deleting-destructor convention seen again: flags bit 0 deletes the object, `2` means destroy in place, `3` both. `VIFlatFileDirectory`
  and `RealmGatewayAPI` call a second-entry table slot (offset 8 of the owned object's table, halfword adjustment then function address).

## Imports closed

`Begin__Ct6VIPool1Z19VIRasterMaterialLRU`, `DestroyPool__t6VIPool1ZP13VISpellEffect`, `DestroyPool__t7VIArray1Z14VIResourceElem`,
`End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry`, `EraseFixup__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`, `Erase__t6VIList1Z7VIActori`,
`FreeNode__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`, `Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`, `__t6VIList1ZP10VIFlatFile`,
`__t7VIArray1Zc`, `_$_11VIUDPSocket`, `_$_14VIEffectVolume`, `_$_19VIFlatFileDirectory`, `_$_8VISprite`,
`_$_Q212RealmService15RealmGatewayAPI`, `_$_t6VIPool1Z13VIHSpritePlay`, `_$_t6VIPool1Z17VIHSpritePlayNode`,
`_$_t6VIPool1Z19VIHSpriteAttachment`, `_$_t6VIPool1ZP15VIParticleMotif`, `_$_t7VIArray1Z16VIHSpriteTrigger`,
`_$_t7VIArray1Z20VICSpritePartEmitter`, `_$_t7VIArray1Zc`, `_$_t8VIVector1Z10VIMatrix44`, `_$_t8VIVector1Z13VIHSpriteNode`,
`Allocate__t7VIArray1Z10VIZoneLeaf`, `Allocate__t7VIArray1Z10VIZoneNode`, `Clear__t7VIArray1Z10VIZoneLeaf`, `Clear__t7VIArray1Z10VIZoneNode`,
`DestroyPool__t7VIArray1Z10VIZoneRoom`, `Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44`.

Opened and closed inside the package: `LeftRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`,
`RightRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`.

## Imports opened

(Callees or data these units reference and do not define; none were stubbed.)

Not defined by any unit here and absent from the older reference list (24 new names in the private link): `Allocate__t8VIVector1Z10VIMatrix44`,
`Begin__Ct6VIList1ZP10VIFlatFile`, `Close__11VIUDPSocket`, `DestroyPool__t6VIPool1Z13VIHSpritePlay`, `DestroyPool__t6VIPool1Z17VIHSpritePlayNode`,
`DestroyPool__t6VIPool1Z19VIHSpriteAttachment`, `DestroyPool__t6VIPool1ZP15VIParticleMotif`, `DestroyPool__t7VIArray1Z10VIZoneLeaf`,
`DestroyPool__t7VIArray1Z10VIZoneNode`, `DestroyPool__t7VIArray1Z16VIHSpriteTrigger`, `DestroyPool__t7VIArray1Z20VICSpritePartEmitter`,
`DestroyPool__t7VIArray1Zc`, `DestroyPool__t8VIVector1Z10VIMatrix44`, `DestroyPool__t8VIVector1Z13VIHSpriteNode`, `End__Ct6VIList1Z7VIActor`,
`End__Ct6VIList1ZP10VIFlatFile`, `End__Ct6VIPool1Z19VIRasterMaterialLRU`, `Erase__t6VIList1ZP10VIFlatFilei`,
`Next__Ct6VIPool1ZP13VISpellEffecti`, `_$_10VIZoneRoom`, `_$_t6VIList1ZP10VIFlatFile`, `__10VIMatrix44RC10VIMatrix44`, `_vt$10VIFlatFile`,
`_vt$Q212RealmService15RealmGatewayAPI`.

Referenced and resolved by other published units in the private link (not opened): `Begin__Ct6VIPool1ZP13VISpellEffect`,
`End__Ct6VIPool1ZP13VISpellEffect`, `__builtin_delete`, `__builtin_vec_delete`, `__builtin_vec_new`, `__wrap_free`, `sceLibnetTerminate`,
`_$_7VIActor`, `_vt$8VISprite`, and (closed here) both rotation routines. Referenced and still unresolved from before this package:
`_vt$19VIFlatFileDirectory`, which the directory destructor stores at +16 and which no package here owns (a data object of 24 bytes in the
image; its layout is not established by this package).

The `DestroyPool` / `Allocate` / `Begin` / `End` / `Next` names above have the same shapes as the ones published here and are the natural
next units for a container instance package.

## Build-plan entries needed

Not edited here. `python3 docs/agents/w15-vi-containers-a-plan-entries.py --toolchain <ee> [--private-root DIR]` prints one JSON entry per
unit (32 lines: `source`, `sha256`, `object_name`, `profile` `authored_cpp`, `header_closure` empty, and the emitted function with its compiled
body size) and writes a private plan and snapshot copy that adds them. The entries' `input_order` values are assigned by the integrator.
Units are one `.cpp` per directory; `host_check.cpp` is not an input. Entries printed on the final commit:

```
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "Allocate__t7VIArray1Z10VIZoneLeaf"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_array_allocate_zone_leaf.o", "profile": "authored_cpp", "sha256": "7b3b7e6caa51545a35e2371b64bff2c6cb9d8fc26eb8fe445b492122d6c90bb3", "source": "sources/w15-vi-containers-a-array-allocate-zone-leaf/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 132, "name": "Allocate__t7VIArray1Z10VIZoneNode"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_array_allocate_zone_node.o", "profile": "authored_cpp", "sha256": "8c91f81248c81f146de1f73c1ac80443d9c7897a7df006a9f9623bb87bd1e312", "source": "sources/w15-vi-containers-a-array-allocate-zone-node/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "Clear__t7VIArray1Z10VIZoneLeaf"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_array_clear_zone_leaf.o", "profile": "authored_cpp", "sha256": "73302d0a152a2c7dacc7d17cd0950466b21cdbbb7a9397ef2b159f7e7a0e209d", "source": "sources/w15-vi-containers-a-array-clear-zone-leaf/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "Clear__t7VIArray1Z10VIZoneNode"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_array_clear_zone_node.o", "profile": "authored_cpp", "sha256": "244d4d43b096c5e1e6b34f7b93a07050c455cb9e52d26f9620ea27d16eb8dfc0", "source": "sources/w15-vi-containers-a-array-clear-zone-node/clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 12, "name": "__t7VIArray1Zc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_array_ctor_char.o", "profile": "authored_cpp", "sha256": "b7747797d75d72aa302939edfeca87d4dbee0b71dee750aab57e76a35b3c15da", "source": "sources/w15-vi-containers-a-array-ctor-char/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "DestroyPool__t7VIArray1Z14VIResourceElem"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_array_destroypool_resource_elem.o", "profile": "authored_cpp", "sha256": "2d8d32c4fcab0e19fe6afdf93a1fba972a065e916e97889a0f4d11da7e97d129", "source": "sources/w15-vi-containers-a-array-destroypool-resource-elem/destroypool.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 116, "name": "DestroyPool__t7VIArray1Z10VIZoneRoom"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_array_destroypool_zone_room.o", "profile": "authored_cpp", "sha256": "8dae3c4c6303d8a886fb538525f3a5a2c1c28b52b0336ded023a7cc9ee0e9e6a", "source": "sources/w15-vi-containers-a-array-destroypool-zone-room/destroypool.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 148, "name": "_$_t7VIArray1Zc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_array_dtor_char.o", "profile": "authored_cpp", "sha256": "6678ce3a584bbbd2e6e30f466695f3cb1ed35c6b4b0cba6f3dca455838fab117", "source": "sources/w15-vi-containers-a-array-dtor-char/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 148, "name": "_$_t7VIArray1Z20VICSpritePartEmitter"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_array_dtor_csprite_part_emitter.o", "profile": "authored_cpp", "sha256": "605991a97e4166d369a2d7cca4b13486ff20b7ee0163f1e78f6f4b101b38c202", "source": "sources/w15-vi-containers-a-array-dtor-csprite-part-emitter/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 148, "name": "_$_t7VIArray1Z16VIHSpriteTrigger"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_array_dtor_hsprite_trigger.o", "profile": "authored_cpp", "sha256": "3b8d1c448395ada14248c46ca32703b4e31089c7ed3fea2e018eb595869ec249", "source": "sources/w15-vi-containers-a-array-dtor-hsprite-trigger/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "_$_14VIEffectVolume"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_effect_volume_dtor.o", "profile": "authored_cpp", "sha256": "3500c9d518a20db51b0cab94ed6cc262fa0a53df66580386088ea80c9b28624f", "source": "sources/w15-vi-containers-a-effect-volume-dtor/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 328, "name": "_$_19VIFlatFileDirectory"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_flat_file_directory_dtor.o", "profile": "authored_cpp", "sha256": "a97c506e27c04ba28eebce93c0658872cbb5ffb32b15a5d06655f2d58486cca8", "source": "sources/w15-vi-containers-a-flat-file-directory-dtor/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "__t6VIList1ZP10VIFlatFile"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_list_ctor_p_flat_file.o", "profile": "authored_cpp", "sha256": "98333cddd6e200c30973277ffe9c4dcfb8c44b34381bd50f4e6711b79ddcd277", "source": "sources/w15-vi-containers-a-list-ctor-p-flat-file/ctor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 200, "name": "Erase__t6VIList1Z7VIActori"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_list_erase_actor.o", "profile": "authored_cpp", "sha256": "8db3797a8bd213746db64278c2b059de031b6c45cd499e7484f9428b1151b4ab", "source": "sources/w15-vi-containers-a-list-erase-actor/erase.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_map_end_dict_entry.o", "profile": "authored_cpp", "sha256": "34bed3c095d4ed3d0a19ccd0ae46bd26f4c07cff2155dea3bc4b8a00465691a1", "source": "sources/w15-vi-containers-a-map-end-dict-entry/end.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1016, "name": "EraseFixup__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_map_erasefixup_dict_entry.o", "profile": "authored_cpp", "sha256": "7941efff7d80065c9060b17f4ab5af165f0a3a848c052ecc498fd7aa4b91145b", "source": "sources/w15-vi-containers-a-map-erasefixup-dict-entry/erasefixup.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 68, "name": "FreeNode__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_map_freenode_dict_entry.o", "profile": "authored_cpp", "sha256": "98a4468981271281bc342dc6dc85942a7e5f093a979eb2fc844eeef1c1a5c8cc", "source": "sources/w15-vi-containers-a-map-freenode-dict-entry/freenode.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 244, "name": "LeftRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_map_leftrotate_dict_entry.o", "profile": "authored_cpp", "sha256": "9be8d5e48b963353df3f69c072a629b54e545b141e6e2f533ad608f9eb95fbb1", "source": "sources/w15-vi-containers-a-map-leftrotate-dict-entry/leftrotate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 152, "name": "Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_map_next_dict_entry.o", "profile": "authored_cpp", "sha256": "57f2e14ac18e0b8d44e7ff99bfdd79f67ff70724f4c7c3fcbf7cd2ee19697733", "source": "sources/w15-vi-containers-a-map-next-dict-entry/next.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 240, "name": "RightRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_map_rightrotate_dict_entry.o", "profile": "authored_cpp", "sha256": "09a7c42dc1b47d29b8ddfd54136cb0a2cb95266fde8b7a84a30b02a6f2309c14", "source": "sources/w15-vi-containers-a-map-rightrotate-dict-entry/rightrotate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "Begin__Ct6VIPool1Z19VIRasterMaterialLRU"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_pool_begin_raster_material_lru.o", "profile": "authored_cpp", "sha256": "39aafb449d2697b264681acfc3313e3aaa9b2c7bb405e8426ade2eb8e922555a", "source": "sources/w15-vi-containers-a-pool-begin-raster-material-lru/begin.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "DestroyPool__t6VIPool1ZP13VISpellEffect"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_pool_destroypool_p_spell_effect.o", "profile": "authored_cpp", "sha256": "fc2db4d28c05b4f0a8441343aa94980156e9df030ed8ee7d517df50da571867f", "source": "sources/w15-vi-containers-a-pool-destroypool-p-spell-effect/destroypool.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 148, "name": "_$_t6VIPool1Z19VIHSpriteAttachment"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_pool_dtor_hsprite_attachment.o", "profile": "authored_cpp", "sha256": "bd1597372e79f6df1ff2958e695cbc85925aa151e98425614d9be82c88f2626b", "source": "sources/w15-vi-containers-a-pool-dtor-hsprite-attachment/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 148, "name": "_$_t6VIPool1Z13VIHSpritePlay"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_pool_dtor_hsprite_play.o", "profile": "authored_cpp", "sha256": "f5bb35a934982f7a309d4e30f9a2053c75986824171f35fac4c8bcca700256e8", "source": "sources/w15-vi-containers-a-pool-dtor-hsprite-play/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 148, "name": "_$_t6VIPool1Z17VIHSpritePlayNode"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_pool_dtor_hsprite_play_node.o", "profile": "authored_cpp", "sha256": "ed51380a50a3ebb4026373c847ec9371b3c16057f0b999bc51cb311b96c61251", "source": "sources/w15-vi-containers-a-pool-dtor-hsprite-play-node/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 148, "name": "_$_t6VIPool1ZP15VIParticleMotif"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_pool_dtor_p_particle_motif.o", "profile": "authored_cpp", "sha256": "4612d3a818ea5ce88284cb5efeb4f51872261c582158791f5fa5a44190d83163", "source": "sources/w15-vi-containers-a-pool-dtor-p-particle-motif/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 144, "name": "_$_Q212RealmService15RealmGatewayAPI"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_realm_gateway_api_dtor.o", "profile": "authored_cpp", "sha256": "fb3827d2acba1f46c636acb043b12916a11a14bad85ddc8ebf59870a809b1f44", "source": "sources/w15-vi-containers-a-realm-gateway-api-dtor/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "_$_8VISprite"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_sprite_dtor.o", "profile": "authored_cpp", "sha256": "31a9ca6a77f740c7d6fac0d9733a518a5488f133e357f87d5053065b32ebc2a1", "source": "sources/w15-vi-containers-a-sprite-dtor/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 144, "name": "_$_11VIUDPSocket"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_udp_socket_dtor.o", "profile": "authored_cpp", "sha256": "51d771689a67a2ca80821b90ce9ec48288800846b9458eaf342be60d59b32a00", "source": "sources/w15-vi-containers-a-udp-socket-dtor/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 248, "name": "Init__t8VIVector1Z10VIMatrix44RCt8VIVector1Z10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_vector_copy_init_matrix44.o", "profile": "authored_cpp", "sha256": "6a6c7c2b6b6a24524f29cec34e471bf140951c9af723a51f30cfab9daf30b387", "source": "sources/w15-vi-containers-a-vector-copy-init-matrix44/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 148, "name": "_$_t8VIVector1Z13VIHSpriteNode"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_vector_dtor_hsprite_node.o", "profile": "authored_cpp", "sha256": "b50992039ac3982aa7a7b21bcdb29165496689079aa3b1c9d8d8f9fbed0bc559", "source": "sources/w15-vi-containers-a-vector-dtor-hsprite-node/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 148, "name": "_$_t8VIVector1Z10VIMatrix44"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w15vca_vector_dtor_matrix44.o", "profile": "authored_cpp", "sha256": "77bb77310c261adb803b82dc5e054beeb19d94b0aba0c69456a2bf1a9d8a0475", "source": "sources/w15-vi-containers-a-vector-dtor-matrix44/dtor.cpp"}
```

## Build

Private plan copy (shared plan + these 32 units) built with the reviewed toolchain: 6,795 source compilations, one link, 5,416 function
bodies, 909 unresolved imports, `original_files_used` false, no Newlib input. The 30 assigned names are no longer unresolved and the
24 listed above were added to the list. This is a partial relocatable build, not an executable; it shows only that the 32 units compile and
link with the rest of the selection. The plan copy and build output are outside the repository.

## Authored but never executed

Everything. No unit has run on the EE or inside the game; the paired runs use an R5900 interpreter with scripted callees, which does not
model interrupts, the TLB or hardware timing, and the host checks run on the build machine's CPU. Specific limits:

- `_$_19VIFlatFileDirectory`, `_$_Q212RealmService15RealmGatewayAPI` and the table calls in them were exercised with synthetic tables whose
  second entry points at a hooked address; real element destructors were not run.
- The rotation and repair routines were exercised on synthetic trees (valid binary trees with arbitrary colours, plus overlapped and relocated
  node arrays), never on a map the engine built. Whether the engine keeps these maps red-black balanced is inferred from the code only.
- The negative-count loops of the two `DestroyPool` index loops are not reproduced.

## Unknown

- The element types' members: every `T` here is only a mangled-name fact. Elements whose constructor or destructor is inlined as nothing
  (`VIZoneLeaf`, `VIZoneNode` zeroing four words, `VIResourceElem`) may have fields the compiler removed.
- Whether `DestroyPool` / `Allocate` of the array, vector and pool types opened above are single shared routines per type or inlined
  differently; they are owned outside this package.
- The colour word's name and the meaning of dictionary node words +20 and +24 (never touched by these routines).
- What `VIFlatFileDirectory`'s second-entry table slot is called, and the base class that owns `+0`/`+4` buffers (`_$_10VIFlatFile` is inlined
  into the directory destructor; `+0`/`+4` are buffers freed with `__wrap_free`).
- Mutation survivors: the re-read of the node base in `LeftRotate` and `RightRotate` after the parent-link store is only distinguishable when
  the map word is the same as the stored link; a dedicated scenario covers it. `words[0] = count` versus `from[0]` in the matrix vector
  copy-initialiser is an equivalent mutant. The `parent == 0` pre-loop test in `Next__Ct5VIMap2...` is equivalent code-wise and is kept
  because the image has it.
