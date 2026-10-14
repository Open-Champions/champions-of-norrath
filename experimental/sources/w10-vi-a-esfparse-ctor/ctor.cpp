// Observed: __10VIESFParse at 0x0117aa28 is 88 bytes and returns nothing.
// It fills 256 four-byte entries starting at offset 40 (to offset 1064), each
// with bytes 0, 0, 0, 0xff. It then stores 1 at offset 0 and zero at offsets 4,
// 8, 12, 20, 24, 28, 32 and 36. Offset 16 is not written.
// Unknown: what the entries are, the meaning of the scalar words, and the size
// of VIESFParse.
extern "C" {
void __10VIESFParse(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    for (int index = 0; index < 256; ++index) {
        unsigned char* entry = bytes + 40 + index * 4;
        entry[0] = 0;
        entry[1] = 0;
        entry[2] = 0;
        entry[3] = 0xff;
    }
    int* words = reinterpret_cast<int*>(object);
    words[2] = 0;
    words[0] = 1;
    words[9] = 0;
    words[3] = 0;
    words[5] = 0;
    words[6] = 0;
    words[8] = 0;
    words[7] = 0;
    words[1] = 0;
}
}
