// Host contract for msg_hurt__11VampireLordfP10GameObjecti; not an EE input. Words are host offsets.
#define W10_ACTOR_P_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_p_host_base;
#include "msg_hurt.cpp"

char bossMeterName[128];
int bossMeterCurrent;
static unsigned char arena[0x1000] __attribute__((aligned(16)));
static std::string calls;
extern "C" void msg_hurt__8CreaturefP10GameObjecti(void* s, float a, void* src, int k) {
    calls += "base(" + std::to_string(static_cast<int>(a)) + "," + std::to_string(k) + (src == reinterpret_cast<void*>(8) ? ",src" : ",other") + ");";
}
unsigned w10_actor_p_host_call(unsigned fn, void* adjusted, int n, unsigned long a1, unsigned long) {
    calls += "slot(" + std::to_string(fn) + "," + (adjusted == arena + 0x200 + 5 ? "adj" : "bad") + "," + std::to_string(n) + "," + std::to_string(a1) + ");";
    return 0;
}
static void put32(unsigned char* base, unsigned at, unsigned v) { std::memcpy(base + at, &v, 4); }
static void putf(unsigned char* base, unsigned at, float v) { std::memcpy(base + at, &v, 4); }
int main() {
    w10_actor_p_host_base = arena;
    unsigned char* const obj = arena + 0x200;
    struct Case { float cur, max; unsigned flag; bool call; };
    const Case cases[] = {{40, 100, 0, true}, {50, 100, 0, false}, {49.9f, 100, 0, true}, {10, 100, 1, false}, {-3.5f, 100, 0, true}, {75, 100, 7, false}, {60, 0, 0, false}};
    for (const Case& c : cases) {
        std::memset(arena, 0, sizeof arena); calls.clear();
        std::memset(bossMeterName, 'x', sizeof bossMeterName); bossMeterCurrent = -77;
        putf(obj, 560, c.cur); putf(obj, 564, c.max); put32(obj, 1008, c.flag);
        put32(obj, 72, 0x800); put32(arena + 0x800, 188, 0x4321); { short adj = 5; std::memcpy(arena + 0x800 + 184, &adj, 2); }
        msg_hurt__11VampireLordfP10GameObjecti(obj, 6.0f, reinterpret_cast<void*>(8), 3);
        std::string want = "base(6,3,src);";
        if (c.call) want += "slot(17185,adj,1,21);";
        unsigned flag; std::memcpy(&flag, obj + 1008, 4);
        if (calls != want || flag != (c.call ? 1u : c.flag)) { std::printf("fail cur=%g: %s\n", c.cur, calls.c_str()); return 1; }
        if (std::strcmp(bossMeterName, "VampireLord") != 0 || bossMeterName[12] != 'x' || bossMeterCurrent != static_cast<int>(c.cur)) return 2;
    }
    return 0;
}
