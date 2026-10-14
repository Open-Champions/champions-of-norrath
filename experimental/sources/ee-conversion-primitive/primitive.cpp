// Independently authored EE architectural primitive; private feasibility only.
// Returns instruction result bits; conversion/status behavior belongs to the CPU.
static_assert(sizeof(float) == 4, "requires target float word");
static_assert(sizeof(unsigned) == 4 && ~0u == 0xffffffffu, "requires unsigned32");
extern "C" unsigned ee_cvt_w_s_word(float value)
{
    unsigned result;
    unsigned fpr_word;
    __asm__ volatile (
        "cvt.w.s %1, %2\n\t"
        "mfc1 %0, %1"
        : "=r" (result), "=&f" (fpr_word)
        : "f" (value));
    return result;
}
