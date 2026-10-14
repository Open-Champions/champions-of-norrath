// Observed compareToItem__Q24item11descriptionPQ24item11description at 0x001084e0,
// 68 bytes, in the pinned SLUS_205.65. Two item description pointers (left, right).
// The result is 0 when the first word of right is zero (an empty slot). Otherwise it
// is 1 only when all of these hold: the 8 bytes at +8 are equal, the word at +0 is
// equal, and the word at +32 (read zero-extended) is also equal; else 0.
// Inferred: the 8 bytes at +8 are compared as one 64-bit value.
// Unknown: what the fields at +0, +8 and +32 mean; the first word is only seen
// as the non-empty test.
extern "C" int compareToItem__Q24item11descriptionPQ24item11description(const void* left, const void* right) {
    const unsigned* l = static_cast<const unsigned*>(left);
    const unsigned* r = static_cast<const unsigned*>(right);
    const unsigned first = r[0];
    if (first == 0)
        return 0;
    const unsigned long long* l64 = reinterpret_cast<const unsigned long long*>(l + 2);
    const unsigned long long* r64 = reinterpret_cast<const unsigned long long*>(r + 2);
    if (*l64 != *r64)
        return 0;
    if (l[0] != first)
        return 0;
    return l[8] == r[8] ? 1 : 0;
}
