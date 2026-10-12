#include "particle_def_default_ctor.cpp"

#include <cstdio>

static int calls;
static void* seen_object;
static int seen_ints[8];
static float seen_floats[3];
static int seen_tail[3];
static void (*seen_callback)(void*) = reinterpret_cast<void (*)(void*)>(1);
static int seen_word1;
static int seen_word2;

extern "C" void setParticleDef__FP11ParticleDefiiiiiiiifffiiiPFPv_v(void* object, int a, int b, int c, int d, int e,
                                                                    int f, int g, int h, float x, float y, float z,
                                                                    int i, int j, int k, void (*callback)(void*)) {
    ++calls;
    seen_object = object;
    const int ints[8] = {a, b, c, d, e, f, g, h};
    for (int n = 0; n < 8; ++n) seen_ints[n] = ints[n];
    seen_floats[0] = x;
    seen_floats[1] = y;
    seen_floats[2] = z;
    seen_tail[0] = i;
    seen_tail[1] = j;
    seen_tail[2] = k;
    seen_callback = callback;
    const int* words = static_cast<const int*>(object);
    seen_word1 = words[1];
    seen_word2 = words[2];
}

int main() {
    int words[20];
    for (int n = 0; n < 20; ++n) words[n] = 0x7777 + n;
    __11ParticleDef(words);
    if (calls != 1 || seen_object != words) return 1;
    if (seen_word1 != -1 || seen_word2 != 0) return 2;
    for (int n = 0; n < 8; ++n) if (seen_ints[n] != 0) return 3;
    if (seen_floats[0] != 1.0f || seen_floats[1] != 1.0f || seen_floats[2] != 0.0f) return 4;
    if (seen_tail[0] != 3 || seen_tail[1] != 1 || seen_tail[2] != 1 || seen_callback != nullptr) return 5;
    // Only the two stores are made here; every other word is the callee's business.
    for (int n = 0; n < 20; ++n) {
        if (n == 1) { if (words[n] != -1) return 6; continue; }
        if (n == 2) { if (words[n] != 0) return 7; continue; }
        if (words[n] != 0x7777 + n) return 8;
    }
    std::puts("particle def default constructor contract passed");
    return 0;
}
