# Entry kernel interfaces

These independently authored complete wrappers are experimental startup inputs.
The original baseline hash and full symbol extents are freshly checked. Their
source, compiled object and comparison identities are recorded separately from
the established exact-match ledger. No kernel service is executed by these checks.

## Root callback

The original local `_root` at 0x00100250 has an eight-byte extent: put 35 in the
kernel service register and issue the syscall. The source publishes it globally
because the separate entry unit references it. Fresh compilation reproduces all
eight bytes, with no imports. Neither original nor authored extent contains a
return instruction. Behavior if that kernel service returns is not invented;
termination or scheduling effects require actual kernel validation. This is a
complete service request wrapper, not a source implementation of kernel service 35.

## Cache service wrapper

The original `FlushCache` at 0x002d5360 has a sixteen-byte extent. The source sets
service register V1 to 100, issues the syscall without changing A0, and returns
through RA with a no-op delay slot. Fresh target compilation reproduces the full
sixteen bytes, with no imports or extra code. This preserves the service request
and kernel result; it does not prove the kernel's cache effects. The existing
sound-specific source wrapper is untouched.
