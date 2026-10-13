/* Observed: rollover_counter at 0x00447f50 (8 bytes) and timer_handle at 0x00447f58
 * (4 bytes), both in initialised data and zero in the image. The symbol table
 * carries these two names. counter_overflow_handler adds one to rollover_counter,
 * get_time reads it, and init_ee_timer stores AddIntcHandler's result in
 * timer_handle.
 * Inferred: rollover_counter counts overflows of the 16-bit timer-0 count; the
 * handle is the interrupt handler identifier.
 * Unknown: whether anything reads timer_handle (nothing authored does). Placement
 * differs from the original: zero-initialised definitions here land in
 * uninitialised data.
 */
extern "C" {
unsigned long long rollover_counter = 0;
int timer_handle = 0;
}
