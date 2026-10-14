# w15-libs-eh-a: C runtime math, GSM codec, exception unwinding, inflate and string support

Branch `agents/w15-libs-eh-a`. New source is under `experimental/sources/w15-libs-eh-a-*`. Every function has its own commit. The
private reference executable was used only to read behaviour; nothing derived from it is in this branch. This package
did not edit `experimental/build-plan.json`, `experimental/snapshot.json`, any receipt, `README.md`, `docs/ROADMAP.md`,
`docs/STARTUP*.md` or anything under `tools/`.

## How each function was checked

Each function is a complete authored body, compiled with the reviewed `authored_cpp` profile. Verification was a private
paired run against the original: the original and the authored function start from identical memory snapshots, identical
Python stand-ins replace the callees (call arguments, order and results are logged and compared), and every changed byte
outside the stack is compared. Where a function reads a value that the original keeps in a register, the stand-in writes
a controlled value. Coverage of the original's instruction words is recorded in the commit message of each function; the
uncovered words are branch-delay nops, annulled slots, or arms the code cannot reach (stated per function). Mutations of
constants, shifts, comparisons and ordering were run against each authored body and each was caught, except those
listed as equivalent in the commit message. Where a mutation made a loop run forever, the step limit or time limit ended
the run and counted as a failure. Host tests (`*_host_test.cpp`, not plan inputs) exist for the older simple functions
and the filter stages; the larger routines rely on the paired runs, which are private and not reproducible from this
repository.

Nothing in this package was executed on a PlayStation 2 or in the game. "Paired run" means the original function bytes
and the authored function ran on a small private interpreter.

## Observed

- GSM 06.10 codec (16-bit samples held as 64-bit words, state record of 2552 bytes, see `w15-libs-eh-a-gsm-common/gsm.hpp`):
  `Gsm_LPC_Analysis` 0x002ed940, `Gsm_Long_Term_Predictor` 0x002ef410, `Gsm_Long_Term_Synthesis_Filtering` 0x002ef4b8,
  `Gsm_RPE_Encoding` 0x002ee0f0, `Gsm_RPE_Decoding` 0x002ee3f0, `Gsm_Short_Term_Analysis_Filter` 0x002f00c0,
  `Gsm_Short_Term_Synthesis_Filter` 0x002f0480, with their local helpers `Autocorrelation` 0x002ecee0,
  `Reflection_coefficients` 0x002ed4a0, `Quantization_and_coding` 0x002ed700,
  `Decoding_of_the_coded_Log_Area_Ratios` 0x002ef5c8, `Short_term_analysis_filtering` 0x002ef8e0,
  `Short_term_synthesis_filtering` 0x002efce0, `gsm_norm` 0x002ed9e0 and the tables `gsm_FAC`, `gsm_NRFAC`, `gsm_H`, `gsm_QLB`,
  `gsm_DLB`, `bitoff`. The weighting filter in `Gsm_RPE_Encoding` uses the coefficients -128, -256, 0, 2048, 5120, 8192,
  5120, 2048, 0, -256, -128 formed from shifts; they are not the published 06.10 values.
- Exception runtime (a gcc 2.95-style unwinder over registered object records): `find_exception_handler`,
  `old_find_exception_handler`, `next_stack_level`, `get_reg_addr`, `copy_reg`, `instmap_bounds`, `decode_uleb128`,
  `dwarf_reg_size_table`, `__unwinding_cleanup`, `find_fde` 0x002fc8c8, `extract_cie_info` 0x002fca70 and
  `execute_cfa_insn` 0x002fcca0. The frame-state record is 1144 bytes (`w15-libs-eh-a-common/eh.hpp`), extended by one chain
  word to 1152 bytes by the remember/restore-state opcodes. The interpreter's third test (`insn & 0xc0` after the 0x40
  and 0x80 tests have failed) can never succeed, so the arm that would clear a saved marker is unreachable; the 0x40 test
  also catches opcodes 0xc0..0xff.
- Decompressor (layouts in `w15-libs-eh-a-zlib-common/inflate.hpp`): `inflateInit2_` 0x010a2d60, `inflate_blocks_reset`
  0x01097f60, `inflate_blocks_free` 0x01098010, `inflate_blocks` 0x01097360. The image's `inflate_blocks_free` inlines
  the reset sequence. `inflate_blocks` frees the temporary length table only when a table-building step returns -3, and
  does not free it when the later codes allocation fails.
