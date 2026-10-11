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
