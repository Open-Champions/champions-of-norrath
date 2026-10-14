// Host contract for msg_draw__14FireStormFlare; not an EE input. Stubs record the calls in order. The authored function was also
// compared with the original in a private R5900 run (12 scenarios with random object bytes, the word at +88 zero and nonzero;
// every original word covered).
#include "msg_draw.cpp"
#include <cstring>
#include <string>
static std::string trace;
static short ambient_seen[4];
static float scale_seen, position_seen[3];
static void* matrix_seen;
extern "C" {
void lightSetAmbientOffset__FPs(short* offsets) { std::memcpy(ambient_seen, offsets, 8); trace += "amb;"; }
void lightSetAmbientOnly__Fv() { trace += "only;"; }
void matMakeTransYawScale__FP8Matrix34G6Point3sf(void* matrix, Point3 position, short yaw, float scale) {
    matrix_seen = matrix; position_seen[0] = position.x; position_seen[1] = position.y; position_seen[2] = position.z; scale_seen = scale;
    trace += "mat" + std::to_string(yaw) + ";";
}
void* lumpFindResource__FPcT0(const char* lump, const char* resource) {
    trace += std::string("lump:") + lump + ":" + resource + ";";
    return std::strcmp(resource, "boulder.vif") == 0 ? reinterpret_cast<void*>(0x10) : reinterpret_cast<void*>(0x20);
}
void modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, Point3 position, void* matrix, void* animation, unsigned long long flags, void* record, int extra) {
    trace += "draw" + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(model)) + "," + std::to_string(reinterpret_cast<__UINTPTR_TYPE__>(texture)) + "," +
             std::to_string(mode) + (matrix == matrix_seen ? ",m" : ",?") + (animation == nullptr ? ",n" : ",?") + (flags == ~0ull ? ",f" : ",?") +
             (record == nullptr ? ",r" : ",?") + std::to_string(extra) + (position.y == position_seen[1] ? ";" : "!;");
}
void lightResetOptionalParams__Fv() { trace += "reset;"; }
}
int main() {
    alignas(16) unsigned char object[128] = {};
    float x = 1.5f, y = -2.0f, z = 7.0f, size = 4.0f;
    std::memcpy(object + 16, &x, 4); std::memcpy(object + 20, &y, 4); std::memcpy(object + 24, &z, 4); std::memcpy(object + 80, &size, 4);
    unsigned flag = 1;
    std::memcpy(object + 88, &flag, 4);
    msg_draw__14FireStormFlare(object);
    if (!trace.empty()) return 1;
    flag = 0;
    std::memcpy(object + 88, &flag, 4);
    msg_draw__14FireStormFlare(object);
    if (trace != "amb;only;mat0;lump:fx.lmp:boulder.vif;lump:fx.lmp:boulder.tex;draw16,32,3,m,n,f,r0;reset;") return 2;
    if (ambient_seen[0] != 200 || ambient_seen[1] != 0 || ambient_seen[2] != -150 || ambient_seen[3] != 50) return 3;
    if (position_seen[0] != 1.5f || position_seen[1] != -2.0f || position_seen[2] != 7.0f) return 4;
    return scale_seen == 4.0f * __builtin_bit_cast(float, 0x3f599999u) ? 0 : 5;
}