- Strings: `clone__Q2t12basic_string...Rep` at 0x011f6cc8. The symbol table gives 284 bytes, but the code runs to
  0x011f6eb4 (492 bytes) with the small-block allocator inlined. `_t12basic_string...$nilRep` (0x011f6cb8, words 0, 0, 1, 0)
  and `_t24__default_alloc_template2b0i0$_S_free_list` (0x011f2910, 64 zero bytes) live in the overlay image.
- Math and strings support: `floor` 0x002f0b98, `expf` 0x002f16a0 (a short wrapper around `__ieee754_expf` at 0x002f4b58),
  `__ieee754_sqrt` 0x002f4230, `scalbn` 0x002f7e88, `__wrap_strdup` 0x00144af0, `__unwinding_cleanup` 0x002facd8 (an 8-byte
  routine that does nothing).

## Inferred

- Names of every field in the shared layouts (`FrameState`, `BlockState`, `ZStream`, the GSM state record, the string
  representation) come from how the observed code uses the bytes. Nothing in the image names them.
- `find_fde` records are the registered exception-table descriptors; the sorted array is ordered by start address.
- `execute_cfa_insn` opcode names follow the numeric dispatch; only the numbers and actions are observed.
- The string representation header is length, capacity, reference count, shared flag.

## Corrections to earlier commit messages

- `Autocorrelation` (87918608): the message says 62 paired scenarios and lists sine-plus-noise inputs. The run had 61 (sixty
  generated from five input kinds plus one all-zero case). The kinds were full-range noise, noise within +-100, a sparse
  signal, a choice among 32767, -32768, 0, 1 and -1, and noise with a random power-of-two range. The result (zero
  differences, 366 of 367 words) stands.
- `Decoding_of_the_coded_Log_Area_Ratios` (609101e2): the message gives the address as 0x002ef9a0-area. The routine is at
  0x002ef5c8 (788 bytes); its source comment has the correct address.

## Imports closed (names)

`Gsm_LPC_Analysis`, `Gsm_Long_Term_Predictor`, `Gsm_Long_Term_Synthesis_Filtering`, `Gsm_RPE_Decoding`, `Gsm_RPE_Encoding`,
`Gsm_Short_Term_Analysis_Filter`, `Gsm_Short_Term_Synthesis_Filter`, `__ieee754_sqrt`, `__wrap_strdup`, `copy_reg`, `expf`,
`floor`, `inflateInit2_`, `inflate_blocks`, `inflate_blocks_free`, `inflate_blocks_reset`, `scalbn`,
`__unwinding_cleanup`, `_t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0$nilRep`,
`_t24__default_alloc_template2b0i0$_S_free_list`,
`clone__Q2t12basic_string3ZcZt18string_char_traits1ZcZt24__default_alloc_template2b0i0_3Rep`, `decode_uleb128`,
`dwarf_reg_size_table`, `execute_cfa_insn`, `extract_cie_info`, `find_exception_handler`, `find_fde`, `get_reg_addr`,
`instmap_bounds`, `next_stack_level`, `old_find_exception_handler`.

Not closed by this package and why:

- `__dynamic_cast_2`: already authored as `w13-startup-fn-a-dynamic-cast-2/dynamic_cast_2.cpp` (same behaviour; I read the
  original and the existing source agrees). That source is not in `build-plan.json` in this worktree, so the name stays
  unresolved in the shared build; the lead integrator should check why the plan lacks it. This package did not write a
  second copy.
- `pack`: already defined as `ParticleDefStorage pack;` by
  `w8-spell-static-init-a-cold-arrow-projectile/cold_arrow_projectile_static_init.cpp`. It is also not in
  the plan, so the name stays unresolved in the shared build. Its type belongs to the particle packages, so it was not
  redefined here.

## Imports opened

Measured by a private partial build (`tools/build_partial.py` with the reviewed EE toolchain, a private copy of the plan with
the 44 entries printed by `docs/agents/w15-libs-eh-a-plan-entries.py` added, kept outside git): 6807 sources compiled, one
link, status `partial_build_complete`. The shared selection alone listed 915 unresolved imports; with this package it lists
**914**. Thirty-one names of this package's scope are closed and none outside it. The authored bodies call deeper routines
that nothing in the plan defines, which opens 30 imports:

