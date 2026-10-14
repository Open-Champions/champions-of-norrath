# Numeric and typeinfo tables

Package work is on `agents/runtime-tables`. It closes the libm leaves,
the player number-conversion views, the assert words, and the typeinfo
and list-head objects that the partial link still imports. Takeover
note: the previous worker left no commits and no notes file on this
branch; the work below was done fresh in this worktree, which was clean
at takeover.

Private reference is the hash-pinned `SLUS_205.65` executable
(SHA-256 `9a60f13c...`); all addresses and byte values below were read
from it locally with the repo's own inventory tooling plus targeted
byte/decoder inspection. No game binary, disc image, or disassembly
listing is committed. Each unit compiles with the reviewed EE g++
`authored_cpp` flags; each has a host test beside its source that is not
a plan input. Host tests build with the host C++ compiler; three of them
(the two vtable checks and the rtti_user check) link the implementation
compiled for the host, the rest include it.

A private copy of the plan with these 23 units appended (kept outside
git under `work/`) compiled all 934 selected inputs and linked one
relocatable object: 775 function bodies (up from 768), 923 unresolved
imports (down from 947). `python3 -m unittest discover -s tools -p
'test_*.py'` passes (214 tests).

## Evidence

Observed, inferred, and unknown are separated in each source file; short
form per unit:

- `icos__Fi` at `0x15a070` (144 bytes) sign-extends the argument from 16
  bits, negates and wraps to 15 bits in the far half, scales by the
  `.lit4` coefficient, evaluates `x - cubic*x^3 + quintic*x^5 -
  septic*x^7`, and returns `sign * (1 - 2*sine^2)`. The compiled float
  sequence was checked op-for-op against the image.
- `isin__Fi` at `0x15a100` (148 bytes) is the same body with the argument
  pre-shifted by -16384, i.e. sine as shifted cosine.
- `powf` at `0x2f16d0` (20 bytes) is prologue words around a jump to
  `__ieee754_powf` at `0x2f4f80` (2016 bytes). It forwards to the already
  defined `norrath_math_ieee754_powf`.
- `__eq__C9type_infoRC9type_info` at `0x2fd2e0` (64 bytes) returns 1 for
  identical pointers, else compares the two name words with `strcmp`
  (`0x3044fc`).
- `norrath_rtti_user` is `__rtti_user` at `0x2fd368` (28 bytes): null
  stores nothing, else word 0 gets the name and word 1 gets `0x4537e0`
  (`_vt$16__user_type_info`). This unit stores the address of the
  separately authored user vtable view instead of the absolute address.
- `norrath_player_proxy_mesh_id` is
  `getProxyMeshId__4itemPQ24item11description` at `0x108498` (68 bytes).
  The item pointer is never read. Bit `0x100000` of the 64-bit word at
  description+8 selects adding its low five bits to the signed halfword
  at +52 of the record word 0 points to.
- `norrath_factory_sscanf` is the original `sscanf` at `0x303f50`,
  confirmed by the `jal` at `0x268a9c` in `specialparse__FPPciiG6Point3Pc`.
  Same read-only stream setup as the existing `sscanf` and
  `norrath_player_sscanf` bodies; third name for one original.
- Legacy vtables `_vt$14__si_type_info` at `0x4537b0` and
  `_vt$16__user_type_info` at `0x4537e0` (48 bytes each): six
  `{0, function}` slots ending null. Slot targets were identified by
  address lookup (`__tf`, `_$_`, `do_upcast`, `do_dyncast`,
  `do_find_public_subobj` of each class).
- Prop vtable `_vt$4Prop` at `0x44d018` (144 bytes): eighteen `{0,
  function}` slots ending null. Every function word was identified by
  address lookup; all seventeen names were already unresolved imports.
- `tens` at `0x134d120` (184 bytes, 1e0..1e22), `bigtens` at `0x134d1d8`
  (40 bytes), `tinytens` at `0x134d200` (40 bytes), all in `.srs`. The
  large/tiny tables carry historical one-ulp-below rounding on several
  entries; every entry is pinned by exact bits.
- `p05.0` at `0x47f4e0` (12 bytes): words 5, 25, 125.
- Half/quarter bounds at `0x47ed48/50/58` (8 bytes each): 0.4999999,
  0.5000001, 0.24999995, immediately before the formatter cluster at
  `0x47ed60`. Overflow is `__infinity` at `0x47f608`.
