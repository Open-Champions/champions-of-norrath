# VIFile memory/buffered I/O, string-table loading, ESF world/sprite parsers and sound-device free list (w16-io-a)

Package work is on `agents/w16-io-a`. Sources are under `experimental/sources/w16-io-a-*`, one function per directory, each with a `host_check.cpp` that is a host test and not a build input. No shared file was edited (no build plan, snapshot, receipt, tool or roadmap change); the plan entries are listed at the end.

Each function was also run in a private paired harness against the original image (outside git, never committed): the original and the authored object ran from identical memory snapshots with every callee bound to the same Python stub, and the stub call logs (every argument register, plus pointed-to contents where noted) and every changed byte outside the stack were compared. Scenario counts, coverage and mutation results are in each commit message.

## Functions

| Authored symbol | Original address | Original size | Compiled size |
| --- | --- | --- | --- |
| `MemorySeek__6VIFilelQ26VIFile10SeekOrigin` | `0x010021b0` | 104 | 104 |
| `MemoryRead__6VIFilePvi` | `0x01002218` | 156 | 208 |
| `MemoryWrite__6VIFilePCvi` | `0x010022b8` | 152 | 204 |
| `MemorySegmRead__6VIFilePvi` | `0x01001ea8` | 336 | 332 |
| `MemorySegmWrite__6VIFilePCvi` | `0x01001ff8` | 332 | 332 |
| `MemorySegmSeek__6VIFilelQ26VIFile10SeekOrigin` | `0x01002488` | 128 | 136 |
| `Eof__6VIFile` | `0x010caf78` | 48 | 56 |
| `BufferedRead__6VIFilePvi` | `0x010ca088` | 1352 | 1080 (+204 local helper) |
| `BufferedWrite__6VIFilePCvi` | `0x010ca5d0` | 768 | 676 (+204 local helper) |
| `BufferedSeek__6VIFilelQ26VIFile10SeekOrigin` | `0x010ca8d0` | 292 | 340 |
| `MergeFreeSpuMem__13VISoundDevice` | `0x01126bc8` | 156 | 184 |
| `ParseSound__10VIESFParsePCcR7VIScene` | `0x0116dc38` | 272 | 280 |
| `ParseSprite__10VIESFParse` | `0x0116e320` | 484 | 472 |
| `ParseSubSprite__10VIESFParse` | `0x01179b88` | 220 | 184 |
| `ParseZoneBaseObj__10VIESFParse` | `0x011750f8` | 284 | 316 |
| `ParseWorldBaseHeader__10VIESFParseR7VIWorld` | `0x01176580` | 480 | 400 |
| `ParseWorldRegions__10VIESFParseR7VIWorld` | `0x0117b320` | 184 | 168 |
| `ParseWorldTree__10VIESFParseR11VIWorldTree` | `0x01176940` | 392 | 392 |
| `ParseWorldZoneProxies__10VIESFParseR7VIWorld` | `0x01176760` | 480 | 404 |
| `ParsePrimBufferObjV0__10VIESFParse` | `0x0116f2d8` | 800 | 660 |
| `FixupTextBuffer__13VIStringTable` | `0x0101b5f0` | 472 | 556 |
| `LoadRawFile__13VIStringTablePCcRt7VIArray1Zc` | `0x0101b7c8` | 340 | (existing w15 source, see below) |

No function is claimed as a byte match: compiled sizes differ and no instruction comparison was made. Equivalence evidence is the paired runs only.

## Observed layouts

Kept as offsets, not member names.

VIFile (this), the part the memory and buffered routines use: word +0 descriptor (negative selects the memory or segment path, see `Read__6VIFilePvi`); word +8 end-of-data flag; 64-bit +16 position; +24 the descriptor's own position (buffered files); +32 capacity and +40 size (memory files, `MemoryWrite` raises +40 to the new position); word +48 memory base; +52 segment count minus one, +56 segment-record table (24-byte records: 64-bit length at +8, pointer word at +16), +60 current segment, 64-bit +64 offset in the segment; buffered files: word +72 first sector, word +76 size, +108 CD read-mode block, 64-bit +136 file offset of the cached block, +144/+152 dirty range, +160 position in the cache, word +168 end-of-file-in-cache flag, 64-bit +176 bytes cached, word +188 cache address. Seek origins 0, 1 and 2 are current-relative, absolute and end-relative (values observed, names not).

