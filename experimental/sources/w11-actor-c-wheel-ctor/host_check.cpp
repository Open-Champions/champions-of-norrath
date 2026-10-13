// Host contract for __5WheelPcifP7Vehiclei; not an EE input. Pointer words are offsets from a host base and
// every callee is a recording stub (strcpy and strstr are the host's own).
#define W11_ACTOR_C_HOST 1
#include "wheel_ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w11_actor_c_host_base;
W11ActorCHostSlot w11_actor_c_host_slots[1];
unsigned char vt_Wheel[8];
static std::string calls;
static int box_values[6];
static long long chunks_value;
extern "C" void* gameobject_construct(void* self, Point3 p, void* map, char** args) {
    calls += "ctor(" + std::to_string(p.x) + "," + std::to_string(p.y) + "," + std::to_string(p.z) + "," + (map ? "map" : "null") + "," + (args ? "args" : "null") + ");";
    std::memset(static_cast<unsigned char*>(self) + 12, 0x01, 4);   // proves the or happens after the base constructor
    return nullptr;
}
extern "C" void* lumpFindResource__FPcT0(void* lump, const char* name) {
    calls += std::string("lump(") + std::to_string(static_cast<unsigned char*>(lump) - w11_actor_c_host_base) + "," + name + ");";
    return w11_actor_c_host_base + 3000 + calls.size();
}
extern "C" long long modelGetActiveChunks__FP12_modelHeaderi(void* model, int arg) {
    calls += "chunks(" + std::to_string(static_cast<unsigned char*>(model) - w11_actor_c_host_base) + "," + std::to_string(arg) + ");";
    return chunks_value;
}
extern "C" void objectAddToRunList__FP10GameObject(void* o) { calls += "run(" + std::to_string(static_cast<unsigned char*>(o) - w11_actor_c_host_base) + ");"; }
extern "C" void modelGetBoundingBox__FP12_modelHeaderlPiN52(void* model, long long chunks, int* a, int* b, int* c, int* d, int* e, int* f) {
    calls += "box(" + std::to_string(chunks) + ");";
    int* p[6] = {a, b, c, d, e, f};
    for (int i = 0; i < 6; ++i) *p[i] = box_values[i];
}
int main() {
    static unsigned char base[8192];
    w11_actor_c_host_base = base;
    int failures = 0;
    const char* names[] = {"wheel.vif", "a/b.vif.x", "x.vifz", ".vif"};
    const int boxes[][6] = {{0, 0, 10, 0, 0, 40}, {1, 2, 3, 4, 5, 8}, {0, 0, 7, 0, 0, 2}, {0, 0, -5, 0, 0, 6}};
    for (const char* name : names) for (const auto& bx : boxes) for (float f : {0.0f, 1.5f}) {
        std::memset(base, 0xa5, sizeof base);
        unsigned char* wheel = base + 100;
        unsigned char* vehicle = base + 2048;
        const float pos[3] = {1.0f, 2.0f, 3.0f};
        std::memcpy(vehicle + 16, pos, 12);
        std::memcpy(box_values, bx, sizeof box_values);
        chunks_value = 0x1234567890LL;
        calls.clear();
        wheel_construct(wheel, name, 77, f, vehicle, -9);
        // the five bytes copied over ".vif" include a terminator, which truncates whatever followed it
        const std::string tex = std::string(name).substr(0, std::string(name).find(".vif")) + ".tex";
        const int vehicleOffset = 2048 + 76;
        if (calls.find("ctor(") != 0 || calls.find("lump(") == std::string::npos) { std::fprintf(stderr, "calls: %s\n", calls.c_str()); ++failures; }
        // names and the order of the calls
        const size_t l1 = calls.find("lump("), l2 = calls.find("lump(", l1 + 1), c1 = calls.find("chunks("), r1 = calls.find("run("), b1 = calls.find("box(");
        if (!(l1 < l2 && l2 < c1 && c1 < r1 && r1 < b1)) { std::fprintf(stderr, "call order: %s\n", calls.c_str()); ++failures; }
        if (calls.find(std::string(",") + name + ");") == std::string::npos || calls.find("," + tex + ");") == std::string::npos) { std::fprintf(stderr, "names: %s want %s\n", calls.c_str(), tex.c_str()); ++failures; }
        auto W = [&](int off) { unsigned v; std::memcpy(&v, wheel + off, 4); return v; };
        auto Fl = [&](int off) { float v; std::memcpy(&v, wheel + off, 4); return v; };
        const float radius = static_cast<float>((bx[5] - bx[2]) / 2) * 0.025641024112701416f;
        const float circumference = radius * 3.141592502593994f * 2.0f;
        long long chunks; std::memcpy(&chunks, wheel + 352, 8);
        if (W(12) != (0x01010101u | 0x4001u) || wheel[89] != 8 || wheel[40] != 24 || Fl(28) != 24.0f || W(396) != 2048u || W(72) != static_cast<unsigned>(vt_Wheel - base) ||
            Fl(372) != f || W(384) != static_cast<unsigned>(-9) || W(388) != 77u || Fl(376) != radius || Fl(380) != circumference || chunks != chunks_value) {
            std::fprintf(stderr, "fields differ for %s\n", name); ++failures;
        }
        unsigned lump1 = W(76), lump2 = W(80);
        if (lump1 == 0 || lump2 == 0 || lump1 == lump2) { std::fprintf(stderr, "resource words\n"); ++failures; }
    }
    if (!failures) std::puts("Wheel ctor ok");
    return failures != 0;
}