- `basefix.0` at `0x47ef70` (34 bytes): halfwords 10, 1..16. Exponent
  format `"e%ld"` at `0x47ef98` (5 bytes, the only occurrence in
  `.rodata`).
- Assert format at `0x4635e8` (49 bytes):
  `Assertion "%s" failed in %s:%d  context:%s (%x)\n`. Context word is
  `context` at `0x4840f0` (4 bytes), imaging `0x004840e8` (`"none"`).
- Heap pointer word: no ELF symbol; a 4-byte zero-initialized word, kept
  separate from `heap_ptr.39` (see Unknown).
- Services float at `0x48068c` in `.lit4` (4 bytes): bits `0x3ecccccc`,
  one ulp below 0.4.
- `fsList` at `0x48690c`, `arList` at `0x486914`, `arListBack` at
  `0x486918`: one nobits word each in `.sbss` (`fsListBack` sits between
  but is not this package). `wigglyTab` at `0x6ffe60` (544),
  `descListMark` at `0x780280` (128), `identityResult` at `0x6ca1c0`
  (1600), `stack.15` at `0x8ddcc0` (1024), `retval` at `0x8e8100` (64):
  nobits extents from next-symbol gaps, zero-initialized.
- `srcfile` at `0x445200` (1856 bytes) and `eenull` at `0x445940` (40
  bytes) in `.data`: copied verbatim as image bytes.

Two `const` linkage notes for future units: a `const` array inside an
`extern "C" { }` block keeps C++ internal linkage and the compiler
discards it; spell it `extern "C" const unsigned char name[N] = ...`
so the symbol is emitted (found via the tens unit).

## Imports closed

34 names: `__eq__C9type_infoRC9type_info`, `arList`, `arListBack`,
`descListMark`, `eenull`, `fsList`, `icos__Fi`, `identityResult`,
`isin__Fi`, `norrath_assert_context_word_view`,
`norrath_assert_format_view`, `norrath_factory_prop_vtable`,
`norrath_factory_sscanf`,
`norrath_legacy_single_inheritance_type_info_vtable_view`,
`norrath_legacy_user_type_info_vtable_view`,
`norrath_player_bigint_small_powers5_view`,
`norrath_player_heap_pointer_word`,
`norrath_player_number_big_tens_view`,
`norrath_player_number_half_lower_view`,
`norrath_player_number_half_upper_view`,
`norrath_player_number_overflow_view`,
`norrath_player_number_quarter_lower_view`,
`norrath_player_number_tens_view`,
`norrath_player_number_tiny_tens_view`,
`norrath_player_proxy_mesh_id`,
`norrath_player_scan_base_normalization_view`,
`norrath_player_scan_exponent_format_view`,
`norrath_player_services_view_48068c`, `norrath_rtti_user`, `powf`,
`retval`, `srcfile`, `stack.15`, `wigglyTab`.

`floorf` and `ceilf` stay unresolved by design: they are the plan's two
optional Newlib external inputs, and the math package's
`norrath_entity_floorf` is a separate symbol.

## Imports opened

10 names, all legacy-RTTI slot functions referenced by the two vtable
views, none previously in the link:

- `__tf14__si_type_info`, `_$_14__si_type_info`,
  `do_upcast__C14__si_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result`,
  `do_dyncast__C14__si_type_infoiQ216__user_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result`,
  `do_find_public_subobj__C14__si_type_infoiRC9type_infoPvT3`
- `__tf16__user_type_info`, `_$_16__user_type_info`,
  `do_upcast__C16__user_type_infoQ216__user_type_info8sub_kindRC9type_infoPvRQ216__user_type_info13upcast_result`,
  `do_dyncast__C16__user_type_infoiQ216__user_type_info8sub_kindRC9type_infoPvT3T4RQ216__user_type_info14dyncast_result`,
  `do_find_public_subobj__C16__user_type_infoiRC9type_infoPvT3`

These bodies belong to future typeinfo work; nothing calls through the
tables in the partial link. The Prop vtable's seventeen slot references
were all already unresolved, so that unit opens nothing.

## Build-plan entries needed

Append these authored inputs with fresh `input_order` values (the
private trial used 913 through 935 against this tip, whose plan has 913
inputs). `default_authored_inputs` increases by 23. `header_closure` is
empty for every unit. `profile` is `authored_cpp`. Do not add any
`host_test.cpp` file: the include-style ones pull in host headers and
the link-style ones are separate host programs. Body sizes are from the
reviewed EE g++ `authored_cpp` profile, not from the original
executable. `emitted_functions` belongs in the snapshot input, not in
the plan object; listed here so the integrator can copy it.

