// Observed checkCanAllocDescriptor__Fv at 0x00187268, 88 bytes, a file-local
// symbol in the image. It scans the 32 words of descListMark (0x00780280) from
// slot 0 upward and stops at the first word equal to 0xffffffff. It returns 1
// when such a word exists and 0 when all 32 words differ. It stores nothing.
// Inferred: a word of 0xffffffff marks an unused descriptor slot, which matches
// the reset that P_Clear__Fv writes to the same words.
// Unknown: the meaning of the other values.
extern "C" {
extern unsigned descListMark[32];

int checkCanAllocDescriptor__Fv() {
    int slot = 0;
    while (slot < 32 && descListMark[slot] != 0xffffffffu) ++slot;
    return slot != 32;
}
}
