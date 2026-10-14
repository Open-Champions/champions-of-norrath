// Observed: mcHeartBeatRun__Fv at 0x002511f0 is 384 bytes. It adds the
// word 0x3faaaaaa to progress while progress is below 402 and asyncMode is
// 3, and 0x3f555554 otherwise. At or above 402 it stores 402. Unless
// gamePause is nonzero, shopRunning is zero, and interfaceOn is zero, it
// advances the local frame counter and, on the step from 60 to 61, clears
// that counter and adds one to curPlaySeconds. It always adds one to
// MC_CheckPointAge. The save tail runs only when the asyncMode captured
// above is -1, disableMC is zero, saveCheckPointCount is positive,
// frontEndRunning and gameNoDraw are zero, gamePause is zero, the word at
// gameCam+52 is 1, and engineCurrentLevelName does not contain "kelsel".
// A nonzero gameRecallInhibit also requires the name "arena1". A count of
// 99 stores 0 and returns. Any other positive count decreases by one, and
// the step to 0 prints "mcHeartBeat RUN\n" and calls MC_SaveCheckPoint(0, 1, 0).
// Inferred: 402 is the float 0x43c90000. The frame counter is the local
// localFrameCount.1158 word; the dotted ELF name is not a C identifier.
// Unknown: what progress measures, and who sets saveCheckPointCount to 99.

extern "C" int asyncMode;
extern "C" int gamePause;
extern "C" int shopRunning;
extern "C" signed char interfaceOn;
extern "C" int curPlaySeconds;
extern "C" int MC_CheckPointAge;
extern "C" int disableMC;
extern "C" int saveCheckPointCount;
extern "C" int frontEndRunning;
extern "C" int gameRecallInhibit;
extern "C" int gameNoDraw;
extern "C" unsigned char* gameCam;
extern "C" char engineCurrentLevelName[];
extern "C" int strcmp(const char* left, const char* right);
extern "C" char* strstr(const char* text, const char* needle);
extern "C" int scePrintf(const char* text);
extern "C" void MC_SaveCheckPoint__Fiii(int first, int second, int third);

namespace {
float from_bits(unsigned bits) {
    union {
        unsigned bits;
        float value;
    } pun;
    pun.bits = bits;
    return pun.value;
}

unsigned load_word(const unsigned char* at) {
    return static_cast<unsigned>(at[0]) | (static_cast<unsigned>(at[1]) << 8) |
           (static_cast<unsigned>(at[2]) << 16) | (static_cast<unsigned>(at[3]) << 24);
}

float progress;
int local_frame_count;
}

extern "C" void mcHeartBeatRun__Fv() {
    float limit = from_bits(0x43c90000u);
    int mode;
    if (progress < limit) {
        mode = asyncMode;
        unsigned addend = mode == 3 ? 0x3faaaaaau : 0x3f555554u;
        progress = progress + from_bits(addend);
    } else {
        progress = limit;
        mode = asyncMode;
    }

    if (!(gamePause != 0 && shopRunning == 0 && interfaceOn == 0)) {
        int frame = local_frame_count + 1;
        if (frame < 61) local_frame_count = frame;
        else {
            local_frame_count = 0;
            curPlaySeconds += 1;
        }
    }
    MC_CheckPointAge += 1;

    if (disableMC != 0 && mode != 0 && mode != 7) return;
    if (mode != -1) return;
    if (saveCheckPointCount <= 0) return;
    if (frontEndRunning != 0) return;
    if (gameRecallInhibit != 0 && strcmp(engineCurrentLevelName, "arena1") != 0) return;
    if (gameNoDraw != 0) return;
    if (load_word(gameCam + 52) != 1u) return;
    if (strstr(engineCurrentLevelName, "kelsel") != 0) return;
    if (gamePause != 0) return;
    if (saveCheckPointCount == 99) {
        saveCheckPointCount = 0;
        return;
    }
    saveCheckPointCount -= 1;
    if (saveCheckPointCount != 0) return;
    scePrintf("mcHeartBeat RUN\n");
    MC_SaveCheckPoint__Fiii(0, 1, 0);
}
