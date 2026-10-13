// Observed _free_r at 0x00300608, 756 bytes, pinned SLUS_205.65: the reentrant free of the C library's
// boundary-tag allocator (arguments: a reentrancy record and the allocation, which may be null). The
// body is already authored as norrath_player_free_reentrant (game-player-free-build); this unit only
// supplies the original symbol name so that callers that import _free_r (the aligned allocator, the
// signal-state setup) resolve without a symbol alias. It passes both arguments through and returns
// nothing, like the original's last act of tail-calling the unlock service inside the body.
// Evidence for the pair (this wrapper plus the authored body): a private paired run against the original
// _free_r over heaps built by the original allocator (see docs/agents/w12-startup-rt-a.md).
// Unknown: callers that rely on the original's code size or register state.
extern "C" {
void norrath_player_free_reentrant(unsigned char* reent, unsigned char* allocation);

void _free_r(unsigned char* reent, unsigned char* allocation) {
    norrath_player_free_reentrant(reent, allocation);
}
}
