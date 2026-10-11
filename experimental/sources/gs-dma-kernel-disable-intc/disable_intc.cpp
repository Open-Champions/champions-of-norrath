/* Observed DisableIntc at 0x002d5bc0, 104 bytes. Same Status bit 0x10000
 * sample as EnableIntc, then DIntr when that bit is set, service 21 with the
 * incoming cause, sync, and EIntr on the same bit. The original requests
 * service 21 through _DisableIntc at 0x002d4e30. This source issues that
 * service directly. It is not an original-byte match.
 */
extern "C" int DIntr();
extern "C" int EIntr();

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_status_word;
extern "C" unsigned long long norrath_host_intc_service(int service, int cause);
extern "C" void norrath_host_pipeline_sync();
#define NORRATH_STATUS_WORD (norrath_host_status_word)
#define NORRATH_INTC_SERVICE(service, cause) norrath_host_intc_service((service), (cause))
#define NORRATH_SYNC() norrath_host_pipeline_sync()
#else
static inline unsigned norrath_status_word() {
    unsigned long long status;
    asm volatile("mfc0 %0, $12" : "=r"(status) : : "memory");
    return static_cast<unsigned>(status);
}
static inline unsigned long long norrath_intc_service(int service, int cause) {
    register unsigned long long result asm("$2");
    register int operation asm("$3") = service;
    register int argument asm("$4") = cause;
    asm volatile("syscall"
                 : "=r"(result), "+r"(operation), "+r"(argument)
                 :
                 : "$5", "$6", "$7", "$8", "$9", "$10", "$11",
                   "$12", "$13", "$14", "$15", "$24", "$25", "hi", "lo", "memory");
    return result;
}
static inline void norrath_pipeline_sync() {
    asm volatile("sync" ::: "memory");
}
#define NORRATH_STATUS_WORD (norrath_status_word())
#define NORRATH_INTC_SERVICE(service, cause) norrath_intc_service((service), (cause))
#define NORRATH_SYNC() norrath_pipeline_sync()
#endif

extern "C" unsigned long long DisableIntc(int cause) {
    const unsigned interrupts_enabled = NORRATH_STATUS_WORD & 0x10000u;
    if (interrupts_enabled != 0u)
        DIntr();
    const unsigned long long result = NORRATH_INTC_SERVICE(21, cause);
    NORRATH_SYNC();
    if (interrupts_enabled != 0u)
        EIntr();
    return result;
}
