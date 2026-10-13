// Observed msg_save__9SavepointPc at 0x0021c398, 56 bytes. Writes one 32-bit word, then two bytes,
// and returns 6. The word is the one at object+132 when that is nonzero, otherwise the word at
// +128. The bytes are the unsigned bytes at +197 and +198, in that order. No callee is used.
// Unknown: the meaning of the fields. Nothing here ran on the EE.
extern "C" int msg_save__9SavepointPc(void* self, char* buffer) {
    const unsigned char* const o = static_cast<const unsigned char*>(self);
    char* const begin = buffer;
    int word = *reinterpret_cast<const int*>(o + 132);
    if (word == 0) word = *reinterpret_cast<const int*>(o + 128);
    *reinterpret_cast<int*>(buffer) = word;
    buffer += 4;
    *buffer = static_cast<char>(o[197]);
    buffer += 1;
    *buffer = static_cast<char>(o[198]);
    buffer += 1;
    return static_cast<int>(buffer - begin);
}
