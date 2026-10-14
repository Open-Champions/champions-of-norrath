# Ordinary startup constructor dispatch

This package implements complete experimental source bodies for the pinned
ordinary runtime initialization path. It does not implement game `main`, entry,
callback bodies, or the separate SRS tables. No original-byte match or game
runtime execution is claimed. Original extents and state were inspected after
freshly verifying the target SHA-256 in `targets/SLUS-20565/baseline.json`.

## __main guard

The complete 56-byte original at 0x002f9fa8 reads the word at 0x008ea998, returns
when nonzero, otherwise sets it to one before dispatching `__do_global_ctors`.
That original word lies inside the entry's explicit BSS-clear range. The source
owns a zero-initialized static unsigned guard. It preserves sequential/reentrant
behavior, not thread-safe concurrent initialization. No external guard address
or original bytes are imported.

The target build emits 36 bytes and only the constructor-dispatch import. The
emitted branch/store/tail-call order was inspected. The host ASan/UBSan check
covers 10,000 repeated calls and reentry from the first dispatch: one dispatch.

```sh
mkdir -p work/ctor-check
clang++ -std=c++17 -O2 -fsanitize=address,undefined -D__main=norrath_test_main -D__do_global_ctors=norrath_test_ctors experimental/sources/runtime-startup/main_guard.cpp tools/checks/main_guard_contract.cpp -o work/ctor-check/main
work/ctor-check/main
```

The snapshot records the fresh target object and source identities. The public
partial driver independently recompiles the selected source after this package.

## Persistent destructor walker

The complete 140-byte original at 0x002f9e58 uses a persistent cursor whose
original initializer at 0x004480d0 is 0x0031f3f0 (`__DTOR_LIST__ + 1`). The separate
completed word at 0x004480d4 is initially zero. Both initializers were read from
the pinned image. The walker advances the stored cursor before each callback,
then sets the completion guard before deregistering `__EH_FRAME_BEGIN__`.

The source owns the cursor and zero guard, preserving forward order, callback
reentry, and one-time deregistration. The callback table, frame metadata and
frame deregistration routine stay unresolved: no original callback bytes or
addresses supply them. The zero guard is emitted in BSS rather than the original
initialized data; future executable layout must initialize this source storage.
The target emits 128 bytes. Its indirect calls, pre-call cursor update and
post-walk guard were inspected; there is no new exact-byte match.

Three isolated ASan/UBSan host runs cover an ordinary table, callback reentry,
and an empty table, plus repeated dispatch and reentry from frame deregistration.

```sh
clang++ -std=c++17 -O2 -fsanitize=address,undefined -D__do_global_dtors=norrath_test_dtors -D__DTOR_LIST__=norrath_test_dtor_list -D__EH_FRAME_BEGIN__=norrath_test_frames -D__deregister_frame_info=norrath_test_deregister experimental/sources/runtime-startup/destructors.cpp tools/checks/destructors_contract.cpp -o work/ctor-check/dtors
work/ctor-check/dtors
work/ctor-check/dtors reentrant
work/ctor-check/dtors empty table
```

## Reverse constructor walker

The complete 192-byte original at 0x002f9ee8 first registers frame information,
then obtains the count from slot zero or scans through the null terminator when
that slot is all ones. It calls slots count through one in reverse order and
finally registers `__do_global_dtors` with `atexit`, without inspecting failure.
This preserves both forms of well-formed table; corrupt/unbounded tables remain
outside the memory-valid contract. The pinned table contains 101 callbacks.

The source owns a six-word frame-registration object: the inspected registrar
at 0x002fc428 initializes words at offsets 0, 4, 8, 12, 16 and 20; the original
object at 0x008ea980 ends where the main guard at 0x008ea998 begins. This establishes
the 24-byte storage span, not the rest of the exception runtime. The registrar,
frame contents, callback tables and `atexit` remain external dependencies.
The target emits 176 bytes. Generated call order, sentinel scan and reverse loop
were inspected. Host ASan/UBSan checks cover 204 tables (counts 0–101 in both
counted and sentinel form), registration-before-callbacks and exit-registration
after callbacks, including ignored registration failure. These checks execute
host stand-ins for dependencies, not original callbacks or PS2 startup.

```sh
clang++ -std=c++17 -O2 -fsanitize=address,undefined -D__do_global_ctors=norrath_test_ctors -D__do_global_dtors=norrath_test_dtors -D__CTOR_LIST__=norrath_test_ctor_list -D__EH_FRAME_BEGIN__=norrath_test_frames -D__register_frame_info=norrath_test_register -Datexit=norrath_test_atexit experimental/sources/runtime-startup/constructors.cpp tools/checks/constructors_contract.cpp -o work/ctor-check/ctors
work/ctor-check/ctors
```

The constructor package is implemented before low-level entry because these
complete C++ contracts can be checked with the established compiler profile.
Entry requires a separate pre-stack CPU/kernel ABI implementation and validation;
putting an ordinary C++ prologue at `_start` would be incorrect. No startup stage
is described as reached merely because its dispatcher now compiles.
