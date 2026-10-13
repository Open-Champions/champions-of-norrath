// Host check only; not an EE build input.
#define W10_ACTOR_Q_HOST 1
#include "msg_draw.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_q_host_base;
static std::string calls; static int failures;
extern "C" void modelSetFade__Fc(char f) { calls += "fade(" + std::to_string(int(f)) + ");"; }
extern "C" int draw__8CreatureP12_modelHeaderP8_textureT1(void* s, void* m, void* t, void* z) {
    calls += "draw(" + std::to_string(w10_actor_q_host_base ? (long)((unsigned char*)m - w10_actor_q_host_base) : 0) + "," +
             std::to_string((long)((unsigned char*)t - w10_actor_q_host_base)) + "," + (z ? "nz" : "0") + ");";
    return 0;
}
extern "C" void modelResetOptionalParams__Fv() { calls += "reset;"; }
static void expect(bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s (%s)\n", m, calls.c_str()); ++failures; } }
int main() {
    static unsigned char base[4096];
    w10_actor_q_host_base = base;
    unsigned char* object = base + 256; unsigned char* block = base + 1024;
    std::memset(base, 0, sizeof base);
    *reinterpret_cast<unsigned*>(object + 36) = 1024;
    *reinterpret_cast<unsigned*>(block + 12) = 3000; *reinterpret_cast<unsigned*>(block + 20) = 3100;
    object[960] = 50; calls.clear(); msg_draw__6Wraith(object);
    expect(calls == "fade(50);draw(3000,3100,0);reset;", "below 98 draws");
    object[960] = 97; calls.clear(); msg_draw__6Wraith(object);
    expect(calls == "fade(97);draw(3000,3100,0);reset;", "97 draws");
    object[960] = 98; calls.clear(); msg_draw__6Wraith(object);
    expect(calls == "fade(98);reset;", "98 skips draw");
    object[960] = 120; calls.clear(); msg_draw__6Wraith(object);
    expect(calls == "fade(120);reset;", "120 skips draw");
    object[960] = 0xff; calls.clear(); msg_draw__6Wraith(object);
    expect(calls == "fade(-1);draw(3000,3100,0);reset;", "signed compare: negative draws");
    if (!failures) std::puts("wraith msg_draw ok");
    return failures ? 1 : 0;
}
