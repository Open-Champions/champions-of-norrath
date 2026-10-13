// Host contract for the MummySnake constructor; not an EE input. Stubs log the call order and the
// object words at each call. The GameObject stub fills the object with 0x21 and sets +12 to 0x100.
#include "ctor.cpp"
#include <cstring>
#include <string>
unsigned char vt_MummySnake[8];
int frameNm;
static std::string calls; static unsigned char* the_obj;
static std::string snap() {
    unsigned a[5];
    std::memcpy(&a[0], the_obj + 12, 4); std::memcpy(&a[1], the_obj + 76, 4); std::memcpy(&a[2], the_obj + 80, 4);
    std::memcpy(&a[3], the_obj + 88, 4); std::memcpy(&a[4], the_obj + 72, 4);
    return "[" + std::to_string(a[0]) + "," + std::to_string(a[1]) + "," + std::to_string(a[2]) + "," + std::to_string(a[3]) + "," + std::to_string(a[4] == w11::word(vt_MummySnake)) + "]";
}
extern "C" void game_object_construct(void* self, Point3 p, void* map, char** names) {
    calls += "go(" + std::to_string(self == the_obj) + "," + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "," + std::to_string(map == nullptr) + "," + std::to_string(names == nullptr) + ");";
    std::memset(self, 0x21, 128);
    const unsigned w12 = 0x100; std::memcpy(static_cast<unsigned char*>(self) + 12, &w12, 4);
}
extern "C" void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data) {
    calls += "link(" + std::to_string(static_cast<unsigned char*>(head) - the_obj) + "," + std::to_string(reinterpret_cast<unsigned long>(link)) + "," + std::to_string(link == data) + ")" + snap() + ";";
}
extern "C" void objectAddToRunList__FP10GameObject(void* o) { calls += "run" + std::to_string(o == the_obj) + snap() + ";"; }
int main() {
    alignas(16) static unsigned char o[256];
    the_obj = o;
    frameNm = 1000;
    int bad = 0;
    for (void* second : {static_cast<void*>(nullptr), reinterpret_cast<void*>(0x66)}) {
        std::memset(o, 0, sizeof o); calls.clear();
        void* const header = reinterpret_cast<void*>(0x2468);
        __10MummySnakeG6Point3sP10GameObjectT3P15AnimationHeader(o, Point3{1.0f, -2.0f, 3.5f}, 77, reinterpret_cast<void*>(0x55), second, header);
        std::string want = "go(1,1.000000,-2.000000,3.500000,1,1);link(76,85,1)[265,0,0,555819297,1];";
        if (second) want += "link(80,102,1)[265,0,0,555819297,1];";
        want += "run1[265,0,0,1360,1];";
        unsigned w96; std::memcpy(&w96, o + 96, 4);
        if (calls != want || w96 != 0x2468 || o[100] != 0x21 || o[127] != 0x21) { bad = 1; }
    }
    return bad;
}
