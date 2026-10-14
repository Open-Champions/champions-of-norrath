# w16-rt-a: startup runtime, unwinder, libm and libc helpers

Branch `agents/w16-rt-a`. New source is under `experimental/sources/w16-rt-a-*`. Every function is its own commit and has a
committed host check in its directory (`host_check.cpp`; not a plan input). The private reference executable was used only
to read behaviour; nothing derived from it is in this branch. This package did not edit `experimental/build-plan.json`,
`experimental/snapshot.json`, any receipt, `README.md`, `docs/ROADMAP.md`, `docs/STARTUP*.md` or anything under `tools/`.

## How each function was checked

* Host check: a native test in the unit's directory with the build command at the top. It checks the authored function
  against an independent model or the host C library (`strupr`, `copysign`, `__muldi3`, `__ieee754_expf` within 3 ulp, the
  unwinder pieces against a model of each opcode/record layout) and, for the functions that hold 32-bit address words
  (`find_fde`, `execute_cfa_insn`, the string representation copy), runs over a host arena with `-DW16_HOST_CHECK`.
  `SetAlarm` is assembly: its check compares the assembled 16-byte body with the image bytes, given the private executable
  path as an argument.
* Paired run (private, not reproducible from this repository): the original function bytes and the authored function run
  on a small private R5900 interpreter from identical memory snapshots, with identical Python stand-ins for the callees
  (arguments, order and results logged and compared) and every changed byte outside the stack compared. Instruction-word
  coverage and mutation results are in each commit message. A paired run against the string-representation copy first used a
  chunk-block area inside the ignored stack range; that was caught by a surviving mutation and the area was moved, so the
  committed result is from the corrected run.
* Nothing in this package was executed on a PlayStation 2 or in the game.

## Observed

All addresses are in the pinned `SLUS_205.65`.

* `strupr` 0x003068c8, 72 bytes: walks to the zero byte; a byte whose class-table word (view index byte + 128) has bit 1 set
  has 32 subtracted. Returns its argument. Compiled 72 bytes.
* `strdup` 0x00304758, 32 bytes: loads the word at `_impure_ptr` and tail-calls `_strdup_r`. Compiled 16.
* `_strdup_r` 0x00304778, 108 bytes (opened by `strdup`, so also authored): `strlen`, `_malloc_r(reent, length + 1)`, `memcpy`
  only when the block is not null, returns the block. Compiled 104.
* `SetAlarm` 0x002d4e60, 16 bytes: kernel service 252, return. Byte-identical when assembled.
* `copysign` 0x002f8138, 68 bytes: a double is one 64-bit value in an integer register; low word of the first argument kept,
  high word = first high word with the sign cleared | second high word's sign bit. Compiled 44.
* `__muldi3` 0x002f81c0, 112 bytes: low 64 bits of the product from 32-bit pieces. The original also leaves values in HI/LO;
  only the returned value is authored. Compiled 84.
* `what__C9exception` 0x002ff468, 64 bytes: null object calls `__throw_bad_typeid` and the code after it reads a word
  through the call's return register (the same read as the normal path); otherwise the object's table slot at offset 4 is
  called with the object and the first word of the returned record is returned. Compiled 72.
* `__dynamic_cast_2` 0x002fe7a8, 140 bytes: calls its first, second and fifth arguments (no arguments) in that order and
  tail-calls `dyncast__C16__user_type_infoiRC9type_infoPvT2T3` with (r1, third argument, r2, fourth, r5, sixth). Compiled
  100.
* `__ieee754_expf` 0x002f4b58, 596 bytes: fdlibm-style single-precision exp. Signed bit pattern above 0x42b2d4fc returns
  0x7f7fffff (also for NaN and +infinity); negative with magnitude above 0x42aeac50 returns 0; reduction above 0x3eb17218;
  magnitude up to 0x317fffff returns 1 + x. Constants are the image's bit patterns. Compiled 880.
* `find_fde` 0x002fc8c8, 424 bytes; `extract_cie_info` 0x002fca70, 268 bytes; `execute_cfa_insn` 0x002fcca0, 1100 bytes: the
  exception runtime pieces; behaviour and record layouts are in the source comments. `execute_cfa_insn` has an arm (the test
  for 0xc0 after 0x40 and 0x80 both failed) that cannot be reached, so the opcodes 0xc0..0xff take the 0x40 arm.
* `_$_t6VIList1ZP10VIFlatFile` 0x011f3b70, 84 bytes: `Clear__t6VIList1ZP10VIFlatFile`, then `__builtin_delete` when bit 0 of the
  flags is set.
* `clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep` 0x011f6cc8: the symbol size is
  284 bytes but the code runs to 0x011f6eb4 (492 bytes) with the small-block allocator inlined.

## Inferred

* Field names of the frame-state record, the common-information record and the string representation, and the DWARF opcode
  names implied by `execute_cfa_insn`'s numeric dispatch: from use only, nothing in the image names them.
* The shape of `what__C9exception` is a type-name query on the dynamic type; the table slot's own name is not established.
* The flag bit in the VIList destructor and the integer in `__dynamic_cast_2` follow the compiler's usual conventions; the
  image only shows how the bit/value is used.

