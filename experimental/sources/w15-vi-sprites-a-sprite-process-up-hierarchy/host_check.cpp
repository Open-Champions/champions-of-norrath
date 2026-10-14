// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 14];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "process_up_hierarchy.cpp"
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
static std::vector<std::string> calls;
static float seenVec[8][3];
extern "C" void Transform__10VIMatrix44RC6VIQuatRC7VIVect3T2(void* m, const void* r, const void* v, const void* t) {
    char b[160];
    std::snprintf(b, sizeof b, "T %ld %ld %ld", (long)(static_cast<unsigned char*>(m) - arena), (long)(static_cast<const unsigned char*>(r) - arena), (long)(static_cast<const unsigned char*>(t) - arena));
    // local matrices are not in the arena: report them as -1
    if (static_cast<unsigned char*>(m) < arena || static_cast<unsigned char*>(m) >= arena + sizeof arena) std::snprintf(b, sizeof b, "T local %ld %ld", (long)(static_cast<const unsigned char*>(r) - arena), (long)(static_cast<const unsigned char*>(t) - arena));
    calls.push_back(b);
    const float* f = static_cast<const float*>(v);
    seenVec[calls.size() - 1][0] = f[0]; seenVec[calls.size() - 1][1] = f[1]; seenVec[calls.size() - 1][2] = f[2];
}
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(void* o, const void* a, const void* b) {
    char t[160];
    std::snprintf(t, sizeof t, "M %ld local=%d %ld", (long)(static_cast<unsigned char*>(o) - arena), (int)(static_cast<const unsigned char*>(a) < arena || static_cast<const unsigned char*>(a) >= arena + sizeof arena), (long)(static_cast<const unsigned char*>(b) - arena));
    calls.push_back(t);
}
static int dtorFlag = -1, dtorCalls;
extern "C" void _$_10VIMatrix44(void*, int f) { dtorFlag = f; ++dtorCalls; calls.push_back("D"); }
int main() {
    alignas(8) unsigned char sprite[128] = {};
    *reinterpret_cast<unsigned*>(sprite + 72) = arena_base;
    for (int n = 0; n < 3; ++n) {
        unsigned char* e = arena + n * 256;
        *reinterpret_cast<int*>(e) = n - 1;
        for (int k = 0; k < 3; ++k) *reinterpret_cast<float*>(e + 112 + 4 * k) = (n + 1) * 1.0f + k;
        *reinterpret_cast<float*>(e + 32) = 2.0f;
    }
    if (ProcessUpHierarchy__9VIHSpritei(sprite, 2) != 0) return 1;
    // expected: node 0 root, node 1 child, node 2 grandchild, each with its own destructor call
    const char* want[] = {"T 48 16 36", "D", "T local 272 292", "M 304 local=1 48", "D", "T local 528 548", "M 560 local=1 304", "D"};
    if (calls.size() != 8) return 2;
    for (int i = 0; i < 8; ++i) if (calls[i] != want[i]) { std::puts(calls[i].c_str()); return 3 + i; }
    if (seenVec[0][0] != 2.0f || seenVec[0][1] != 4.0f || seenVec[0][2] != 6.0f) return 12;
    if (seenVec[2][0] != 4.0f || seenVec[2][1] != 6.0f || seenVec[2][2] != 8.0f) return 13;
    if (seenVec[5][0] != 6.0f || seenVec[5][1] != 8.0f || seenVec[5][2] != 10.0f) return 15;
    if (dtorFlag != 2 || dtorCalls != 3) return 14;
    return 0;
}
