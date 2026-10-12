/* Observed TimerUSec2BusClock at 0x002df108, 112 bytes. The two incoming words
 * are widened to 64 bits. The first is multiplied by 0x08ca0000 (147,456,000)
 * and the second by the same constant and divided by 1,000,000 with the
 * library 64-bit unsigned division (__udivdi3). The result is the sum of the
 * two, as a 64-bit value. DelayThread passes 0 for the first word and its
 * microsecond count for the second, which is why the first is called seconds
 * here; that reading is inferred from the call, not from a document. Compiled
 * output is not claimed to match the original.
 */
extern "C" unsigned long long TimerUSec2BusClock(unsigned seconds, unsigned microseconds) {
    const unsigned long long rate = 147456000ull;
    return static_cast<unsigned long long>(seconds) * rate +
           static_cast<unsigned long long>(microseconds) * rate / 1000000ull;
}
