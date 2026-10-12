/* soundStartGlobalMusic__FPci at 0x0024a390 is 156 bytes.
 *
 * If soundMusicVolume (0x00485934) is below the float constant at 0x0048200c
 * (bits 0x3d4ccccc, one unit under the nearest 0.05f), nothing happens. Otherwise stream 0 gets its volume set to
 * musicDuckAmount * soundMusicVolume * 0.46875f. The name is then looked up
 * with soundFindGlobalDialog__FPc. A found record whose words at offsets 64
 * and 144 differ starts stream 0 with soundStartStream__FiPciiiiii:
 *
 *   file name   "mouth.vat" (rodata 0x00474630)
 *   flags       the second argument | 0x2 | 0x20
 *   offset      word at 64
 *   length      word at 144 minus word at 64
 *   next three  word at 68, halfword at 72 (signed), word at 76
 *
 * The meaning of the last three arguments is established by neither this
 * function nor the record layout read here.
 */
extern "C" {
extern float soundMusicVolume;
extern float musicDuckAmount;
unsigned char* soundFindGlobalDialog__FPc(char* name);
void soundSetStreamVolume__Fif(int stream, float volume);
void soundStartStream__FiPciiiiii(int stream, char* file, int flags, int offset, int length,
                                  int a5, int a6, int a7);

void soundStartGlobalMusic__FPci(char* name, int flags) {
    if (soundMusicVolume < __builtin_bit_cast(float, 0x3d4ccccc)) return;
    soundSetStreamVolume__Fif(0, musicDuckAmount * soundMusicVolume * 0.46875f);
    unsigned char* record = soundFindGlobalDialog__FPc(name);
    if (record == nullptr) return;
    const int end = *reinterpret_cast<int*>(record + 144);
    const int start = *reinterpret_cast<int*>(record + 64);
    if (end == start) return;
    soundStartStream__FiPciiiiii(0, const_cast<char*>("mouth.vat"), flags | 0x2 | 0x20, start,
                                 end - start, *reinterpret_cast<int*>(record + 68),
                                 *reinterpret_cast<short*>(record + 72),
                                 *reinterpret_cast<int*>(record + 76));
}
}
