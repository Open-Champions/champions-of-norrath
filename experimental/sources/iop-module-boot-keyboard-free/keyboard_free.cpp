// Observed Free__10VIKeyboard at 0x002cbf88, 80 bytes. It prints the free
// banner, then reads the word at offset 4. When that word is not -1 it calls
// lgKbdClose with the word and stores -1 there. When the word is already -1
// it does not call and does not store. Other object bytes are not written.
extern "C" int printf(const char* text, ...);
extern "C" void lgKbdClose(int handle);

extern "C" void Free__10VIKeyboard(void* keyboard) {
    auto* words = static_cast<unsigned*>(keyboard);
    printf("======= FREE KEYBOARD =======\n");
    const unsigned handle = words[1];
    if (handle != 0xffffffffu) {
        lgKbdClose(static_cast<int>(handle));
        words[1] = 0xffffffffu;
    }
}
