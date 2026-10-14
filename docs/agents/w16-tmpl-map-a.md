# Map, multimap and set templates, surface-cache routines (`w16-tmpl-map-a`)

Package `w16-tmpl-map-a`, branch `agents/w16-tmpl-map-a`. Addresses and sizes were read from the private
pinned image `work/private/SLUS_205.65` with the reviewed EE binutils; no game bytes, listings or decompiler
output are committed. Each source opens with an `Observed` comment (address, size, behaviour) with
`Inferred` and `Unknown` kept separate. Every function is its own commit and was compared with the original
in a private R5900 paired run (identical call logs against identical stubbed callees, identical return
values, every changed byte outside the stack, instruction coverage of the original recorded). Each
comparison was shown not to be vacuous by mutating the authored source and watching it fail; the commit
messages summarise the mutations. Mutations that survived are noted in the messages (one equivalent
mutant in `Deallocate`). A mutation that makes the harness loop forever was counted as detected by the
harness time limit (element-stride mutations of the two big inserts).

Host checks exist only for `Log2__13VIEESurfCachei` and `End__Ct5VIMap2ZiZi`; every other routine reads
32-bit address words and is covered by the paired runs only (earlier packages recorded that x86_64 host
builds hang on this machine and that a native arm64 build cannot hold 32-bit address words; I did not
re-test either). Host checks are not plan inputs. Compiled bodies
are not the original sizes and are not byte matches.

## Functions authored (26, compiled under the plan profile)

| symbol | image address | original bytes | compiled bytes | unit |
|---|---|---|---|---|
| `AddToFreeMap__13VIEESurfCachei` | 0x01132b20 | 148 | 152 | `w16-tmpl-map-a-esc-addtofreemap` |
| `ConstructDMAPacket__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag` | 0x01133890 | 912 | 776 | `w16-tmpl-map-a-esc-construct-dma-packet` |
| `Deallocate__13VIEESurfCacheR13VIEESurfAlloc` | 0x01133050 | 1436 | 1552 | `w16-tmpl-map-a-esc-deallocate` |
| `FindFree__13VIEESurfCacheR13VIEESurfAlloc` | 0x01132cf8 | 252 | 288 | `w16-tmpl-map-a-esc-findfree` |
| `Log2__13VIEESurfCachei` | 0x01133c68 | 44 | 40 | `w16-tmpl-map-a-esc-log2` |
| `Allocate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry` | 0x0120a758 | 396 | 364 | `w16-tmpl-map-a-map-dict-allocate` |
| `Begin__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry` | 0x0120a940 | 72 | 64 | `w16-tmpl-map-a-map-dict-begin` |
| `Insert__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUiRCQ212VIDictionary11VIDictEntry` | 0x01209870 | 780 | 764 | `w16-tmpl-map-a-map-dict-insert` |
| `Begin__Ct5VIMap2ZiZi` | 0x012261f0 | 72 | 64 | `w16-tmpl-map-a-map-ii-begin` |
| `End__Ct5VIMap2ZiZi` | 0x01203428 | 8 | 8 | `w16-tmpl-map-a-map-ii-end` |
| `Init__t5VIMap2ZiZii` | 0x01227060 | 92 | 72 | `w16-tmpl-map-a-map-ii-init` |
| `Insert__t5VIMap2ZiZiRCiT1` | 0x01226960 | 772 | 748 | `w16-tmpl-map-a-map-ii-insert` |
| `Next__Ct5VIMap2ZiZii` | 0x01226238 | 220 | 160 | `w16-tmpl-map-a-map-ii-next` |
| `AddNode__t5VIMap2ZiZ17VIEESurfCacheInfoii` | 0x01221518 | 276 | 240 | `w16-tmpl-map-a-map-surfinfo-addnode` |
| `Erase__t5VIMap2ZiZ17VIEESurfCacheInfoi` | 0x01220a58 | 600 | 612 | `w16-tmpl-map-a-map-surfinfo-erase` |
| `Key__Ct5VIMap2ZiZ17VIEESurfCacheInfoi` | 0x01222838 | 36 | 24 | `w16-tmpl-map-a-map-surfinfo-key` |
| `LeftRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi` | 0x01221630 | 236 | 236 | `w16-tmpl-map-a-map-surfinfo-leftrotate` |
| `RightRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi` | 0x01221720 | 240 | 236 | `w16-tmpl-map-a-map-surfinfo-rightrotate` |
| `AddNode__t10VIMultiMap2ZfZ14VIEmitterEntryii` | 0x011f4368 | 252 | 248 | `w16-tmpl-map-a-multimap-emitter-addnode` |
| `EraseFixup__t10VIMultiMap2ZfZ14VIEmitterEntryi` | 0x011f4608 | 1020 | 1092 | `w16-tmpl-map-a-multimap-emitter-erasefixup` |
| `FreeNode__t10VIMultiMap2ZfZ14VIEmitterEntryi` | 0x011f4f20 | 136 | 136 | `w16-tmpl-map-a-multimap-emitter-freenode` |
| `LeftRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi` | 0x011f4468 | 204 | 260 | `w16-tmpl-map-a-multimap-emitter-leftrotate` |
| `LinkFreeChain__t10VIMultiMap2ZfZ14VIEmitterEntryi` | 0x011f4fa8 | 176 | 176 | `w16-tmpl-map-a-multimap-emitter-linkfreechain` |
| `RightRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi` | 0x011f4538 | 208 | 260 | `w16-tmpl-map-a-multimap-emitter-rightrotate` |
| `Erase__t10VIMultiMap2ZUiZii` | 0x01220fa8 | 512 | 660 | `w16-tmpl-map-a-multimap-ui-erase` |
| `Key__Ct10VIMultiMap2ZUiZii` | 0x01222910 | 28 | 28 | `w16-tmpl-map-a-multimap-ui-key` |

