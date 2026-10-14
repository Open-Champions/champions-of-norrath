/* Observed DisableDmac at 0x002d5c90, 104 bytes. It samples Status bit 0x10000,
 * calls DIntr when that bit is set, calls _DisableDmac (0x002d4e50, service 23)
 * with the incoming argument, executes sync, and calls EIntr on the same
 * sampled bit. The result of _DisableDmac is returned. What the argument
 * means is unknown. Compiled output is not claimed to match the original.
 */
extern "C" int DIntr();
extern "C" int EIntr();
extern "C" int _DisableDmac(int channel);

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_status_word;
extern "C" void norrath_host_pipeline_sync();
#define NORRATH_STATUS_WORD (norrath_host_status_word)
#define NORRATH_SYNC() norrath_host_pipeline_sync()
#else
static inline unsigned norrath_status_word() {
    unsigned status;
    asm volatile("mfc0 %0, $12" : "=r"(status) : : "memory");
    return status;
}
#define NORRATH_STATUS_WORD (norrath_status_word())
#define NORRATH_SYNC() asm volatile("sync" ::: "memory")
#endif

extern "C" int DisableDmac(int channel) {
    const unsigned interrupts_enabled = NORRATH_STATUS_WORD & 0x10000u;
    if (interrupts_enabled != 0u)
        DIntr();
    const int result = _DisableDmac(channel);
    NORRATH_SYNC();
    if (interrupts_enabled != 0u)
        EIntr();
    return result;
}
