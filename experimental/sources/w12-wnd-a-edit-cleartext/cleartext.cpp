// Observed: ClearText__9VIWndEdit at 0x010ad5f0 is 36 bytes. It reads the pointer word at
// this+424, stores a zero halfword through it, then calls SetCaretOffset__9VIWndEditi(this, 0).
// Inferred: this+424 points at the edit's 16-bit text buffer, so this empties it and homes the caret.
// Unknown: the buffer's owner and capacity.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_ADDRESS(word) reinterpret_cast<__UINTPTR_TYPE__>(norrath_host_pointer(word))
#else
#define NORRATH_WORD_ADDRESS(word) static_cast<__UINTPTR_TYPE__>(word)
#endif
extern "C" {
void SetCaretOffset__9VIWndEditi(void* self, int offset);

void ClearText__9VIWndEdit(void* self) {
    unsigned short* text = reinterpret_cast<unsigned short*>(
        NORRATH_WORD_ADDRESS(*reinterpret_cast<unsigned*>(static_cast<unsigned char*>(self) + 424)));
    *text = 0;
    SetCaretOffset__9VIWndEditi(self, 0);
}
}
