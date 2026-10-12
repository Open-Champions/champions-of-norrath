// Observed: netBlankController__Fii at 0x00135710 is 68 bytes. It addresses a
// 12-byte record of padBlank by the first argument. The record's first word
// becomes the larger of its old value and the second argument, its second word
// becomes -1 and its third word becomes 1. The second argument is compared as
// signed.
// Inferred: nothing.
// Unknown: what a nonzero third word makes the reader do.
struct PadBlankRecord {
    int count;
    int buttons;
    int flag;
};

extern "C" PadBlankRecord padBlank[4];

extern "C" void netBlankController__Fii(int pad, int count) {
    PadBlankRecord* record = &padBlank[pad];
    int old = record->count;
    record->flag = 1;
    record->count = count > old ? count : old;
    record->buttons = -1;
}
