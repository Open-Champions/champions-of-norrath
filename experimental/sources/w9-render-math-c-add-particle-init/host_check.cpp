// Host contract for particleInit__11AddParticlePPc. Not an EE plan input. sprintf and strcat are
// the host's; findTagInt and P_UpdateParticleType are recorded stubs.
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
static std::map<std::string, int> tags;
static std::string keys;
static void* upd_def = nullptr;
static int upd_ints[11];
static float upd_floats[3];
static int upd_calls = 0;
extern "C" int findTagInt__10GameObjectPci(void*, char* tag, int occurrence) {
    keys += tag; keys += ";";
    if (occurrence != 0) return -99;
    auto it = tags.find(tag);
    return it == tags.end() ? 0 : it->second;
}
extern "C" void P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(void* d, int a, int b, int c, int e, int f, int g, int h, int i, float x, float y, float z, int j, int k, int l, void (*cb)(void*)) {
    ++upd_calls; upd_def = d;
    const int v[11] = {a, b, c, e, f, g, h, i, j, k, l};
    std::memcpy(upd_ints, v, sizeof v);
    upd_floats[0] = x; upd_floats[1] = y; upd_floats[2] = z;
    if (cb != nullptr) upd_calls += 100;
}
extern "C" void particleInit__11AddParticlePPc(unsigned char*, char**);
int main() {
    alignas(8) unsigned char obj[400];
    std::memset(obj, 0xaa, sizeof obj);
    int slot = -1;
    std::memcpy(obj + 76, &slot, 4);
    // Emitter tag absent: only that lookup happens.
    particleInit__11AddParticlePPc(obj, nullptr);
    if (keys != "ParticleEmitter;" || upd_calls != 0 || obj[80] != 0xaa) return 1;
    // Present, with a slot suffix.
    slot = 3; std::memcpy(obj + 76, &slot, 4);
    keys.clear();
    tags["ParticleEmitter3"] = 1;
    tags["psr3"] = 10; tags["psg3"] = 11; tags["pse3"] = 12; tags["psa3"] = 13; tags["per3"] = 14; tags["peg3"] = 15; tags["peb3"] = 16; tags["pea3"] = 17;
    tags["pss3"] = 300; tags["pes3"] = 70000 + 5; tags["pgr3"] = -2;
    tags["plf3"] = 20; tags["prm3"] = 21; tags["ptx3"] = 22; tags["bcn3"] = 23;
    tags["bxo3"] = -5; tags["bot3"] = 123456; tags["bft3"] = -9; tags["bwa3"] = 7; tags["bfa3"] = 0;
    obj[152] = 0x10;
    particleInit__11AddParticlePPc(obj, nullptr);
    if (obj[80] != 1 || obj[81] != 0 || obj[84] != 0xaa) return 2;
    if (obj[100] != 10 || obj[103] != 13 || obj[107] != 17 || obj[114] != 20 || obj[117] != 23) return 3;
    short h; std::memcpy(&h, obj + 108, 2); if (h != 300) return 4;
    std::memcpy(&h, obj + 110, 2); if (h != short(70005)) return 5;
    std::memcpy(&h, obj + 118, 2); if (h != -5) return 6;
    int w; std::memcpy(&w, obj + 144, 4); if (w != 123456) return 7;
    std::memcpy(&w, obj + 148, 4); if (w != -9) return 8;
    if (obj[152] != 0x11) return 9;                     // bwa set bit 0, bfa left bit 1 clear
    if (upd_calls != 1 || upd_def != obj + 172) return 10;
    const int want[8] = {10, 11, 12, 13, 14, 15, 16, 17};
    for (int i = 0; i < 8; ++i) if (upd_ints[i] != want[i]) return 11;
    if (upd_ints[8] != 20 || upd_ints[9] != 21 || upd_ints[10] != 22) return 12;
    if (upd_floats[0] != 300.0f || upd_floats[1] != 4469.0f || upd_floats[2] != -2.0f) return 13;
    if (keys.rfind("ParticleEmitter3;psr3;psg3;", 0) != 0 || keys.find(";bfa3;") == std::string::npos) return 14;
    tags["bfa3"] = 1;
    particleInit__11AddParticlePPc(obj, nullptr);
    if (obj[152] != 0x13) return 15;
    std::printf("add particle init contract passed\n");
    return 0;
}
