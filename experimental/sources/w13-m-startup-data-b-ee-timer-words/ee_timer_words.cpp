// Observed data words of the EE timer state. The image holds rollover_counter at
// 0x00447f50 (8 bytes, the span up to timer_handle) and timer_handle at 0x00447f58
// (4 bytes, up to the next symbol). Both are zero in the image bytes before startup.
// Inferred: rollover_counter counts wraps of the 16-bit timer count, and timer_handle
// is the word that the timer setup stores. Both names are the observed image symbol names.
// Unknown: the signedness and internal split of rollover_counter (it is held as 8 bytes),
// and whether anything reads timer_handle. Nothing here ran on the EE.
extern "C" {
alignas(8) unsigned char rollover_counter[8];
int timer_handle;
}