```json
{"source": "sources/runtime-tables-icos/icos.cpp", "object_name": "runtime_tables_icos.o", "sha256": "6988f7d1e3515f5302ddb04ff2cc28279f5d2de14f847e0e08d92198b578277f", "input_order": 913, "emitted_functions": [{"name": "icos__Fi", "compiled_body_bytes": 172}]}
{"source": "sources/runtime-tables-isin/isin.cpp", "object_name": "runtime_tables_isin.o", "sha256": "79d4e0841a7bea2326c4f0fbbe686a8ac783ff91b74a94faabcaf9c55b287954", "input_order": 914, "emitted_functions": [{"name": "isin__Fi", "compiled_body_bytes": 172}]}
{"source": "sources/runtime-tables-powf/powf.cpp", "object_name": "runtime_tables_powf.o", "sha256": "d0363cf6bd3fbdbe4e65a52fcb356cda72f7b1ce855de15c760044d28fafea65", "input_order": 915, "emitted_functions": [{"name": "powf", "compiled_body_bytes": 8}]}
{"source": "sources/runtime-tables-typeinfo-eq/typeinfo_eq.cpp", "object_name": "runtime_tables_typeinfo_eq.o", "sha256": "c99b8c0ed15113e290aa79865645cdc68e3ba1b415f1fb975155fb7f830f2c0c", "input_order": 916, "emitted_functions": [{"name": "__eq__C9type_infoRC9type_info", "compiled_body_bytes": 56}]}
{"source": "sources/runtime-tables-rtti-user/rtti_user.cpp", "object_name": "runtime_tables_rtti_user.o", "sha256": "af9f50a498e1d0f46bcc6d552258e54054309a2e9f53932530aa9fe656343704", "input_order": 917, "emitted_functions": [{"name": "norrath_rtti_user", "compiled_body_bytes": 80}]}
{"source": "sources/runtime-tables-proxy-mesh/proxy_mesh.cpp", "object_name": "runtime_tables_proxy_mesh.o", "sha256": "440874dd67d35736b0a7cc43d072621fffc743439ea596efe3731ce2f879ec48", "input_order": 918, "emitted_functions": [{"name": "norrath_player_proxy_mesh_id", "compiled_body_bytes": 84}]}
{"source": "sources/runtime-tables-factory-sscanf/factory_sscanf.cpp", "object_name": "runtime_tables_factory_sscanf.o", "sha256": "482ecfce7b7c8723e52df5480b9906da985bb34e30751d0f7d6c93fed962fb74", "input_order": 919, "emitted_functions": [{"name": "norrath_factory_sscanf", "compiled_body_bytes": 344}]}
{"source": "sources/runtime-tables-legacy-vtables/legacy_vtables.cpp", "object_name": "runtime_tables_legacy_vtables.o", "sha256": "81f16137572840fd2358cf38d9deffb6a0e7bcbb67aa61ff011e81ad0a29b159", "input_order": 920}
{"source": "sources/runtime-tables-prop-vtable/prop_vtable.cpp", "object_name": "runtime_tables_prop_vtable.o", "sha256": "37a2467e812eb4ae23e8ee998950cff15ee518299d3c5f33cf00679ee60c48ad", "input_order": 921}
{"source": "sources/runtime-tables-number-tens/number_tens.cpp", "object_name": "runtime_tables_number_tens.o", "sha256": "8754080cdc08bf0ee7b913593f608c62f0a8c577e0a9107c9e95f42ce20bac5e", "input_order": 922}
{"source": "sources/runtime-tables-number-refine/number_refine.cpp", "object_name": "runtime_tables_number_refine.o", "sha256": "e333d0469648ff9f7f9d5b1a15080409ef9315e15471f9649788eb169bfef663", "input_order": 923}
{"source": "sources/runtime-tables-bigint-powers5/bigint_powers5.cpp", "object_name": "runtime_tables_bigint_powers5.o", "sha256": "048b21eb5a1e424d1f81e261cf4fe11ad83234eb382be69eb5e2074d3345463e", "input_order": 924}
{"source": "sources/runtime-tables-scan-views/scan_views.cpp", "object_name": "runtime_tables_scan_views.o", "sha256": "230bf228e19d86333bd6897d2dbc0c8d5d6c60ff6578e635ef0ab5c9cac9b8ad", "input_order": 925}
{"source": "sources/runtime-tables-assert-views/assert_views.cpp", "object_name": "runtime_tables_assert_views.o", "sha256": "7adc1a93ee871abdefa5c79d44d5c680ad9a466dc58c5d21f32fd7018673b528", "input_order": 926}
{"source": "sources/runtime-tables-heap-word/heap_word.cpp", "object_name": "runtime_tables_heap_word.o", "sha256": "478a3807bebe5c6d915f3c9d76736a2ed7b74eef1e4f70e3a3b7136d415a8cd0", "input_order": 927}
{"source": "sources/runtime-tables-services-word/services_word.cpp", "object_name": "runtime_tables_services_word.o", "sha256": "fb942e6cbc56cc51e4926e1031a2ee50ed2dfd7b54ec85542399b4c8678cf525", "input_order": 928}
{"source": "sources/runtime-tables-anim-lists/anim_lists.cpp", "object_name": "runtime_tables_anim_lists.o", "sha256": "7f8edb141ca13f724423c7dc37f14145985859f3e972ac8b158988ee1b0dae80", "input_order": 929}
{"source": "sources/runtime-tables-particle-storage/particle_storage.cpp", "object_name": "runtime_tables_particle_storage.o", "sha256": "d742a26d9f067f9989d120f273660b4c5df29031fbac33494825e9ec72d17c63", "input_order": 930}
{"source": "sources/runtime-tables-anim-identity/anim_identity.cpp", "object_name": "runtime_tables_anim_identity.o", "sha256": "ed2ff080b95df543911ca2b95bc40e55d2bc9eb7e78c4322544aba44f09dda7f", "input_order": 931}
{"source": "sources/runtime-tables-thread-stack/thread_stack.cpp", "object_name": "runtime_tables_thread_stack.o", "sha256": "3f751d259a7c2df41881c19d89ca3e7a628dab1a16c94753d3b036785f1a7325", "input_order": 932}
{"source": "sources/runtime-tables-sce-retval/sce_retval.cpp", "object_name": "runtime_tables_sce_retval.o", "sha256": "75481acd4cc9b64b2c94e7d31bf9ddb70e05fb54b21b66138a277ec2b9185e2d", "input_order": 933}
{"source": "sources/runtime-tables-srcfile/srcfile.cpp", "object_name": "runtime_tables_srcfile.o", "sha256": "3f636ff78ac08b7f0066c1108fc2141706f1627bc93d978206c3c88c277905ce", "input_order": 934}
{"source": "sources/runtime-tables-eenull/eenull.cpp", "object_name": "runtime_tables_eenull.o", "sha256": "5fd0665a6e733d3f378219e0c5b5c912abfde8385617e695d2a83f97b87250e8", "input_order": 935}
```

