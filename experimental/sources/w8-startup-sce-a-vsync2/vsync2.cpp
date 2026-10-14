/* Observed VSync2 at 0x002d5600, 164 bytes. The incoming argument is not read.
 * A word and a 64-bit slot on the stack are handed to SetVSyncFlag
 * (0x002d5450, service 115) after the word is cleared. The word at 0x1000f000
 * is then written with 4 under DIntr/EIntr (DIntr's result decides whether
 * EIntr runs), with a sync. The function polls until bit 4 of that register is
 * set or the stack word becomes nonzero. A second DIntr, a repeat write of 4,
 * sync, and EIntr on the same condition follow, and the 64-bit slot is
 * returned. What the kernel writes into the word and the slot is unknown, and
 * the slot's content is not described here. Compiled output is not claimed to
 * match the original.
 */
extern "C" int DIntr();
extern "C" int EIntr();
extern "C" void SetVSyncFlag(volatile unsigned* flag, volatile unsigned long long* value);

#if defined(NORRATH_HOST_CONTRACT)
extern "C" unsigned norrath_host_intc_read();
extern "C" void norrath_host_intc_write(unsigned value);
extern "C" void norrath_host_pipeline_sync();
#define NORRATH_INTC_READ() norrath_host_intc_read()
#define NORRATH_INTC_WRITE(value) norrath_host_intc_write(value)
#define NORRATH_SYNC() norrath_host_pipeline_sync()
#else
#define NORRATH_INTC_READ() (*reinterpret_cast<volatile unsigned*>(0x1000f000u))
#define NORRATH_INTC_WRITE(value) (*reinterpret_cast<volatile unsigned*>(0x1000f000u) = (value))
#define NORRATH_SYNC() asm volatile("sync" ::: "memory")
#endif

extern "C" unsigned long long VSync2(int) {
    volatile unsigned flag = 0u;
    volatile unsigned long long value;
    SetVSyncFlag(&flag, &value);
    int enabled = DIntr();
    NORRATH_INTC_WRITE(4u);
    NORRATH_SYNC();
    if (enabled != 0) EIntr();
    while ((NORRATH_INTC_READ() & 4u) == 0u && flag == 0u) {
    }
    enabled = DIntr();
    NORRATH_INTC_WRITE(4u);
    NORRATH_SYNC();
    if (enabled != 0) EIntr();
    return value;
}
