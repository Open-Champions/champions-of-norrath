// Host contract for msg_poke__6Switchi; not an EE input. Includes the source.
#include "poke.cpp"
#include <cstdio>
#include <cstring>
#include <string>

int world;
static std::string calls;
static unsigned char arena[128] __attribute__((aligned(16)));
extern "C" int worldMoveDoor__FP12_worldHeaderiG6Point3s(void* w, int index, Point3 p, short extra) {
    unsigned flags; std::memcpy(&flags, arena + 12, 4);
    calls += "door(" + std::to_string(static_cast<int>(reinterpret_cast<__UINTPTR_TYPE__>(w) == 0x1234)) + "," + std::to_string(index) + "," +
             std::to_string(static_cast<int>(p.x)) + ":" + std::to_string(static_cast<int>(p.y)) + ":" + std::to_string(static_cast<int>(p.z)) + "," + std::to_string(extra) +
             ",flags" + std::to_string(flags) + ");";
    return 0;
}
extern "C" void objectDoMyActions__FP10GameObjecti(void* o, int on) { calls += std::string(o == arena ? "act" : "bad") + std::to_string(on) + ";"; }

int main() {
    world = 0x1234;
    unsigned flags = 0xffffffffu; std::memcpy(arena + 12, &flags, 4);
    short door = -3; std::memcpy(arena + 76, &door, 2);
    msg_poke__6Switchi(arena, 77);
    std::memcpy(&flags, arena + 12, 4);
    if (flags != 0xffefffffu) { std::puts("fail: flags"); return 1; }
    if (calls != "door(1,-3,8:0:0,0,flags4293918719);act1;") { std::puts(calls.c_str()); return 2; }
    return 0;
}
