// Observed: VIEEFileDelayTh__FiUsPv at 0x010cb560 is 36 bytes. Arguments (id, time, common); only
// the third is used. It calls iSignalSema(common), then executes `sync` and `ei`, and returns
// with the register still holding the iSignalSema result. It is the alarm callback armed by
// VIEEFileDelay__Fi, whose `common` argument is the delay's semaphore id.
// Inferred: the `sync; ei` pair is the SDK's end-of-handler sequence for alarm callbacks that
// run with interrupts disabled. The returned value is incidental: this unit returns the
// iSignalSema result because that is what the register holds; it is not shown to be read by
// the SDK as a re-arm time.
// Unknown: whether the original declares an int or unsigned return, and why `ei` is explicit.
#ifndef NORRATH_END_OF_HANDLER
#define NORRATH_END_OF_HANDLER() __asm__ volatile("sync\n\tei" ::: "memory")
#endif
extern "C" {
int iSignalSema(int semaphore);

unsigned VIEEFileDelayTh__FiUsPv(int, unsigned short, void* common) {
    const int result = iSignalSema(static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(common)));
    NORRATH_END_OF_HANDLER();
    return static_cast<unsigned>(result);
}
}
