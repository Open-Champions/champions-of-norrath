// Host contract for msg_draw__8RollDoor; not an EE input. The three callees are stubs: the
// matrix builder fills the matrix from its arguments, the rotation edits it, and the draw stub
// records everything it receives, so the contract fails if any argument or the data flow differs.
#include <cstdio>
#include <cstring>
#include <string>
static std::string calls;
#include "msg_draw.cpp"
extern "C" void matMakeTransYaw__FP8Matrix34G6Point3s(Matrix34* m, Point3 p, short a) {
    for (int i = 0; i < 16; ++i) m->m[i] = p.x + 2 * p.y + 3 * p.z + a + i;
    calls += "make(" + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "," + std::to_string(a) + ");";
}
extern "C" void matRot__FP8Matrix34is(Matrix34* m, int axis, short a) {
    m->m[0] += 1000 * a + axis;
    calls += "rot(" + std::to_string(axis) + "," + std::to_string(a) + ");";
}
extern "C" void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* tex, int mode, Point3 p, Matrix34* m, void* anim, unsigned long flags, void* rec, int extra) {
    calls += "draw(" + std::to_string(reinterpret_cast<unsigned long>(model)) + "," + std::to_string(reinterpret_cast<unsigned long>(tex)) + "," + std::to_string(mode) + ","
        + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "," + std::to_string(m->m[0]) + "," + std::to_string(m->m[15]) + ","
        + std::to_string(reinterpret_cast<unsigned long>(anim)) + "," + std::to_string(flags) + "," + std::to_string(reinterpret_cast<unsigned long>(rec)) + "," + std::to_string(extra) + ");";
}
int main() {
    alignas(16) static unsigned char o[256];
    alignas(16) static unsigned char t[64];
    int bad = 0;
    for (unsigned short yaw : {0, 0x8000, 0x1234, 0xffff}) for (short rot : {0, 100, -7}) {
        std::memset(o, 0, sizeof o); calls.clear();
        void* pm = reinterpret_cast<void*>(0x1111); void* pt = reinterpret_cast<void*>(0x2222);
        std::memcpy(t + 12, &pm, sizeof pm); std::memcpy(t + 20, &pt, sizeof pt);
        unsigned char* tp = t; std::memcpy(o + 36, &tp, sizeof tp);
        Point3 p = {1.5f, -2.0f, 4.25f}; std::memcpy(o + 16, &p, 12);
        std::memcpy(o + 56, &yaw, 2); std::memcpy(o + 88, &rot, 2);
        msg_draw__8RollDoor(o);
        const short a = static_cast<short>(yaw - 0x8000);
        const float m0 = (p.x + 2 * p.y + 3 * p.z + a) + (1000 * rot), m15 = p.x + 2 * p.y + 3 * p.z + a + 15;
        std::string want = "make(1.500000,-2.000000,4.250000," + std::to_string(a) + ");rot(0," + std::to_string(rot) + ");draw(4369,8738,3,1.500000,-2.000000,4.250000,"
            + std::to_string(m0) + "," + std::to_string(m15) + ",0," + std::to_string(~0ul) + ",0,0);";
        if (calls != want) { std::printf("got  %s\nwant %s\n", calls.c_str(), want.c_str()); bad = 1; }
    }
    return bad;
}
