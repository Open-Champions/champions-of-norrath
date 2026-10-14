// Observed: AsWave__7VISound at 0x0117b7c8, 16 bytes. It returns its argument when the word at +8 is zero and a null pointer
// for any other value. Nothing is called or written.
// Inferred: the word at +8 is the kind read by Type__C7VISound, and zero is the wave kind; the result is the same object seen as
// a wave.
// Unknown: whether the original is a cast helper or a virtual-free downcast; the base/derived layout is not examined here.
extern "C" {
void* AsWave__7VISound(void* sound) {
    return static_cast<int*>(sound)[2] == 0 ? sound : nullptr;
}
}
