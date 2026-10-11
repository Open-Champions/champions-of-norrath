/* Observed gsIntHandler__Fi at 0x0015fac8, 40 bytes. The cause is unread.
 * reloadDMA__Fi is called with -2, then sync and ei run, and the result is 1.
 * Fresh compilation is not an original-byte match.
 */
extern "C" void reloadDMA__Fi(int cause);

#if defined(NORRATH_HOST_CONTRACT)
extern "C" void norrath_host_pipeline_sync();
extern "C" void norrath_host_enable_interrupts();
#define NORRATH_SYNC() norrath_host_pipeline_sync()
#define NORRATH_EI() norrath_host_enable_interrupts()
#else
static inline void norrath_pipeline_sync() {
    asm volatile("sync" ::: "memory");
}
static inline void norrath_enable_interrupts() {
    asm volatile("ei" ::: "memory");
}
#define NORRATH_SYNC() norrath_pipeline_sync()
#define NORRATH_EI() norrath_enable_interrupts()
#endif

extern "C" int gsIntHandler__Fi(int) {
    reloadDMA__Fi(-2);
    NORRATH_SYNC();
    NORRATH_EI();
    return 1;
}
