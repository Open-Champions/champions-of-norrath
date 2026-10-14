// Host contract for P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v.
// Not an EE plan input.
#include <cstdio>
#include <cstring>

extern "C" {
unsigned char descList[2048] = {0};
void P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(unsigned char* definition, int a, int b, int c, int d, int e, int f, int g, int h,
                                                               float x, float y, float z, int i, int j, int k, void (*callback)(void*));
}

static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) {
        std::fprintf(stderr, "fail: %s\n", message);
        ++failures;
    }
}

// setParticleDef stand-in: records its arguments and applies a configured edit.
static int calls = 0;
static unsigned char* seen_def = nullptr;
static int seen_ints[11];
static float seen_floats[3];
static void (*seen_callback)(void*) = nullptr;
static int edit_offset = -1;
static unsigned char edit_value = 0;
extern "C" void setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v(unsigned char* definition, int a, int b, int c, int d, int e, int f, int g, int h,
                                                                    float x, float y, float z, int i, int j, int k, void (*callback)(void*)) {
    ++calls;
    seen_def = definition;
    const int ints[11] = {a, b, c, d, e, f, g, h, i, j, k};
    std::memcpy(seen_ints, ints, sizeof ints);
    seen_floats[0] = x; seen_floats[1] = y; seen_floats[2] = z;
    seen_callback = callback;
    if (edit_offset >= 0) definition[edit_offset] = edit_value;
}
extern "C" int norrath_memory_compare(const void* left, const void* right, unsigned long count) {
    const unsigned char* a = static_cast<const unsigned char*>(left);
    const unsigned char* b = static_cast<const unsigned char*>(right);
    for (unsigned long n = 0; n < count; ++n) if (a[n] != b[n]) return a[n] - b[n];
    return 0;
}
static void callback(void*) {}
static unsigned word(const unsigned char* p) { unsigned v; std::memcpy(&v, p, 4); return v; }

static void call(unsigned char* def) {
    P_UpdateParticleType__FP11ParticleDefiiiiiiiifffiiiPFPv_v(def, 1, 2, 3, 4, 5, 6, 7, 8, 1.5f, 2.5f, 3.5f, 9, 10, 11, callback);
}

int main() {
    alignas(8) unsigned char storage[96];
    unsigned char* def = storage + 4;   // only word aligned
    std::memset(storage, 0, sizeof storage);
    for (int n = 0; n < 80; ++n) def[n] = static_cast<unsigned char>(n + 1);
    const unsigned slot = 3;
    std::memcpy(def + 4, &slot, 4);

    // Unchanged by setParticleDef: no copy into descList.
    edit_offset = -1;
    call(def);
    expect(calls == 1 && seen_def == def, "setParticleDef called once with the definition");
    for (int n = 0; n < 8; ++n) expect(seen_ints[n] == n + 1, "first eight integers forwarded");
    expect(seen_ints[8] == 9 && seen_ints[9] == 10 && seen_ints[10] == 11, "later integers forwarded");
    expect(seen_floats[0] == 1.5f && seen_floats[1] == 2.5f && seen_floats[2] == 3.5f, "floats forwarded");
    expect(seen_callback == callback, "callback forwarded");
    expect(word(descList + 3 * 64) == 0u, "no copy when the definition is unchanged");

    // A change in the copied area copies bytes 16..79 to the slot record.
    edit_offset = 70;
    edit_value = 0xee;
    call(def);
    expect(std::memcmp(descList + 3 * 64, def + 16, 64) == 0, "bytes 16 to 79 land in the slot record");
    expect(descList[3 * 64 + 54] == 0xee, "the edit is part of the copy");
    expect(descList[2 * 64 + 63] == 0 && descList[4 * 64] == 0, "neighbouring records are untouched");

    // A change at offset 0 (before the copied range) still counts as a change.
    std::memset(descList, 0, sizeof descList);
    edit_offset = 0;
    edit_value = 0x99;
    call(def);
    expect(std::memcmp(descList + 3 * 64, def + 16, 64) == 0, "a change in the first 16 bytes also triggers the copy");

    // A slot of -1 stops after setParticleDef, even when bytes changed.
    std::memset(descList, 0, sizeof descList);
    const unsigned none = 0xffffffffu;
    std::memcpy(def + 4, &none, 4);
    edit_offset = 20;
    edit_value = 0x42;
    call(def);
    expect(word(descList) == 0u && word(descList + 3 * 64) == 0u, "slot -1 copies nothing");
    if (failures != 0) return 1;
    std::printf("update-particle-type contract passed\n");
    return 0;
}