VIESFParse (parse): +4 progress callback, +8 its argument, +12, +16, +20 and +32 are set by `ParseSound__10VIESFParsePCcR7VIScene` from scene words +19356, +19360, +19364 and +19352 (+12 is used as the raster by `ParsePrimBufferObjV0`, +32 as the dictionary by the ADPCM parser), +24 scene, +28 current zone, +36 object file.

VIStringTable (this): +0 rows, +4 columns, +8 text, +12 grid of string starts.

## Inferred

The memory/segment/buffered routines are the three backends behind `Read`/`Write`/`Seek`/`Eof` of VIFile; the buffered backend is a 2048-byte-sector read cache (up to 32 KiB written through a dirty range). The ESF parsers are tag dispatchers and record readers; the tag-to-kind pairings follow the handler names. Names for fields are not claimed.

## Checks performed

Every source compiled with the reviewed toolchain flags (`authored_cpp` profile, no warnings treated as errors). Every host check was built natively and passed. Paired runs: zero mismatches for every function (HALT lines grepped: none). The existing unit tests pass (214 tests). `tools/check_source_snapshot.py` does not accept the tree because the new sources are not yet recorded in `snapshot.json`, which this package may not edit.

## Full partial build

One private build was run with a private snapshot and plan copy kept outside git (the 21 entries below appended after input 7328): 7348 source compilations, one link, 5857 function bodies, zero COMMON, 834 unresolved imports. A build without these entries was not run, so the base count is taken from the integrator's last record rather than re-measured here.

## Imports closed

`BufferedRead__6VIFilePvi`, `BufferedSeek__6VIFilelQ26VIFile10SeekOrigin`, `BufferedWrite__6VIFilePCvi`, `Eof__6VIFile`, `FixupTextBuffer__13VIStringTable`, `MemoryRead__6VIFilePvi`, `MemorySeek__6VIFilelQ26VIFile10SeekOrigin`, `MemorySegmRead__6VIFilePvi`, `MemorySegmSeek__6VIFilelQ26VIFile10SeekOrigin`, `MemorySegmWrite__6VIFilePCvi`, `MemoryWrite__6VIFilePCvi`, `MergeFreeSpuMem__13VISoundDevice`, `ParsePrimBufferObjV0__10VIESFParse`, `ParseSound__10VIESFParsePCcR7VIScene`, `ParseSprite__10VIESFParse`, `ParseSubSprite__10VIESFParse`, `ParseWorldBaseHeader__10VIESFParseR7VIWorld`, `ParseWorldRegions__10VIESFParseR7VIWorld`, `ParseWorldTree__10VIESFParseR11VIWorldTree`, `ParseWorldZoneProxies__10VIESFParseR7VIWorld`, `ParseZoneBaseObj__10VIESFParse` (21 of the 22 scope names; confirmed absent from `unresolved-imports.json` of the private build).

`LoadRawFile__13VIStringTablePCcRt7VIArray1Zc` is the 22nd name. Its source already exists (`w15-vi-strings-dict-a-vistringtable-loadrawfile`, not in the plan because it had no committed host check). This package adds the missing check in `w16-io-a-vistringtable-load-raw-file-check/host_check.cpp` (it includes the w15 source by relative path) and ran it against the image in a private paired run (270 scenarios, 0 mismatches, 85 of 85 words). The integrator should add the w15 unit to the plan to close the name; it is not among the entries below because the source is not mine.

## Imports opened

