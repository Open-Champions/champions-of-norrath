/* Observed get_time at 0x002e7810, 56 bytes. It forms a 64-bit value from
 * rollover_counter (0x00447f50) shifted left 16 ORed with the low 16 bits of the
 * word at 0x10000000, calls __udivdi3 with that value and 576, and returns the
 * 64-bit quotient. lgKbMInit stores its address as the keyboard library's time
 * source when the caller supplies none.
 * Inferred, from the public EE timer map: 0x10000000 is the timer-0 count, which
 * init_ee_timer configures to count at 1/256 of the bus clock (147.456 MHz), so
 * dividing by 576 gives milliseconds.
 * Unknown: whether the two reads can tear across an overflow. Compiled output is
 * not claimed to match the original.
 */
#if defined(NORRATH_HOST_CONTRACT)
extern "C" volatile unsigned norrath_host_timer0_count;
#define NORRATH_TIMER0_COUNT (norrath_host_timer0_count)
#else
#define NORRATH_TIMER0_COUNT (*(volatile unsigned*)0x10000000u)
#endif
extern "C" {
extern unsigned long long rollover_counter;
unsigned long long __udivdi3(unsigned long long numerator, unsigned long long denominator);

unsigned long long get_time() {
    const unsigned long long high = rollover_counter << 16;
    const unsigned long long low = NORRATH_TIMER0_COUNT & 0xffffu;
    return __udivdi3(high | low, 576u);
}
}
