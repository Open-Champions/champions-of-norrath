# w8-runtime-libc-a

C runtime and RTTI support (1/2). Every unit was compiled with the reviewed `authored_cpp` flags and has a host check beside it (`host_test.cpp` or equivalent). Host checks include the host C library and stub callees, so they must stay out of the EE plan. Comparisons are against the local executable whose SHA-256 is the snapshot `target_sha256`; sizes called "compiled" are `st_size` from the EE compile and are not original-byte matches. Nothing here was executed on the EE or in game.

## Verification performed

- Each unit compiled alone with the plan's `authored_cpp` flags and its host check passed.
- A private copy of the plan and snapshot (kept outside git) adding the 23 units passed `--check-plan` (1430 selected inputs) and a full partial build: 1430 compilations, one relocatable link, 1124 function bodies, 1191 unresolved imports, COMMON zero. Of the 24 package names, 23 are no longer unresolved; `__throw_bad_typeid` remains. The 1191 total is that private plan on this branch only (other agents' branches are not merged in), so it is not a comparable integration count.
- Against the objects of that build, the 23 new objects reference 19 symbols that no other object imported (listed below).

## Observed

- `floorf` at `0x002f1390` (216 bytes) and `ceilf` at `0x002f11b8` (216 bytes) share one shape. Exponent 23 or above returns the input. A fractional input needs the probe sum with `0x7149f2c9` to compare above +0 before it changes. Below 1: floor gives +0 for positives and -1 for negative normals and leaves -0 and negative subnormals; ceil gives -0 for negatives and +1 for positives at or above the smallest normal, and leaves +0 and positive subnormals. Compiled: floorf 232, ceilf 240. The host checks sweep stepped finite bit patterns of both signs plus edge cases against libm, with the subnormal rules above as the model.
- `atoi` at `0x002ff990` (28 bytes) calls `strtol` (`0x00306638`) with a null end pointer and base 10, and returns the low 32 bits. The authored `norrath_factory_strtol` is the callee. Compiled: 36.
- `memcmp` at `0x00301f68` (148 bytes): a 16-byte aligned block scan for counts of 16 or more, then a byte loop; result is the unsigned byte difference or 0, count unsigned. Only the byte result is authored. Compiled: 64.
- `close` at `0x002d57c8` and `lseek` at `0x002d57d8` are 8-byte stubs that return -1 and do not touch errno. Compiled: 8 each.
- `exit` at `0x002ffb08` (168 bytes): reads `_impure_ptr`, walks the registered-function lists at impure+328 (node: next at +0, count at +4, up to 32 pointers from +8) calling each node's functions last to first, calls the word at impure+60 with the impure pointer if nonzero, and tail-calls `_exit` (`0x00100248`). Compiled: 172.
- `abort` at `0x002ff8c0`: `raise(6)` at `0x00303bf8`, then `_exit(1)`. The result of `raise` is not tested. Compiled: 32.
- `__pure_virtual` at `0x002f9fe0`: tail call to `__terminate` at `0x002fa010`. Compiled: 8.
- `__eq__C9type_infoRC9type_info` at `0x002fd2e0` (60 bytes): equal if same object, otherwise `strcmp` of the first words (name pointers) equal. Compiled: 56.
- `__rtti_si` at `0x002fd348` (24 bytes): null object returns; otherwise stores name at +0, the address `0x004537b0` at +4, and the base at +8. Compiled: 36.
- `_vt$14__si_type_info` at `0x004537b0` and `_vt$16__user_type_info` at `0x004537e0` are 48 bytes each: five (zero, code address) pairs and a zero final pair. The five addresses of each were matched to symbols in the order recorded in the sources. Both objects live in `.data` in the image; the authored views are read-only (`const`), as the callers' declarations are.
- `malloc` at `0x00301548` (80 bytes): lock service, `_malloc_r`, unlock service, each with a fresh read of `_impure_ptr`. Callees are the authored `norrath_player_malloc_lock`, `norrath_player_malloc_reentrant`, `norrath_player_malloc_unlock`. Compiled: 76.
- `__builtin_delete` at `0x002fedf0` (weak): null check, then `__wrap_free`. Compiled: 24.
- `__assert__FPCcUiT0i` at `0x00172890` (168 bytes): three `scePrintf` calls with the format at `0x004635e8`, then condition text, file, line, the word at gp-14592 (`context`, `0x004840f0`, re-read every time) and the fourth argument; then a store of zero to address 0. The format text is 48 characters plus NUL. `context` is a pointer, initially `0x004840e8`, to the text `none`. Compiled: 152.
- `_listCheckAllIntegrity__FPCci` at `0x00194158` (424 bytes): over the 6096 twenty-byte records of `nodes` (`0x007ad000`), words 0 and 8 must be zero or inside the table (first byte through one past the last record, inclusive); words 4 and 12 must be zero, or in `0x00100000..0x08000000`, or in `0x70000000..0x70004000`. A failure prints `list integrity breach! %s:%d` and stores zero to address 0. Compiled: 400.
- `norrath_factory_sscanf` is the original `sscanf` at `0x00303f50` under the name megatile imports; same body as the libc-format `sscanf`. Compiled: 344.
- `_printf` at `0x002d6aa0` (1524 bytes). Conversions are selected by the character minus `'0'` (0..72): `0` width (one or two digits after it, limit 31, fills zeros), `l`, `h`, `d`, `u`, `o`, `x`, `e`/`f`, `s`, `c`. Everything else, including `1`..`9`, `%` and NUL, skips one character and resumes plain scanning, so `"%5d"` prints `d`. `s` prints `(null)` when the first byte is zero (a null pointer faults). Signed values print `-` before any zero pad. Float conversions go through `dptofp`, a zero test, `fptodp`, `printfloat` (`0x002d6938`). The signed path divides with `__divdi3`/`__moddi3` after negating, the unsigned and radix paths with `__udivdi3`/`__umoddi3`. DIntr/EIntr bracket the whole call. Compiled: 1680.
- `p05.0` at `0x0047f4e0`: three little-endian words 5, 25, 125. `__mprec_bigtens` at `0x0047f5b8`: five little-endian doubles; entry 0 is exactly 1e16, entries 1 to 4 are one unit in the last place below the correctly rounded powers of ten (bytes kept as found).

## Inferred

- The first word of a `type_info` is the name pointer (`__eq__...` passes it to `strcmp`); word 2 of the single-inheritance form is the base `type_info`.
- The 16 KiB window at `0x70000000` in `_listCheckAllIntegrity__FPCci` is probably scratchpad; not confirmed.
- `impure+60` in `exit` is a cleanup hook; no name is claimed.
- Vtable slot meanings follow the symbol names only.

## Unknown

- `__throw_bad_typeid` at `0x002ff3c8` is not authored. It allocates with `__eh_alloc`, builds a `bad_typeid` with `__tf10bad_typeid`, pushes it with `__cp_push_exception` and calls `__throw`, then has a cleanup path calling `__rethrow` tied to the exception tables. The authored profile is `-fno-exceptions`; reproducing the cleanup region would need exception-table and personality evidence this package does not have.
- `printfloat` at `0x002d6938` (local routine, 360 bytes) is outside this package and depends on the soft-double helpers, `__fixunsdfdi`, `ftoi`, `kprintf` and several constants. `_printf` imports it as `printfloat`.
- What `raise`, `_exit`, `__terminate`, `dptofp`, the four 64-bit division helpers and the ten type_info slot targets do beyond what their callers here need.
- The fault behavior of the zero stores to address 0 in `__assert__FPCcUiT0i` and `_listCheckAllIntegrity__FPCci`.
- The quadword scans in `memcmp` are not reproduced; only byte results are.

## Build-plan entries

Do not add any `host_test.cpp` file. The sources have no includes, so `header_closure` is empty. Profile is `authored_cpp`. Assign fresh `input_order` values at the end of the plan, add every source to the `files` map with the same hash, add a snapshot input with `component`, `object_name`, `kind`, `source` and `emitted_functions`, and raise `default_authored_inputs` by 23. No `private_object_sha256` is claimed. Object names are suggestions (`w8a_` prefix); the sha256 values and sizes come from one EE compile with the reviewed flags. If any source changes, regenerate its hash.

```
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_abort.o", "profile": "authored_cpp", "sha256": "16a9d15240e7f906ec3ff1f6ee4bf10e0c119f756ca1f82228fd88d3daa101f8", "source": "sources/w8-runtime-libc-a-abort/abort.cpp", "emitted_functions": [{"name": "abort", "compiled_body_bytes": 32}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_assert.o", "profile": "authored_cpp", "sha256": "c4db58309eaeab12ea244de0b5ec336bd6776519db58bb38fed58aa3e382031d", "source": "sources/w8-runtime-libc-a-assert/assert.cpp", "emitted_functions": [{"name": "__assert__FPCcUiT0i", "compiled_body_bytes": 152}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_assert-context.o", "profile": "authored_cpp", "sha256": "0f52a448a31c2c2ef0e6bea75783ce3fe5f0a6f2cdc808ec6240c734646970e8", "source": "sources/w8-runtime-libc-a-assert-context/assert_context.cpp", "emitted_functions": [] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_assert-format.o", "profile": "authored_cpp", "sha256": "c6c1347bf892d751bed7d71ea631c86b86c7a24a40486fb4054779708776577c", "source": "sources/w8-runtime-libc-a-assert-format/assert_format.cpp", "emitted_functions": [] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_atoi.o", "profile": "authored_cpp", "sha256": "a18a51207e210e612ffe89373c346fb5943f35a8d4bd6a44b140bcbae20ca2d9", "source": "sources/w8-runtime-libc-a-atoi/atoi.cpp", "emitted_functions": [{"name": "atoi", "compiled_body_bytes": 36}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_big-tens.o", "profile": "authored_cpp", "sha256": "1175fcd3f09e74df106c1cb89d0271923398aa747f9aa23e99d1574199abc2dc", "source": "sources/w8-runtime-libc-a-big-tens/big_tens.cpp", "emitted_functions": [] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_builtin-delete.o", "profile": "authored_cpp", "sha256": "44c048e8057cfa254a6c1c368401e53f13bc574a9238747896c572c681e045aa", "source": "sources/w8-runtime-libc-a-builtin-delete/builtin_delete.cpp", "emitted_functions": [{"name": "__builtin_delete", "compiled_body_bytes": 24}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_ceilf.o", "profile": "authored_cpp", "sha256": "ea73faf30b9867b4c250c53e844efd25232e84477ed448923681e562607f5fb0", "source": "sources/w8-runtime-libc-a-ceilf/ceilf.cpp", "emitted_functions": [{"name": "ceilf", "compiled_body_bytes": 240}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_close.o", "profile": "authored_cpp", "sha256": "9efe770b6703bb41fa64df04f9b4f231ad1635ec65d6f6a12af8e1cc0972621a", "source": "sources/w8-runtime-libc-a-close/close.cpp", "emitted_functions": [{"name": "close", "compiled_body_bytes": 8}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_exit.o", "profile": "authored_cpp", "sha256": "71d7dfea3b22593a5dfdf3cd87cc53ca4e99aad4d2e2b1e778f28b98631423ff", "source": "sources/w8-runtime-libc-a-exit/exit.cpp", "emitted_functions": [{"name": "exit", "compiled_body_bytes": 172}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_factory-sscanf.o", "profile": "authored_cpp", "sha256": "2fbfbd4739d131512906bdbccd0d4606f88d9f01a49281d2a81bd90e5d4bd267", "source": "sources/w8-runtime-libc-a-factory-sscanf/factory_sscanf.cpp", "emitted_functions": [{"name": "norrath_factory_sscanf", "compiled_body_bytes": 344}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_floorf.o", "profile": "authored_cpp", "sha256": "376299e3432d133ca7f44ab6cb56489dde990cec06bc333bf6e9080d70de26ad", "source": "sources/w8-runtime-libc-a-floorf/floorf.cpp", "emitted_functions": [{"name": "floorf", "compiled_body_bytes": 232}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_list-check.o", "profile": "authored_cpp", "sha256": "6c28855094cf817cf12443690d54f21204b12acc9d210f4e85610b6df9d7e505", "source": "sources/w8-runtime-libc-a-list-check/list_check.cpp", "emitted_functions": [{"name": "_listCheckAllIntegrity__FPCci", "compiled_body_bytes": 400}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_lseek.o", "profile": "authored_cpp", "sha256": "c951d3f51d0f109c0035039d7e125a09105d210fb7fd029efa14775a56e8a919", "source": "sources/w8-runtime-libc-a-lseek/lseek.cpp", "emitted_functions": [{"name": "lseek", "compiled_body_bytes": 8}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_malloc.o", "profile": "authored_cpp", "sha256": "8a4c9c660d980ad279b4bc7c7dc03771d9c2567e698f375b7110579fd3e164d7", "source": "sources/w8-runtime-libc-a-malloc/malloc.cpp", "emitted_functions": [{"name": "malloc", "compiled_body_bytes": 76}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_memcmp.o", "profile": "authored_cpp", "sha256": "30aaf87cac5541854896e614ca7283787645d687abe3fb25492867484c30ede7", "source": "sources/w8-runtime-libc-a-memcmp/memcmp.cpp", "emitted_functions": [{"name": "memcmp", "compiled_body_bytes": 64}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_powers5.o", "profile": "authored_cpp", "sha256": "73259fd95194737c9ba88e28d9b51d237ca076e0fac73cba1ed54f8f5dd654d1", "source": "sources/w8-runtime-libc-a-powers5/powers5.cpp", "emitted_functions": [] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_printf.o", "profile": "authored_cpp", "sha256": "621923f52e93d25a4f1323d803c11518dbb7cfcbea27734f1c6c3dad4432a764", "source": "sources/w8-runtime-libc-a-printf/printf.cpp", "emitted_functions": [{"name": "_printf", "compiled_body_bytes": 1680}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_pure-virtual.o", "profile": "authored_cpp", "sha256": "4025cebde75ff36cb8db54f11d90034027f061376462d96236b78f95d146a5bb", "source": "sources/w8-runtime-libc-a-pure-virtual/pure_virtual.cpp", "emitted_functions": [{"name": "__pure_virtual", "compiled_body_bytes": 8}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_rtti-si.o", "profile": "authored_cpp", "sha256": "6c2e8d22728ac52bd5d4618bb1bf71ff3047be72802d339baef3bca386de8909", "source": "sources/w8-runtime-libc-a-rtti-si/rtti_si.cpp", "emitted_functions": [{"name": "__rtti_si", "compiled_body_bytes": 36}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_si-vtable.o", "profile": "authored_cpp", "sha256": "e0406ff7e6e0d650a3109e6daa9c56d4d5142051298f4522042070f446e8f053", "source": "sources/w8-runtime-libc-a-si-vtable/si_vtable.cpp", "emitted_functions": [] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_type-info-eq.o", "profile": "authored_cpp", "sha256": "255fd0c4c54ab772abb883a7440c0532771eda06532774289e0c1d602fca557b", "source": "sources/w8-runtime-libc-a-type-info-eq/type_info_eq.cpp", "emitted_functions": [{"name": "__eq__C9type_infoRC9type_info", "compiled_body_bytes": 56}] }
{ "header_closure": [], "kind": "independently_authored_source", "object_name": "w8a_user-vtable.o", "profile": "authored_cpp", "sha256": "dbd4690bfbafaac83ab38ba443913e6ed0a25ba9dac8fce6d2673250b3de8d96", "source": "sources/w8-runtime-libc-a-user-vtable/user_vtable.cpp", "emitted_functions": [] }
```

## Imports closed

`__assert__FPCcUiT0i`, `__builtin_delete`, `__eq__C9type_infoRC9type_info`, `__pure_virtual`, `__rtti_si`, `_listCheckAllIntegrity__FPCci`, `_printf`, `abort`, `atoi`, `ceilf`, `close`, `exit`, `floorf`, `lseek`, `malloc`, `memcmp`, `norrath_assert_context_word_view`, `norrath_assert_format_view`, `norrath_factory_sscanf`, `norrath_legacy_single_inheritance_type_info_vtable_view`, `norrath_legacy_user_type_info_vtable_view`, `norrath_player_bigint_small_powers5_view`, `norrath_player_number_big_tens_view`.

`floorf` and `ceilf` here are independently authored, so the optional Newlib inputs are not needed to close them. Do not select both.

Not closed: `__throw_bad_typeid` (see Unknown).

## Imports opened

`_exit`, `raise`, `__terminate`, `dptofp`, `printfloat`, `__divdi3`, `__moddi3`, `__udivdi3`, `__umoddi3`, and the ten vtable targets: `__tf14__si_type_info`, `_$_14__si_type_info`, `do_upcast__C14__si_type_info...`, `do_dyncast__C14__si_type_info...`, `do_find_public_subobj__C14__si_type_info...`, and the five `16__user_type_info` equivalents (full mangled names are in the two vtable sources). That is 19 symbols that the build with these units adds to the unresolved list; the integrator may prefer to hold the two vtable units until the targets exist. Already-defined symbols used and not opened: `scePrintf`, `strcmp`, `_putchar`, `nodes`, `norrath_factory_strtol`, `norrath_entity_fptodp`, the three `norrath_player_malloc_*` routines, `norrath_factory_impure_pointer_word`, `DIntr`, `EIntr`, `__wrap_free`.

## Authored but never executed

Everything. Host checks exercise the logic with stubbed callees on the build machine. No unit has run on the EE, with the original callees, or in game. The stores to address 0 are an inline store on the EE build and a stub probe on the host; the host builds of `_printf` map `%s` words to host strings through a test hook.

## Duplicates to be aware of

`norrath_sound_assert` (game-sound-memory-lifecycle-build) has the same body as `__assert__FPCcUiT0i`. `norrath_entity_floorf` has the same body as `floorf`. `norrath_factory_atoi` and `atoi` are the same original function. `norrath_type_info_equal` is the same routine as `__eq__C9type_infoRC9type_info`. `norrath_rtti_si` (package b) is `__rtti_si` under a reconstruction name; both read the single-inheritance table view defined here.
