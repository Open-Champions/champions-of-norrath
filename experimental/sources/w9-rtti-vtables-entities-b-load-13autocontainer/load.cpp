// Observed msg_load__13AutoContainerPc at 0x224fc8, 104 bytes. Reads a word from the
// stream and stores it at object+316 (the word msg_save__13AutoContainerPc writes).
// If the word is 2, it also clears bit 20 (0x100000) of the flag word at object+12,
// stores 180 at object+304 and adds the object to the run list. Returns 4, the bytes
// consumed. Unknown: what the values of the +316 word mean beyond the tests in this
// and the sibling methods (1 and 4 are checked by hurt, 3 is set by poke).

extern "C" void objectAddToRunList__FP10GameObject(void* object);

typedef unsigned Word __attribute__((may_alias));

extern "C" int msg_load__13AutoContainerPc(void* self, const char* in) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned value = *reinterpret_cast<const Word*>(in);
    *reinterpret_cast<Word*>(object + 316) = value;
    if (value == 2) {
        *reinterpret_cast<Word*>(object + 304) = 180;
        *reinterpret_cast<Word*>(object + 12) &= 0xffefffffu;
        objectAddToRunList__FP10GameObject(self);
    }
    return 4;
}
