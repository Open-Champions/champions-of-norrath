// Host contract for the two-position WebTrap constructor; not an EE input. The GameObject stub fills the object
// with 0x21 and leaves a known position at +16; the lump stub returns a fixed word.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char vt_WebTrap[8];
int frameNm;
static std::string calls;
static unsigned char* the_obj;
static float stub_position[3];
extern "C" void game_object_construct(void* self, Point3 p, void* map, char** names) {
    calls += "go(" + std::to_string(self == the_obj) + "," + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "," +
             std::to_string(map == nullptr) + "," + std::to_string(names == nullptr) + ");";
    std::memset(self, 0x21, 192);
    const unsigned flags = 0x100;
    std::memcpy(static_cast<unsigned char*>(self) + 12, &flags, 4);
    std::memcpy(static_cast<unsigned char*>(self) + 16, stub_position, 12);
}
extern "C" void objectAddToRunList__FP10GameObject(void* o) {
    unsigned w12, w140;
    std::memcpy(&w12, the_obj + 12, 4); std::memcpy(&w140, the_obj + 140, 4);
    calls += "run(" + std::to_string(o == the_obj) + "," + std::to_string(w12) + "," + std::to_string(w140) + ");";
}
extern "C" void* lumpFindResource__FPcT0(char* lump, char* name) {
    calls += std::string("lump(") + lump + "," + name + ");";
    return reinterpret_cast<void*>(0x5a5a);
}
int main() {
    alignas(16) static unsigned char o[256];
    the_obj = o;
    int bad = 0;
    for (int k = 0; k < 4; ++k) {
        const float sp[3] = {5.5f + k, -3.25f * k, 9.0f - k}, off[3] = {0.5f * k, 2.0f, -7.25f};
        std::memcpy(stub_position, sp, 12);
        std::memset(o, 0, sizeof o);
        calls.clear();
        frameNm = 1000 + k;
        __7WebTrapG6Point3T1(o, Point3{1.0f, 2.0f, 3.0f}, Point3{off[0], off[1], off[2]});
        const std::string want = "go(1,1.000000,2.000000,3.000000,1,1);run(1,257,555819297);lump(fx.lmp,spiderweb.tex);";
        if (calls != want) { std::fprintf(stderr, "calls %s\nwant  %s\n", calls.c_str(), want.c_str()); bad = 1; }
        float q[3], half[3], half2[3];
        std::memcpy(q, o + 84, 12); std::memcpy(half, o + 108, 12); std::memcpy(half2, o + 120, 12);
        unsigned vt, w76, w80, w136, w140, w144, w148, w152, w156, w12;
        std::memcpy(&vt, o + 72, 4); std::memcpy(&w76, o + 76, 4); std::memcpy(&w80, o + 80, 4); std::memcpy(&w136, o + 136, 4);
        std::memcpy(&w140, o + 140, 4); std::memcpy(&w144, o + 144, 4); std::memcpy(&w148, o + 148, 4); std::memcpy(&w152, o + 152, 4);
        std::memcpy(&w156, o + 156, 4); std::memcpy(&w12, o + 12, 4);
        bool ok = std::memcmp(q, off, 12) == 0 && std::memcmp(half, half2, 12) == 0 && vt == w11::word(vt_WebTrap) && w76 == 0 && w80 == 1600u + k &&
                  w136 == 0 && w140 == 0x5a5a && w144 == 0 && w148 == 0 && w152 == 1 && w156 == 0 && w12 == 0x101 && o[40] == 0xff && o[41] == 0xff && o[132] == 50;
        for (int i = 0; i < 3; ++i) ok = ok && half[i] == (sp[i] + off[i]) * 0.5f;
        if (!ok) { std::fprintf(stderr, "object words wrong (case %d)\n", k); bad = 1; }
        if (o[39] != 0x21 || o[42] != 0x21 || o[107] != 0x21 || o[133] != 0x21) { std::fprintf(stderr, "stray store %02x %02x %02x %02x\n", o[39], o[42], o[107], o[133]); bad = 1; }
    }
    if (!bad) std::puts("webtrap points ctor ok");
    return bad;
}
