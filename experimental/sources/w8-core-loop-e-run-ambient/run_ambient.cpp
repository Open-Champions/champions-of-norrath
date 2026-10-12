// Observed runAmbient__Fv at 0x0024da80, 1052 bytes.
// It returns at once when soundGetStreamPlayStatus(2) has bit 0x20, when the
// busy words of stream records 2 or 0 have bit 0x20, or when stream record 2
// has a positive entry count (the word at +448). The busy word of a record
// is zero when its word at +20 is zero, else the word at +32, OR-ed with the
// first word of each of its entries (48-byte entries from +108, the count at
// +448). Stream records are 452 bytes (three, at `stream`, 0x007dd290).
// Then, for each of nmAmbientGroups groups (680 bytes at ambientGroup,
// 0x007dd8c0): the countdown word at +12 is decremented every call and the
// group is skipped while it was positive. Otherwise the next countdown is
// (int)(60.0f * (min + r * (max - min))), with min the word at +0, max the
// word at +4 and r the top 16 bits of the next eRandState value
// (state * 1664525 + 1013904223) divided by 65536. The group entries are
// 41 bytes from +24 (a 40-byte name and a flag byte, at +64 for the first).
// The entry count is the word at +8. The free entries are those whose flag is
// zero. Without any free entry, eRandState keeps that first step and nothing
// is played. Otherwise eRandState takes a second step and its top 16 bits
// modulo the free count select the (index)th free entry among the first
// count - 1 entries; if the search runs out, the last entry is used.
// The volume scale animGetVolumeScale(word +16, word +20) is multiplied by
// min(1, 2 * soundMusicVolume), limited to 0..1, and stored into
// streamVolume[2] (soundUpdateAdjustments follows when it changed). The name
// of the chosen entry goes to soundFindDialog. A dialog whose words at +64
// and +144 differ starts a stream: the name "%s.vat" built from
// engineCurrentLevelName, then soundStartStream(2, name, 128, d[64],
// d[144] - d[64], d[68], (short)d[72], d[76]). A dialog with equal words only
// stops here. With fewer than two free entries every flag of the group is
// cleared first; the chosen entry's flag is then set to 1.
// Inferred: these are ambient sound groups with timed random starts. The
// meaning of the dialog words is unknown, as are the entry name layout beyond
// its use as the lookup text and the vat file naming role.
extern "C" {
int soundGetStreamPlayStatus__Fi(int stream);
float animGetVolumeScale__Fii(int a, int b);
void soundUpdateAdjustments__Fv();
unsigned char* soundFindDialog__FPc(char* name);
int sprintf(char* destination, const char* format, ...);
void soundStartStream__FiPciiiiii(int stream, char* name, int volume, int a, int b, int c, int d, int e);
extern unsigned char stream[];
extern unsigned char ambientGroup[];
extern int nmAmbientGroups;
extern unsigned eRandState;
extern float soundMusicVolume;
extern float streamVolume[3];
extern char engineCurrentLevelName[];
}

namespace {
__attribute__((always_inline)) inline int word_at(const unsigned char* p) {
    return static_cast<int>(p[0] | (unsigned(p[1]) << 8) | (unsigned(p[2]) << 16) |
                            (unsigned(p[3]) << 24));
}
__attribute__((always_inline)) inline void set_word(unsigned char* p, int value) {
    const unsigned v = static_cast<unsigned>(value);
    p[0] = static_cast<unsigned char>(v);
    p[1] = static_cast<unsigned char>(v >> 8);
    p[2] = static_cast<unsigned char>(v >> 16);
    p[3] = static_cast<unsigned char>(v >> 24);
}
__attribute__((always_inline)) inline int busy_word(const unsigned char* record) {
    int combined = 0;
    if (word_at(record + 20) != 0) combined = word_at(record + 32);
    const int count = word_at(record + 448);
    for (int i = 0; i < count; ++i) combined |= word_at(record + 108 + i * 48);
    return combined;
}
}

extern "C" void runAmbient__Fv() {
    // The original keeps this text in a file-local buffer (vatname.844, 80
    // bytes up to the next symbol); the size is the symbol span.
    static char vatName[80];
    if ((soundGetStreamPlayStatus__Fi(2) & 0x20) != 0) return;
    if ((busy_word(stream + 2 * 452) & 0x20) != 0) return;
    if ((busy_word(stream) & 0x20) != 0) return;
    if (word_at(stream + 2 * 452 + 448) > 0) return;
    const int groups = nmAmbientGroups;
    for (int group_index = 0; group_index < groups; ++group_index) {
        unsigned char* const group = ambientGroup + group_index * 680;
        const int countdown = word_at(group + 12);
        set_word(group + 12, countdown - 1);
        if (countdown > 0) continue;
        const unsigned first = eRandState * 1664525u + 1013904223u;
        const float low = static_cast<float>(word_at(group));
        const float span = static_cast<float>(word_at(group + 4) - word_at(group));
        const float fraction = static_cast<float>(first >> 16) * 1.52587890625e-05f * span;
        set_word(group + 12, static_cast<int>((low + fraction) * 60.0f));
        const int count = word_at(group + 8);
        int free_count = 0;
        for (int i = 0; i < count; ++i)
            if (group[64 + i * 41] == 0) ++free_count;
        if (free_count == 0) {
            eRandState = first;
            continue;
        }
        const unsigned second = first * 1664525u + 1013904223u;
        eRandState = second;
        int remaining = static_cast<int>((second >> 16) % static_cast<unsigned>(free_count));
        int chosen = 0;
        if (count - 1 > 0) {
            for (; chosen < count - 1; ++chosen) {
                if (group[64 + chosen * 41] == 0) {
                    --remaining;
                    if (remaining < 0) break;
                }
            }
        }
        float scale = animGetVolumeScale__Fii(word_at(group + 16), word_at(group + 20));
        float music = soundMusicVolume + soundMusicVolume;
        if (1.0f < music) music = 1.0f;
        scale = scale * music;
        if (scale < 0.0f) scale = 0.0f;
        if (1.0f < scale) scale = 1.0f;
        if (scale != streamVolume[2]) {
            streamVolume[2] = scale;
            soundUpdateAdjustments__Fv();
        }
        unsigned char* const dialog = soundFindDialog__FPc(reinterpret_cast<char*>(group + 24 + chosen * 41));
        if (dialog != nullptr) {
            if (word_at(dialog + 144) != word_at(dialog + 64)) {
                sprintf(vatName, "%s.vat", engineCurrentLevelName);
                const int start = word_at(dialog + 64);
                soundStartStream__FiPciiiiii(2, vatName, 128, start,
                                             word_at(dialog + 144) - start, word_at(dialog + 68),
                                             static_cast<short>(dialog[72] | (dialog[73] << 8)),
                                             word_at(dialog + 76));
            }
        }
        if (free_count < 2) {
            for (int i = 0; i < count; ++i) group[64 + i * 41] = 0;
        }
        group[64 + chosen * 41] = 1;
    }
}
