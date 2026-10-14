// Host contract for __7Weatheri; not an EE input. Table words are offsets from a host base.
#define W10_ACTOR_P_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_p_host_base;
#include "weather.cpp"

static unsigned char arena[0x400] __attribute__((aligned(16)));
unsigned char vt_Weather[8];
static std::string calls;
static int sentinel;
extern "C" void* gameobject_construct(void* self, Point3 p, void* map, char** args) __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
extern "C" void* gameobject_construct(void* self, Point3 p, void* map, char** args) {
    unsigned w72, w76; std::memcpy(&w72, arena + 72, 4); std::memcpy(&w76, arena + 76, 4);
    calls += std::string(self == arena ? "base" : "bad") + "(" + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "," +
             (map ? "map" : "null") + "," + (args ? "args" : "null") + ")" + (w72 == 0xa5a5a5a5u && w76 == 0xa5a5a5a5u ? "untouched;" : "touched;");
    return &sentinel;
}
int main() {
    w10_actor_p_host_base = arena;
    for (int arg : {0, 1, -1, 7, 0x12345678}) {
        std::memset(arena, 0xa5, sizeof arena); calls.clear();
        void* r = weather_construct(arena, arg);
        if (r != &sentinel || calls != "base(0.000000,0.000000,0.000000,null,null)untouched;") { std::puts(calls.c_str()); return 1; }
        int w76; unsigned w72; std::memcpy(&w76, arena + 76, 4); std::memcpy(&w72, arena + 72, 4);
        if (w76 != arg || w72 != static_cast<unsigned>(vt_Weather - arena)) return 2;
        for (int i = 0; i < 0x400; ++i) if ((i < 72 || i >= 80) && arena[i] != 0xa5) return 3;
    }
    return 0;
}