## Observed

- Every tree container in this family shares one header: word +0 element count (used), +4 capacity,
  +8 element array base (a 32-bit address), +12 root index, +16 head of the free chain. Index 0 is the nil
  element. Elements start with +0 colour (0 black, 1 red in the insert/erase routines), +4 parent, +8 left,
  +12 right, +16 the key; free elements chain through +4 and carry -2 in +8. Element sizes:
  `VIMap<int,int>` and `VIMultiMap<unsigned,int>` 24 bytes (value at +20), `VIMap<unsigned,VIDictEntry>`
  28 (an 8-byte value at +20), `VIMap<int,VIEESurfCacheInfo>` 104 (84-byte value at +20),
  `VIMultiMap<float,VIEmitterEntry>` 112 (a `VIMatrix44` is destroyed in place at +48 by `FreeNode`).
- `VIMap<unsigned,VIDictEntry>::Allocate` copies elements 1.. of the old array, runs a Begin/Next/End pass
  with no body, frees the old block and installs the new one, then relinks the free chain.
- `VIEESurfCache`: records of 24 bytes at the array whose base is the word at cache+40 (the pool at
  cache+32), a `VIMultiMap<unsigned,int>` of free entries embedded at cache+76, a running total at cache+20.
  `AddToFreeMap` keys records by `(record+8 << 24) | record+4`; `FindFree` searches with
  `(request+4 << 24) | request+8`, then with `request+8 | 0xff000000`. `Deallocate` merges free neighbours,
  splits at 8192-byte boundaries and uses class value 255 for whole-page buckets.
- `ConstructDMAPacket` writes the exact word values listed in its source comment (DMA tags with byte 16 or
  48, GIF tag descriptors, registers 80, 81, 82 and 63).

## Inferred

- The three-register groups (80, 81, 82) and 63 in `ConstructDMAPacket` fit the GS BITBLTBUF, TRXPOS,
  TRXREG and TEXFLUSH addresses; the source states this as inference and claims no field names.
- Colour 1 is red and 0 black (insert/erase/fixup are standard red-black routines); element 0 can receive
  stores, as in standard sentinel-based red-black trees.
- `Log2__13VIEESurfCachei` returns the bit length (2^k gives k+1).

## Unknown

