// Observed: OnKeyUp__9VIWndEulai at 0x010b58e0 is 8 bytes: it stores a zero word at byte offset 9756
// (0x261c) of the window object and returns. The integer argument is not read.
// Inferred: the word is state of the EULA window that a key release clears.
// Unknown: the field's meaning, and what the integer argument is.
extern "C" void OnKeyUp__9VIWndEulai(void* self, int) {
    *reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 9756) = 0;
}
