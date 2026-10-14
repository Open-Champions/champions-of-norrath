// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
#include "add_play.cpp"
#include <cstring>
#include <string>
static std::string trace;
static int shareRet, playIndex, preRet;
static int nodeIndexes[4], nodeCalls;
static long long clock64 = 1000;
static int foundIndex[4] = {0, 7, 1, 7};
extern "C" unsigned char* RefMap__7VIScenei(void*, int index) { trace += "R" + std::to_string(index) + ";"; return arena + 0x2000; }
extern "C" unsigned char* Animation__7VIScenei(void*, int id) { trace += "A" + std::to_string(id) + ";"; return arena + 0x3000; }
extern "C" int ShareAnimation__7VIScenei(void*, int) { trace += "S;"; return shareRet; }
extern "C" long long GetTime__7VIScene(void*) { trace += "T;"; return clock64++; }
extern "C" int Add__t6VIPool1Z13VIHSpritePlay(void*) { trace += "P;"; return playIndex; }
extern "C" int Add__t6VIPool1Z17VIHSpritePlayNode(void*) { trace += "N;"; return nodeIndexes[nodeCalls++]; }
extern "C" int Find__Ct5VIMap2ZiZiRCi(void*, const int* key) { trace += "F" + std::to_string(*key) + ";"; return foundIndex[*key - 100]; }
extern "C" int End__Ct5VIMap2ZiZi(void*) { return 7; }
extern "C" int PrecalcPlayback__9VIHSpritei(void*, int index) { trace += "C" + std::to_string(index) + ";"; return preRet; }
static unsigned& w(unsigned char* base, int off) { return *reinterpret_cast<unsigned*>(base + off); }
int main() {
    alignas(8) static unsigned char self[512];
    unsigned char* anim = arena + 0x3000;
    w(self, 384) = 9;
    w(self, 128) = 0x4000; w(self, 152) = 0x5000;           // element bases of the play pool and the node pool
    w(arena + 0x2000, 16) = 0x2800;                         // map element array
    w(anim, 12) = 3; w(anim, 36) = 0x3800;
    float nominal = 2.5f; std::memcpy(anim + 24, &nominal, 4);
    for (int i = 0; i < 3; ++i) { w(arena + 0x3800, 8 * i) = 100 + i; w(arena + 0x3800, 8 * i + 4) = 0x900 + i; }
    w(arena + 0x2800, 20) = 41; w(arena + 0x2800, 24 + 20) = 42;   // values for map entries 0 and 1
    nodeIndexes[0] = 3; nodeIndexes[1] = 4; playIndex = 2; shareRet = 0; preRet = 0;
    int r = AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene(self, 77, 1, 0.5f, arena);
    if (r != 2) return 1;
    // key 101 maps to End, so only keys 100 and 102 reach the node pool
    if (trace != "R9;A77;P;S;T;T;F100;N;F101;F102;N;C2;") return 2;
    unsigned char* play = arena + 0x4000 + 2 * 104;
    if (w(play, 0) != 77 || w(play, 4) != 1 || w(play, 8) != 1 || w(play, 12) != 0) return 3;
    long long t0, t1; std::memcpy(&t0, play + 16, 8); std::memcpy(&t1, play + 64, 8);
    if (t0 != 1000 || t1 != 1001) return 4;
    float f36, f40; std::memcpy(&f36, play + 36, 4); std::memcpy(&f40, play + 40, 4);
    if (f36 != 2.5f || f40 != 0.5f) return 5;
    if (w(play, 80) != 2 || static_cast<int>(w(play, 84)) != 3 || static_cast<int>(w(play, 88)) != 4) return 6;
    unsigned char* n0 = arena + 0x5000 + 3 * 52;
    unsigned char* n1 = arena + 0x5000 + 4 * 52;
    if (w(n0, 4) != 41 || w(n0, 8) != 0x900 || w(n1, 4) != 42 || w(n1, 8) != 0x902) return 7;
    // failure paths
    trace.clear(); playIndex = -1;
    if (AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene(self, 77, 1, 0.5f, arena) != -1 || trace.find("S;") != std::string::npos) return 8;
    playIndex = 2; shareRet = -1;
    if (AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene(self, 77, 1, 0.5f, arena) != -1) return 9;
    shareRet = 0; preRet = -4; nodeCalls = 0;
    if (AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene(self, 77, 1, 0.5f, arena) != -1) return 10;
    preRet = 0; nodeIndexes[0] = -1; nodeCalls = 0;
    if (AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene(self, 77, 1, 0.5f, arena) != -1) return 11;
    return 0;
}
