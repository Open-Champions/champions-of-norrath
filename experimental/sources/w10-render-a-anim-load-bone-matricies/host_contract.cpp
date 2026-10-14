// Host contract for animLoadBoneMatricies__FP6PacketP14AnimationState. Not an EE plan input.
#include <cstdio>
#include <cstring>
static unsigned char arena[1 << 16];
extern "C" {
unsigned char emergencyResult[1600];
unsigned char identityResult[1600];
int drawFrameNm;
unsigned char* arList;
unsigned char* arListBack;
static int evalCalls;
static unsigned char* evalProduces;
unsigned char* norrath_host_pointer(unsigned a) {
    if (a == 0x600000u) return emergencyResult;
    if (a == 0x610000u) return identityResult;
    return arena + (a - 0x100000u);
}
unsigned norrath_host_address(const unsigned char* p) {
    if (p == emergencyResult) return 0x600000u;
    if (p == identityResult) return 0x610000u;
    return 0x100000u + static_cast<unsigned>(p - arena);
}
void evaluateAnimation__FP14AnimationStateb(unsigned char* state, bool flag) {
    evalCalls += flag ? 1 : 100;
    if (evalProduces) { unsigned a = norrath_host_address(evalProduces); std::memcpy(state, &a, 4); }
}
}
#include "anim_load_bone_matricies.cpp"
#include "../level-anim-load-common/host_check.hpp"
static unsigned char* at(unsigned o) { return arena + o; }
static unsigned ad(const unsigned char* p) { return norrath_host_address(p); }
int main() {
    unsigned char* packet = at(0x10);
    unsigned char* state = at(0x40);
    unsigned char* buf = at(0x1000);
    unsigned char* rec[3] = {at(0x4000), at(0x5000), at(0x6000)};
    for (unsigned i = 0; i != sizeof arena; ++i) arena[i] = 0xee;
    for (int i = 0; i != 3; ++i) { sh_(rec[i] + 8, 5 + i); sw_(rec[i] + 1556, 0); sw_(rec[i] + 1560, 0); }
    // Three-entry list rec0 <-> rec1 <-> rec2; arListBack is the front, arList the tail.
    sw_(rec[0] + 1560, ad(rec[1])); sw_(rec[1] + 1556, ad(rec[0])); sw_(rec[1] + 1560, ad(rec[2])); sw_(rec[2] + 1556, ad(rec[1]));
    arList = rec[2]; arListBack = rec[0]; drawFrameNm = 77;
    sw_(packet, ad(buf)); sw_(packet + 4, 0);
    sw_(state, ad(rec[1]));
    animLoadBoneMatricies__FP6PacketP14AnimationState(packet, state);
    if (evalCalls) return 1;
    // rec1 moves to the tail: rec0 <-> rec2 <-> rec1.
    if (hw(rec[1] + 1564) != 77 || arList != rec[1] || arListBack != rec[0]) return 2;
    if (hw(rec[0] + 1560) != ad(rec[2]) || hw(rec[2] + 1556) != ad(rec[0])) return 4;
    if (hw(rec[2] + 1560) != ad(rec[1]) || hw(rec[1] + 1556) != ad(rec[2]) || hw(rec[1] + 1560) != 0) return 5;
    if (hw(packet) != ad(buf) + 32 || hw(packet + 4) != ad(buf) + 16) return 6;
    unsigned q = (6 * 24 + 31) >> 4;
    if (hw(buf) != (q | 0x30000000u) || hw(buf + 4) != ad(rec[1]) || hw(buf + 8) != 0 || hw(buf + 12) != 0) return 7;
    if (hw(buf + 16) != 0x10000000u || hw(buf + 20) != 0 || hw(buf + 24) != 0x1400003au || hw(buf + 28) != 0) return 8;
    if (buf[32] != 0xee || buf[-1] != 0xee) return 9;
    // A record already at the tail is not relinked.
    sw_(state, ad(rec[1])); drawFrameNm = 78;
    animLoadBoneMatricies__FP6PacketP14AnimationState(packet, state);
    if (hw(rec[1] + 1564) != 78 || arList != rec[1] || hw(rec[1] + 1556) != ad(rec[2])) return 10;
    // A front record with no previous: next becomes arListBack.
    sw_(state, ad(rec[0]));
    animLoadBoneMatricies__FP6PacketP14AnimationState(packet, state);
    if (arListBack != rec[2] || hw(rec[2] + 1556) != 0 || arList != rec[0] || hw(rec[0] + 1556) != ad(rec[1])) return 11;
    if (hw(rec[1] + 1560) != ad(rec[0]) || hw(rec[0] + 1560) != 0) return 12;
    // The emergency record is dropped, evaluateAnimation runs and its result is used.
    sw_(emergencyResult + 1552, 5); sh_(emergencyResult + 8, 3);
    sw_(state, ad(emergencyResult)); evalProduces = rec[2];
    animLoadBoneMatricies__FP6PacketP14AnimationState(packet, state);
    if (evalCalls != 1 || hw(emergencyResult + 1552) != 0 || hw(state) != ad(rec[2])) return 13;
    // No record at all: identityResult supplies the count and address.
    sw_(state, 0); evalProduces = nullptr; sh_(identityResult + 8, 2); evalCalls = 0;
    unsigned char* before = at(0x1000 + 32 * 4);
    animLoadBoneMatricies__FP6PacketP14AnimationState(packet, state);
    if (evalCalls != 1 || hw(before + 4) != ad(identityResult) || hw(before) != (((2 * 24 + 31) >> 4) | 0x30000000u)) return 14;
    std::puts("animLoadBoneMatricies contract passed");
    return 0;
}
