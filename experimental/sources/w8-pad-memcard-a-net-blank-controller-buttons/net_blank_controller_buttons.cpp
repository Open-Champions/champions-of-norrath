// Observed: netBlankControllerButtons__Fiii at 0x00135758 is 60 bytes. It
// addresses a 12-byte record of padBlank by the first argument. The record's
// first word becomes the larger (signed) of its old value and the second
// argument, its second word becomes the third argument and its third word
// becomes 0.
// Inferred: the counterpart of netBlankController__Fii that carries a button
// word and clears the third word instead of setting it.
// Unknown: what the button word is compared against.
struct PadBlankRecord {
    int count;
    int buttons;
    int flag;
};

extern "C" PadBlankRecord padBlank[4];

extern "C" void netBlankControllerButtons__Fiii(int pad, int count, int buttons) {
    PadBlankRecord* record = &padBlank[pad];
    int old = record->count;
    record->flag = 0;
    record->count = count > old ? count : old;
    record->buttons = buttons;
}
