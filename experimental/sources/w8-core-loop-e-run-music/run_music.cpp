// Observed runMusic__Fv at 0x002ce1a0, 3208 bytes. Called once per frame from
// the game loop. Everything below is read from the instructions; names for
// the numbered tags and states are only labels used in this note.
//
// Entry: the 32 bytes at 0x004296c0 (inside the object list.1073) are zeroed
// with memset on every call, before any other work. The words used are:
// musicTimer, cutMode, eRandState, frameNm, drawFrameNm, levelName+81,
// currScorePos, startScorePos (halfwords), numEnvironments (halfword),
// currActivityScore (float), lastMusicState and timeLastTransition.
// Dialog directories are arrays of 80-byte entries whose first bytes are a
// name; a zero first byte ends the array. "Tagged" below means strstr of the
// entry name finds the given text. The tag texts are "_env_", "amb_",
// "mx_env", "mx_theme" and "mx_boss_g". Level names are "kelsel" and "kelsel2".
//
// A. When cutMode is zero and levelName+81 equals neither level name, path C
//    runs. Otherwise path B runs.
// B. musicTimer is decreased; while it stays positive nothing else happens in
//    this part. At zero or below, eRandState advances (state * 1664525 +
//    1013904223), musicTimer becomes 14400 + (state >> 16) % 7200, and unless
//    soundAllFlags(0) has bit 0x20 the first dialog directory entry tagged
//    "_env_" (else "mx_theme", tried per entry) is started with
//    soundStartMusic(entry, 2). Then, when drawFrameNm has its low four bits
//    clear and soundGetStreamPlayStatus(0) is zero, the first entry tagged
//    "amb_" is started with soundStartDialogStream(0, index, 3).
// C. Only on frames where frameNm is a multiple of 30 and cutMode is zero.
//    soundAllFlags(0) is read repeatedly: a set bit among 0x20, 0x40, 0x80 or
//    0x100 with bit 0x200 clear zeroes the ring byte at currScorePos and
//    returns. Otherwise the ring (32 bytes, always zero here after the
//    memset) from currScorePos back to startScorePos is averaged: each signed
//    byte is weighted by (32 - k)^2 / 1024, with k counting from zero, summed
//    and divided by the count, into currActivityScore. When the two positions
//    are equal, a start is attempted instead: with numEnvironments nonzero a
//    random entry tagged "mx_env" (the nth match, n = (state >> 16) %
//    numEnvironments + 1) is started with soundStartMusic(entry, 514) and
//    lastMusicState becomes 1; otherwise the first entry tagged "mx_theme" is
//    started the same way with lastMusicState 0. Either records frameNm in
//    timeLastTransition. The ring then advances: currScorePos wraps at 32,
//    its byte is cleared, and startScorePos advances when it is reached.
//    A state step follows, keyed by lastMusicState with thresholds A=1.742
//    (bits 0x3fdef96e) and B=1.072 (bits 0x3f893719):
//      -1: score > A tries global music (see below, state 2). After 3600
//          frames since the last transition: score in [0, B) starts "mx_theme"
//          (state 0); score in [B, A) starts a "mx_env" pick (state 1) or
//          "mx_theme" without environments (state 0).
//       0: score > A tries global music. Then, unless bit 0x20 of
//          soundAllFlags(0): score in [0, B) starts the first "amb_" entry
//          with soundStartDialogStream(0, index, 515) (state -1); score in
//          [B, A) starts a "mx_env" pick (state 1) or "mx_theme" (state 0).
//       1: score > A tries global music. Unless bit 0x20 is set with a nonzero
//          soundMusicVolume, the first "amb_" entry is started as in state 0
//          (state -1).
//       2: score < 0.25 starts the first "amb_" entry (state -1).
//    "Tries global music": with a global dialog directory, the nth entry
//    tagged "mx_boss_g" (n = ((state >> 16) & 3) + 1, a fresh eRandState step)
//    is started with soundStartGlobalMusic(entry, 515); lastMusicState becomes
//    2 and timeLastTransition frameNm.
// Inferred: this is adaptive background music keyed to an activity score. The
// ring is zeroed on entry, so the score is observed to be zero whenever it is
// computed in this build of the function unless another function writes the
// ring between the memset and the average, which does not happen here.
// Unknown: the origin of the tag strings' roles, the units of the thresholds,
// and why the ring is cleared on entry. Four redundant empty-name checks the
// original makes after a search succeeds are left out.
extern "C" {
void* memset(void* destination, int value, unsigned size);
int strcmp(const char* left, const char* right);
char* strstr(const char* text, const char* pattern);
int soundAllFlags__Fi(int index);
int soundGetStreamPlayStatus__Fi(int index);
char* soundGetDialogDir__Fv();
char* soundGetGlobalDialogDir__Fv();
void soundStartMusic__FPci(char* name, int mode);
void soundStartGlobalMusic__FPci(char* name, int mode);
void soundStartDialogStream__Fiii(int stream, int index, int mode);
extern signed char cutMode;
extern int musicTimer;
extern unsigned eRandState;
extern int frameNm;
extern int drawFrameNm;
extern char levelName[];
extern short currScorePos;
extern short startScorePos;
extern short numEnvironments;
extern float currActivityScore;
extern int lastMusicState;
extern int timeLastTransition;
extern float soundMusicVolume;
extern signed char norrath_music_activity_ring_view[32];
}

