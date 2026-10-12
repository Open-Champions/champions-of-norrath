// Observed Free__10VIKeyboard at 0x002cbf88, 80 bytes. It prints the free
// banner, then reads the word at offset 4. When that word is not -1 it calls
// lgKbdClose with the word and stores -1 there. When the word is already -1
// it does not call and does not store. Other object bytes are not written.
extern "C" int printf(const char* text, ...);
extern "C" void lgKbdClose(int handle);

static unsigned load_word(const unsigned char* slot) {
    return slot[0] | (slot[1] << 8) | (slot[2] << 16) | (slot[3] << 24);
}

static void store_word(unsigned char* slot, unsigned value) {
    slot[0] = static_cast<unsigned char>(value);
    slot[1] = static_cast<unsigned char>(value >> 8);
    slot[2] = static_cast<unsigned char>(value >> 16);
    slot[3] = static_cast<unsigned char>(value >> 24);
}

extern "C" void Free__10VIKeyboard(void* keyboard) {
    auto* object = static_cast<unsigned char*>(keyboard);
    printf("======= FREE KEYBOARD =======\n");
    const unsigned handle = load_word(object + 4);
    if (handle != 0xffffffffu) {
        lgKbdClose(static_cast<int>(handle));
        store_word(object + 4, 0xffffffffu);
    }
}
