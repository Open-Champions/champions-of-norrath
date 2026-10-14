# w10-startup-a: C runtime, libgcc, libm, C++ RTTI and startup data (part 1 of 4)

Sources are under `experimental/sources/w10-startup-a-*`, one verified function (or data object) per
commit. The build-plan entries are printed by `docs/agents/w10-startup-a-plan-entries.py`
(`--private-root DIR` writes a private plan copy outside git); the shared plan is not edited.

## How each unit was checked

Every unit compiles with the reviewed `authored_cpp` (or `authored_entry_asm`) profile. Each has a
`host_check.cpp` that compares the authored code against hand-computed values, except `__throw`,
whose exception-return builtins have no host equivalent. In addition, a private paired run (an R5900
interpreter outside git that runs the original and the authored function from the same memory image
with the same stubbed callees and compares call logs, registers at exit and every changed byte)
was used for the functions with a stub-able environment, with a mutated copy checked to fail.
The private harness and its output are not committed. Nothing was executed on the EE.

## __throw (0x002fb070, 336 bytes) — observed / inferred / unknown

Observed: reads the context through the function pointer at 0x004480dc, calls `__terminate` if
context+8 is zero, calls `__frame_state_for` with the address of a label in itself, stores the
frame's canonical address in word 0 of the record, calls the file-local `throw_helper` (0x002face0)
with context, return address minus one, the record and a stack-adjust word, and leaves through an
exception-return sequence (stack adjust in v1, handler in a0, context in v0).
Inferred: the record is 1144 bytes (throw_helper copies that much).
Unknown/different: the original's explicit `jr a0` stub is replaced by the compiler's own
exception-return expansion, so the handler sees a different `ra`, and **v0 is not set to the
context** at handler entry (the compiler gives no way to place it). Paired run: 16 scenarios
(null info, null frame state, several adjusts), identical call logs, stack adjust, callee-saved
GPRs and f20-f31 at handler entry; a copy with `pc - 2` failed.
`throw_helper` is local in the original, so the authored `__throw` imports it by that name;
that import is opened (see below).

## Imports closed (by authored source in this branch)

AlarmHandler, CreateReaderThread, Deci2Call, Exit, GetMemorySize, InitTLB32MB, SleepThread,
_InitTLB, __cp_push_exception, __divdi3, __eh_alloc, __fixunsdfdi, __fpcmp_parts_d,
__ieee754_sqrtf, __kernel_rem_pio2f, __make_dp, __make_fp, __moddi3, __new_handler, __pack_d,
__ps2_klibinfo__, __rtti_user, __terminate_func, __throw, __unpack_d, __unpack_f, and
`__default_terminate` (a helper of the termination path).

## Imports not closed here

- `_ctype_`: already authored in `w8-startup-sce-a-ctype-table` (another package) but absent from
  the shared plan; the plan-entries script lists it so the integrator can see what is needed.
- `__malloc_current_mallinfo`: the 40-byte zero object at 0x004488b8 is authored by the memory
  package under the name `norrath_player_malloc_current_info_view`
  (`memory-malloc-state/malloc_scalars.cpp`). It needs the real symbol name there; defining it
  here too would give two copies.
- `__extendsfdf2`: **no function of this name exists in the retail image** (the symbol dump has no
  `extend`/`sfdf` entry), so there is no evidence to reconstruct. The retail package that opened it
  calls a name the original does not define; its caller should be checked against the image.
- `_args`, `_bssend`, `__EH_FRAME_BEGIN__`: final-image layout symbols; see
  `docs/agents/link-image-bounds.md` (syscall argument buffer size, section layout and unwind
  metadata policy are not settled). Not guessed here.

## Imports opened

`throw_helper` (file-local at 0x002face0, 908 bytes; it also pulls in the local helpers
`find_exception_handler`, `old_find_exception_handler`, `get_reg_addr`, `next_stack_level`,
`copy_reg`, `__unwinding_cleanup`). `get_eh_context` (data pointer at 0x004480dc, file-local) is
also imported by name. `__frame_state_for` (0x002fd0f0) and `__terminate` are real symbols owned by
nobody in this package.

## Build-plan entries needed

Run `python3 docs/agents/w10-startup-a-plan-entries.py --toolchain /Users/case/norrath-agents/repo/work/toolchains/ee`.
It prints one entry per unit, including `w10-startup-a-throw`; the lead integrator merges them.

## Authored but never executed

Everything: no unit has run on hardware or an emulator. The paired runs execute the original and
authored code in a private interpreter only. `__throw` additionally depends on compiler builtins
(`__builtin_eh_return`, `__builtin_unwind_init`) whose expansion differs from the original's.

## Unknown

The meaning of the unpacked-float class values beyond 2 and 3, the contents of the frame-state
record past word 0, `throw_helper`'s contract, the size of the `_args` buffer, and whether
`__extendsfdf2` was ever linked into the retail image.
