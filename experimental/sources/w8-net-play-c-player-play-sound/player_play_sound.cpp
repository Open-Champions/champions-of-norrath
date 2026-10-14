// Observed playerPlaySound__Fii at 0x001a6760, 3896 bytes (player index, sound
// number). Steps:
//  1. Return unless netPlayerIsLocal(index) is nonzero, cutMode is zero, and
//     (netNetworkMode is nonzero or interfaceOn is zero).
//  2. rec = player + index * 23984; class = word rec[84]. For sound < 10 (signed),
//     return if soundReallyAllFlags__Fi(2) & 0x20. Return if the float rec[172]
//     is not above 0. Return for sound outside 0..9 (unsigned compare) and for
//     sound 6.
//  3. A voice prefix is chosen from ten three-letter strings by the class word
//     (no range check): bwm bwf erm erf ecm ecf ewm ewf esm esf. The generator
//     eRandState steps as x * 1664525 + 1013904223 and r = x >> 16 where used.
//     The name is formatted and passed to a dialog stream function with
//     arguments (2, name, 36):
//       sound 0 "%s001", 3 "%s014", 4 "%s018", 5 "%s019", 7 "%s043": random stream
//       sound 1 "%s%03d" with 11 + r % 3: random stream
//       sound 8 "%s%03d" with 41 + (r & 1), sound 9 with 35 + (r & 1): random stream
//       sound 2: the level name string (levelName + 81) is tested against an
//         ordered list of exact (strcmp) or contained (strstr) names; the first
//         match formats "%s%03d.vag" with a number base plus either nothing,
//         r % 3 or r & 1 (the generator steps only on a match) and calls the
//         non-random stream function. No match plays nothing.
// What the numbers or the streams mean is not established; the names in the
// list are level-name strings from the image.
extern "C" {
extern unsigned char player[];
extern signed char cutMode;
extern signed char netNetworkMode;
extern signed char interfaceOn;
extern unsigned eRandState;
extern char levelName[];

int netPlayerIsLocal__Fi(int index);
int soundReallyAllFlags__Fi(int which);
int sprintf(char* dest, const char* format, ...);
int strcmp(const char* a, const char* b);
char* strstr(const char* haystack, const char* needle);
void soundStartGlobalDialogStream__FiPci(int a, char* name, int b);
void soundStartGlobalDialogStreamRandom__FiPci(int a, char* name, int b);
}

namespace {
enum Form { kConst, kMod3, kAnd1 };
struct LevelSound {
    bool exact;
    const char* name;
    Form form;
    int base;
};

const LevelSound kLevelSounds[] = {
    {true, "kelethin", kMod3, 45},  {true, "faydark1", kConst, 48}, {true, "faydark2", kAnd1, 49},
    {true, "faydark3", kMod3, 51},  {false, "goblin", kMod3, 112},  {true, "orccave1", kConst, 54},
    {true, "orccave2", kConst, 55}, {true, "orccave3", kConst, 56}, {false, "ruinint", kMod3, 57},
    {true, "gnomehub", kMod3, 61},  {false, "anthill", kMod3, 64},  {false, "lava", kMod3, 67},
    {false, "battle", kMod3, 70},   {false, "gothic", kMod3, 73},   {false, "nas", kMod3, 73},
    {true, "beachhub", kMod3, 79},  {true, "des1", kMod3, 85},      {true, "des2", kMod3, 85},
    {false, "grass", kMod3, 88},    {false, "cloud", kMod3, 91},    {false, "oce", kMod3, 97},
    {true, "desert", kMod3, 100},   {true, "bronzeht", kMod3, 103}, {false, "island2", kMod3, 106},
    {false, "kerriel", kMod3, 106}, {false, "islcave", kMod3, 106}, {true, "frozesea", kMod3, 109},
    {true, "arena1", kMod3, 118},   {false, "garden", kMod3, 124},  {true, "airhub", kMod3, 127},
    {true, "hate1", kConst, 130},   {true, "hate2", kConst, 131},   {true, "hate3", kConst, 132},
    {true, "hate4", kMod3, 133},
};

unsigned nextRandom() {
    eRandState = eRandState * 1664525u + 1013904223u;
    return eRandState >> 16;
}
}  // namespace

extern "C" void playerPlaySound__Fii(int index, int sound) {
    const char* voices[10] = {"bwm", "bwf", "erm", "erf", "ecm", "ecf", "ewm", "ewf", "esm", "esf"};
    char name[64];
    if (!netPlayerIsLocal__Fi(index)) return;
    if (cutMode != 0) return;
    if (netNetworkMode == 0 && interfaceOn != 0) return;
    unsigned char* rec = player + index * 23984;
    int cls = *(int*)(rec + 84);
    if (sound < 10 && (soundReallyAllFlags__Fi(2) & 0x20)) return;
    if (*(float*)(rec + 172) <= 0.0f) return;
    if ((unsigned)sound >= 10u) return;
    const char* voice = voices[cls];
    switch (sound) {
    case 0: sprintf(name, "%s001", voice); break;
    case 1: sprintf(name, "%s%03d", voice, (int)(nextRandom() % 3u) + 11); break;
    case 2: {
        const char* level = levelName + 81;
        for (const LevelSound& s : kLevelSounds) {
            bool hit = s.exact ? strcmp(level, s.name) == 0 : strstr(level, s.name) != 0;
            if (!hit) continue;
            int number = s.base;
            if (s.form == kMod3) number += (int)(nextRandom() % 3u);
            else if (s.form == kAnd1) number += (int)(nextRandom() & 1u);
            sprintf(name, "%s%03d.vag", voice, number);
            soundStartGlobalDialogStream__FiPci(2, name, 36);
            return;
        }
        return;
    }
    case 3: sprintf(name, "%s014", voice); break;
    case 4: sprintf(name, "%s018", voice); break;
    case 5: sprintf(name, "%s019", voice); break;
    case 6: return;
    case 7: sprintf(name, "%s043", voice); break;
    case 8: sprintf(name, "%s%03d", voice, (int)(nextRandom() & 1u) + 41); break;
    case 9: sprintf(name, "%s%03d", voice, (int)(nextRandom() & 1u) + 35); break;
    }
    soundStartGlobalDialogStreamRandom__FiPci(2, name, 36);
}
