// Observed: AsXm__7VISound at 0x0117b7d8, 20 bytes. It returns its argument when the word at +8 equals 1 and a null pointer
// otherwise. Nothing is called or written.
// Inferred: the word at +8 is the kind read by Type__C7VISound, and one is the XM kind.
// Unknown: the enum behind the word and the object layout.
extern "C" {
void* AsXm__7VISound(void* sound) {
    return static_cast<int*>(sound)[2] == 1 ? sound : nullptr;
}
}
