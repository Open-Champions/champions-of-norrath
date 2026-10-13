// Host contract for JumpGate msg_load; not an EE input.
#include "msg_load.cpp"
#include <cstdio>
#include <cstring>
#include <string>
static unsigned char object[400];
static std::string calls; static void* found = &calls;
void lightActivate__FP5Light(void* l) { calls += "light" + std::to_string(static_cast<unsigned char*>(l) - object) + ";"; }
void* lumpFindResource__FPcT0(char* n, char* s) { calls += "lump" + std::to_string(reinterpret_cast<unsigned char*>(n) - object) + ":" + s + ";"; return found; }
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* st, void* h, int m) { calls += "anim" + std::to_string(static_cast<unsigned char*>(st) - object) + (h == found ? "ok" : "bad") + std::to_string(m) + ";"; }
void objectAddToRunList__FP10GameObject(void* o) { calls += o == object ? "run;" : "runbad;"; }
int main() {
    int bad = 0;
    const char values[][2] = {{9, 0}, {9, 1}, {0, 1}, {5, -1}, {0, 0}};
    for (auto& v : values) {
        std::memset(object, 0x11, sizeof object); calls.clear();
        char in[4] = {v[0], v[1], 0x55, 0x66};
        int r = msg_load__8JumpGatePc(object, in);
        if (r != 2) { std::puts("fail: result 2"); bad = 1; }
        if (object[77] != static_cast<unsigned char>(v[0]) || object[78] != static_cast<unsigned char>(v[1])) { std::puts("fail: stored bytes"); bad = 1; }
        for (int i = 0; i < 400; ++i) if (i != 77 && i != 78 && object[i] != 0x11) { std::puts("fail: other byte written"); bad = 1; break; }
        std::string want = v[1] != 0 ? "light256;lump144:jumpgate_idle.anm;anim216ok3;run;" : "run;";
        if (calls != want) { std::printf("fail: calls %s\n", calls.c_str()); bad = 1; }
    }
    if (!bad) std::puts("jumpgate msg_load ok");
    return bad;
}
