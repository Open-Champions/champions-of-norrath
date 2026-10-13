// Observed msg_save__8RollDoorPc at 0x0021c560, 60 bytes. Writes four 32-bit words to the buffer in
// order: the sign-extended halfword at object+90, the sign-extended halfword at +76, the float at
// +80 and the word at +84. Returns the byte count, 16. No callee is used. Unknown: the meaning of
// the four fields. Nothing here ran on the EE.
extern "C" int msg_save__8RollDoorPc(void* self, char* buffer) {
    const unsigned char* const o = static_cast<const unsigned char*>(self);
    char* const begin = buffer;
    *reinterpret_cast<int*>(buffer) = *reinterpret_cast<const short*>(o + 90);
    buffer += 4;
    *reinterpret_cast<int*>(buffer) = *reinterpret_cast<const short*>(o + 76);
    buffer += 4;
    *reinterpret_cast<float*>(buffer) = *reinterpret_cast<const float*>(o + 80);
    buffer += 4;
    *reinterpret_cast<int*>(buffer) = *reinterpret_cast<const int*>(o + 84);
    buffer += 4;
    return static_cast<int>(buffer - begin);
}
