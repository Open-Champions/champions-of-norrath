// Host contract for the GutChunk model/animation constructor; not an EE input. The PhProp stub logs its
// arguments and writes a known position into the object at +16.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char vt_GutChunk[8];
static std::string calls;
static unsigned char* the_obj;
extern "C" void __6PhPropG6Point3sP12_modelHeaderP14AnimationStatePcN25liPQ24item11descriptionf(void* self, Point3 p, short yaw, void* model, void* animation,
                                                                                    char* a, char* b, char* c, long n, int flags, void* description,
                                                                                    float scale) {
    calls += "phprop(" + std::to_string(self == the_obj) + "," + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "," +
             std::to_string(yaw) + "," + std::to_string(reinterpret_cast<unsigned long>(model)) + "," + std::to_string(reinterpret_cast<unsigned long>(animation)) + "," +
             std::to_string(reinterpret_cast<unsigned long>(a)) + "," + std::to_string(reinterpret_cast<unsigned long>(b)) + "," + std::to_string(reinterpret_cast<unsigned long>(c)) +
             "," + std::to_string(n) + "," + std::to_string(flags) + "," + std::to_string(description == nullptr) + "," + std::to_string(scale) + ");";
    std::memset(self, 0x21, 128);
    const float pos[3] = {7.0f, 8.0f, 9.0f};
    std::memcpy(static_cast<unsigned char*>(self) + 16, pos, 12);
}
int main() {
    alignas(16) static unsigned char o[1024];
    the_obj = o;
    int bad = 0;
    for (int k = 0; k < 3; ++k) {
        std::memset(o, 0, sizeof o);
        calls.clear();
        __8GutChunkG6Point3sP12_modelHeaderP14AnimationStatePcN25liif(o, Point3{1.0f, -2.0f, 3.5f}, static_cast<short>(-3 * k), reinterpret_cast<void*>(0x10 + k),
                                                                      reinterpret_cast<void*>(0x20), reinterpret_cast<char*>(0x30), reinterpret_cast<char*>(0x40),
                                                                      reinterpret_cast<char*>(0x50), 99 + k, 5 + k, 1000 + k, 0.25f * k);
        std::string want = "phprop(1,1.000000,-2.000000,3.500000," + std::to_string(-3 * k) + "," + std::to_string(0x10 + k) + ",32,48,64,80," +
                           std::to_string(99 + k) + "," + std::to_string(5 + k) + ",1," + std::to_string(0.25f * k) + ");";
        if (calls != want) { std::fprintf(stderr, "calls %s\nwant  %s\n", calls.c_str(), want.c_str()); bad = 1; }
        float pos[3], copy[3];
        std::memcpy(pos, o + 16, 12); std::memcpy(copy, o + 592, 12);
        unsigned sel, two, vt; std::memcpy(&sel, o + 584, 4); std::memcpy(&two, o + 604, 4); std::memcpy(&vt, o + 72, 4);
        if (std::memcmp(pos, copy, 12) != 0 || pos[0] != 7.0f || sel != 1000u + k || two != 2 || o[608] != 0 || vt != w11::word(vt_GutChunk)) {
            std::fprintf(stderr, "object words wrong (case %d)\n", k); bad = 1;
        }
        if (o[583] != 0 || o[609] != 0 || o[100] != 0x21) { std::fprintf(stderr, "stray store\n"); bad = 1; }
    }
    if (!bad) std::puts("gutchunk ctor ok");
    return bad;
}
