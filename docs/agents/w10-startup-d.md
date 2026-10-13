# Startup and runtime support, part 4 (w10-startup-d)

Branch `agents/w10-startup-d`. Private reference: `work/private/SLUS_205.65`; addresses below are
symbol values of that file. No game bytes, disassembly or decompiler output are committed. Every
unit was compiled with the reviewed EE g++ `authored_cpp` flags. Behavior checks were paired runs in
a private R5900 interpreter kept outside git: the original function and the authored object run
from identical memory with identical callee stubs, and call logs, returned registers and every
changed byte are compared. Nothing ran on the EE or as gameplay. Each unit is its own commit.

## Imports closed

Functions (each paired against the original, with at least one mutation of the source shown to
be detected unless noted):

- `sqrt` (0x002f1658): forwards the double to `__ieee754_sqrt`.
- `sceResetttyinit` (0x002d5560): stores zero to `ttyinit`.
- `swap__H1Zi_RX01T0_v` (0x003131a0): int pair exchange, the image's own symbol name.
- `scalbnf` (0x002f8070): 1272 argument pairs, including zero, denormal-range, infinity, NaN, and
  exponent overflow/underflow edges. Mutations of the 256 threshold are detected. The two
  constants `0x7149f2c9` and `0x0da2425f` are not observable: the product overflows or underflows
  to the same value for any nearby constant, so a one-bit change to them is not detected (unlike
  the threshold). The `n > 50000` test in the last case cannot affect a result.
- `qsort_translate_table` (0x002e7a38): quicksort of 10-byte records by the byte at offset 1.
  121 scenarios (duplicate keys, partial ranges, negative indices) compared whole memory; mutations
  of the comparison, the middle index and the key offset are detected.
- `__tf9type_info` (0x0031a950), `__tf9bad_alloc` (0x0031b490), `__tf10bad_typeid` (0x0031aa68):
  type_info accessors. Each defines its own node (`__ti9type_info` 8 bytes at 0x00486de0,
  `__ti9bad_alloc`, `__ti10bad_typeid` 12 bytes at 0x008ec910 / 0x008ec840) in the same unit, so
  `__ti9type_info` is closed with it. Paired runs vary the node's first word; mutated strings and a
  dropped base call are detected.
- `_$_9bad_alloc` (0x0031b458), `_$_10bad_typeid` (0x0031aac8): store `_vt$9exception` into word 0,
  call `__builtin_delete` when bit 0 of the flag is set.

Data:

- `_vt$10bad_typeid` (0x00453810) and `_vt$9bad_alloc` (0x00453950): four (zero word, function)
  slots each. Checked by reading the compiled object's relocations and comparing each referenced
  function address with the image's words; the image's remaining words are zero.
- `ttyinit` (0x00444950): one zero word in `.data`.
- `norrath_w9_startup_crt_b_view_4816dc` (word `0x3891a2b3`), `_483b70` and `_485380` (the text
  `fx.lmp`): compared byte for byte against the image.
- Extra function authored because the vtable needs it: `what__C9bad_alloc` (0x0031b4e0).

Imports named in the task that are authored by another package but missing from the shared plan:
`norrath_rtti_entities_a_view_480bb8`, `_480cb4`, `_480cb8`, `_480cbc`, `_480cc0`, `_4810b4`,
`_4810b8`, `_481710`, `_4818ac`, `_482424`, `_482428`, `_48242c`, `_482430`, `_482434`, `_482438`.
The sources exist in `experimental/sources/w9-rtti-vtables-entities-a-view-*` and are not mine; they
need plan entries only (printed by `docs/agents/w10-startup-d-plan-entries.py` together with mine,
seven units).

## Imports opened

`__rtti_user`, `__rtti_si`, `__builtin_delete` (already closed by other packages),
`__tf9exception`, `__ti9exception` (8-byte node at 0x00486de8), `_vt$9exception` (0x00453930),
`what__C9exception` (0x002ff468), `copysignf`, `__ieee754_sqrt`.

## Build-plan entries needed

Run `python3 docs/agents/w10-startup-d-plan-entries.py --toolchain <ee> [--private-root DIR]`; it
prints one JSON entry per unit (18 units of mine plus the 7 w9 view units named above) with the
source hash and emitted function sizes, and can write a private plan/snapshot copy outside git. I
did not edit `build-plan.json` or `snapshot.json`.

## Authored but never executed

All of it was executed only inside the private interpreter, never on hardware. The vtables and
data views were not executed at all; they were checked as bytes and relocations. `__tf*` and the
destructors were run with stubbed `__rtti_*`, `__tf9exception` and `__builtin_delete`.

## Unknown

- Meaning of the 10-byte records and the caller of `qsort_translate_table`.
- What `ttyinit` is beyond a flag tested by `write` and cleared by `sceResetttyinit`.
- The meaning of the float `0x3891a2b3`.
- Whether the first vtable slot (zero word plus type accessor) is an entry or a header.
- Storage ownership: the weak/COMDAT linkage of the originals (symbols are `W`/`V`) is not
  reproduced; the authored symbols are ordinary strong definitions.
