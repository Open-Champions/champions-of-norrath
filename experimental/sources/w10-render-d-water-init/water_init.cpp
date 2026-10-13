// Observed waterInit__Fv at 0x00178188, 444 bytes (a local symbol of the image). The patch array waterPatch
// (the pointer at 0x00484138) holds nmWaterPatches (0x00484130) records of 4784 bytes. Steps, in order:
// 1. the whole array is cleared with memset;
// 2. the patches are linked as a list: waterList (0x004869b8) is the first patch, the word at +4760 of each
//    patch is the next patch and the word at +4764 the previous one; waterListBack (0x004869bc) is the last
//    patch. With fewer than two patches the original still treats index 1 as the last one and writes its
//    previous link, one record past a one-patch array; that is reproduced;
// 3. the word at +4756 of every patch is set to 0xfffe7960;
// 4. in every patch the 33 blocks of 80 bytes from +64 are filled: the word at +0 of block j (offset 64) is 0
//    for j = 0 and 0x14000038 otherwise, the word at +4 is 0 (offset 68), and from +8 (offset 72) the pair
//    of words at +8 and +12 is, for j below 32, (m * 300 | 0x04000000, m * 300 | 0x61200000) with
//    m = (j + 1) % 3, and zero for j = 32. The words at +16 onward are left as cleared.
// Nothing is returned. Inferred only: the block words look like packed drawing commands. Unknown: the
// meaning of the record beyond these fields and the contents of the other bytes.
extern "C" {
extern int nmWaterPatches;
extern unsigned char* waterPatch;
extern unsigned char* waterList;
extern unsigned char* waterListBack;
void* memset(void* destination, int value, unsigned count);
}

namespace {
const int kPatchSize = 4784;
inline unsigned char* patch(int index) { return waterPatch + index * kPatchSize; }
inline unsigned& word(unsigned char* base, int offset) { return *reinterpret_cast<unsigned*>(base + offset); }
// A patch address as the 32-bit word stored in a link field; the host check turns it into an offset.
inline unsigned link(unsigned char* address) {
#if defined(NORRATH_HOST_WATER)
    return static_cast<unsigned>(address - waterPatch);
#else
    return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(address));
#endif
}
}

extern "C" void waterInit__Fv() {
    memset(waterPatch, 0, nmWaterPatches * kPatchSize);
    waterList = waterPatch;
    int last = 1;
    word(patch(0), 4760) = link(patch(1));
    for (int i = 1; i < nmWaterPatches - 1; ++i) {
        word(patch(i), 4764) = link(patch(i - 1));
        word(patch(i), 4760) = link(patch(i + 1));
        last = i + 1;
    }
    word(patch(last), 4764) = link(patch(last - 1));
    waterListBack = patch(last);
    for (int i = 0; i < nmWaterPatches; ++i) word(patch(i), 4756) = 0xfffe7960u;
    for (int i = 0; i < nmWaterPatches; ++i) {
        for (int j = 0; j < 33; ++j) {
            unsigned char* block = patch(i) + 64 + j * 80;
            word(block, 0) = j == 0 ? 0u : 0x14000038u;
            word(block, 4) = 0;
            if (j == 32) {
                word(block, 8) = 0;
                word(block, 12) = 0;
            } else {
                unsigned scaled = static_cast<unsigned>((j + 1) % 3) * 300u;
                word(block, 8) = scaled | 0x04000000u;
                word(block, 12) = scaled | 0x60000000u | 0x01200000u;
            }
        }
    }
}
