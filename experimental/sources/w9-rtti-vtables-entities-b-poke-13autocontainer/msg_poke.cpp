// Observed msg_poke__13AutoContaineri at 0x225090, 96 bytes. The int is not read.
// Does nothing unless bit 2 (value 4) of the word at object+292 is set. Then it clears
// bit 20 (0x100000) of the flag word at object+12; and if the word at object+316 is
// zero it stores 3 there, adds the object to the run list and stores 180 at object+304.
// Unknown: what the +316 states and the +304 counter mean.

extern "C" void objectAddToRunList__FP10GameObject(void* object);

typedef unsigned Word __attribute__((may_alias));

extern "C" void msg_poke__13AutoContaineri(void* self, int value) {
    (void)value;
    unsigned char* object = static_cast<unsigned char*>(self);
    if ((*reinterpret_cast<Word*>(object + 292) & 4) == 0) return;
    *reinterpret_cast<Word*>(object + 12) &= 0xffefffffu;
    if (*reinterpret_cast<Word*>(object + 316) != 0) return;
    *reinterpret_cast<Word*>(object + 316) = 3;
    objectAddToRunList__FP10GameObject(self);
    *reinterpret_cast<Word*>(object + 304) = 180;
}