namespace {
__attribute__((always_inline)) inline float bitsToFloat(unsigned bits) {
    union {
        unsigned word;
        float value;
    } u = {bits};
    return u.value;
}

__attribute__((always_inline)) inline unsigned stepRandom() {
    eRandState = eRandState * 1664525u + 1013904223u;
    return eRandState;
}

// Index of the first entry whose name contains the tag, or -1.
__attribute__((always_inline)) inline int findTagged(char* directory, const char* tag) {
    int index = 0;
    for (char* entry = directory; entry[0] != 0; entry += 80, ++index)
        if (strstr(entry, tag) != nullptr) return index;
    return -1;
}

// Same, trying the first tag and then the second on each entry.
__attribute__((always_inline)) inline int findTaggedEither(char* directory, const char* first,
                                                           const char* second) {
    int index = 0;
    for (char* entry = directory; entry[0] != 0; entry += 80, ++index)
        if (strstr(entry, first) != nullptr || strstr(entry, second) != nullptr) return index;
    return -1;
}

// Index of the nth (1-based) entry whose name contains the tag, or -1.
__attribute__((always_inline)) inline int findNthTagged(char* directory, const char* tag, int n) {
    int index = 0;
    int matches = 0;
    for (char* entry = directory; entry[0] != 0; entry += 80, ++index) {
        if (strstr(entry, tag) != nullptr) ++matches;
        if (matches == n) return index;
    }
    return -1;
}

__attribute__((always_inline)) inline bool flagSet(int mask) {
    return (soundAllFlags__Fi(0) & mask) != 0;
}

__attribute__((always_inline)) inline bool anyActivityFlag() {
    return flagSet(0x20) || flagSet(0x40) || flagSet(0x80) || flagSet(0x100);
}

__attribute__((always_inline)) inline void recordTransition(int state) {
    lastMusicState = state;
    timeLastTransition = frameNm;
}

// Starts the first "mx_theme" entry; state 0.
__attribute__((always_inline)) inline void startTheme(char* directory) {
    const int index = findTagged(directory, "mx_theme");
    if (index < 0) return;
    soundStartMusic__FPci(directory + index * 80, 514);
    recordTransition(0);
}

// Starts a random "mx_env" entry; state 1.
__attribute__((always_inline)) inline void startEnvironment(char* directory) {
    const unsigned state = stepRandom();
    const int n = static_cast<int>((state >> 16) % static_cast<unsigned>(numEnvironments)) + 1;
    const int index = findNthTagged(directory, "mx_env", n);
    if (index < 0) return;
    soundStartMusic__FPci(directory + index * 80, 514);
    recordTransition(1);
}

// "mx_env" when numEnvironments is nonzero, else "mx_theme".
__attribute__((always_inline)) inline void startThemeOrEnvironment() {
    char* const directory = soundGetDialogDir__Fv();
    if (directory == nullptr) return;
    if (numEnvironments != 0)
        startEnvironment(directory);
    else
        startTheme(directory);
}

// Starts the first "amb_" entry as a dialog stream; state -1.
__attribute__((always_inline)) inline void startAmbient() {
    char* const directory = soundGetDialogDir__Fv();
    if (directory == nullptr) return;
    const int index = findTagged(directory, "amb_");
    if (index < 0) return;
    soundStartDialogStream__Fiii(0, index, 515);
    recordTransition(-1);
}

__attribute__((always_inline)) inline void tryGlobalMusic() {
    char* const directory = soundGetGlobalDialogDir__Fv();
    if (directory == nullptr) return;
    const unsigned state = stepRandom();
    const int n = static_cast<int>((state >> 16) & 3u) + 1;
    const int index = findNthTagged(directory, "mx_boss_g", n);
    if (index < 0) return;
    soundStartGlobalMusic__FPci(directory + index * 80, 515);
    recordTransition(2);
}
}