- Names of the fields of the surface, surface-info, request and bucket structures; the meaning of class
  values other than 255; what the 8192 constant counts (assumed to be the page size of the cached surface
  memory); the element layouts beyond the link words, key and the value ranges stated above.
- Template parameters beyond the mangled names. Whether `VIMap` and `VIMultiMap` differ in duplicate-key
  handling (the unsigned multimap `Erase` here has the same shape as the window set's `Erase`; the
  multimap `Insert` routines are not in this package).

## Imports closed (26)

`AddNode__t10VIMultiMap2ZfZ14VIEmitterEntryii`, `AddNode__t5VIMap2ZiZ17VIEESurfCacheInfoii`,
`AddToFreeMap__13VIEESurfCachei`, `Allocate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry`,
`Begin__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry`, `Begin__Ct5VIMap2ZiZi`,
`ConstructDMAPacket__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag`,
`Deallocate__13VIEESurfCacheR13VIEESurfAlloc`, `End__Ct5VIMap2ZiZi`,
`EraseFixup__t10VIMultiMap2ZfZ14VIEmitterEntryi`, `Erase__t10VIMultiMap2ZUiZii`,
`Erase__t5VIMap2ZiZ17VIEESurfCacheInfoi`, `FindFree__13VIEESurfCacheR13VIEESurfAlloc`,
`FreeNode__t10VIMultiMap2ZfZ14VIEmitterEntryi`, `Init__t5VIMap2ZiZii`,
`Insert__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUiRCQ212VIDictionary11VIDictEntry`,
`Insert__t5VIMap2ZiZiRCiT1`, `Key__Ct10VIMultiMap2ZUiZii`, `Key__Ct5VIMap2ZiZ17VIEESurfCacheInfoi`,
`LeftRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi`, `LeftRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi`,
`LinkFreeChain__t10VIMultiMap2ZfZ14VIEmitterEntryi`, `Log2__13VIEESurfCachei`,
`Next__Ct5VIMap2ZiZii`, `RightRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi`,
`RightRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi`. That is all 26 names of the package scope.

A 27th unit, `Insert__t5VIMap2ZiZ17VIEESurfCacheInfoRCiRC17VIEESurfCacheInfo`, was authored (commit
`fe86209c`) and then withdrawn (commit `686ce427`): the shared snapshot already defines it in
`sources/w15-vi-raster-b-vimap-insert`, and it was not in this package's list. Both sources agreed with the
original in the paired run, so the history entry is an independent cross-check of the w15 routine; it is not
a plan input and must not be added.

## Imports opened

No full partial build was run: a clean build with this package's private plan was started and stopped
after about 370 of the roughly 3,500 compilations because other agents' builds loaded the machine. What was
done instead, with the reviewed toolchain: each of the 26 units was compiled alone with the plan's
`authored_cpp` flags (the plan-entry script in this directory does the same and prints the entries below),
the defined names were checked against every function in `experimental/snapshot.json` (no duplicates), and
the external references were compared with that snapshot and with the 841-entry unresolved list of
`work/plan16-build` (older than the current branch).

References to names that no authored unit defines (they stay unresolved imports; those marked * were
already unresolved in the plan16 list):
`AddNode__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryii`, `AddNode__t5VIMap2ZiZiii`,
`Allocate__t5VIMap2ZiZ17VIEESurfCacheInfo`, `Allocate__t5VIMap2ZiZi`, `Begin__Ct6VIPool1Z10VIEEBucket`,
`Clear__t5VIMap2ZiZi`, `End__Ct10VIMultiMap2ZUiZi`, `End__Ct6VIPool1Z10VIEEBucket`,
`EraseFixup__t10VIMultiMap2ZUiZii`, `EraseFixup__t5VIMap2ZiZ17VIEESurfCacheInfoi`,
`Erase__t6VIPool1Z10VIEEBucketi`, `FreeNode__t10VIMultiMap2ZUiZii`,
`FreeNode__t5VIMap2ZiZ17VIEESurfCacheInfoi`, `Insert__t10VIMultiMap2ZUiZiRCUiRCi`,
`Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi`*, `LeftRotate__t5VIMap2ZiZii`,
`LinkFreeChain__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`, `LowerBound__Ct10VIMultiMap2ZUiZiRCUi`,
`Next__Ct10VIMultiMap2ZUiZii`, `Next__Ct5VIMap2ZiZ17VIEESurfCacheInfoi`,
`Next__Ct6VIPool1Z10VIEEBucketi`*, `Prev__Ct6VIPool1Z10VIEEBucketi`, `RightRotate__t5VIMap2ZiZii`.
Names the plan16 list did not contain are members of the same template families or of the surface-cache
pool whose callers were not yet authored then; the lead integrator's build gives the real count. Eight more
references (`Allocate__t10VIMultiMap2ZfZ14VIEmitterEntry`, `End__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry`,
`LeftRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`, `Next__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`,
`RightRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi`, `_$_10VIMatrix44`, `__builtin_vec_delete`,
`__builtin_vec_new`) are defined by other authored units in the shared snapshot.

