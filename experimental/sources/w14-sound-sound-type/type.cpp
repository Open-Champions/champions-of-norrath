// Observed: Type__C7VISound at 0x0117b7c0, 8 bytes. It returns the word at +8 of the object and touches nothing else.
// Inferred: that word is the sound kind. AsWave__7VISound and AsXm__7VISound compare it with 0 and 1, so 0 is the wave kind
// and 1 the XM kind by those two callers only.
// Unknown: the enum behind the word and the rest of the object layout.
extern "C" {
int Type__C7VISound(const void* sound) {
    return static_cast<const int*>(sound)[2];
}
}
