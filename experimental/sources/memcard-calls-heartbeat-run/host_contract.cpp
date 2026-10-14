#include "heartbeat_run.cpp"

#include <cstdio>

extern "C" int asyncMode = -1;
extern "C" int gamePause = 0;
extern "C" int shopRunning = 0;
extern "C" signed char interfaceOn = 0;
extern "C" int curPlaySeconds = 0;
extern "C" int MC_CheckPointAge = 0;
extern "C" int disableMC = 0;
extern "C" int saveCheckPointCount = 0;
extern "C" int frontEndRunning = 0;
extern "C" int gameRecallInhibit = 0;
extern "C" int gameNoDraw = 0;
extern "C" {
unsigned char camera_bytes[56];
char engineCurrentLevelName[80];
}
extern "C" unsigned char* gameCam = camera_bytes;

static int prints;
static const char* printed;
static int saves;
static int save_a, save_b, save_c;

void copy_text(char* to, const char* from) {
    while ((*to++ = *from++) != 0) {}
}

unsigned float_bits(float value) {
    union {
        unsigned bits;
        float value;
    } pun;
    pun.value = value;
    return pun.bits;
}

extern "C" int scePrintf(const char* text) {
    ++prints;
    printed = text;
    return 0;
}
extern "C" void MC_SaveCheckPoint__Fiii(int first, int second, int third) {
    ++saves;
    save_a = first;
    save_b = second;
    save_c = third;
}

void reset_gates() {
    asyncMode = -1;
    gamePause = 0;
    shopRunning = 0;
    interfaceOn = 1;
    disableMC = 0;
    frontEndRunning = 0;
    gameRecallInhibit = 0;
    gameNoDraw = 0;
    saveCheckPointCount = 0;
    camera_bytes[52] = 1;
    camera_bytes[53] = 0;
    camera_bytes[54] = 0;
    camera_bytes[55] = 0;
    copy_text(engineCurrentLevelName, "town");
    prints = 0;
    saves = 0;
}

int main() {
    progress = 500.0f;
    reset_gates();
    local_frame_count = 0;
    MC_CheckPointAge = 0;
    curPlaySeconds = 0;
    mcHeartBeatRun__Fv();
    if (float_bits(progress) != 0x43c90000u) return 1;

    progress = 0.0f;
    asyncMode = 3;
    mcHeartBeatRun__Fv();
    if (float_bits(progress) != 0x3faaaaaau) return 2;
    progress = 0.0f;
    asyncMode = 0;
    mcHeartBeatRun__Fv();
    if (float_bits(progress) != 0x3f555554u) return 3;

    reset_gates();
    progress = 500.0f;
    local_frame_count = 60;
    curPlaySeconds = 4;
    MC_CheckPointAge = 8;
    mcHeartBeatRun__Fv();
    if (local_frame_count != 0 || curPlaySeconds != 5 || MC_CheckPointAge != 9) return 4;

    gamePause = 1;
    shopRunning = 0;
    interfaceOn = 0;
    local_frame_count = 3;
    curPlaySeconds = 5;
    mcHeartBeatRun__Fv();
    if (local_frame_count != 3 || curPlaySeconds != 5 || MC_CheckPointAge != 10) return 5;

    reset_gates();
    progress = 500.0f;
    saveCheckPointCount = 1;
    MC_CheckPointAge = 0;
    mcHeartBeatRun__Fv();
    if (saves != 1 || save_a != 0 || save_b != 1 || save_c != 0) return 6;
    if (prints != 1 || strcmp(printed, "mcHeartBeat RUN\n") != 0) return 7;
    if (saveCheckPointCount != 0) return 8;

    reset_gates();
    progress = 500.0f;
    saveCheckPointCount = 99;
    mcHeartBeatRun__Fv();
    if (saveCheckPointCount != 0 || saves != 0 || prints != 0) return 9;

    reset_gates();
    progress = 500.0f;
    saveCheckPointCount = 2;
    mcHeartBeatRun__Fv();
    if (saveCheckPointCount != 1 || saves != 0) return 10;

    reset_gates();
    progress = 500.0f;
    saveCheckPointCount = 1;
    copy_text(engineCurrentLevelName, "xkelsely");
    mcHeartBeatRun__Fv();
    if (saves != 0 || saveCheckPointCount != 1) return 11;

    reset_gates();
    progress = 500.0f;
    saveCheckPointCount = 1;
    gameRecallInhibit = 1;
    copy_text(engineCurrentLevelName, "arena2");
    mcHeartBeatRun__Fv();
    if (saves != 0) return 12;
    copy_text(engineCurrentLevelName, "arena1");
    saveCheckPointCount = 1;
    mcHeartBeatRun__Fv();
    if (saves != 1) return 13;

    reset_gates();
    progress = 500.0f;
    saveCheckPointCount = 1;
    camera_bytes[52] = 0;
    mcHeartBeatRun__Fv();
    if (saves != 0) return 14;
    std::puts("heartbeat run passed");
    return 0;
}
