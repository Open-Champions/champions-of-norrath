// Observed CheckforModifierKeyEvents at 0x002e8300, 400 bytes (a local routine of the keyboard reader), pinned
// SLUS_205.65. Called by process_kbd_data as fn(slot, record), before the key-list passes.
// The modifier state is the byte at record +11 (current) and the byte at slot +33 (previous, which the caller
// copies from the record afterwards). For each of the eight bits, lowest first, that differs between them:
//   - the key code is ModifierCodes[bit] (0xe0 + bit);
//   - the event type is 1 when the bit is set in the record's byte (pressed) and 2 when it is clear (released);
//   - the held-key bitmap at slot +224 gets that code's bit set for type 1 and cleared for type 2;
//   - when fewer than ten events are queued (unsigned halfword at slot +38) an entry is appended to the queue at
//     slot +40 with the same layout as the key-list passes: word 0 = record word 0, halfwords at +4 the code,
//     +8 record byte 10, +10 record byte 11, +12 the type; the count is then incremented. A full queue appends
//     nothing and no result is reported.
// Nothing is returned. The image performs the bitmap update a second time after the queue append; the second
// pass re-applies the same value, so it is not repeated here.
// Inferred: the bits are the eight modifier keys of a keyboard.
// Unknown: why the image's switch also handles type 3 (it sets the bit, like type 1; no caller passes it).
extern "C" {
extern unsigned char ModifierCodes[8];

typedef int __attribute__((may_alias)) AliasedInt;
typedef short __attribute__((may_alias)) AliasedShort;
typedef unsigned short __attribute__((may_alias)) AliasedUShort;

void CheckforModifierKeyEvents(unsigned char* slot, unsigned char* record) {
    for (int bit = 0; bit < 8; ++bit) {
        const unsigned mask = 1u << bit;
        if (((slot[33] ^ record[11]) & mask) == 0) continue;
        const unsigned char code = ModifierCodes[bit];
        const int type = (record[11] & mask) ? 1 : 2;
        if (type == 2) {
            slot[224 + (code >> 3)] &= static_cast<unsigned char>(~(1 << (code & 7)));
        } else {
            slot[224 + (code >> 3)] |= static_cast<unsigned char>(1 << (code & 7));
        }
        const unsigned count = *reinterpret_cast<AliasedUShort*>(slot + 38);
        if (count < 10) {
            unsigned char* const entry = slot + 40 + 16 * count;
            *reinterpret_cast<AliasedInt*>(entry) = *reinterpret_cast<AliasedInt*>(record);
            *reinterpret_cast<AliasedShort*>(entry + 4) = code;
            *reinterpret_cast<AliasedShort*>(entry + 8) = record[10];
            *reinterpret_cast<AliasedShort*>(entry + 10) = record[11];
            *reinterpret_cast<AliasedShort*>(entry + 12) = type;
            *reinterpret_cast<AliasedUShort*>(slot + 38) = static_cast<unsigned short>(count + 1);
        }
    }
}
}
