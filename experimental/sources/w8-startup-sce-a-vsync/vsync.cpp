/* Observed VSync at 0x002d5570, 144 bytes. The incoming argument is not read.
 * With interrupts disabled around each step (DIntr's result decides whether
 * EIntr runs), the word at 0x1000f000 is written with 4, then sync. After
 * interrupts are restored, the same word is read until bit 4 is set. A second
 * DIntr, a repeat write of 4 and sync follow, and EIntr is tail-called when
 * the second DIntr reported enabled interrupts. The original returns whatever
 * the last call left in the result register; this source returns EIntr's
 * result in that case and 0 otherwise, which is DIntr's result there. The
 * register at 0x1000f000 is read here as the interrupt status word of the
 * interrupt controller; that name is inferred. Compiled output is not claimed
 * to match the original.
 */
extern "C" int DIntr();
extern "C" int EIntr();

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

extern "C" int VSync(int) {
    int enabled = DIntr();
    NORRATH_INTC_WRITE(4u);
    NORRATH_SYNC();
    if (enabled != 0) EIntr();
    while ((NORRATH_INTC_READ() & 4u) == 0u) {
    }
    enabled = DIntr();
    NORRATH_INTC_WRITE(4u);
    NORRATH_SYNC();
    if (enabled != 0) return EIntr();
    return 0;
}
