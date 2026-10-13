// Host contract for draw__3WebG6Point3; not an EE input. Callees are stubs that log; the creature's table
// slot at byte 128 is a host function chosen through a function-word index.
#define W11_ACTOR_C_HOST 1
#include "draw.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_c_host_base;
static std::string calls;
static unsigned char base[8192];
static std::string num(double v) { char b[40]; std::snprintf(b, sizeof b, "%.9g", v); return b; }
static std::string off(const void* p) { return p == nullptr ? "null" : std::to_string(static_cast<const unsigned char*>(p) - base); }
static std::string pt(const Point3& p) { return num(p.x) + ":" + num(p.y) + ":" + num(p.z); }
static void* model_marker = base + 5000;
static void* slot_getModel(void* object) { calls += "slot(" + off(object) + ");"; return model_marker; }
W11ActorCHostSlot w11_actor_c_host_slots[] = {nullptr, slot_getModel};
extern "C" void P_Line__FG6Point3T0UcUcUcUcUcUcUcUcf(Point3 from, Point3 to, unsigned char a, unsigned char b, unsigned char c, unsigned char d,
                                                     unsigned char e, unsigned char f, unsigned char g, unsigned char h, float width) {
    calls += "line(" + pt(from) + "," + pt(to) + "," + std::to_string(a) + std::to_string(b) + std::to_string(c) + "," + std::to_string(d) + "," +
             std::to_string(e) + std::to_string(f) + std::to_string(g) + std::to_string(h) + "," + num(width) + ");";
}
extern "C" void DrawWebSplay__FG6Point3iT0iiifP10GameObjectP12_modelHeaderP14AnimationState(Point3 a, int b, Point3 c, int d, int e, int f, float g,
                                                                                              void* creature, void* model, void* animation) {
    calls += "splay(" + pt(a) + "," + std::to_string(b) + "," + pt(c) + "," + std::to_string(d) + "," + std::to_string(e) + "," + std::to_string(f) + "," +
             num(g) + "," + off(creature) + "," + (model == nullptr ? "null" : model == model_marker ? "M" : "?") + "," + off(animation) + ");";
}
int main() {
    w11_actor_c_host_base = base;
    int failures = 0;
    for (unsigned count : {0u, 1u, 0xffffffffu, 255u, 256u, 300u, 0x80000000u})
        for (unsigned w12 : {0u, 9u})
            for (unsigned node : {0u, 1024u})
                for (unsigned flags : {0u, 0x20u, 0xffffffdfu}) {
                    std::memset(base, 0, sizeof base);
                    unsigned char* web = base + 64;
                    unsigned char* nodep = base + 1024;
                    unsigned char* creature = base + 2048;
                    unsigned char* table = base + 3000;
                    *reinterpret_cast<unsigned*>(web + 0) = 77;
                    *reinterpret_cast<float*>(web + 4) = 1.75f;
                    *reinterpret_cast<unsigned*>(web + 8) = count;
                    *reinterpret_cast<unsigned*>(web + 12) = w12;
                    float v[3] = {4.0f, -6.0f, 10.0f};
                    std::memcpy(web + 16, v, 12);
                    *reinterpret_cast<unsigned*>(web + 28) = node;
                    *reinterpret_cast<unsigned*>(nodep + 12) = 2048;
                    *reinterpret_cast<unsigned*>(creature + 12) = flags;
                    *reinterpret_cast<unsigned*>(creature + 72) = 3000;
                    *reinterpret_cast<short*>(table + 128) = 16;
                    *reinterpret_cast<unsigned*>(table + 132) = 1;
                    Point3 p = {1.0f, 2.0f, 3.0f};
                    calls.clear();
                    draw__3WebG6Point3(web, p);
                    std::string e;
                    if (static_cast<int>(count) > 0) {
                        e += "line(1:2:3,2.5:-2:6.5,255255255," + std::to_string(count & 0xffu) + ",2552552550,0);";
                        std::string creatureText = "null", model = "null", anim = "null";
                        if (w12 != 0u && node != 0u) {
                            e += "slot(" + std::to_string(2048 + 16) + ");";
                            creatureText = "2048"; model = "M";
                            if ((flags & 0x20u) != 0u) anim = std::to_string(2048 + 288);
                        }
                        e += "splay(2.5:-2:6.5,0,4:-6:10," + std::to_string(static_cast<int>(count)) + ",77,5,1.75," + creatureText + "," + model + "," + anim + ");";
                    }
                    if (calls != e) { std::fprintf(stderr, "got  %s\nwant %s\n", calls.c_str(), e.c_str()); ++failures; }
                }
    if (!failures) std::puts("web draw ok");
    return failures != 0;
}