None new. Every callee left unresolved by these units was already referenced by another plan unit (checked against the undefined symbols of all other objects of the private build): `BeginMaterial__12VIPrimBufferi`, `Begin__12VIPrimBufferi`, `BufferedReadCopy__6VIFilePvi`, `CDRead__6VIFileUiUiPvP10sceCdRMode`, `Color__12VIPrimBufferR9VIColor32`, `CreatePrimBuffer__8VIRasteri`, `EndMaterial__12VIPrimBuffer`, `End__12VIPrimBuffer`, `Init__12VIPrimBufferR8VIRaster16VIPrimBufferTypeiii`, `Lock__12VIPrimBuffer`, `Normal__12VIPrimBufferR7VIVect3`, `Open__9VIObjFilePCcQ29VIObjFile8FileMode`, `ParseSimpleSubSpriteObj__10VIESFParse`, `ParseSkinLODSpriteObj__10VIESFParse`, `ParseSkinSubSpriteObj__10VIESFParse`, `ParseZoneFlora__10VIESFParse`, `ParseZonePreTranslations__10VIESFParseR6VIZone`, `ParseZoneRooms__10VIESFParseR6VIZone`, `ParseZoneStaticTable__10VIESFParseRt7VIArray1Z21VIZoneStaticTableElem`, `ParseZoneTree__10VIESFParseR10VIZoneTree`, `Precalc__17VIZoneRadialFloraR7VIScene`, `PrimBuffer__8VIRasteri`, `Read__9VIObjFileRUl`, `RemoveActorsFromZone__7VISceneR6VIZoneRt7VIArray1Zi`, `Remove__7VILNode`, `UV0__12VIPrimBufferR7VIVect2`, `Unlock__12VIPrimBuffer`, `UpdateActors__7VISceneRt7VIArray1Zi`, `Vertex__12VIPrimBufferR7VIVect3`. (`memcpy` and `memset` resolve to the authored providers already in the plan.)

## Build-plan entries needed

