/* Observed counter_overflow_handler at 0x002e7898, 52 bytes (a local symbol). It adds
 * one to the 64-bit word rollover_counter (0x00447f50), reads the word at
 * 0x10000010 and writes the value it read back to the same address, executes sync
 * and then the interrupt-enable instruction, and returns 0. Its address is passed
 * by init_ee_timer to AddIntcHandler for cause 9.
 * Inferred, from the public EE timer map: 0x10000010 is the timer-0 mode register
 * and writing the read value back clears its write-one-to-clear overflow flags;
 * cause 9 is the timer-0 interrupt.
 * Unknown: whether the enable instruction is needed by the dispatcher that calls
 * it. Compiled output is not claimed to match the original.
 */
#if defined(NORRATH_HOST_CONTRACT)
extern "C" volatile unsigned norrath_host_timer0_mode;
#define NORRATH_TIMER0_MODE (norrath_host_timer0_mode)
#define NORRATH_SYNC_EI() (norrath_host_sync_ei())
extern "C" void norrath_host_sync_ei();
#else
#define NORRATH_TIMER0_MODE (*(volatile unsigned*)0x10000010u)
#define NORRATH_SYNC_EI() asm volatile("sync\n\tei" ::: "memory")
#endif
extern "C" {
extern unsigned long long rollover_counter;

int counter_overflow_handler() {
    rollover_counter += 1;
    const unsigned mode = NORRATH_TIMER0_MODE;
    NORRATH_TIMER0_MODE = mode;
    NORRATH_SYNC_EI();
    return 0;
}
}