extern "C" void runMusic__Fv() {
    memset(norrath_music_activity_ring_view, 0, 32);
    if (cutMode != 0 || strcmp(levelName + 81, "kelsel") == 0 ||
        strcmp(levelName + 81, "kelsel2") == 0) {
        // Path B.
        const int next = musicTimer - 1;
        if (next > 0) {
            musicTimer = next;
        } else {
            const unsigned state = stepRandom();
            musicTimer = 14400 + static_cast<int>((state >> 16) % 7200u);
            if (!flagSet(0x20)) {
                char* const directory = soundGetDialogDir__Fv();
                if (directory != nullptr) {
                    const int index = findTaggedEither(directory, "_env_", "mx_theme");
                    if (index >= 0) soundStartMusic__FPci(directory + index * 80, 2);
                }
            }
        }
        if ((drawFrameNm & 0xf) != 0) return;
        if (soundGetStreamPlayStatus__Fi(0) != 0) return;
        char* const directory = soundGetDialogDir__Fv();
        if (directory == nullptr) return;
        const int index = findTagged(directory, "amb_");
        if (index < 0) return;
        soundStartDialogStream__Fiii(0, index, 3);
        return;
    }
    // Path C.
    if (frameNm % 30 != 0) return;
    if (cutMode != 0) return;
    // The four bits are tested in order, each with a fresh soundAllFlags read.
    // A set bit together with bit 0x200 goes straight to the average. Otherwise
    // the four bits are tested once more and a set one clears the ring byte.
    const bool anyFirst = anyActivityFlag();
    if (!(anyFirst && flagSet(0x200))) {
        if (anyActivityFlag()) {
            norrath_music_activity_ring_view[currScorePos] = 0;
            return;
        }
    }
    if (currScorePos != startScorePos) {
        float sum = 0.0f;
        int position = currScorePos;
        const int stop = startScorePos;
        int count = 0;
        do {
            const int weight = (32 - count) * (32 - count);
            ++count;
            const int sample = norrath_music_activity_ring_view[position];
            --position;
            if (position < 0) position = 31;
            const float scaled = static_cast<float>(weight) * bitsToFloat(0x3a800000u);
            sum = sum + static_cast<float>(sample) * scaled;
        } while (position != stop);
        currActivityScore = sum / static_cast<float>(count);
    } else if (numEnvironments != 0) {
        char* const directory = soundGetDialogDir__Fv();
        if (directory != nullptr) startEnvironment(directory);
    } else {
        char* const directory = soundGetDialogDir__Fv();
        if (directory != nullptr) startTheme(directory);
    }
    // Advance the ring.
    const int old_start = startScorePos;
    int position = static_cast<short>(currScorePos + 1);
    if (position == 32) position = 0;
    currScorePos = static_cast<short>(position);
    norrath_music_activity_ring_view[position] = 0;
    if (position == old_start) startScorePos = (old_start + 1 < 32) ? old_start + 1 : 0;

    const float upper = bitsToFloat(0x3fdef96eu);
    const float lower = bitsToFloat(0x3f893719u);
    switch (lastMusicState) {
    case 0: {
        float score = currActivityScore;
        if (upper < score) tryGlobalMusic();
        if (flagSet(0x20)) return;
        score = currActivityScore;
        if (score < lower) {
            if (0.0f <= score) {
                startAmbient();
                return;
            }
        }
        if (!(lower <= score)) return;
        if (!(score < upper)) return;
        startThemeOrEnvironment();
        return;
    }
    case 1: {
        if (bitsToFloat(0x3fdef96eu) < currActivityScore) tryGlobalMusic();
        if (flagSet(0x20) && soundMusicVolume != 0.0f) return;
        startAmbient();
        return;
    }
    case 2: {
        if (!(currActivityScore < bitsToFloat(0x3e800000u))) return;
        startAmbient();
        return;
    }
    case -1: {
        float score = currActivityScore;
        if (upper < score) tryGlobalMusic();
        if (!(timeLastTransition < frameNm - 3600)) return;
        score = currActivityScore;
        if (score < lower) {
            if (0.0f <= score) {
                char* const directory = soundGetDialogDir__Fv();
                if (directory != nullptr) startTheme(directory);
                return;
            }
        }
        if (!(lower <= score)) return;
        if (!(score < upper)) return;
        startThemeOrEnvironment();
        return;
    }
    default:
        return;
    }
}
