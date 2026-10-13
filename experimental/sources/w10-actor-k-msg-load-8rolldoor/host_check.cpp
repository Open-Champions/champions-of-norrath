// Host contract for msg_load__8RollDoorPc; not an EE input. The world calls and the run-list
// call are stubs that log; the object bytes are compared against an expected image.
#include <cstdio>
#include <cstring>
#include <string>
static std::string calls; static int height_to_return; static unsigned char* the_object;
extern "C" { void* world = reinterpret_cast<void*>(0x1234); }
extern "C" void worldSetDoorCollision__FP12_worldHeaderii(void* w, int d, int v) { calls += "set(" + std::to_string(reinterpret_cast<unsigned long>(w)) + "," + std::to_string(d) + "," + std::to_string(v) + ");"; }
extern "C" int worldGetDoorHeight__FP12_worldHeaderi(void* w, int d) { calls += "get(" + std::to_string(reinterpret_cast<unsigned long>(w)) + "," + std::to_string(d) + ");"; return height_to_return; }
extern "C" void objectAddToRunList__FP10GameObject(void* o) {
    unsigned w; std::memcpy(&w, the_object + 12, 4);
    calls += std::string("run(") + (o == the_object ? "self" : "other") + "," + std::to_string(w) + ");";
}
#include "msg_load.cpp"
int main() {
    alignas(16) static unsigned char o[256], buf[64];
    int bad = 0;
    struct Case { int w0, w1; float f; int w3; int height; };
    const Case cases[] = {{0, 5, 1.5f, 77, 9}, {1, -3, -2.25f, -1, -40000}, {0x10000, 0x12345, 0.0f, 0, 3}, {0x20001, 70000, 8.0f, 5, 100000}};
    for (const Case& c : cases) {
        std::memset(o, 0xff, sizeof o); std::memset(buf, 0, sizeof buf); calls.clear(); the_object = o;
        std::memcpy(buf, &c.w0, 4); std::memcpy(buf + 4, &c.w1, 4); std::memcpy(buf + 8, &c.f, 4); std::memcpy(buf + 12, &c.w3, 4);
        height_to_return = c.height;
        const int r = msg_load__8RollDoorPc(o, reinterpret_cast<char*>(buf));
        short h76, h78, h90; float f80; int w84; unsigned w12;
        std::memcpy(&h76, o + 76, 2); std::memcpy(&h78, o + 78, 2); std::memcpy(&h90, o + 90, 2);
        std::memcpy(&f80, o + 80, 4); std::memcpy(&w84, o + 84, 4); std::memcpy(&w12, o + 12, 4);
        const short s1 = static_cast<short>(c.w1), s0 = static_cast<short>(c.w0);
        if (r != 16 || h76 != s1 || h90 != s0 || f80 != c.f || w84 != c.w3 || h78 != static_cast<short>(c.height)) bad = 1;
        std::string want = "set(4660," + std::to_string(s1) + ",1);get(4660," + std::to_string(s1) + ");";
        if (s0 != 0) want += "run(self," + std::to_string(0xffefffffu) + ");";
        if (calls != want) { std::printf("calls %s want %s\n", calls.c_str(), want.c_str()); bad = 1; }
        if ((w12 == 0xffefffffu) != (s0 != 0)) bad = 1;
        unsigned char* p = o + 100; for (int i = 100; i < 256; ++i, ++p) if (*p != 0xff) bad = 1;
    }
    return bad;
}
