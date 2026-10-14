// Observed: SetRepeat__4VIXmi at 0x0113a870 is 8 bytes. It stores its second
// argument as a word at offset 52 of the object in the first argument and
// returns nothing.
// Inferred: the word is a repeat count or flag of the Xmi music object; only
// the offset is established.
// Unknown: the object's size, other fields and the meaning of the value.
struct VIXmi {
    unsigned char before[52];
    int repeat;
};

extern "C" void SetRepeat__4VIXmi(VIXmi* self, int repeat) {
    self->repeat = repeat;
}
