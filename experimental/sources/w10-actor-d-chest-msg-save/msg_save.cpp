// Observed msg_save__5ChestPc at 0x0021da68, 72 bytes.
// Writes 12 bytes at the buffer, in order: the word at object+84, the halfword at +76, the bytes at
// +88 and +89, then the word at +132. Returns the byte count written (12). It calls nothing.
// Inferred: these are the Chest's saved state fields (msg_load__5ChestPc reads the same order).
// Unknown: what each field means. Nothing here ran on the EE.
extern "C" int msg_save__5ChestPc(void* self, char* buffer) {
    const unsigned char* object = static_cast<const unsigned char*>(self);
    unsigned char* cursor = reinterpret_cast<unsigned char*>(buffer);
    *reinterpret_cast<unsigned*>(cursor) = *reinterpret_cast<const unsigned*>(object + 84);
    cursor += 4;
    *reinterpret_cast<unsigned short*>(cursor) = *reinterpret_cast<const unsigned short*>(object + 76);
    cursor += 2;
    *cursor = object[88];
    cursor += 1;
    *cursor = object[89];
    cursor += 1;
    *reinterpret_cast<unsigned*>(cursor) = *reinterpret_cast<const unsigned*>(object + 132);
    cursor += 4;
    return static_cast<int>(cursor - reinterpret_cast<unsigned char*>(buffer));
}
