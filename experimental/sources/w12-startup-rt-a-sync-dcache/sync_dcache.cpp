/* Observed SyncDCache at 0x002d59d8, 124 bytes, pinned SLUS_205.65. Takes a begin and an end address.
 * It samples COP0 Status bit 0x10000 (the interrupt-enable bit), calls DIntr only when the bit was set,
 * hands both addresses rounded down to a multiple of 64 to the local cache routine _sceSDC at
 * 0x002d5930, and calls EIntr only when the bit was set. The EIntr call is the last act, so its result
 * is the result; when the bit was clear nothing is returned. Both addresses are masked, so the end
 * address is rounded down, not up.
 * Inferred: the pair describes a byte range whose lines are written back; _sceSDC's own behaviour is
 * not authored here.
 */
extern "C" int DIntr();
extern "C" int EIntr();
extern "C" void _sceSDC(unsigned begin, unsigned end);

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_status_word;
#define NORRATH_STATUS_WORD (norrath_host_status_word)
#else
static inline unsigned norrath_status_word() {
    unsigned long long status;
    asm volatile("mfc0 %0, $12" : "=r"(status) : : "memory");
    return static_cast<unsigned>(status);
}
#define NORRATH_STATUS_WORD (norrath_status_word())
#endif

extern "C" void SyncDCache(void* begin, void* end) {
    const unsigned interrupts_enabled = NORRATH_STATUS_WORD & 0x10000u;
    if (interrupts_enabled != 0u)
        DIntr();
    _sceSDC(static_cast<unsigned>(reinterpret_cast<unsigned long>(begin)) & 0xffffffc0u,
           static_cast<unsigned>(reinterpret_cast<unsigned long>(end)) & 0xffffffc0u);
    if (interrupts_enabled != 0u)
        EIntr();
}