## Build-plan entries needed

Twenty-six entries, one per `w16-tmpl-map-a-*` directory, in directory-name order; the lead integrator
merges them. `docs/agents/w16-tmpl-map-a-plan-entries.py --toolchain <ee> [--private-root DIR]` prints them
and writes a private plan/snapshot copy outside git (`.py` notes are ignored by git; it was added with `-f`).
Note `Deallocate__13VIEESurfCacheR13VIEESurfAlloc` is a single emitted function (its helpers are forced
inline); `Erase__t10VIMultiMap2ZUiZii` and `Erase__t5VIMap2ZiZ17VIEESurfCacheInfoi` use the three tree
routines of the same family as plain imports.

```
{"emitted_functions": [{"compiled_body_bytes": 152, "name": "AddToFreeMap__13VIEESurfCachei"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_esc_addtofreemap.o", "profile": "authored_cpp", "sha256": "624a55ca72cfc9f593fc93e97d69120b7890dda3d2274b1431d725be6d08b96f", "source": "sources/w16-tmpl-map-a-esc-addtofreemap/addtofreemap.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 776, "name": "ConstructDMAPacket__13VIEESurfCacheR9VISurfaceR17VIEESurfCacheInfoPP10_sceDmaTag"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_esc_construct_dma_packet.o", "profile": "authored_cpp", "sha256": "5a5b6272b0668b26605950c4a7f5e8b8016ac591ffa696bcc7dd04a1c4683372", "source": "sources/w16-tmpl-map-a-esc-construct-dma-packet/construct.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1552, "name": "Deallocate__13VIEESurfCacheR13VIEESurfAlloc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_esc_deallocate.o", "profile": "authored_cpp", "sha256": "cadb98a2030053063d04d13c9d472146ebcdfd27a8cb39c05aa8f004371c75ed", "source": "sources/w16-tmpl-map-a-esc-deallocate/deallocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 288, "name": "FindFree__13VIEESurfCacheR13VIEESurfAlloc"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_esc_findfree.o", "profile": "authored_cpp", "sha256": "5ab513af6419998c7f01637248a610d13254af169a61d4b4d4192dbbe86b87cb", "source": "sources/w16-tmpl-map-a-esc-findfree/findfree.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "Log2__13VIEESurfCachei"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_esc_log2.o", "profile": "authored_cpp", "sha256": "c1c6a1ae57a44342d18e0af03a601881556ecd5096abc98f8d3117c59f113cf9", "source": "sources/w16-tmpl-map-a-esc-log2/log2.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 364, "name": "Allocate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_map_dict_allocate.o", "profile": "authored_cpp", "sha256": "b0323704a0796b73646bb00cde6b85042733a86ebdbc754c6c2548b1c29dd233", "source": "sources/w16-tmpl-map-a-map-dict-allocate/allocate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 64, "name": "Begin__Ct5VIMap2ZUiZQ212VIDictionary11VIDictEntry"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_map_dict_begin.o", "profile": "authored_cpp", "sha256": "cd619afb0921b3fb5d10c71632ac97685154f6ba58a6e5d3f53ed3bdb8ab0590", "source": "sources/w16-tmpl-map-a-map-dict-begin/begin.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 764, "name": "Insert__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryRCUiRCQ212VIDictionary11VIDictEntry"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_map_dict_insert.o", "profile": "authored_cpp", "sha256": "c5b69bb9c6072e8518db70bd0c5bb53745de1abe1481034bc991fbe23ee25847", "source": "sources/w16-tmpl-map-a-map-dict-insert/insert.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 64, "name": "Begin__Ct5VIMap2ZiZi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_map_ii_begin.o", "profile": "authored_cpp", "sha256": "2a831359a4c160364ed9376071a262e24e8c80e0a79ce0ce2268fea4ca02ce7f", "source": "sources/w16-tmpl-map-a-map-ii-begin/begin.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "End__Ct5VIMap2ZiZi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_map_ii_end.o", "profile": "authored_cpp", "sha256": "464ca49c12dee41d93ad4ff9d2859106c42c225fea7ba0d46792b00b203a19c6", "source": "sources/w16-tmpl-map-a-map-ii-end/end.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "Init__t5VIMap2ZiZii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_map_ii_init.o", "profile": "authored_cpp", "sha256": "11f770cad1f5f05ece4d06662fa3ba1f67c6c058d36ed94183d18294a6d095f4", "source": "sources/w16-tmpl-map-a-map-ii-init/init.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 748, "name": "Insert__t5VIMap2ZiZiRCiT1"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_map_ii_insert.o", "profile": "authored_cpp", "sha256": "d4ea8d0baf5e2655d427898fba8db43e1780725ccbfb031423d060308fc5b457", "source": "sources/w16-tmpl-map-a-map-ii-insert/insert.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 160, "name": "Next__Ct5VIMap2ZiZii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_map_ii_next.o", "profile": "authored_cpp", "sha256": "e47ba5a1d929183e4bfb0c7ab9ca55b431121e9af749c713bbf0eaab5e72cbcc", "source": "sources/w16-tmpl-map-a-map-ii-next/next.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 240, "name": "AddNode__t5VIMap2ZiZ17VIEESurfCacheInfoii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_map_surfinfo_addnode.o", "profile": "authored_cpp", "sha256": "cb8f45fadd4910b7620ce249cba2cd4b9eede4614ecbe934c50c98b59013aa53", "source": "sources/w16-tmpl-map-a-map-surfinfo-addnode/addnode.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 612, "name": "Erase__t5VIMap2ZiZ17VIEESurfCacheInfoi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_map_surfinfo_erase.o", "profile": "authored_cpp", "sha256": "2bb907fc4b36774f315202037d88a8b555b46acd76323b506e1f3f7ca68936f6", "source": "sources/w16-tmpl-map-a-map-surfinfo-erase/erase.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 24, "name": "Key__Ct5VIMap2ZiZ17VIEESurfCacheInfoi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_map_surfinfo_key.o", "profile": "authored_cpp", "sha256": "08234a46f8046fba7bd4948b41fbd18b737a808fde4b384361142b24f5f6382c", "source": "sources/w16-tmpl-map-a-map-surfinfo-key/key.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 236, "name": "LeftRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_map_surfinfo_leftrotate.o", "profile": "authored_cpp", "sha256": "5849ecb13e7c865c8db83993f719892d025547b824b797f66a5a06c743bb499b", "source": "sources/w16-tmpl-map-a-map-surfinfo-leftrotate/leftrotate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 236, "name": "RightRotate__t5VIMap2ZiZ17VIEESurfCacheInfoi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_map_surfinfo_rightrotate.o", "profile": "authored_cpp", "sha256": "b7bdf8b08e097fc4bf44f7d78cb15943a5ede29ef4e6dee42acaaddb6c490536", "source": "sources/w16-tmpl-map-a-map-surfinfo-rightrotate/rightrotate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 248, "name": "AddNode__t10VIMultiMap2ZfZ14VIEmitterEntryii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_multimap_emitter_addnode.o", "profile": "authored_cpp", "sha256": "9d0b0a9a4b06fa8766ae7f35a634c758fdc1b4b680af26da9af7cf32550d6b0e", "source": "sources/w16-tmpl-map-a-multimap-emitter-addnode/addnode.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1092, "name": "EraseFixup__t10VIMultiMap2ZfZ14VIEmitterEntryi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_multimap_emitter_erasefixup.o", "profile": "authored_cpp", "sha256": "f4993bd5f1c0a035a85624607bccaac0e9ec361ab2e43cd0476213761690d8ff", "source": "sources/w16-tmpl-map-a-multimap-emitter-erasefixup/erasefixup.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "FreeNode__t10VIMultiMap2ZfZ14VIEmitterEntryi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_multimap_emitter_freenode.o", "profile": "authored_cpp", "sha256": "2cb5c00dd0af574e04bdb5a4c0e411c0cc7a9ea18f56e74441c7a4ad098ced61", "source": "sources/w16-tmpl-map-a-multimap-emitter-freenode/freenode.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 260, "name": "LeftRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_multimap_emitter_leftrotate.o", "profile": "authored_cpp", "sha256": "44c640044fd1240fc24341d557f4c4957b4782103e7aa1b64f860a6a65ddd81d", "source": "sources/w16-tmpl-map-a-multimap-emitter-leftrotate/leftrotate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 176, "name": "LinkFreeChain__t10VIMultiMap2ZfZ14VIEmitterEntryi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_multimap_emitter_linkfreechain.o", "profile": "authored_cpp", "sha256": "af83259fc3698b9148ddadec9ddfce58dca3e5dcd3a4f4ed7ab549a7eacda781", "source": "sources/w16-tmpl-map-a-multimap-emitter-linkfreechain/linkfreechain.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 260, "name": "RightRotate__t10VIMultiMap2ZfZ14VIEmitterEntryi"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_multimap_emitter_rightrotate.o", "profile": "authored_cpp", "sha256": "d69a806daee0801fdd9384289a4edc3c7060531286f38de0ead501f6629500c8", "source": "sources/w16-tmpl-map-a-multimap-emitter-rightrotate/rightrotate.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 660, "name": "Erase__t10VIMultiMap2ZUiZii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_multimap_ui_erase.o", "profile": "authored_cpp", "sha256": "332373f5e69e1396dbfe28917beed838129e008f0692192fed72fe468086ab16", "source": "sources/w16-tmpl-map-a-multimap-ui-erase/erase.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 28, "name": "Key__Ct10VIMultiMap2ZUiZii"}], "header_closure": [], "kind": "independently_authored_source", "object_name": "w16tma_multimap_ui_key.o", "profile": "authored_cpp", "sha256": "4bc75dd6272f907695a8d1b97a1c9f7d1640795532c914ae32f4ccb08e264154", "source": "sources/w16-tmpl-map-a-multimap-ui-key/key.cpp"}
```

## Authored but never executed

All of it. Nothing ran on the EE or in gameplay. The paired runs use fabricated tree and cache memory and
stubbed callees (allocation, pool and free-multimap calls), so the growth path of the allocate routines,
the real `VIPool<VIEEBucket>` behaviour, and the real interplay of insert, rotate, fixup and erase on live
data were not exercised against the original beyond the cases the tests built. `Insert`/`Erase` were also
run against the original's own rotation and node routines in the paired runs for the map/multimap
containers (not for `Deallocate`, whose pool callees were always stubbed). The Begin/Next/End pass in
`Allocate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntry` is an empty loop with calls; the original's null test
on the old base before the delete is unreachable in any input and is kept.

## What is unknown / blocked

Nothing is blocked on an owner. Open: the field names above, the owners of the remaining family members
listed under "Imports opened" (`Clear__t5VIMap2ZiZi`, `Allocate__t5VIMap2ZiZi`, the `VIPool<VIEEBucket>` members,
`LowerBound`/`Insert` of the unsigned multimap, and the second `EraseFixup`/`FreeNode`/`Next` copies), and whether
the integrated build keeps the `Deallocate` body size close to the original (it compiles to 1,552 bytes
against 1,436).
