// Observed: Type__C7VISound at 0x0117b7c0 is 8 bytes. It returns the word at
// offset 8 of the object and changes nothing. Inferred from the callers' use:
// 0 selects a wave sound and 1 an XM sound (see AsWave__7VISound and
// AsXm__7VISound). The sizes and other fields are unknown.
extern "C" {
int Type__C7VISound(const void* object) {
    return *reinterpret_cast<const int*>(static_cast<const char*>(object) + 8);
}
}
