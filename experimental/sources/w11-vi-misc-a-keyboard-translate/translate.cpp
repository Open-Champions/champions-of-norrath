// Observed: TranslateEvent__10VIKeyboardP11_lgKbdEvent at 0x002cbfd8 is 148 bytes;
// the keyboard object is not read. It scans the 74 ten-byte records of the read-only
// table USEnglishKbdMapTable (0x0047a750, 740 bytes) for one whose byte 1 equals the
// event halfword at offset 4. A miss gives 0. A hit picks a byte from the record by
// the event halfword at offset 10: if (value & 0x22) record byte 4; else if
// (value & 0x40) record byte 6; else record byte 2. If the event halfword at offset 8
// has bit 1 set and the chosen byte is in 97..122 the result is the byte minus 32
// (the original adds 224 and masks to a byte). The result is returned as a byte.
// Inferred: bytes 2, 4 and 6 are plain, shifted and a third variant (alt-style)
// character; the 0x22 and 0x40 bits are modifier flags and bit 1 of the other
// halfword a caps-style flag. Unknown: the real flag names and the other record bytes.
extern "C" {
extern const unsigned char USEnglishKbdMapTable[];

unsigned char TranslateEvent__10VIKeyboardP11_lgKbdEvent(void* keyboard, void* event) {
    (void)keyboard;
    const unsigned char* bytes = static_cast<const unsigned char*>(event);
    unsigned code = *reinterpret_cast<const unsigned short*>(bytes + 4);
    unsigned char key = 0;
    const unsigned char* record = USEnglishKbdMapTable;
    for (int index = 0; index < 74; ++index, record += 10) {
        if (record[1] != code) continue;
        unsigned flags = *reinterpret_cast<const unsigned short*>(bytes + 10);
        if ((flags & 0x22) != 0) key = record[4];
        else if ((flags & 0x40) != 0) key = record[6];
        else key = record[2];
        if ((*reinterpret_cast<const unsigned short*>(bytes + 8) & 2) != 0 && key >= 97 && key < 123)
            key = static_cast<unsigned char>(key + 224);
        break;
    }
    return key;
}
}
