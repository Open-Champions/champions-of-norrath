// Observed msg_hurt__13AutoContainerfP10GameObjecti at 0x2250f0, 92 bytes. The float,
// the object pointer and the int are not read. If the word at object+316 is neither 1
// nor 4 and bit 0 of the word at object+292 is set, it adds the object to the run list,
// then stores 1 at object+316 and 0 at object+304. Otherwise it does nothing.
// Unknown: what the +316 states and the +304 counter mean.

extern "C" void objectAddToRunList__FP10GameObject(void* object);

typedef unsigned Word __attribute__((may_alias));

extern "C" void msg_hurt__13AutoContainerfP10GameObjecti(void* self, float amount, void* source, int kind) {
    (void)amount;
    (void)source;
    (void)kind;
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned state = *reinterpret_cast<Word*>(object + 316);
    if (state == 1 || state == 4) return;
    if ((*reinterpret_cast<Word*>(object + 292) & 1) == 0) return;
    objectAddToRunList__FP10GameObject(self);
    *reinterpret_cast<Word*>(object + 316) = 1;
    *reinterpret_cast<Word*>(object + 304) = 0;
}
