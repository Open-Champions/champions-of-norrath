#define NORRATH_HOST_ARENA 1
#include "unload.cpp"
#include <string.h>
#include <string>

static unsigned char arena[0x8000];
extern "C" unsigned char* norrath_host_pointer(unsigned a) { return arena + a; }
static std::string tr; static int flags[4], fcall;
extern "C" int VIGetFeatureFlags__Fv() { tr += "F;"; return flags[fcall++]; }
extern "C" void StopBgm__13VISoundDevicei(void* d, int s) { tr += "B" + std::to_string((unsigned char*)d - arena) + "," + std::to_string(s) + ";"; }
extern "C" void Stop__13VISoundDevicei(void* d, int id) { tr += "S" + std::to_string((unsigned char*)d - arena) + "," + std::to_string(id) + ";"; }
extern "C" void ReleaseSound__13VISoundDeviceiR12VIDictionary(void* d, int s, void* dict) { tr += "R" + std::to_string((unsigned char*)d - arena) + "," + std::to_string(s) + "," + std::to_string((unsigned char*)dict - arena) + ";"; }

// music object at arena 0x100; its owner (arena 0x40) holds the device (0x300) and dictionary (0x400) words
static unsigned char* music = arena + 0x100;
int main() {
    unsigned char* o = arena + 0x40;                // owner at arena offset 0x40
    auto setup = [&] {
        memset(arena, 0x11, sizeof arena); tr.clear(); fcall = 0; flags[0] = flags[1] = 0;
        *(unsigned*)music = 0x40;
        *(unsigned*)(o + 19360) = 0x300;             // device
        *(unsigned*)(o + 19352) = 0x400;             // dictionary
        *(int*)(music + 4) = 3; *(int*)(music + 8) = 4; *(int*)(music + 24) = 12; *(int*)(music + 28) = 9;
    };
    setup(); flags[0] = 0x100;
    unsigned char before[0x200]; memcpy(before, music, sizeof before);
    Unload__13VIBattleMusic(music);
    if (tr != "F;" || memcmp(before, music, sizeof before) != 0) return 1;
    setup(); *(unsigned*)music = 0;
    Unload__13VIBattleMusic(music);
    if (tr != "F;" || *(int*)(music + 4) != 3) return 2;
    setup(); *(unsigned*)(o + 19360) = 0;
    Unload__13VIBattleMusic(music);
    if (tr != "F;" || *(int*)(music + 4) != 3) return 3;
    // normal path
    setup();
    Unload__13VIBattleMusic(music);
    if (tr != "F;F;S768,9;R768,12,1024;") return 4;
    if (*(int*)(music + 28) != -1 || *(int*)(music + 8) != 0 || *(int*)(music + 24) != -1 || *(int*)(music + 4) != -1) return 5;
    // no playback and no loaded sound
    setup(); *(int*)(music + 28) = -1; *(int*)(music + 24) = -1;
    Unload__13VIBattleMusic(music);
    if (tr != "F;F;" || *(int*)(music + 8) != 0 || *(int*)(music + 4) != -1 || *(int*)(music + 24) != -1) return 6;
    // sound id 0 is non-negative: released
    setup(); *(int*)(music + 24) = 0;
    Unload__13VIBattleMusic(music);
    if (tr != "F;F;S768,9;R768,0,1024;") return 7;
    // the second flag read differs (only reachable if the flags change): StopBgm path
    setup(); flags[1] = 0x100;
    Unload__13VIBattleMusic(music);
    if (tr != "F;F;B768,0;R768,12,1024;" || *(int*)(music + 28) != 9) return 8;
    return 0;
}