## Authored but never executed

All seven functions and every view in this package. Host comparisons
ran on this Mac; the reviewed EE compiler produced relocatable objects
and the private 934-input link only checked symbol closure. No routine
was executed on the EE, under the original process, or against gameplay.
The vtable slots are never called through. The `srcfile`/`eenull`
images are copied as data, never run.

## Unknown

- `floorf`/`ceilf` remain the plan's optional Newlib inputs by design;
  the upstream C files supply them, not this package.
- Whether `norrath_factory_sscanf` should become one symbol with the
  already defined `sscanf`/`norrath_player_sscanf` (same original
  `0x303f50`). Likewise `norrath_factory_prop_vtable` with items-player's
  `norrath_player_view_44d018` (same `_vt$4Prop`), and
  `norrath_assert_context_word_view` with the original `context` name.
- Whether `norrath_player_heap_pointer_word` is the same word as
  `heap_ptr.39` at `0x444954`. This package deliberately does not alias
  them.
- The heap word's original section/address and its pre-first-break
  value; zero-init is inferred from the nobits convention.
- `norrath_rtti_si` (observed `__rtti_si` at `0x2fd348`) is still
  unresolved; it was not in this package's name list and is left for
  future typeinfo work, as are the ten opened slot bodies.
- The historical one-ulp rounding of `bigtens[1..4]`,
  `tinytens[1]/[3]`, and the services float is observed but unexplained.
- `fsListBack` (between `fsList` and `arList`) is not this package and
  stays unresolved for whoever owns the animation lists.
- These definitions close imports. They do not place objects at the
  original addresses and do not claim byte-identical code.
