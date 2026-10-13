/* Observed _fpadd_parts at 0x002fb360, 604 bytes (a local routine of the soft-double
 * code). Arguments: two unpacked-double records and a scratch record, each 32 bytes
 * (class word, sign word, exponent word, a spare word, a 64-bit fraction at offset 16).
 * It returns a pointer to the record that holds the sum: one of the two inputs when the
 * other is zero-class or exceptional, otherwise the scratch record after adding or
 * subtracting the aligned fractions. dpadd and dpsub (w9-startup-crt-d) pass the result
 * to __pack_d.
 * An independently authored body for this routine already exists as
 * norrath_player_double_add_parts (game-player-softdouble-build); this unit does not
 * duplicate it. It passes the three pointers through unchanged and returns the result
 * pointer unchanged, so that the callers that import the original name resolve without a
 * symbol alias.
 * Evidence for the target: a private paired run called the original _fpadd_parts and the
 * authored norrath_player_double_add_parts on 1500 pseudo-random record pairs (all
 * classes, both signs, equal and near-equal exponents); the returned pointer and every
 * changed byte matched.
 * Unknown: the record field names. Compiled output is not claimed to match the original.
 */
extern "C" {
const unsigned char* norrath_player_double_add_parts(const unsigned char* left, const unsigned char* right,
                                                       unsigned char* output);

const unsigned char* _fpadd_parts(const unsigned char* left, const unsigned char* right, unsigned char* output) {
    return norrath_player_double_add_parts(left, right, output);
}
}
