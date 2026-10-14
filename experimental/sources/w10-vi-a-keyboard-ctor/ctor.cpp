// Observed: __10VIKeyboard at 0x002cbef0 is 12 bytes and returns nothing. It
// stores -1 in the word at offset 4 and writes nothing else.
// Unknown: what that word is (a device or port handle is plausible) and the
// size of VIKeyboard.
extern "C" {
void __10VIKeyboard(void* object) {
    static_cast<int*>(object)[1] = -1;
}
}