Append after the current last input (7328 on this branch's base), profile `authored_cpp`, header closure empty, `input_order` as shown; also add each source path with its hash to the plan `files` map and record the sources in `snapshot.json` (neither was edited here). Do not add `host_check.cpp`. `BufferedRead` and `BufferedWrite` each emit one local helper, `_ZL9writeBackPh` (204 bytes, a file-local symbol), listed in their entries; if the integrator prefers not to list a helper, mark it inline. With these entries the plan check (`--check-plan`) passes against a private snapshot copy.

```json
{"emitted_functions": [{"compiled_body_bytes": 280, "name": "ParseSound__10VIESFParsePCcR7VIScene"}], "header_closure": [], "input_order": 7329, "kind": "independently_authored_source", "object_name": "w16_io_a_esfparse_parse_sound_file.o", "profile": "authored_cpp", "sha256": "4860c6979a333577c3157373c6597dfbdbed514be14d10fb8191c9db15567e9a", "source": "sources/w16-io-a-esfparse-parse-sound-file/parse_sound_file.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 660, "name": "ParsePrimBufferObjV0__10VIESFParse"}], "header_closure": [], "input_order": 7330, "kind": "independently_authored_source", "object_name": "w16_io_a_esfparse_prim_buffer_obj_v0.o", "profile": "authored_cpp", "sha256": "6ba6168361060cd373835a91465faf234e98879fe00633787e43711a72dc0d30", "source": "sources/w16-io-a-esfparse-prim-buffer-obj-v0/parse_prim_buffer_obj_v0.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 472, "name": "ParseSprite__10VIESFParse"}], "header_closure": [], "input_order": 7331, "kind": "independently_authored_source", "object_name": "w16_io_a_esfparse_sprite.o", "profile": "authored_cpp", "sha256": "b31025522d6315e07b6beab3084198a6d4d66fc3f72da214118755597576e0df", "source": "sources/w16-io-a-esfparse-sprite/parse_sprite.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "ParseSubSprite__10VIESFParse"}], "header_closure": [], "input_order": 7332, "kind": "independently_authored_source", "object_name": "w16_io_a_esfparse_sub_sprite.o", "profile": "authored_cpp", "sha256": "44c6777d01754ba305c8a8bc5076821407e3b750ed2878107f47266b4de7a04c", "source": "sources/w16-io-a-esfparse-sub-sprite/parse_sub_sprite.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 400, "name": "ParseWorldBaseHeader__10VIESFParseR7VIWorld"}], "header_closure": [], "input_order": 7333, "kind": "independently_authored_source", "object_name": "w16_io_a_esfparse_world_base_header.o", "profile": "authored_cpp", "sha256": "53a6d5138d76e93062c29b19efd43cfb236c3ce53ba30b5a5339eb0e5cdc07dc", "source": "sources/w16-io-a-esfparse-world-base-header/parse_world_base_header.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 168, "name": "ParseWorldRegions__10VIESFParseR7VIWorld"}], "header_closure": [], "input_order": 7334, "kind": "independently_authored_source", "object_name": "w16_io_a_esfparse_world_regions.o", "profile": "authored_cpp", "sha256": "e70acfa8d59a86371fc2d1abf21c3218b77377a129c5b9a9930def46f387f4b9", "source": "sources/w16-io-a-esfparse-world-regions/parse_world_regions.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 392, "name": "ParseWorldTree__10VIESFParseR11VIWorldTree"}], "header_closure": [], "input_order": 7335, "kind": "independently_authored_source", "object_name": "w16_io_a_esfparse_world_tree.o", "profile": "authored_cpp", "sha256": "3ba45f8a995685f8378b17087de90518ae54759e0e2dc775bc6b28f1e3b65bc2", "source": "sources/w16-io-a-esfparse-world-tree/parse_world_tree.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 404, "name": "ParseWorldZoneProxies__10VIESFParseR7VIWorld"}], "header_closure": [], "input_order": 7336, "kind": "independently_authored_source", "object_name": "w16_io_a_esfparse_world_zone_proxies.o", "profile": "authored_cpp", "sha256": "3e388f1f6e3e9afbcec03a9520fc4704f10b3e5f9a9b9ee19da08adc28e521f9", "source": "sources/w16-io-a-esfparse-world-zone-proxies/parse_world_zone_proxies.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 316, "name": "ParseZoneBaseObj__10VIESFParse"}], "header_closure": [], "input_order": 7337, "kind": "independently_authored_source", "object_name": "w16_io_a_esfparse_zone_base_obj.o", "profile": "authored_cpp", "sha256": "bb8368df0d8b7c1a041f68a86e4bdec0306553a7312b25ff8282157a337171ef", "source": "sources/w16-io-a-esfparse-zone-base-obj/parse_zone_base_obj.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "MergeFreeSpuMem__13VISoundDevice"}], "header_closure": [], "input_order": 7338, "kind": "independently_authored_source", "object_name": "w16_io_a_sound_device_merge_free_spu_mem.o", "profile": "authored_cpp", "sha256": "f5a4b11f883f25454684c55ed16c1a03624255b6a2d6b8db78359dcbfc2bb151", "source": "sources/w16-io-a-sound-device-merge-free-spu-mem/merge_free_spu_mem.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1080, "name": "BufferedRead__6VIFilePvi"}, {"compiled_body_bytes": 204, "name": "_ZL9writeBackPh"}], "header_closure": [], "input_order": 7339, "kind": "independently_authored_source", "object_name": "w16_io_a_vifile_buffered_read.o", "profile": "authored_cpp", "sha256": "68daa58f0257d6e91ac1c983f32cc4bddbadc261349afb7fa756ee5f7d3611a5", "source": "sources/w16-io-a-vifile-buffered-read/buffered_read.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 340, "name": "BufferedSeek__6VIFilelQ26VIFile10SeekOrigin"}], "header_closure": [], "input_order": 7340, "kind": "independently_authored_source", "object_name": "w16_io_a_vifile_buffered_seek.o", "profile": "authored_cpp", "sha256": "baedd5759735a9a9f1085262ee4d0041b43aa062350feff1126b9553c6b8b37a", "source": "sources/w16-io-a-vifile-buffered-seek/buffered_seek.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 676, "name": "BufferedWrite__6VIFilePCvi"}, {"compiled_body_bytes": 204, "name": "_ZL9writeBackPh"}], "header_closure": [], "input_order": 7341, "kind": "independently_authored_source", "object_name": "w16_io_a_vifile_buffered_write.o", "profile": "authored_cpp", "sha256": "101466ad98a3bacd2eabf357f367a13cefa274d0157eba8fbe8b041837c8adad", "source": "sources/w16-io-a-vifile-buffered-write/buffered_write.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 56, "name": "Eof__6VIFile"}], "header_closure": [], "input_order": 7342, "kind": "independently_authored_source", "object_name": "w16_io_a_vifile_eof.o", "profile": "authored_cpp", "sha256": "db3858304dcf844f3ebfe90b11ee3ccfdf7d6aa59308876c109b9ac942e4d9d3", "source": "sources/w16-io-a-vifile-eof/eof.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 208, "name": "MemoryRead__6VIFilePvi"}], "header_closure": [], "input_order": 7343, "kind": "independently_authored_source", "object_name": "w16_io_a_vifile_memory_read.o", "profile": "authored_cpp", "sha256": "6ee1f081ca94817e5bfa612c740f31c9dea387fad58259de8c46c78ea75d8983", "source": "sources/w16-io-a-vifile-memory-read/memory_read.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "MemorySeek__6VIFilelQ26VIFile10SeekOrigin"}], "header_closure": [], "input_order": 7344, "kind": "independently_authored_source", "object_name": "w16_io_a_vifile_memory_seek.o", "profile": "authored_cpp", "sha256": "10c47a153149c8a03d690bc1126cf937fe3f3b4ef29b94c5d381daebe4bf8142", "source": "sources/w16-io-a-vifile-memory-seek/memory_seek.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 332, "name": "MemorySegmRead__6VIFilePvi"}], "header_closure": [], "input_order": 7345, "kind": "independently_authored_source", "object_name": "w16_io_a_vifile_memory_segm_read.o", "profile": "authored_cpp", "sha256": "432f45eb11efc25428a98389ee8e0eaa72cc04edf137f58a844dc613d6331b60", "source": "sources/w16-io-a-vifile-memory-segm-read/memory_segm_read.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "MemorySegmSeek__6VIFilelQ26VIFile10SeekOrigin"}], "header_closure": [], "input_order": 7346, "kind": "independently_authored_source", "object_name": "w16_io_a_vifile_memory_segm_seek.o", "profile": "authored_cpp", "sha256": "11f53e9182509a4ef4ea4cd9b3f573cb6c5bc01f874929308e68b2bea6074b2d", "source": "sources/w16-io-a-vifile-memory-segm-seek/memory_segm_seek.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 332, "name": "MemorySegmWrite__6VIFilePCvi"}], "header_closure": [], "input_order": 7347, "kind": "independently_authored_source", "object_name": "w16_io_a_vifile_memory_segm_write.o", "profile": "authored_cpp", "sha256": "5dcc34eab7c2f3bcdcdade373ce517c3ac4afaf140492aa237d83f59c252f2c5", "source": "sources/w16-io-a-vifile-memory-segm-write/memory_segm_write.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 204, "name": "MemoryWrite__6VIFilePCvi"}], "header_closure": [], "input_order": 7348, "kind": "independently_authored_source", "object_name": "w16_io_a_vifile_memory_write.o", "profile": "authored_cpp", "sha256": "a6e6f2d23cf5de0254e31520a20a20bea4fc2c247479296f8ff32152d37050d0", "source": "sources/w16-io-a-vifile-memory-write/memory_write.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 556, "name": "FixupTextBuffer__13VIStringTable"}], "header_closure": [], "input_order": 7349, "kind": "independently_authored_source", "object_name": "w16_io_a_vistringtable_fixup_text_buffer.o", "profile": "authored_cpp", "sha256": "e79f83e2385946592d59d112983fb008350ee86069dad08dbc65b21e2532dd29", "source": "sources/w16-io-a-vistringtable-fixup-text-buffer/fixup.cpp"}
```

## Authored but never executed

- The byte-copy loop in `BufferedWrite__6VIFilePCvi` (12 original words): it needs a position of 2^31 or more, and then does not terminate in the image either; written from the disassembly and never run.
- The progress-callback call in `ParseSubSprite__10VIESFParse` is run in the paired harness but not in the host check (the callback word is a 32-bit target address).
- The initial alpha byte (255) of the colour local in `ParsePrimBufferObjV0__10VIESFParse` is not observable: the four colour reads always overwrite it.
- No function has run on hardware or in the game; all evidence is host checks plus paired runs of a private interpreter.
- Two units were written to stand beside existing ones: `w16-io-a-vistringtable-fixup-text-buffer` defines the same symbol as `w15-vi-strings-dict-a-vistringtable-fixuptextbuffer` (word-based so it can be host-tested); select only one.

## Unknown

Member names of VIFile, VIObjFile-adjacent parse records, VIWorld, VIZone and the table objects; the enum spellings of the seek origins and FileMode; the meaning of the world-header integers A to E and of the tree/zone-proxy fields; the VIPrimBufferType value 0; why `BufferedWrite` sets the dirty start to min(start, position) for an empty dirty range; the size of VIObjFile (taken from the 592 bytes of stack the image reserves).
