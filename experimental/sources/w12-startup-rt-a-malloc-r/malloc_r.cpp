// Observed _malloc_r at 0x00301840, 1832 bytes, pinned SLUS_205.65: the reentrant malloc of the C
// library's boundary-tag allocator (arguments: a reentrancy record and the requested size; returns the
// allocation or null). The body is already authored as norrath_player_malloc_reentrant
// (game-player-malloc-build-v2); this unit only supplies the original symbol name so that callers that
// import _malloc_r (the aligned allocator, the signal-state setup) resolve without a symbol alias. It
// passes both arguments through and returns the body's result.
// Evidence for the pair (this wrapper plus the authored body): a private paired run against the original
// _malloc_r over heaps built by the original allocator (see docs/agents/w12-startup-rt-a.md).
// Unknown: callers that rely on the original's code size or register state.
extern "C" {
unsigned char* norrath_player_malloc_reentrant(unsigned char* reent, unsigned requested);

unsigned char* _malloc_r(unsigned char* reent, unsigned requested) {
    return norrath_player_malloc_reentrant(reent, requested);
}
}
