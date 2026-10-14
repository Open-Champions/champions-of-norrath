# VIString, string table, UDP socket and camera helpers (w15-vi-strings-dict-a)

Branch `agents/w15-vi-strings-dict-a`. Fifteen symbols, fifteen units, one per commit, each under
`experimental/sources/w15-vi-strings-dict-a-<unit>/` with the implementation and a `host_check.cpp`
(host compiler only; not a build input). Reference image: `work/private/SLUS_205.65` (read-only,
never committed). Names, addresses and extents are the image's own symbol table. No disassembly or
image bytes are committed; the only image-derived data is the 32 table values described below.

## Checks performed

- Each implementation compiled with the reviewed EE g++ and the `authored_cpp` flags. Every object
  defines only its own symbol (the string-table units also hold file-local read-only items) and no
  COMMON symbol.
- Each unit was compared against the original function in a private R5900 interpreter harness
  (outside git): original and authored code ran from identical memory snapshots with identical
  stub callees, and call logs, return values and every changed byte outside the stack had to match.
  Scenario counts are in the commit messages. For every unit I also mutated the source (offset,
  bound, signedness, constant, argument) and confirmed the comparison reports a mismatch; one
  mutation in FixupTextBuffer (the delete of a pointer that was just set to null) is a dead branch
  in the original as well and is not observable.
- Each `host_check.cpp` compiled with the host compiler and ran to exit 0. The two snprintf16 and
  one snprintf check read variadic slots through the argument pointer and only run their body on
  Apple arm64; elsewhere they compile to a no-op.
- The table bytes were compared against the image privately: the compiled object's 64 bytes equal
  the image's.
- Body sizes are from the EE profile on this machine; no unit is claimed to be a byte match with the
  original, and no EE or gameplay execution of the full game happened. A full partial build was not
  rerun (a run takes hours); the lead integrator's link gives the real unresolved-count delta.

## Observed, inferred, unknown (per symbol; details are in each source header)

| Symbol | Address, extent | Compiled bytes | Note |
|---|---|---|---|
| `GetMode__6Camera` | 0x0026fc60, 8 | 8 | returns int at camera+52 |
| `strupr__8VIStringPc` | 0x0101a820, 28 | 8 | forwards to `strupr`, returns its result |
| `strcpy8__8VIStringPcUiPCc` | 0x0101aa68, 100 | 80 | bounded copy, always terminates, returns dst |
| `strcpy16__8VIStringPUsUiPCUs` | 0x0101aac8, 100 | 80 | same for 16-bit; limit compared in BYTES |
| `strcat16__8VIStringPUsUiPCUs` | 0x0101adb8, 160 | 100 | append, limit in bytes, no clamp when dst already longer |
| `strcat16__8VIStringPUsUiPCc` | 0x0101ae58, 176 | 100 | narrow source is sign-extended (0xE9 -> 0xFFE9), no table |
| `str8to16__8VIStringPUsUiPCc` | 0x0101ad28, 144 | 108 | bytes 0x80..0x9f go through the table, others unchanged |
| `snprintf__8VIStringPciPCce` | 0x0101a030, 88 | 56 | variadic forwarder to `VI_vsnprintf<char>` |
| `snprintf16__8VIStringPUsiPCce` | 0x0101a088, 88 | 56 | forwarder to `vsnprintf16__8VIStringPUsiPCcPv` |
| `snprintf16__8VIStringPUsiPCUse` | 0x0101a0e0, 88 | 56 | forwarder to `VI_vsnprintf<unsigned short>` |
| `_8VIString$ConversionTable1252` | 0x0118e658, 64 | n/a | 32 x u16; see below |
| `__11VIUDPSocketi` | 0x0102a178, 72 | 76 | stores int at +0, -1 at +68, memalign(64, 20480) to +4, tail call to `sceLibnetInitialize(this+8, 2048, 8192, 32)` |
| `FixupTextBuffer__13VIStringTable` | 0x0101b5f0, 472 | 524 | rows/columns of a tab/newline text blob and an in-place pointer grid |
| `LoadRawFile__13VIStringTablePCcRt7VIArray1Zc` | 0x0101b7c8, 340 | 472 | static; reads a whole file into a sized byte array |
| `ProcessRawFile__13VIStringTableRCt7VIArray1Zc` | 0x0101b920, 600 | 512 | UTF-8 / UTF-16 LE / UTF-16 BE / plain text into the 16-bit buffer at +8 |

