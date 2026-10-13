// Observed lgKbdEnumHint at 0x002e73c8, 48 bytes, from the keyboard library the image links in.
// The library state is the object GLibData (0x008ebdd8). With a null destination it returns
// 0x80000004 and touches nothing. Otherwise it stores the word at offset 100 through the
// destination, clears that word, and returns 0.
// Unknown: what writes the word at offset 100 and what the value means. The caller in
// VIKeyboard compares the delivered word with 1 and closes the device on a match, which is
// the only use seen. Nothing was executed on the EE.
extern "C" {
extern unsigned char GLibData[];

int lgKbdEnumHint(int* hint) {
    if (hint == 0) {
        return static_cast<int>(0x80000004u);
    }
    int* hint_word = reinterpret_cast<int*>(GLibData + 100);
    *hint = *hint_word;
    *hint_word = 0;
    return 0;
}
}
