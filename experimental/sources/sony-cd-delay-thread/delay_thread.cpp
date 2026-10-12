// Observed DelayThread at 0x002d6140, 200 bytes, and its local alarm handler at
// 0x002d6208. Bit 0x10000 of the status register must be set; otherwise the
// result is 0x80008008. The semaphore is created with initial count 1, maximum
// 0, and the name "DelayThread". A negative identifier returns 0x80008003.
// TimerUSec2BusClock is called with a zero first argument. A negative alarm
// deletes the semaphore and returns that result. Otherwise this waits and deletes
// it. The handler signals its fourth argument with iSignalSema, then sync and ei.
extern "C" {
int CreateSema(void* parameters);
int DeleteSema(int semaphore);
int norrath_player_wait_semaphore(int semaphore);
int TimerUSec2BusClock(int unused, int microseconds);
int iSignalSema(int semaphore);
int SetTimerAlarm(int clocks, void (*handler)(int, int, int, int), int semaphore);
#if defined(NORRATH_HOST_CONTRACT)
unsigned norrath_sony_cd_status();
void norrath_sony_cd_after_signal();
#else
static unsigned status_register() {
    unsigned value;
    asm volatile("mfc0 %0, $12" : "=r"(value));
    return value;
}
#endif
static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}
static unsigned pointer_bits(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer));
}
static void delay_handler(int, int, int, int semaphore) {
    iSignalSema(semaphore);
#if defined(NORRATH_HOST_CONTRACT)
    norrath_sony_cd_after_signal();
#else
    asm volatile("sync" ::: "memory");
    asm volatile("ei" ::: "memory");
#endif
}
int DelayThread(int microseconds) {
#if defined(NORRATH_HOST_CONTRACT)
    const unsigned status = norrath_sony_cd_status();
#else
    const unsigned status = status_register();
#endif
    if ((status & 0x10000u) == 0u) return static_cast<int>(0x80008008u);
    unsigned char workspace[24];
    store_word(workspace + 4, 1u);
    store_word(workspace + 20, pointer_bits("DelayThread"));
    store_word(workspace + 8, 0u);
    const int semaphore = CreateSema(workspace);
    if (semaphore < 0) return static_cast<int>(0x80008003u);
    const int clocks = TimerUSec2BusClock(0, microseconds);
    const int alarm = SetTimerAlarm(clocks, delay_handler, semaphore);
    if (alarm < 0) {
        DeleteSema(semaphore);
        return alarm;
    }
    norrath_player_wait_semaphore(semaphore);
    DeleteSema(semaphore);
    return 0;
}
}