Observed details worth keeping:

- The conversion table is NOT the published Windows-1252 mapping in two places: byte 0x82 maps to
  0x20a1 and byte 0x9e to 0xfffe. Positions with no character in Windows-1252 hold 0xfffe. The
  compile keeps the image's values. Whether the table is read-only in the original is unknown; it is
  only read by observed code, so `const` is an assumption. The overlay (`.srs`) the image keeps it in
  is not modelled.
- The size/limit argument of the 16-bit string helpers is compared against byte offsets, so callers
  must pass a byte size; this is observed in all four helpers, not a guess about callers.
- `strupr`, the vsnprintf callees and `__builtin_vec_new` and similar are called by name; their
  behaviour is outside this package.
- FixupTextBuffer folds the column count into the column maximum only at newlines, so a final line
  with more tabs than any earlier line would index past the allocated grid (observed in the
  original's logic, not guarded here).
- LoadRawFile's `Tell` returns a 64-bit value (`ld`); the assertion compares it signed with
  0xFFFFFFFF, the later size is the low 32 bits. The two assertion macros differ: one stores
  `__VIAssertf_line`/`__VIAssertf_file` first and passes `(Open(...) == 0)`, the other passes the
  expression text, file and line 253 directly to `_VIAssert`.
- ProcessRawFile re-creates three file-local items (headers EF BB BF and FF FE, a 16-bit item set to
  0xFFFE on first use behind a flag word) as local statics; their original storage is separate.
  Failure code 1 when the UTF-8 conversion returns a different count is an observed value.
- `__11VIUDPSocketi` returns whatever `sceLibnetInitialize` returned (it is a tail call); it is
  declared void here. The int argument's meaning (port or handle) is unknown. The sibling
  destructor `_$_11VIUDPSocket` is unresolved too but was not in this package's list; not authored.

## Imports closed

```
FixupTextBuffer__13VIStringTable
LoadRawFile__13VIStringTablePCcRt7VIArray1Zc
ProcessRawFile__13VIStringTableRCt7VIArray1Zc
_8VIString$ConversionTable1252
snprintf16__8VIStringPUsiPCUse
snprintf16__8VIStringPUsiPCce
snprintf__8VIStringPciPCce
str8to16__8VIStringPUsUiPCc
strcat16__8VIStringPUsUiPCUs
strcat16__8VIStringPUsUiPCc
strcpy16__8VIStringPUsUiPCUs
strcpy8__8VIStringPcUiPCc
strupr__8VIStringPc
__11VIUDPSocketi
GetMode__6Camera
```

## Imports opened

The units call these without defining them. Several are already defined by the plan or other
packages (for example `strlen16__8VIStringPCUs`, `memcmp`, `__builtin_vec_new`,
`__builtin_vec_delete`, `Open__6VIFilePCci`, `__wrap_memalign`, `sceLibnetInitialize`); the others
were not in `work/plan15-build/unresolved-imports.json`, so they are presumably supplied elsewhere,
but I did not confirm that in a link. Upper bound:

```
UTF8_convertToUTF16__FPCcPUsi
UTF8_strlen__FPCci
VI_vsnprintf__H1ZUs_PX01iPCX01Pv_v
VI_vsnprintf__H1Zc_PX01iPCX01Pv_v
__6VIFile
_$_6VIFile
__VIAssertf_file
__VIAssertf_line
_VIAssert
_VIAssertf
Init__t7VIArray1Zci
Read__6VIFilePvi
Seek__6VIFilelQ26VIFile10SeekOrigin
Tell__6VIFile
memcmp
memcpy
memset
strupr
vsnprintf16__8VIStringPUsiPCcPv
```

Five of these (`__6VIFile`, `_$_6VIFile`, `Read__6VIFilePvi`, `Seek__6VIFilelQ26VIFile10SeekOrigin`,
`Tell__6VIFile`) were already in the plan15 unresolved list, so they are not newly opened.

## Build-plan entries needed

Append these inputs with fresh `input_order` values (profile `authored_cpp`, kind
`independently_authored_source`, empty `header_closure`; no unit has an `#include`). Do not add any
`host_check.cpp`. `default_authored_inputs` increases by 15. The table unit emits no function, so its
`emitted_functions` is empty; it defines one external read-only data symbol. No
`private_object_sha256` is claimed.

```json
{"source":"sources/w15-vi-strings-dict-a-camera-getmode/getmode.cpp","object_name":"w15_vi_strings_dict_a_camera_getmode.o","sha256":"d00a2098ad0084ea10f80e1af7df5abbe23fe8adad93ef2797991f1340dfba43","emitted_functions":[{"name":"GetMode__6Camera","compiled_body_bytes":8}]}
{"source":"sources/w15-vi-strings-dict-a-vistring-conversion-table-1252/table.cpp","object_name":"w15_vi_strings_dict_a_vistring_conversion_table_1252.o","sha256":"316c18ebfd093dd1081691016411cadb4587e4e16b666010d835ba1b555e42a9","emitted_functions":[]}
{"source":"sources/w15-vi-strings-dict-a-vistring-snprintf/snprintf.cpp","object_name":"w15_vi_strings_dict_a_vistring_snprintf.o","sha256":"1fd19f5db6f9ea1b83375f6aa225c1c23185594dca8711a0e1e4997453661268","emitted_functions":[{"name":"snprintf__8VIStringPciPCce","compiled_body_bytes":56}]}
{"source":"sources/w15-vi-strings-dict-a-vistring-snprintf16-narrow/snprintf16.cpp","object_name":"w15_vi_strings_dict_a_vistring_snprintf16_narrow.o","sha256":"7cfbdd7abcf18d5a4d512ccb28760704e9cb4248f0878a5a64cbae60a96b89da","emitted_functions":[{"name":"snprintf16__8VIStringPUsiPCce","compiled_body_bytes":56}]}
{"source":"sources/w15-vi-strings-dict-a-vistring-snprintf16-wide/snprintf16.cpp","object_name":"w15_vi_strings_dict_a_vistring_snprintf16_wide.o","sha256":"c97929a32aecfe72d7216f3bf6d47ad808eda28cac81d1d270d56806c40295b1","emitted_functions":[{"name":"snprintf16__8VIStringPUsiPCUse","compiled_body_bytes":56}]}
{"source":"sources/w15-vi-strings-dict-a-vistring-str8to16/str8to16.cpp","object_name":"w15_vi_strings_dict_a_vistring_str8to16.o","sha256":"1acd465384fff09803343f2520582c5569d7b4ce645aae7cb61f215b7be2cd6b","emitted_functions":[{"name":"str8to16__8VIStringPUsUiPCc","compiled_body_bytes":108}]}
{"source":"sources/w15-vi-strings-dict-a-vistring-strcat16-narrow/strcat16.cpp","object_name":"w15_vi_strings_dict_a_vistring_strcat16_narrow.o","sha256":"627da898f5b39efb59c0f1f52a3d4adfdf5e93c1c25ecd052ce35af3f0be626c","emitted_functions":[{"name":"strcat16__8VIStringPUsUiPCc","compiled_body_bytes":100}]}
{"source":"sources/w15-vi-strings-dict-a-vistring-strcat16-wide/strcat16.cpp","object_name":"w15_vi_strings_dict_a_vistring_strcat16_wide.o","sha256":"7c1898348df9ffb889fd1402c05df01b39a6d20a37c4b377946496b3af2fb11e","emitted_functions":[{"name":"strcat16__8VIStringPUsUiPCUs","compiled_body_bytes":100}]}
{"source":"sources/w15-vi-strings-dict-a-vistring-strcpy16/strcpy16.cpp","object_name":"w15_vi_strings_dict_a_vistring_strcpy16.o","sha256":"eb044be6dac89de82fd37566e6291e41aedcdf1182416e9387b52738c284380f","emitted_functions":[{"name":"strcpy16__8VIStringPUsUiPCUs","compiled_body_bytes":80}]}
{"source":"sources/w15-vi-strings-dict-a-vistring-strcpy8/strcpy8.cpp","object_name":"w15_vi_strings_dict_a_vistring_strcpy8.o","sha256":"e3fc0e8da6cd2104f94952750274e791e0b676e3bd72d362a6058bf19dc95b00","emitted_functions":[{"name":"strcpy8__8VIStringPcUiPCc","compiled_body_bytes":80}]}
{"source":"sources/w15-vi-strings-dict-a-vistring-strupr/strupr.cpp","object_name":"w15_vi_strings_dict_a_vistring_strupr.o","sha256":"23009d64b35bd4bff4cbce64c4b132b2da23e074a3fddc18b55fe69869e3d20b","emitted_functions":[{"name":"strupr__8VIStringPc","compiled_body_bytes":8}]}
{"source":"sources/w15-vi-strings-dict-a-vistringtable-fixuptextbuffer/fixup.cpp","object_name":"w15_vi_strings_dict_a_vistringtable_fixuptextbuffer.o","sha256":"958fd4601f7f0c39bc0859158577480d91611649a80782b7a3443dabc0ca252c","emitted_functions":[{"name":"FixupTextBuffer__13VIStringTable","compiled_body_bytes":524}]}
{"source":"sources/w15-vi-strings-dict-a-vistringtable-loadrawfile/loadrawfile.cpp","object_name":"w15_vi_strings_dict_a_vistringtable_loadrawfile.o","sha256":"5562dde01b11489773dc241d0837a89159c9e6d46d3b55ab01899dd970618db8","emitted_functions":[{"name":"LoadRawFile__13VIStringTablePCcRt7VIArray1Zc","compiled_body_bytes":472}]}
{"source":"sources/w15-vi-strings-dict-a-vistringtable-processrawfile/processrawfile.cpp","object_name":"w15_vi_strings_dict_a_vistringtable_processrawfile.o","sha256":"e575ab39370b11267218a196f02b51702db278169c52588acbb255558e18fa46","emitted_functions":[{"name":"ProcessRawFile__13VIStringTableRCt7VIArray1Zc","compiled_body_bytes":512}]}
{"source":"sources/w15-vi-strings-dict-a-viudpsocket-ctor/ctor.cpp","object_name":"w15_vi_strings_dict_a_viudpsocket_ctor.o","sha256":"28c45eb3b4d92a4609ba842044be62dcd711970f824af943519171f2ee62cb57","emitted_functions":[{"name":"__11VIUDPSocketi","compiled_body_bytes":76}]}
```

## Authored but never executed

All fifteen units are authored but have only run inside the private interpreter harness and the host
checks; none has run in a full game link or on hardware. In particular the three snprintf functions,
the UDP socket constructor (its callees are stubs), LoadRawFile (all VIFile calls are stubs) and the
whole text-loading chain are untested against the real VIFile, vsnprintf and UTF-8 routines.

## Unknown

- The meaning of the Open mode 33 and the Seek origin values 1 and 2 in LoadRawFile.
- The VIStringTable and VIUDPSocket object sizes, and what the int in the UDP socket constructor is.
- Whether `_8VIString$ConversionTable1252` is writable in the original, and why two entries
  deviate from the published mapping.
- The return type of the vsnprintf callees beyond the void return their mangled names show, and
  whether the snprintf functions' return value is meaningful (they return nothing of their own).
- Behaviour the callers expect when the 16-bit helpers receive a limit of 0 (they still write one
  terminator unit, as the original does).
