// Observed lumpCheck__Fv at 0x001663b8, 52 bytes. It reads the signed word
// nmLumps (0x00483f8c). When that is above zero it runs a counting loop that
// executes nmLumps times and has no stores, no calls and no other reads, then
// returns nothing. Nothing is written anywhere.
// Inferred: the loop is what is left of a per-lump consistency check whose body
// is not present in this build; the name suggests that but the image shows only
// the empty iteration.
// Unknown: what the check once did, and whether the iteration count matters.
#ifdef NORRATH_W8_HOST
extern unsigned norrath_w8_lump_check_steps;
#define NORRATH_W8_LUMP_CHECK_STEP() (++norrath_w8_lump_check_steps)
#else
#define NORRATH_W8_LUMP_CHECK_STEP() __asm__ volatile("")
#endif

extern "C" {
extern int nmLumps;

void lumpCheck__Fv() {
    for (int remaining = nmLumps; remaining > 0; --remaining) {
        NORRATH_W8_LUMP_CHECK_STEP();
    }
}
}
