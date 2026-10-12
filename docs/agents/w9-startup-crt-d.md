# w9-startup-crt-d

Startup and runtime support (d). Sources are under `experimental/sources/w9-startup-crt-d-*`. Each unit compiled alone with the plan's `authored_cpp` flags; host checks (`host_test.cpp`) include host headers and stay out of the EE plan. Addresses are from the private executable whose SHA-256 is the snapshot `target_sha256`. Compiled sizes are `st_size` from the EE compile, not original-byte matches. Nothing was executed on the EE or in game.

## Per-function notes

- `free` (`0x003015a0`, 72 bytes; compiled 64). Observed: lock service, `_free_r` (`0x00300608`) with the pointer re-read, unlock service as a tail call; each `_impure_ptr` read is a fresh load. Callee for `_free_r` is the authored `norrath_player_free_reentrant`. Host check covers order, re-reads and the null pointer reaching `_free_r`.
- `memalign` (`0x00301318`, 100 bytes; compiled 88). Observed: lock, `_memalign_r` (`0x00301380`) with the pointer re-read, then (alignment, size), unlock; returns the `_memalign_r` result. `_memalign_r` is not authored and is a newly opened import.
- `memmove` (`0x003020ac`, 260 bytes; compiled 132). Observed: backward byte copy only when source < destination < source + count (unsigned), otherwise forward; returns the destination. The original's quadword and doubleword fast paths are not reproduced; only byte results are. Host check sweeps all overlap offsets and counts 0..40 against a temp-buffer model.