## Unknown

* `__ieee754_expf` was compared bit-for-bit on the private interpreter, which rounds to nearest; the console's float unit
  differs (no infinities or denormals). The saturating returns are the observed constants, but nothing about the original's
  EE float behaviour beyond that was tested.
* `frame_init`, `search_fdes`, `decode_sleb128`, `decode_stack_op`, `_S_chunk_alloc...`, the stream routines used by the
  out-of-memory path and `Clear__t6VIList1ZP10VIFlatFile` bodies are not authored here.
* What kernel service 252 does with its arguments; whether `__throw_bad_typeid` ever returns.
* Whether any caller depends on `__muldi3`'s HI/LO side effects.

## Imports closed (names)

`SetAlarm`, `_$_t6VIList1ZP10VIFlatFile`, `__dynamic_cast_2`, `__ieee754_expf`, `__muldi3`,
`clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep`, `copysign`, `execute_cfa_insn`,
`extract_cie_info`, `find_fde`, `strdup`, `strupr`, `what__C9exception`. All 13 names of the package scope. (`_strdup_r` is also
authored; it was not on the unresolved list because `strdup` was the only caller.) Method: each unit was compiled alone with the reviewed
flags and its defined and undefined symbols were compared with `unresolved-imports.json` and the symbol table of the 7327-unit
`plan16-build` link; no full partial build was run for this branch.

## Imports opened

These names are referenced by the new units and are not defined by anything in the current link. Callers of the closed names
(`strdup` through `__wrap_strdup`, `find_fde` through `__frame_state_for`, ...) already existed, so only the callees below are new:
`Clear__t6VIList1ZP10VIFlatFile`, `_S_chunk_alloc__t24__default_alloc_template2b0i0UiRi`, `__ls__7ostreamPCc`,
`_t23__malloc_alloc_template1i0$__malloc_alloc_oom_handler`, `cerr`, `decode_sleb128`, `decode_stack_op`,
`dyncast__C16__user_type_infoiRC9type_infoPvT2T3`, `endl__FR7ostream`, `frame_init`, `search_fdes`.
`objects`, `__sce_eh_sema_id`, `WaitSema`, `SignalSema`, `_malloc_r`, `memcpy`, `strlen`, `strcmp`, `decode_uleb128`,
`__wrap_malloc`, `__wrap_free`, `exit`, `abort`, `__throw_bad_typeid`, `__builtin_delete` and `norrath_factory_ctype_minimum_view`
resolve against the current link. The chunk-allocator and out-of-memory symbols may belong to other packages (the overlay string
library, the stream library); they are listed here rather than guessed.

## Build-plan entries needed

