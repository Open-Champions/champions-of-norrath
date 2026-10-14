// Observed: TranslateEvent__10VIKeyboardP11_lgKbdEvent at 0x002cbfd8 is 148 bytes and
// returns a byte; the keyboard object is not touched. It scans the first 74
// ten-byte records of USEnglishKbdMapTable in order for a record whose byte at
// offset 1 equals the event's halfword at offset 4. With no match it returns 0.
// With a match it picks a byte from that record by the event's halfword at
// offset 10: if (value & 0x22) the byte at record offset 4; else if (value & 0x40)
// the byte at offset 6; else the byte at offset 2. If the event's halfword at
// offset 8 has bit 1 (value 2) set and the chosen byte is from 97 to 122, 32 is
// subtracted (modulo 256); that value is returned.
// Inferred: the event offsets 4, 8 and 10 are the code, flags A and flags B that
// Read__10VIKeyboardR12VIConsoleKey also uses; bit 1 of flags A looks like a
// caps-lock style state and 0x22 / 0x40 in flags B like shift and alt-graph style
// states. The bytes 97 to 122 are lower-case ASCII letters.
// Unknown: names of the flag bits, the use of record bytes 0, 3, 5, 7, 8 and 9,
// and whether 74 is the table's true length.
extern "C" {
extern unsigned char USEnglishKbdMapTable[];

unsigned char TranslateEvent__10VIKeyboardP11_lgKbdEvent(void* keyboard, void* event) {
    (void)keyboard;
    const unsigned char* bytes = static_cast<const unsigned char*>(event);
    unsigned code = *reinterpret_cast<const unsigned short*>(bytes + 4);
    const unsigned char* record = USEnglishKbdMapTable;
    unsigned char key = 0;
    for (int index = 0; index < 74; ++index, record += 10) {
        if (code != record[1]) continue;
        unsigned flags_b = *reinterpret_cast<const unsigned short*>(bytes + 10);
        if ((flags_b & 0x22) != 0) key = record[4];
        else if ((flags_b & 0x40) != 0) key = record[6];
        else key = record[2];
        unsigned flags_a = *reinterpret_cast<const unsigned short*>(bytes + 8);
        if ((flags_a & 2) != 0 && key >= 97 && key < 123) key = static_cast<unsigned char>(key + 224);
        break;
    }
    return key;
}
}