- Exception runtime: `decode_sleb128` (0x002fbd60), `decode_stack_op` (0x002fcb80), `frame_init` (0x002fc760), `search_fdes`
  (0x002fc708).
- GSM: `RPE_grid_selection` (0x002edae8), `APCM_quantization` (0x002edee8), `Cut_Calculation_of_the_LTP_parameters`
  (0x002ef028), `Calculation_of_the_LTP_parameters` (0x002ee650), `Long_term_analysis_filtering` (0x002ef250), `__muldi3`
  (0x002f81c0, the compiler's 64-bit multiply helper).
- Decompressor: `inflate_codes_free` (0x0109bf60), `inflate_codes_new` (0x0109bee8), `inflate_codes` (0x0109b6d8),
  `inflate_blocks_new` (0x01097248), `inflate_flush` (0x010a85d8), `inflate_trees_bits` (0x010a4048), `inflate_trees_dynamic`
  (0x010a4148), `inflate_trees_fixed` (0x010a4340), `inflate_mask` (0x01192af8, a 17-word table), `zcalloc` (0x0116d6a8),
  `zcfree` (0x0116d6c8), `adler32` (0x01000000).
- Strings: `_S_chunk_alloc__t24__default_alloc_template2b0i0UiRi` (0x011f3318), `_t23__malloc_alloc_template1i0$__malloc_alloc_oom_handler`
  (0x011f2b34, one data word), `cerr` (0x013577d0), `__ls__7ostreamPCc` (0x01353498), `endl__FR7ostream` (0x01353ad0).
- Math and strings support opened by the earlier commits of this branch: `__ieee754_expf` (0x002f4b58, called by `expf`),
  `copysign`, `strdup` (called by `__wrap_strdup`).

Names these bodies also reference that the plan already defines, so they did not become imports: `strcmp`, `strlen`, `memcpy`,
`exit`, `abort`, `__wrap_malloc`, `__wrap_free`, `WaitSema`, `SignalSema`, `__divdi3`, `gsm_FAC`, `objects`, `__sce_eh_sema_id`.

## Build-plan entries needed

Run `python3 docs/agents/w15-libs-eh-a-plan-entries.py --toolchain /path/to/reviewed/ee` from the repository root. It prints one
JSON object per source unit (all directories named `w15-libs-eh-a-*` that contain a non-host-test `.cpp`) with
the file hash, header closure and emitted function sizes under the reviewed `authored_cpp` profile, in the same form as
the other packages' generators. `--private-root DIR` writes a private copy of the plan and snapshot with those entries
added, for a local build; nothing is written inside the repository. The `*_host_test.cpp` files are not plan inputs. The
header-only directories (`w15-libs-eh-a-common`, `w15-libs-eh-a-gsm-common`, `w15-libs-eh-a-zlib-common`) need no entry
of their own; they belong to the header closure of the units that include them. Entry order is alphabetical; the units do
not depend on each other at compile time. Data objects (`gsm_*` tables, `dwarf_reg_size_table`, `bitoff`, `nilRep`,
`_S_free_list`) emit no function.

## Authored but never executed

Everything in this package. The paired runs executed the authored bodies on a private interpreter against the
original's bytes with Python stand-ins for callees, and the host tests executed some of the arithmetic on the host. No
function ran on the PlayStation 2, in an emulator of the whole game, or on any real input stream. In particular
`Gsm_*` and `inflate_*` have not decoded a real GSM or deflate stream; the stand-ins supplied the callee behaviour.

## Unknown

- The bodies of every routine this package calls but does not define, listed under "imports opened".
- Who else uses `_S_free_list` and `nilRep`, and the allocator internals behind `_S_chunk_alloc`.
- Whether the original `inflate_blocks_free` was written with the reset inlined or the compiler inlined it.
- Behaviour on real hardware, including floating-point edge cases of `expf`, `scalbn`, `floor` and `__ieee754_sqrt` (checked
  only on the interpreter's arithmetic).

Not attempted: the other helpers the decompressor and GSM stages call (`inflate_codes`, `inflate_trees_*`, `inflate_flush`,
`inflate_blocks_new`, `RPE_grid_selection`, `APCM_quantization`, the LTP search routines), `frame_init`, `search_fdes`,
`decode_stack_op` and `decode_sleb128`; they are listed under the opened imports for whoever owns the next package.