Not added to the shared plan. `input_order` values 16000..16013 are suggestions (the plan's highest is 7328); renumber freely.
Also add each source path with its sha256 to the plan `files` map, bump `default_authored_inputs` by 14, and add the sources to
`experimental/snapshot.json` (the plan validator requires both to agree). Profile `authored_cpp` except `SetAlarm`
(`authored_entry_asm`). Do NOT add any `host_check.cpp`.

```
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "strupr"}], "header_closure": [], "input_order": 16000, "kind": "independently_authored_source", "object_name": "w16_rt_a_strupr.o", "profile": "authored_cpp", "sha256": "239e8b2db8fa9eb315a1ef99b2ca857f1e8d5c2bbe6be919428604ed71e80345", "source": "sources/w16-rt-a-strupr/strupr.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "strdup"}], "header_closure": [], "input_order": 16001, "kind": "independently_authored_source", "object_name": "w16_rt_a_strdup.o", "profile": "authored_cpp", "sha256": "2df759e1a5433b3ae4bff5682c7c22f8b8edcf893a1de1efb597e57b073a7541", "source": "sources/w16-rt-a-strdup/strdup.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 104, "name": "_strdup_r"}], "header_closure": [], "input_order": 16002, "kind": "independently_authored_source", "object_name": "w16_rt_a_strdup_r.o", "profile": "authored_cpp", "sha256": "ec1bd67fc2575e3ff9c7822fe0deeed60303d161741a3a8597c316bb477330a0", "source": "sources/w16-rt-a-strdup-r/strdup_r.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 16, "name": "SetAlarm"}], "header_closure": [], "input_order": 16003, "kind": "independently_authored_source", "object_name": "w16_rt_a_set_alarm.o", "profile": "authored_entry_asm", "sha256": "e1136bc37caa7fdedfe0b2fe9d22fe164dc8885be87d1b4856ba05b93a8c96b6", "source": "sources/w16-rt-a-set-alarm/set_alarm.S"}
{"emitted_functions": [{"compiled_body_bytes": 44, "name": "copysign"}], "header_closure": [], "input_order": 16004, "kind": "independently_authored_source", "object_name": "w16_rt_a_copysign.o", "profile": "authored_cpp", "sha256": "bafa3ca275c825818a16f7bee49525806ea97fcf055b7d1f4cb9d07b434d7791", "source": "sources/w16-rt-a-copysign/copysign.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 84, "name": "__muldi3"}], "header_closure": [], "input_order": 16005, "kind": "independently_authored_source", "object_name": "w16_rt_a_muldi3.o", "profile": "authored_cpp", "sha256": "3fc84f84423ca367e94f6897a9c862868f22051db04cb926e7d16d14e46146f1", "source": "sources/w16-rt-a-muldi3/muldi3.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 72, "name": "what__C9exception"}], "header_closure": [], "input_order": 16006, "kind": "independently_authored_source", "object_name": "w16_rt_a_exception_what.o", "profile": "authored_cpp", "sha256": "c542eb1dfacd93755c3b9070575f0d312ba8f2bd0754476a3717d71999cad3af", "source": "sources/w16-rt-a-exception-what/exception_what.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 100, "name": "__dynamic_cast_2"}], "header_closure": [], "input_order": 16007, "kind": "independently_authored_source", "object_name": "w16_rt_a_dynamic_cast_2.o", "profile": "authored_cpp", "sha256": "f0da4d0e37530bb1c88c8b66986ce8f35498e507d140cf0149fe2dd405bcadec", "source": "sources/w16-rt-a-dynamic-cast-2/dynamic_cast_2.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 880, "name": "__ieee754_expf"}], "header_closure": [], "input_order": 16008, "kind": "independently_authored_source", "object_name": "w16_rt_a_ieee754_expf.o", "profile": "authored_cpp", "sha256": "77d8ab344bc49026339bd7b2c15c98e1eac8b41d2b026954b2da9c0d4f53a44b", "source": "sources/w16-rt-a-ieee754-expf/ieee754_expf.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 484, "name": "find_fde"}], "header_closure": [], "input_order": 16009, "kind": "independently_authored_source", "object_name": "w16_rt_a_find_fde.o", "profile": "authored_cpp", "sha256": "7276a45bdf7a125fc09d7169459a6dc1d0336c1694367236ec9bd4d4d73f5e81", "source": "sources/w16-rt-a-find-fde/find_fde.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 300, "name": "extract_cie_info"}], "header_closure": [], "input_order": 16010, "kind": "independently_authored_source", "object_name": "w16_rt_a_extract_cie_info.o", "profile": "authored_cpp", "sha256": "93c6082910db1c401fc2f513e5b53798fe6e52fc21758098d7399da0f63aca4e", "source": "sources/w16-rt-a-extract-cie-info/extract_cie_info.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 1096, "name": "execute_cfa_insn"}], "header_closure": [], "input_order": 16011, "kind": "independently_authored_source", "object_name": "w16_rt_a_execute_cfa_insn.o", "profile": "authored_cpp", "sha256": "8f9dac3675337df27249626c5afcb5e979c56d13f8852e24b1b0784eb488f860", "source": "sources/w16-rt-a-execute-cfa-insn/execute_cfa_insn.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 76, "name": "_$_t6VIList1ZP10VIFlatFile"}], "header_closure": [], "input_order": 16012, "kind": "independently_authored_source", "object_name": "w16_rt_a_vilist_dtor_p10viflatfile.o", "profile": "authored_cpp", "sha256": "3ffc3be6031926762663894e55cdd468d325ef29d1a57b9ef584d67ccf8ae18f", "source": "sources/w16-rt-a-vilist-dtor-p10viflatfile/dtor.cpp"}
{"emitted_functions": [{"compiled_body_bytes": 592, "name": "clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep"}], "header_closure": [], "input_order": 16013, "kind": "independently_authored_source", "object_name": "w16_rt_a_string_rep_clone.o", "profile": "authored_cpp", "sha256": "479552fbb543d1803013b65e1476b6de232904d7147d5b16c3c0d78331356dbf", "source": "sources/w16-rt-a-string-rep-clone/rep_clone.cpp"}
```

Duplicates to avoid when merging: these units define `execute_cfa_insn`, `extract_cie_info`, `find_fde`, `__dynamic_cast_2` and
the string representation copy, which also exist as sources (without committed host checks, so unplanned) in
`w15-libs-eh-a-*` and `w13-startup-fn-a-dynamic-cast-2`; select only one definition of each. The w15 and w13 versions were read
as references; the w16 versions were re-verified against the image and carry their own self-contained headers.

## Authored but never executed

Every function above. The paired runs use a private interpreter, not the console. `SetAlarm` has only a byte comparison.
The `W16_HOST_CHECK` arena helpers in `find_fde`, `execute_cfa_insn` and the string copy are inactive in the plan build.

## Corrections to commit messages

* The `execute_cfa_insn` commit says "25 mutations: 23 caught, 1 equivalent, 1 that did not apply". The actual run was 25
  patterns: 22 caught (differences reported), 1 equivalent (a post-increment split), and 2 whose multi-line patterns did not
  apply or errored. A Python-visible difference was seen for every pattern that applied except the equivalent one.
* The `_strdup_r` commit reports 8 scenarios; the run used 4 scenarios in each of two allocation modes (success, failure).
* The `__ieee754_expf` commit says 8 of 12 mutations were caught; the survivors listed there are equivalent in effect for the
  scenario set, and the host check bounds the result to 3 ulp of the host `expf` (worst 1).
