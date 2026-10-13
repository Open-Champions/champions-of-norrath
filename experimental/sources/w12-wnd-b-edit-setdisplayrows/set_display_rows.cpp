// Observed: SetDisplayRows__9VIWndEditi at 0x010ad558 is 8 bytes: it stores its integer argument at
// byte offset 392 (0x188) of the object and returns nothing. No clamping or other call.
// Inferred: the word is the number of text rows the edit window shows, from the name.
// Unknown: whether other code recomputes layout after the store (this routine does not).
extern "C" void SetDisplayRows__9VIWndEditi(void* self, int rows) {
    *reinterpret_cast<int*>(static_cast<unsigned char*>(self) + 392) = rows;
}
