// Observed IsEventModifier at 0x002e8d88, 72 bytes, from the keyboard library the image links in.
// Takes a pointer to a 16-byte event record and reads the unsigned halfword at offset 4. Returns 1 when
// that value equals any of the eight bytes of ModifierCodes (0x00447f60), 0 otherwise. The table is
// scanned from index 0 to 7 and stops at the first hit. Nothing else is read or written.
// Caller seen: lgKbdRead tests the saved event with it before repeating a key.
extern "C" {
extern unsigned char ModifierCodes[8];

int IsEventModifier(const unsigned char* event) {
    const unsigned key = *reinterpret_cast<const unsigned short*>(event + 4);
    for (int i = 0; i < 8; ++i) {
        if (key == ModifierCodes[i]) return 1;
    }
    return 0;
}
}
