// Host check only; not a build input.
#include "set_defaults.cpp"
#include <cstring>
#include <initializer_list>
static int apRet[2], apN, attRet, hairRet, hairN, rollV; static long long timeV; static int apIds[2], apPb[2]; static float apF[2];
static int attId, order[8], ordN;
extern "C" int AddPlay__9VIHSpritei14VIPlaybackTypefR7VIScene(void*, int id, int pb, float v, void*) { order[ordN++] = 1; apIds[apN] = id; apPb[apN] = pb; apF[apN] = v; return apRet[apN++]; }
extern "C" int AttachSkin__9VIHSpriteiR7VIScene(void*, int id, void*) { order[ordN++] = 2; attId = id; return attRet; }
extern "C" long long GetTime__7VIScene(void*) { order[ordN++] = 3; return timeV; }
extern "C" int rand(void) { order[ordN++] = 4; return rollV; }
extern "C" int SetHair__9VICSpriteiiiR7VIScene(void*, int a, int b, int c, void*) { order[ordN++] = 5; ++hairN; return a == 0 && b == 2 && c == 1 ? hairRet : 99; }
static void put(unsigned char* b, int off, int v) { std::memcpy(b + off, &v, 4); }
int main() {
    alignas(8) static unsigned char o[5000], scene[20000];
    for (int kind : {0, 1}) for (int a0 : {3, -1}) for (int a1 : {4, -2}) for (int att : {7, -3}) for (int flag : {0, 9}) for (int hair : {0, -1}) {
        std::memset(o, 0x33, sizeof o); std::memset(scene, 0, sizeof scene);
        put(o, 404, kind); put(o, 424, 100); put(o, 428, 200); put(o, 436, 5); put(o, 3856, 300);
        float rate = 1.25f; std::memcpy(o + 432, &rate, 4);
        put(scene, 19356, flag);
        apRet[0] = a0; apRet[1] = a1; apN = hairN = ordN = 0; attRet = att; hairRet = hair; rollV = -45678; timeV = 0x100000005LL;
        int r = SetDefaults__9VICSpriteR7VIScene(o, scene);
        int want;
        if (a0 < 0) want = -1; else if (kind == 1 && a1 < 0) want = -1; else if (att < 0) want = -1; else if (!flag) want = 0; else want = hair < 0 ? -1 : 0;
        if (r != want) return 1;
        int w; std::memcpy(&w, o + 4496, 4); if (w != a0) return 2;
        if (apIds[0] != 100 || apPb[0] != 5 || apF[0] != 1.25f) return 3;
        if (a0 >= 0 && kind == 1) { std::memcpy(&w, o + 4512, 4); if (w != a1 || apIds[1] != 200 || apPb[1] != 5 || apF[1] != 1.25f) return 4; }
        if (kind != 1 && apN != 1) return 5;
        if (a0 >= 0 && (kind != 1 || a1 >= 0)) { if (attId != 300) return 6; std::memcpy(&w, o + 4412, 4); if (w != att) return 7; }
        bool reached = want == 0 || (att >= 0 && (a0 >= 0 && (kind != 1 || a1 >= 0)));
        if (reached) {
            long long t; std::memcpy(&t, o + 4528, 8);
            if (t != timeV + (rollV % 20000) + 20000) return 8;
            float f[8]; std::memcpy(f, o + 4424, 4 * 8);
            const float ex[8] = {0, 0, 0, 0, 1, 0, 2, 0.5f};
            for (int i = 0; i < 8; ++i) if (f[i] != ex[i]) return 9;
            if (hairN != (flag ? 1 : 0)) return 10;
            if (order[ordN - 1] != (flag ? 5 : 4)) return 11;
        } else {
            long long t; std::memcpy(&t, o + 4528, 8);
            if (t != 0x3333333333333333LL || hairN) return 12;
        }
    }
    return 0;
}
