// Observed soundMapSound__FPci at 0x0024f608, 124 bytes.
// The second argument is unused. nmSounds at 0x0048594c is the signed count.
// A count that is not positive returns 0. soundList at 0x007d9280 is walked
// in 16-byte records. strcasecmp compares the pointer at +0 with the name.
// A zero result returns the word at +4. A missed list returns 0.
extern "C" {
extern int nmSounds;
extern unsigned char soundList[];
int strcasecmp(const char* left, const char* right);

static unsigned load_word(const unsigned char* address) {
    return address[0] | (address[1] << 8) | (address[2] << 16) | (address[3] << 24);
}
#if defined(NORRATH_HOST_CONTRACT)
const char* norrath_host_sound_name(unsigned word);
#endif
static const char* name_pointer(unsigned word) {
#if defined(NORRATH_HOST_CONTRACT)
    return norrath_host_sound_name(word);
#else
    return reinterpret_cast<const char*>(static_cast<unsigned long>(word));
#endif
}

int soundMapSound__FPci(char* name, int) {
    if (nmSounds <= 0) return 0;
    for (int index = 0; index < nmSounds; ++index) {
        unsigned char* const record = soundList + index * 16;
        if (strcasecmp(name_pointer(load_word(record)), name) == 0) return static_cast<int>(load_word(record + 4));
    }
    return 0;
}
}
