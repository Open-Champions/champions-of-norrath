// Observed: QueueKeyEvent__10VIKeyboardP12VIConsoleKey at 0x002cc070, 64 bytes. Arguments: the keyboard and a
// pointer to a two-byte console key record. The count word at offset 12 is read; when it is below 128 (signed),
// the two signed bytes of the record are copied to the byte pair at (word at offset 8) + count * 2 and the
// count is incremented. Otherwise nothing is stored. No result.
// Inferred: a fixed 128-entry queue of two-byte key events, drop-on-full, with the buffer pointer at offset 8
// and the fill count at offset 12.
// Unknown: the meaning of the two record bytes, and whether anything drains the queue other than Read.
#ifdef NORRATH_HOST_ARENA
extern "C" unsigned char* norrath_host_pointer(unsigned address);
#define NORRATH_WORD_TO_POINTER(word) norrath_host_pointer(word)
#else
#define NORRATH_WORD_TO_POINTER(word) reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(word))
#endif

extern "C" {
struct VIKeyboard;
struct VIConsoleKey;

void QueueKeyEvent__10VIKeyboardP12VIConsoleKey(VIKeyboard* self, VIConsoleKey* key) {
    char* base = reinterpret_cast<char*>(self);
    int& count = *reinterpret_cast<int*>(base + 12);
    if (count < 128) {
        signed char* slot = reinterpret_cast<signed char*>(
            NORRATH_WORD_TO_POINTER(*reinterpret_cast<unsigned*>(base + 8))) + count * 2;
        const signed char* source = reinterpret_cast<const signed char*>(key);
        slot[0] = source[0];
        slot[1] = source[1];
        count = count + 1;
    }
}
}
