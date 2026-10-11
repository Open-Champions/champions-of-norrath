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
