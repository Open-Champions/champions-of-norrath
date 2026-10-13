/* Observed init_ee_timer at 0x002e7848, 76 bytes. It calls AddIntcHandler with 9,
 * the address of counter_overflow_handler (0x002e7898) and 0, stores the result
 * in timer_handle (0x00447f58), writes 0 to the word at 0x10000000 and 0x282 to
 * the word at 0x10000010 (the order is: handle store, count, mode), then calls
 * EnableIntc(9) as a tail call and returns its result. lgKbMInit calls it when
 * the caller supplies no time source.
 * Inferred, from the public EE timer map: the two addresses are the timer-0 count
 * and mode registers; 0x282 selects clock 1/256 of the bus clock, count enable and
 * overflow interrupt; cause 9 is the timer-0 interrupt.
 * Unknown: the third AddIntcHandler argument (passed as 0). Compiled output is not
 * claimed to match the original.
 */
#if defined(NORRATH_HOST_CONTRACT)
extern "C" volatile unsigned norrath_host_timer0_count;
extern "C" volatile unsigned norrath_host_timer0_mode;
#define NORRATH_TIMER0_COUNT (norrath_host_timer0_count)
#define NORRATH_TIMER0_MODE (norrath_host_timer0_mode)
#else
#define NORRATH_TIMER0_COUNT (*(volatile unsigned*)0x10000000u)
#define NORRATH_TIMER0_MODE (*(volatile unsigned*)0x10000010u)
#endif
extern "C" {
extern int timer_handle;
int counter_overflow_handler();
int AddIntcHandler(int cause, int (*handler)(), int next);
int EnableIntc(int cause);

int init_ee_timer() {
    timer_handle = AddIntcHandler(9, counter_overflow_handler, 0);
    NORRATH_TIMER0_COUNT = 0;
    NORRATH_TIMER0_MODE = 0x282;
    return EnableIntc(9);
}
}
