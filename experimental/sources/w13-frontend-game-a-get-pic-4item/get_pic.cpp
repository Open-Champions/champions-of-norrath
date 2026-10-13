// Observed getPic__4itemiPQ24item11description at 0x00107a80, 228 bytes, 96-byte frame (a 64-byte
// name buffer). Arguments: an integer n and an item::description pointer, both passed on to
// getPicName__4itemPciPQ24item11description(name, n, description). The result name is searched in
// the 64 records of _4item$picList (192 bytes each, the name string first) with strcmp, the first
// equal record winning.
//  - Found: when the halfword at record + 64 is not negative, the word at record + 124 is set to
//    drawFrameNm and the result is record + 128; with a negative halfword the result stays zero.
//  - Not found (all 64 compared): picAdd__4itemPc(name) is called and its result is not used; the
//    result stays zero.
// A zero result is replaced by lumpFindResource__FPcT0("equipmnt.lmp", "pic_dagger1.tex").
// Inferred: record + 128 is the picture data a caller draws, record + 124 a last-used frame stamp
// (picKickOneOut__4itemv reads it), and a negative first slot means the picture is still loading.
// Unknown: the return type of picAdd__4itemPc.
extern "C" {
extern unsigned char _4item$picList[];
extern int drawFrameNm;
void getPicName__4itemPciPQ24item11description(char* out, int n, void* description);
int strcmp(const char* left, const char* right);
void picAdd__4itemPc(char* name);
void* lumpFindResource__FPcT0(const char* lump, const char* name);
}

extern "C" void* getPic__4itemiPQ24item11description(int n, void* description) {
    char name[64];
    getPicName__4itemPciPQ24item11description(name, n, description);
    unsigned char* record = _4item$picList;
    int index = 0;
    for (; index < 64; ++index, record += 192) {
        if (strcmp(name, reinterpret_cast<char*>(record)) == 0) break;
    }
    void* result = nullptr;
    if (index == 64) {
        picAdd__4itemPc(name);
    } else if (*reinterpret_cast<short*>(record + 64) >= 0) {
        *reinterpret_cast<int*>(record + 124) = drawFrameNm;
        result = record + 128;
    }
    if (result == nullptr) result = lumpFindResource__FPcT0("equipmnt.lmp", "pic_dagger1.tex");
    return result;
}
