#include <cstring>

extern "C" unsigned chatOn;
extern "C" unsigned commEcho;
extern "C" unsigned commEnable;
extern "C" unsigned commPlayback;
extern "C" unsigned commRecord;
extern "C" unsigned commThresh;
extern "C" unsigned damageDisplayOn;
extern "C" unsigned char debugPauseMenuOn;
extern "C" unsigned g_BloodLevel;
extern "C" unsigned char hudNetWaiting;
extern "C" unsigned subtitlesOn;
extern "C" unsigned char particlesPaused;
extern "C" unsigned char recallOn;
extern "C" unsigned char supersampleMode;
extern "C" unsigned char waitNetExitCount;

static int word_is(unsigned* object, unsigned char b0) {
    const unsigned char want[4] = {b0, 0, 0, 0};
    return std::memcmp(object, want, 4) == 0 ? 0 : 1;
}

int main() {
    if (word_is(&chatOn, 1) || chatOn != 1u) return 1;
    if (word_is(&commEcho, 0) || word_is(&commEnable, 0) || word_is(&commPlayback, 0)) return 2;
    if (word_is(&commRecord, 0) || word_is(&commThresh, 0)) return 3;
    if (word_is(&damageDisplayOn, 0) || word_is(&g_BloodLevel, 0) || word_is(&subtitlesOn, 0)) return 4;
    if (sizeof debugPauseMenuOn != 1 || debugPauseMenuOn != 0) return 5;
    if (hudNetWaiting != 0 || particlesPaused != 0 || recallOn != 0 || waitNetExitCount != 0) return 6;
    if (supersampleMode != 1) return 7;
    return 0;
}
