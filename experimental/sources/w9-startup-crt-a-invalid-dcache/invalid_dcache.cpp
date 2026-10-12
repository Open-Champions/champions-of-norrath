/* Observed InvalidDCache at 0x002d5b18, 124 bytes. It samples Status bit 0x10000
 * and calls DIntr when that bit is set. Both incoming addresses are rounded down
 * to a multiple of 64 (mask 0xffffffc0) and passed in order to _sceIDC
 * (0x002d5a70, which walks the 128 sets of the data cache). EIntr runs on the same
 * sampled bit, as a tail call. Nothing is returned meaningfully. What the second
 * address means (an end, a length-like bound) is not established. Compiled output
 * is not claimed to match the original.
 */
extern "C" int DIntr();
extern "C" int EIntr();
extern "C" void _sceIDC(unsigned first, unsigned second);

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_status_word;
#define NORRATH_STATUS_WORD (norrath_host_status_word)
#else
static inline unsigned norrath_status_word() {
    unsigned status;
    asm volatile("mfc0 %0, $12" : "=r"(status) : : "memory");
    return status;
}
#define NORRATH_STATUS_WORD (norrath_status_word())
#endif

extern "C" void InvalidDCache(unsigned first, unsigned second) {
    const unsigned interrupts_enabled = NORRATH_STATUS_WORD & 0x10000u;
    if (interrupts_enabled != 0u)
        DIntr();
    _sceIDC(first & 0xffffffc0u, second & 0xffffffc0u);
    if (interrupts_enabled != 0u)
        EIntr();
}
