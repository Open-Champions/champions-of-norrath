/* Observed dpmul at 0x002fb680, 644 bytes: soft-double multiplication. Two 64-bit
 * registers hold double-precision bits and the 64-bit product bits are returned. The
 * original unpacks both operands with __unpack_d, handles the exceptional classes,
 * forms the 128-bit product with four __muldi3 calls, normalises it and packs with
 * __pack_d.
 * An independently authored body for this routine already exists as
 * norrath_player_double_multiply (game-player-softdouble-build); this unit does not
 * duplicate it. It passes both operands through unchanged and returns the result
 * unchanged, so that the callers that import the original name resolve without a symbol
 * alias (the same approach as w9-startup-crt-e-softdouble-names).
 * Evidence for the target: a private paired run called the original dpmul and the
 * authored norrath_player_double_multiply, with the helper symbols bound to the
 * original's __unpack_d, __pack_d and __muldi3, on 1500 pseudo-random operand pairs
 * (zeros, infinities, NaNs, subnormals and normals of both signs); results and every
 * changed byte matched.
 * Unknown: operands passed in other registers. Compiled output is not claimed to match
 * the original.
 */
extern "C" {
unsigned long long norrath_player_double_multiply(unsigned long long left, unsigned long long right);

unsigned long long dpmul(unsigned long long left, unsigned long long right) {
    return norrath_player_double_multiply(left, right);
}
}
