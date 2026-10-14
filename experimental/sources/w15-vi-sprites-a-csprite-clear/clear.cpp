// Observed: Clear__9VICSprite at 0x010add20, 472 bytes. It calls Clear__9VIHSprite(this) and returns -1 if that is negative, writing nothing
// else. Otherwise (word offsets from this): +400 = 1, +52 = 5, +416 = -1, +420 = -1, +3856 = -1, +4456 = 0, +4588 = 0, +4584 = -1;
// the words at +4492..+4504 become 0, -1, 0, -1 and that 16-byte group is copied to +4460, +4476 and +4508; +4528..+4543 and +4544 are zeroed
// and +4568..+4579 zeroed, +4580 = -1; the 16 bytes at +4548 become the floats 0.05, 0.05, 0.3, 1.0; three 12-byte groups from +4364 get
// -1, 11, -1 (the middle word is 11); nine words +3860..+3892 and six words +4400..+4420 are set to -1. It returns zero.
// Inferred: the sprite object is at least 4592 bytes with these reset fields in its character part; the groups of -1 are "none" indices.
// Unknown: the meaning of every one of these fields, of the value 11 and of the two 16-byte groups beyond their values.
extern "C" {
int Clear__9VIHSprite(void* object);

static void put(unsigned char* base, int offset, int value) { *reinterpret_cast<int*>(base + offset) = value; }
static void putf(unsigned char* base, int offset, float value) { *reinterpret_cast<float*>(base + offset) = value; }

int Clear__9VICSprite(void* object) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    if (Clear__9VIHSprite(object) < 0) return -1;
    put(bytes, 400, 1);
    put(bytes, 52, 5);
    put(bytes, 416, -1);
    put(bytes, 420, -1);
    put(bytes, 4456, 0);
    put(bytes, 4588, 0);
    put(bytes, 4584, -1);
    put(bytes, 4492, 0);
    put(bytes, 4496, -1);
    put(bytes, 4500, 0);
    put(bytes, 4504, -1);
    for (int group = 0; group < 3; ++group) {
        int base = group == 0 ? 4460 : group == 1 ? 4476 : 4508;
        put(bytes, base, 0);
        put(bytes, base + 4, -1);
        put(bytes, base + 8, 0);
        put(bytes, base + 12, -1);
    }
    putf(bytes, 4548, 0.05f);
    putf(bytes, 4552, 0.05f);
    putf(bytes, 4556, 0.3f);
    putf(bytes, 4560, 1.0f);
    for (int offset = 4528; offset < 4548; offset += 4) put(bytes, offset, 0);
    put(bytes, 3856, -1);
    for (int offset = 4568; offset < 4580; offset += 4) put(bytes, offset, 0);
    put(bytes, 4580, -1);
    for (int group = 0; group < 3; ++group) {
        put(bytes, 4364 + group * 12, -1);
        put(bytes, 4368 + group * 12, 11);
        put(bytes, 4372 + group * 12, -1);
    }
    for (int offset = 3860; offset <= 3892; offset += 4) put(bytes, offset, -1);
    for (int offset = 4400; offset <= 4420; offset += 4) put(bytes, offset, -1);
    return 0;
}
}
