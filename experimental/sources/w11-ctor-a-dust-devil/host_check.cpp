// Host contract for __9DustDevilG6Point3; not an EE input. The stubs record the call order and the
// object bytes seen at each call; the GameObject constructor stub fills the object with a pattern
// and a known word at +12.
#include "ctor.cpp"
#include <cstring>
#include <string>
unsigned char vt_DustDevil[8];
int frameNm;
static std::string calls; static unsigned char* the_obj;
static std::string snap() {
    unsigned a, b, c, d;
    std::memcpy(&a, the_obj + 12, 4); std::memcpy(&b, the_obj + 76, 4); std::memcpy(&c, the_obj + 80, 4); std::memcpy(&d, the_obj + 72, 4);
    return "[" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + "," + std::to_string(d == w11::word(vt_DustDevil)) + "]";
}
extern "C" void game_object_construct(void* self, Point3 p, void* map, char** names) {
    calls += "go(" + std::to_string(self == the_obj) + "," + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "," + std::to_string(map == nullptr) + "," + std::to_string(names == nullptr) + ");";
    std::memset(self, 0x21, 128);
    const unsigned w12 = 0x100; std::memcpy(static_cast<unsigned char*>(self) + 12, &w12, 4);
}
extern "C" void* P_CreateParticleSystem__Fv() { calls += "ps" + snap() + ";"; return reinterpret_cast<void*>(0x1234); }
extern "C" void objectAddToRunList__FP10GameObject(void* o) { calls += "run" + std::to_string(o == the_obj) + snap() + ";"; }
int main() {
    alignas(16) static unsigned char o[256];
    the_obj = o;
    frameNm = 1000;
    std::memset(o, 0, sizeof o);
    __9DustDevilG6Point3(o, Point3{1.0f, -2.0f, 3.5f});
    unsigned w84; std::memcpy(&w84, o + 84, 4);
    const std::string want =
        "go(1,1.000000,-2.000000,3.500000,1,1);ps[265,555819297,555819297,1];run1[265,0,1060,1];";
    if (calls != want || w84 != 0x1234 || o[127] != 0x21 || o[88] != 0x21) return 1;
    return 0;
}
