// Observed msg_hurt__9ContainerfP10GameObjecti at 0x0021d870, 144 bytes.
// The float and the GameObject pointer are not read; only the int (call it flags).
// Nothing happens when the word at object+128 is already 1. Otherwise, with options
// = the word at object+104:
//   bit 2 (value 4) set: add the object to the run list, store 1 at +128, store 0 at
//     +116, then store 7 at +116 if flags has bit 11 (0x800).
//   else bit 0 (value 1) set: add the object to the run list, store 1 at +128 and 0 at +116.
//   else: nothing.
// Unknown: what the fields and the 0x800 flag mean.

extern "C" void objectAddToRunList__FP10GameObject(void* object);

namespace {
typedef int Word __attribute__((may_alias));
}

extern "C" void msg_hurt__9ContainerfP10GameObjecti(void* self, float amount, void* attacker, int flags) {
    (void)amount;
    (void)attacker;
    unsigned char* object = static_cast<unsigned char*>(self);
    Word* state = reinterpret_cast<Word*>(object + 128);
    Word* counter = reinterpret_cast<Word*>(object + 116);
    if (*state == 1) return;
    int options = *reinterpret_cast<Word*>(object + 104);
    if ((options & 4) != 0) {
        objectAddToRunList__FP10GameObject(self);
        *state = 1;
        *counter = 0;
        if ((flags & 0x800) != 0) *counter = 7;
    } else if ((options & 1) != 0) {
        objectAddToRunList__FP10GameObject(self);
        *state = 1;
        *counter = 0;
    }
}
