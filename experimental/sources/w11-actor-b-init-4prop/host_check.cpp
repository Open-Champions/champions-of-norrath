// Host contract for init__4Propi; not an EE input. Includes the source (host mode: words are offsets).
#define W11_ACTOR_B_HOST 1
#include "init.cpp"
#include <cstdio>
#include <cstring>
#include <string>

unsigned char* w11_actor_b_host_base = nullptr;
unsigned w11_actor_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
int levelFlags;
static unsigned char obj[512] __attribute__((aligned(16)));
static std::string calls;
extern "C" {
void objectUpdateInGrid__FP10GameObject(void* o) { calls += "grid:" + std::to_string(*reinterpret_cast<unsigned*>(static_cast<unsigned char*>(o) + 12)) + ";"; }
void objectAddToRunList__FP10GameObject(void*) { calls += "add;"; }
void objectRemoveFromRunList__FP10GameObject(void*) { calls += "remove;"; }
}
template <class T> static T get(unsigned off) { T v; std::memcpy(&v, obj + off, sizeof v); return v; }
template <class T> static void put(unsigned off, T v) { std::memcpy(obj + off, &v, sizeof v); }
static unsigned step(unsigned s) { return s * 1664525u + 1013904223u; }

int main() {
    // plain: no flags
    std::memset(obj, 0, sizeof obj); calls.clear(); levelFlags = 0;
    init__4Propi(obj, 0);
    if (calls != "grid:6153;add;") { std::puts("fail: plain calls"); return 1; }
    if (get<unsigned>(12) != 0x1809 || get<float>(28) != 4.0f || get<short>(270) != 180) { std::puts("fail: plain fields"); return 2; }
    // flags 0x100 | 0x200 | 0x2000 | 0x10000 | 2 | 0x8000
    std::memset(obj, 0, sizeof obj); calls.clear();
    init__4Propi(obj, 0x100 | 0x200 | 0x2000 | 0x10000 | 2 | 0x8000);
    if (get<unsigned>(12) != (0x1809 | 0x4000) || get<unsigned short>(320) != (2 | 4 | 8 | 0x20)) { std::puts("fail: bits"); return 3; }
    if (get<short>(268) != 2000 || get<short>(270) != 32767) { std::puts("fail: shorts"); return 4; }
    // 0x4000 selects 0x10 when 0x2000 is clear
    std::memset(obj, 0, sizeof obj); init__4Propi(obj, 0x4000);
    if (get<unsigned short>(320) != 0x10) { std::puts("fail: 0x4000"); return 5; }
    // 0x80: halfword 270 cleared, bit 1 absent -> remove call
    std::memset(obj, 0, sizeof obj); calls.clear();
    init__4Propi(obj, 0x80);
    if (calls != "grid:6153;add;remove;" || get<unsigned short>(320) != 1 || get<short>(270) != 0) { std::puts("fail: 0x80"); return 6; }
    std::memset(obj, 0, sizeof obj); calls.clear();
    init__4Propi(obj, 0x80 | 0x100);
    if (calls != "grid:6153;add;" || get<unsigned short>(320) != 3) { std::puts("fail: 0x80|0x100"); return 7; }
    // random draws: flag 1 uses two draws, flag 0x10 three
    {
        std::memset(obj, 0, sizeof obj); put<unsigned>(68, 12345);
        init__4Propi(obj, 1);
        unsigned s1 = step(12345), s2 = step(s1);
        float a = (static_cast<float>(s1 >> 16) * 1.52587890625e-05f - 0.5f) * 124.0f;
        float b = (static_cast<float>(s2 >> 16) * 1.52587890625e-05f - 0.5f) * 124.0f;
        if (get<float>(252) != a || get<float>(256) != b || get<unsigned>(68) != s2) { std::puts("fail: flag 1"); return 8; }
        std::memset(obj, 0, sizeof obj); put<unsigned>(68, 99);
        init__4Propi(obj, 0x10);
        s1 = step(99); s2 = step(s1); unsigned s3 = step(s2);
        if (get<unsigned>(68) != s3 || get<float>(48) != static_cast<float>(s3 >> 16) * 1.52587890625e-05f * 480.0f) { std::puts("fail: flag 0x10"); return 9; }
        std::memset(obj, 0, sizeof obj); put<unsigned>(68, 7);
        init__4Propi(obj, 0x20);
        s1 = step(7);
        if (get<float>(48) != static_cast<float>(s1 >> 16) * 1.52587890625e-05f * 30.0f + 130.0f) { std::puts("fail: flag 0x20"); return 10; }
        std::memset(obj, 0, sizeof obj); put<unsigned>(68, 7);
        init__4Propi(obj, 0x40);
        if (get<float>(48) != static_cast<float>(s1 >> 16) * 1.52587890625e-05f * 60.0f + 260.0f) { std::puts("fail: flag 0x40"); return 11; }
        std::memset(obj, 0, sizeof obj); put<unsigned>(68, 5);
        init__4Propi(obj, 8);
        s1 = step(5); s2 = step(s1);
        if (get<float>(256) != (static_cast<float>(s2 >> 16) * 1.52587890625e-05f - 0.5f) * 224.0f) { std::puts("fail: flag 8"); return 12; }
    }
    // level flag 0x100: clamp and scale
    levelFlags = 0x100;
    std::memset(obj, 0, sizeof obj); put<float>(48, 1000.0f); put<float>(252, 8.0f); put<float>(256, -6.0f);
    init__4Propi(obj, 0);
    if (get<float>(48) != 100.0f || get<float>(252) != 4.0f || get<float>(256) != -3.0f) { std::puts("fail: level scale"); return 13; }
    std::memset(obj, 0, sizeof obj); put<float>(48, 40.0f);
    init__4Propi(obj, 0);
    if (get<float>(48) != 10.0f) { std::puts("fail: level no clamp"); return 14; }
    return 0;
}
