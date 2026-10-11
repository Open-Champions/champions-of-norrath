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
