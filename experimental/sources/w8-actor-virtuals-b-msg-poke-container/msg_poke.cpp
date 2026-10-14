// Observed msg_poke__9Containeri at 0x0021da08, 96 bytes. The int argument is not read.
// Does nothing unless bit 2 (value 4) of the word at object+104 is set.
// When set: clears bit 20 (0x100000) of the flag word at object+12. Then, only if the
// word at object+128 is zero, stores 3 there, adds the object to the run list, and
// stores 180 at object+116.
// Unknown: what the fields mean (+128 is a state word, +116 a counter in the image).

extern "C" void objectAddToRunList__FP10GameObject(void* object);

namespace {
typedef int Word __attribute__((may_alias));
}

extern "C" void msg_poke__9Containeri(void* self, int value) {
    (void)value;
    unsigned char* object = static_cast<unsigned char*>(self);
    Word* flags = reinterpret_cast<Word*>(object + 12);
    Word* options = reinterpret_cast<Word*>(object + 104);
    Word* state = reinterpret_cast<Word*>(object + 128);
    Word* counter = reinterpret_cast<Word*>(object + 116);
    if ((*options & 4) == 0) return;
    int was = *state;
    *flags &= static_cast<int>(0xffefffffu);
    if (was == 0) {
        *state = 3;
        objectAddToRunList__FP10GameObject(self);
        *counter = 180;
    }
}
