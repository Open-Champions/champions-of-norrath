# VIFile, object file reader, flat file, sound device and battle music (w15-vi-io-sound-a)

Package work is on `agents/w15-vi-io-sound-a`. Sources are under `experimental/sources/w15-vi-io-sound-a-*`, one function per directory, each with a `host_check.cpp` that is a host test and not a build input. Every function was compiled with the reviewed EE g++ `authored_cpp` flags (the same flag list as the plan's profile). Original names and addresses were read from the pinned SLUS-20565 symbol table and instruction bytes. No byte-identity claim is made for any function; compiled sizes are listed only for orientation (a few equal the original size, most differ).

Each function was also run in a private paired harness against the original image (outside git): the original and the authored object were run from identical memory snapshots, with callees bound to the original addresses or hooked with logged arguments, and return values, call logs and every changed byte were compared. Each source had its instruction coverage checked where it has branches, and between two and ten hand-made mutations of the source failed the same comparison. The harness is not part of the repository.

## Functions

| Authored symbol | Original address | Original size | Compiled size |
| --- | --- | --- | --- |
| `Preload__13VIBattleMusici` | `0x0106dc48` | 236 | 236 |
| `ParseParticleDefinition__10VIESFParseR9VIObjFileR20VIParticleDefinition` | `0x011790e8` | 1148 | 1152 |
| `BufferClear__6VIFile` | `0x010cb228` | 32 | 32 |
| `CDDiskReady__6VIFilei` | `0x010cb290` | 56 | 32 |
| `CDSearchFile__6VIFileP10sceCdlFILEPCc` | `0x010cb2c8` | 252 | 280 |
| `Flush__6VIFile` | `0x010caf50` | 40 | 56 |
| `MemorySegmCalcSegm__6VIFile` | `0x01002508` | 176 | 176 |
| `Write__6VIFilePCvi` | `0x010cb028` | 124 | 112 |
| `NumSubObjects__9VIObjFile` | `0x010a65c8` | 32 | 36 |
| `ObjectSize__9VIObjFile` | `0x010a6628` | 32 | 40 |
| `ObjectVersion__9VIObjFile` | `0x010a65e8` | 32 | 40 |
| `ReadBegin__9VIObjFileRUsT1RUlRUi` | `0x010a58d8` | 468 | 480 |
| `Read__9VIObjFilePUci` | `0x010a7550` | 128 | 136 |
| `Read__9VIObjFileRf` | `0x010a7410` | 160 | 184 |
| `Read__9VIObjFileRi` | `0x010a7190` | 160 | 184 |
| `Read__9VIObjFileRs` | `0x010a7050` | 160 | 184 |
| `ReadString__9VIObjFilePcUs` | `0x010a75d0` | 112 | 88 |
| `Read__9VIObjFileRUc` | `0x010a6fe0` | 108 | 120 |
| `Read__9VIObjFileRUi` | `0x010a7230` | 160 | 184 |
| `Read__9VIObjFileRUs` | `0x010a70f0` | 160 | 184 |
| `WriteEnd__9VIObjFileRUi` | `0x010a5710` | 452 | 504 |
| `BgmBufferSize__13VISoundDevice` | `0x011265c8` | 36 | 40 |
| `ChannelCount__13VISoundDeviceUl` | `0x011267a8` | 56 | 48 |
| `FirstPlayback__13VISoundDevice` | `0x011269e0` | 16 | 16 |
| `FreeSpuMem__13VISoundDeviceUii` | `0x01126b08` | 188 | 176 |
| `IsPlaying__13VISoundDevicei` | `0x011265f0` | 32 | 32 |
| `NextPlayback__13VISoundDeviceP10VIPlayback` | `0x011269f0` | 16 | 16 |
| `SetOutputMode__13VISoundDevicei` | `0x01126718` | 104 | 96 |
| `ShareSound__13VISoundDevicei` | `0x01126388` | 44 | 48 |
| `Sound__13VISoundDevicei` | `0x01126368` | 32 | 40 |
| `StopAllBgm__13VISoundDevice` | `0x01126560` | 72 | 76 |
| `Copy__8VISpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei` | `0x01008150` | 8 | 8 |
| `Release__8VISpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide` | `0x01008158` | 8 | 8 |

## Observed layouts

Observed from the instruction bytes and kept as offsets, not as member names.

VIFile (this): word at +0 is a descriptor, negative for a memory-backed or segment-backed file; word at +48 non-zero selects the memory path; word at +56 non-zero selects the segment path (`Write`, `Flush`). Buffer bookkeeping cleared by `BufferClear` is at +136, +144, +152, +160, +168 and +176. Segment files: count at +52, array pointer at +56 (24-byte records, 64-bit start at +0 and length at +8), current position (64-bit) at +16, found segment index at +60, offset in segment (64-bit) at +64. `_6VIFile$sIopStream` (0x01192c84) switches `CDDiskReady` and `CDSearchFile` to the IOP path.

VIObjFile (this): current nesting index (signed) at +232; entries are 32 bytes and start at +240: halfword +0, halfword +2 (version), word +4 (size), word +8 (sub-object count), 64-bit +16 (file position of the header), word +24 (bytes left to read). Flags at +568 (reading allowed), +572 and +576 (nesting), +580, +584 (set when the object has no sub-objects, tested by the Read overloads). +204 counts finished top-level objects. `ReadBegin` refuses index 9. `VIEndian` equal to exactly 1 skips the byte swaps.

VISoundDevice: playback chain head at +3284 with a terminator node whose first word is zero (`FirstPlayback`, `NextPlayback` never return the terminator); sound table from +2584; free-block list at +3312 and node pool at +3324 (VILList); flags/bytes at +3436, +3692, +3948 (cleared by `StopAllBgm`); output mode word at +3432.

VIBattleMusic: scene pointer +0 (sound device at scene +19360), track number +4, sound handle +24.

## Inferred

The record layout names (type, version, size, count, position, remaining) are inferred from how `ReadBegin`, `WriteEnd` and the `Read` overloads move the values. The base-class `Copy` and `Release` of VISprite return -1 and are inferred to be defaults that derived sprites override. `FreeSpuMem` keeps an address-ordered free list in 64-byte units.

## Checks performed

Each host check built natively and passed. Each source compiled with the reviewed toolchain flags. Paired runs: zero mismatches for every function. No full partial build was run for this package; the existing unit tests are unaffected (no shared files or tools were edited).

## Imports closed

`BgmBufferSize__13VISoundDevice`, `BufferClear__6VIFile`, `CDDiskReady__6VIFilei`, `CDSearchFile__6VIFileP10sceCdlFILEPCc`, `ChannelCount__13VISoundDeviceUl`, `Copy__8VISpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei`, `FirstPlayback__13VISoundDevice`, `Flush__6VIFile`, `FreeSpuMem__13VISoundDeviceUii`, `IsPlaying__13VISoundDevicei`, `MemorySegmCalcSegm__6VIFile`, `NextPlayback__13VISoundDeviceP10VIPlayback`, `NumSubObjects__9VIObjFile`, `ObjectSize__9VIObjFile`, `ObjectVersion__9VIObjFile`, `ParseParticleDefinition__10VIESFParseR9VIObjFileR20VIParticleDefinition`, `Preload__13VIBattleMusici`, `ReadBegin__9VIObjFileRUsT1RUlRUi`, `ReadString__9VIObjFilePcUs`, `Read__9VIObjFilePUci`, `Read__9VIObjFileRUc`, `Read__9VIObjFileRUi`, `Read__9VIObjFileRUs`, `Read__9VIObjFileRf`, `Read__9VIObjFileRi`, `Read__9VIObjFileRs`, `Release__8VISpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide`, `SetOutputMode__13VISoundDevicei`, `ShareSound__13VISoundDevicei`, `Sound__13VISoundDevicei`, `StopAllBgm__13VISoundDevice`, `WriteEnd__9VIObjFileRUi`, `Write__6VIFilePCvi`.

## Imports opened

Left unresolved on purpose; none is authored by this package. Those already in the unresolved list are marked.

`BufferedWrite__6VIFilePCvi`, `CreateMotif__22VIParticleDefinitionEx`, `Eof__6VIFile`, `InsertAfter__7VILNodeP7VILNode`, `MemorySegmWrite__6VIFilePCvi`, `MemoryWrite__6VIFilePCvi`, `MergeFreeSpuMem__13VISoundDevice`, `ParseSound__10VIESFParsePCcR7VIScene`, `ReadStringBody__9VIObjFilePcUsUs`, `ReadStringLength__9VIObjFileRUs`, `Read__6VIFilePvi`, `Seek__6VIFilelQ26VIFile10SeekOrigin`, `SetBlendMode__20VIParticleDefinitionQ222VIParticleDefinitionEx9BLENDMODE`, `SetInnerHprVar__20VIParticleAttributesRC7VIVect3`, `SetInnerOffset__20VIParticleAttributesRC7VIVect3`, `SetName__15VIParticleMotifPCc`, `SetOuterHprVar__20VIParticleAttributesRC7VIVect3`, `SetOuterOffset__20VIParticleAttributesRC7VIVect3`, `SetRepeat__4VIXmi`, `SetTextureConfiguration__20VIParticleDefinitioni`, `SetTexture__20VIParticleDefinitioniR8VIRasterR12VIDictionary`, `SetZTest__20VIParticleDefinitionb`, `SetZWrite__20VIParticleDefinitionb`, `Tell__6VIFile`, `VICdSearchFile__FPCcPUiT1`, `VIEEFileDelay__Fi`, `VIEndian`, `_13VISoundDevice$sBgmVersion`, `_6VIFile$sIopStream`, `norrath_overlay_sIopFunctionData_11edf08`, `sprintf`.

`VIEndian`, `sprintf`, `norrath_overlay_sIopFunctionData_11edf08`, `_13VISoundDevice$sBgmVersion` and `_6VIFile$sIopStream` are data words or libc symbols that other packages may already own; they are referenced by their image names (`VIEndian`, `_13VISoundDevice$sBgmVersion`, `_6VIFile$sIopStream` as assembler labels, and the existing `norrath_overlay_sIopFunctionData_11edf08` view).

## Build-plan entries needed

Append after the current last input (6764 on this branch's base). Profile `authored_cpp`, header closure empty. Do not add the `host_check.cpp` files. Also add each source path with its hash to the plan `files` map. `snapshot.json` must record the sources before `tools/check_source_snapshot.py` accepts the tree; it was not edited here.

```json
{"emitted_functions": [{"compiled_body_bytes": 236, "name": "Preload__13VIBattleMusici"}], "header_closure": [], "input_order": 6765, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_battle_music_preload.o", "profile": "authored_cpp", "sha256": "d47b2f401ec45667d339ad6ad4e187c7065fa573ff8a7cc3aea1959ef3848d53", "source": "sources/w15-vi-io-sound-a-battle-music-preload/preload.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1152, "name": "ParseParticleDefinition__10VIESFParseR9VIObjFileR20VIParticleDefinition"}], "header_closure": [], "input_order": 6766, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_esfparse_particle_definition.o", "profile": "authored_cpp", "sha256": "17388d8852573c886164c715f741838ffe7815e62356745e8d47555503f7d691", "source": "sources/w15-vi-io-sound-a-esfparse-particle-definition/particle_definition.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "BufferClear__6VIFile"}], "header_closure": [], "input_order": 6767, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_file_buffer_clear.o", "profile": "authored_cpp", "sha256": "2c09fefa669cf3b6cfc9f649a860e9543cd418bfed9faee2b6fdf320dae2a577", "source": "sources/w15-vi-io-sound-a-file-buffer-clear/buffer_clear.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "CDDiskReady__6VIFilei"}], "header_closure": [], "input_order": 6768, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_file_cd_disk_ready.o", "profile": "authored_cpp", "sha256": "71faca86f043d3e5c470a42083ad4fc1272f1056ff6de6c52e78c17d12be8f48", "source": "sources/w15-vi-io-sound-a-file-cd-disk-ready/cd_disk_ready.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 280, "name": "CDSearchFile__6VIFileP10sceCdlFILEPCc"}], "header_closure": [], "input_order": 6769, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_file_cd_search_file.o", "profile": "authored_cpp", "sha256": "1924afdd4a8d20c04117cf929fda798aec9f87e0c510ddeba4a50a2494a88d97", "source": "sources/w15-vi-io-sound-a-file-cd-search-file/cd_search_file.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 56, "name": "Flush__6VIFile"}], "header_closure": [], "input_order": 6770, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_file_flush.o", "profile": "authored_cpp", "sha256": "0b5b47e06c1f46f7383506b208a4277958983ae49b52750e3812e568dfc760db", "source": "sources/w15-vi-io-sound-a-file-flush/flush.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 176, "name": "MemorySegmCalcSegm__6VIFile"}], "header_closure": [], "input_order": 6771, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_file_memory_segm_calc_segm.o", "profile": "authored_cpp", "sha256": "7cfe9d4a552af836427f2926ac9200f80e8ddd54988e1b722448b37bef4d5bb5", "source": "sources/w15-vi-io-sound-a-file-memory-segm-calc-segm/calc_segm.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 112, "name": "Write__6VIFilePCvi"}], "header_closure": [], "input_order": 6772, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_file_write.o", "profile": "authored_cpp", "sha256": "14895b9c6ed0598ea16fcdeb9b69125cdb859813449247d33bfa1df54073fe6c", "source": "sources/w15-vi-io-sound-a-file-write/write.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 36, "name": "NumSubObjects__9VIObjFile"}], "header_closure": [], "input_order": 6773, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_objfile_num_sub_objects.o", "profile": "authored_cpp", "sha256": "cfeab57aae4ba64a50c9be1f54d257499fd65d6490c0e36bd1a8fb88cf5ad69d", "source": "sources/w15-vi-io-sound-a-objfile-num-sub-objects/num_sub_objects.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "ObjectSize__9VIObjFile"}], "header_closure": [], "input_order": 6774, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_objfile_object_size.o", "profile": "authored_cpp", "sha256": "8270c53a6e08838ef40ae558b6315e740b568373ec509a448715b55a5d8fd13b", "source": "sources/w15-vi-io-sound-a-objfile-object-size/object_size.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "ObjectVersion__9VIObjFile"}], "header_closure": [], "input_order": 6775, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_objfile_object_version.o", "profile": "authored_cpp", "sha256": "9634179250266bf4af18b4e24f48f871ac9ee73ca43a0ee9c77c9f0f7043edc0", "source": "sources/w15-vi-io-sound-a-objfile-object-version/object_version.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 480, "name": "ReadBegin__9VIObjFileRUsT1RUlRUi"}], "header_closure": [], "input_order": 6776, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_objfile_read_begin.o", "profile": "authored_cpp", "sha256": "2547b78d81b61225708320fc86816eb104aea4fc92fbc3cea46c0cc8bb43fd29", "source": "sources/w15-vi-io-sound-a-objfile-read-begin/read_begin.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 136, "name": "Read__9VIObjFilePUci"}], "header_closure": [], "input_order": 6777, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_objfile_read_bytes.o", "profile": "authored_cpp", "sha256": "b062639d3fa7a71e88ae4dd6ee010d8cecde352d1866efd0c260c14100c12960", "source": "sources/w15-vi-io-sound-a-objfile-read-bytes/read_bytes.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "Read__9VIObjFileRf"}], "header_closure": [], "input_order": 6778, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_objfile_read_float.o", "profile": "authored_cpp", "sha256": "fb66a94034a2fe95961f0a1e6d0a6af80ff030cb349a667a91610f98cebd1ed3", "source": "sources/w15-vi-io-sound-a-objfile-read-float/read_float.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "Read__9VIObjFileRi"}], "header_closure": [], "input_order": 6779, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_objfile_read_int.o", "profile": "authored_cpp", "sha256": "fe202ef85e73d90b8eec8ee6e0a60700da5cfb71209b3bc73989271421494541", "source": "sources/w15-vi-io-sound-a-objfile-read-int/read_int.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "Read__9VIObjFileRs"}], "header_closure": [], "input_order": 6780, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_objfile_read_short.o", "profile": "authored_cpp", "sha256": "024c7519a0b85c71af2addfc3a507e993ca15a99775d7084cd7ee868cb5b0331", "source": "sources/w15-vi-io-sound-a-objfile-read-short/read_short.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 88, "name": "ReadString__9VIObjFilePcUs"}], "header_closure": [], "input_order": 6781, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_objfile_read_string.o", "profile": "authored_cpp", "sha256": "29cdb07b40bcf033db466e9c22a0b357198c926caabec6b8105ee40f458003dd", "source": "sources/w15-vi-io-sound-a-objfile-read-string/read_string.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 120, "name": "Read__9VIObjFileRUc"}], "header_closure": [], "input_order": 6782, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_objfile_read_uchar.o", "profile": "authored_cpp", "sha256": "19fa1894e427b0ec872cfd1af34d823de1112255263b12bbe4d3b24897ef29ff", "source": "sources/w15-vi-io-sound-a-objfile-read-uchar/read_uchar.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "Read__9VIObjFileRUi"}], "header_closure": [], "input_order": 6783, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_objfile_read_uint.o", "profile": "authored_cpp", "sha256": "2209396d907c88684c5ce935b03ca72cc09586ba3ad882c808e3556da7eb48b6", "source": "sources/w15-vi-io-sound-a-objfile-read-uint/read_uint.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 184, "name": "Read__9VIObjFileRUs"}], "header_closure": [], "input_order": 6784, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_objfile_read_ushort.o", "profile": "authored_cpp", "sha256": "3cbfccb4d1ba6228265e56d525869b3e813d43aff561c5a72b077d2e8cefb833", "source": "sources/w15-vi-io-sound-a-objfile-read-ushort/read_ushort.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 504, "name": "WriteEnd__9VIObjFileRUi"}], "header_closure": [], "input_order": 6785, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_objfile_write_end.o", "profile": "authored_cpp", "sha256": "3c4616dec04d553d4c55b0da6d3535e14dcdd263741c2ad9170a61bca5095b80", "source": "sources/w15-vi-io-sound-a-objfile-write-end/write_end.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "BgmBufferSize__13VISoundDevice"}], "header_closure": [], "input_order": 6786, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_sound_device_bgm_buffer_size.o", "profile": "authored_cpp", "sha256": "fa5bf9c1c05a16d65c05f7153aa0f90659ede6494c59a4cf7a78e01128ee8062", "source": "sources/w15-vi-io-sound-a-sound-device-bgm-buffer-size/bgm_buffer_size.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "ChannelCount__13VISoundDeviceUl"}], "header_closure": [], "input_order": 6787, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_sound_device_channel_count.o", "profile": "authored_cpp", "sha256": "f794f36352e5d1beb2ab11d8899949d083a74ead63e7524890a5989611dbbd70", "source": "sources/w15-vi-io-sound-a-sound-device-channel-count/channel_count.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "FirstPlayback__13VISoundDevice"}], "header_closure": [], "input_order": 6788, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_sound_device_first_playback.o", "profile": "authored_cpp", "sha256": "07e7eeb9bc3f1a11b0730a16a6d4a6daaa095888d6d6ea72a16699049f81c929", "source": "sources/w15-vi-io-sound-a-sound-device-first-playback/first_playback.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 176, "name": "FreeSpuMem__13VISoundDeviceUii"}], "header_closure": [], "input_order": 6789, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_sound_device_free_spu_mem.o", "profile": "authored_cpp", "sha256": "a9b5cb1918410c5f3025bd633f72ac734ef7bb0a6c150c61a8158077c4a527d2", "source": "sources/w15-vi-io-sound-a-sound-device-free-spu-mem/free_spu_mem.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 32, "name": "IsPlaying__13VISoundDevicei"}], "header_closure": [], "input_order": 6790, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_sound_device_is_playing.o", "profile": "authored_cpp", "sha256": "21d4c57104456f2e11015dd4cef97fad0e96a2f06de3f97118a7d4e80dad1fc0", "source": "sources/w15-vi-io-sound-a-sound-device-is-playing/is_playing.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "NextPlayback__13VISoundDeviceP10VIPlayback"}], "header_closure": [], "input_order": 6791, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_sound_device_next_playback.o", "profile": "authored_cpp", "sha256": "232ab671b2fb7a82262114942a3ab81e6aa8413128cc8da49655341b62f1341d", "source": "sources/w15-vi-io-sound-a-sound-device-next-playback/next_playback.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 96, "name": "SetOutputMode__13VISoundDevicei"}], "header_closure": [], "input_order": 6792, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_sound_device_set_output_mode.o", "profile": "authored_cpp", "sha256": "86658bdef019f294a39b2bbf524204ec0952af955fcec5f11fbdb5bb4054365f", "source": "sources/w15-vi-io-sound-a-sound-device-set-output-mode/set_output_mode.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 48, "name": "ShareSound__13VISoundDevicei"}], "header_closure": [], "input_order": 6793, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_sound_device_share_sound.o", "profile": "authored_cpp", "sha256": "abbc947b8b9b99432039ed9620ebb69430a2dc1a55605b2c6a29bf4e0095232c", "source": "sources/w15-vi-io-sound-a-sound-device-share-sound/share_sound.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 40, "name": "Sound__13VISoundDevicei"}], "header_closure": [], "input_order": 6794, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_sound_device_sound.o", "profile": "authored_cpp", "sha256": "0c4f7f2fb3e4a40a1b2c4e4bcea868ef03a7dea1cb34ea72cbf87ab28783b067", "source": "sources/w15-vi-io-sound-a-sound-device-sound/sound.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 76, "name": "StopAllBgm__13VISoundDevice"}], "header_closure": [], "input_order": 6795, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_sound_device_stop_all_bgm.o", "profile": "authored_cpp", "sha256": "9d49f533aec8b2ad5644545a78d45393f00bc5e52c899214ded6316aa7f4aa74", "source": "sources/w15-vi-io-sound-a-sound-device-stop-all-bgm/stop_all_bgm.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "Copy__8VISpriteR7VISceneP8VIRasterP13VISoundDeviceR9VICollidei"}], "header_closure": [], "input_order": 6796, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_sprite_copy.o", "profile": "authored_cpp", "sha256": "37b6b0672d7fb1cb9597bd4c4e0fdc6a5697c78c877d74b0d6e80596c930e13c", "source": "sources/w15-vi-io-sound-a-sprite-copy/copy.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 8, "name": "Release__8VISpriteR7VISceneR12VIDictionaryP8VIRasterP13VISoundDeviceR9VICollide"}], "header_closure": [], "input_order": 6797, "kind": "independently_authored_source", "object_name": "w15_vi_io_sound_a_sprite_release.o", "profile": "authored_cpp", "sha256": "ad1eb9bf25a26013abaaa73b648bacf4879f617608f14ace3f3619d0591411a4", "source": "sources/w15-vi-io-sound-a-sprite-release/release.cpp"}
```

## Authored but never executed

All of it, in the sense of the game: nothing here has run inside the game or on hardware. The paired runs use a small private interpreter with hooked callees, so they show equivalence of the call sequence and memory effects for the scenarios tried, not behaviour of the callees. Specifically unexecuted against real callees: every function that calls the CD library (`CDSearchFile`, `CDDiskReady`), the IOP request (`StopAllBgm`), the byte swap on a big-endian file, `WriteEnd`'s file writes and seeks, and the whole particle-definition read.

## Unknown

Member names throughout. What the `+3436/+3692/+3948` bytes mean beyond being cleared together. The unit of the 480 delay in `CDSearchFile` and the contract of `VICdSearchFile`. What resource type 1 is in `ParseParticleDefinition`, and why the gravity read is gated on version above zero. Why depth 9 is refused by `ReadBegin`. The size of the VIESFParse object (1072 bytes is taken from the stack space the image reserves; its constructor writes up to offset 1064). `MemorySegmCalcSegm` with a segment count of zero uses the caller's third argument register as the index in the image; the authored function keeps that as a third parameter, and whether any caller sets it is unknown.
