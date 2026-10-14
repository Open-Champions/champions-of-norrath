// Observed soundStartDialogStream__FiPci at 0x0024ef40, 136 bytes.
// soundFindDialog receives the name. A null result returns without starting
// a stream. Words at dialog+144 and dialog+64 must differ. soundStartStream
// then receives the index, soundGetDialogFilename, the flags, the word at
// +64, the difference of the two words, the word at +68, the signed halfword
// at +72, and the word at +76.
extern "C" {
unsigned char* soundFindDialog__FPc(char* name);
char* soundGetDialogFilename__Fv();
int soundStartStream__FiPciiiiii(int index, char* filename, int flags, unsigned start, unsigned length,
                                 unsigned second, int third, unsigned fourth);

static unsigned load_word(const unsigned char* address) {
    return address[0] | (address[1] << 8) | (address[2] << 16) | (address[3] << 24);
}
static int load_half(const unsigned char* address) {
    const unsigned value = address[0] | (address[1] << 8);
    return value < 0x8000u ? static_cast<int>(value) : static_cast<int>(value) - 0x10000;
}

int soundStartDialogStream__FiPci(int index, char* name, int flags) {
    unsigned char* const dialog = soundFindDialog__FPc(name);
    if (dialog == nullptr) return 0;
    const unsigned end = load_word(dialog + 144);
    const unsigned start = load_word(dialog + 64);
    if (end == start) return static_cast<int>(reinterpret_cast<unsigned long>(dialog));
    return soundStartStream__FiPciiiiii(index, soundGetDialogFilename__Fv(), flags, start, end - start,
                                        load_word(dialog + 68), load_half(dialog + 72), load_word(dialog + 76));
}
}
