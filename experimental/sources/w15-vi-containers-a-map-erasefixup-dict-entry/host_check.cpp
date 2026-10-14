// Host check only; not a build input. Words hold arena offsets (see NORRATH_HOST_ARENA). The rotations are stubs that only log, so
// after a rotation the tree links are unchanged and the code re-reads them as they were.
#define NORRATH_HOST_ARENA 1
#include "erasefixup.cpp"
#include <cstdio>
#include <cstring>
#include <string>
alignas(16) static unsigned char arena[0x2000];
unsigned char* norrath_host_pointer(unsigned address) { return arena + address; }
static std::string trace;
extern "C" int LeftRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void*, int i) { trace += "L" + std::to_string(i) + ";"; return 0; }
extern "C" int RightRotate__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(void*, int i) { trace += "R" + std::to_string(i) + ";"; return 0; }
static unsigned* node(int i) { return reinterpret_cast<unsigned*>(arena + 0x400 + 28 * i); }
static unsigned* map() { return reinterpret_cast<unsigned*>(arena + 0x100); }
// Nodes: 1 = parent P (root), 2 = X, 3 = sibling W, 4 / 5 = W's children (4 is the left one), 6 = grandparent holder unused.
static void build(bool xLeft, unsigned cP, unsigned cW, unsigned cL, unsigned cR) {
    std::memset(arena, 0x5a, sizeof arena);
    trace.clear();
    for (int i = 0; i < 8; ++i) { node(i)[0] = 0; node(i)[1] = 0; node(i)[2] = 0; node(i)[3] = 0; }
    map()[2] = 0x400; map()[3] = 1;
    node(1)[0] = cP;
    node(2)[0] = 0; node(2)[1] = 1;
    node(3)[0] = cW; node(3)[1] = 1; node(3)[2] = 4; node(3)[3] = 5;
    node(4)[0] = cL; node(4)[1] = 3;
    node(5)[0] = cR; node(5)[1] = 3;
    node(1)[xLeft ? 2 : 3] = 2; node(1)[xLeft ? 3 : 2] = 3;
}
static bool run(bool xLeft, unsigned cP, unsigned cW, unsigned cL, unsigned cR, const char* wantTrace, unsigned wP, unsigned wW, unsigned wL, unsigned wR) {
    build(xLeft, cP, cW, cL, cR);
    int r = EraseFixup__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map(), 2);
    bool ok = r == 0 && trace == wantTrace && node(2)[0] == 0 && node(1)[0] == wP && node(3)[0] == wW && node(4)[0] == wL && node(5)[0] == wR;
    if (!ok) std::printf("case xLeft=%d cP=%u cW=%u cL=%u cR=%u: trace '%s'\n", xLeft, cP, cW, cL, cR, trace.c_str());
    return ok;
}
int main() {
    bool ok = true;
    // X is the root: only colour(X) = 0 is stored.
    build(true, 1, 0, 0, 0); map()[3] = 2;
    ok &= EraseFixup__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map(), 2) == 0 && trace.empty() && node(2)[0] == 0 && node(1)[0] == 1;
    // X is red: loop is skipped, colour(X) = 0.
    build(true, 1, 0, 0, 0); node(2)[0] = 1;
    ok &= EraseFixup__t5VIMap2ZUiZQ212VIDictionary11VIDictEntryi(map(), 2) == 0 && trace.empty() && node(2)[0] == 0;
    for (int side = 0; side < 2; ++side) {
        const bool left = side == 0;
        const char* n = left ? "L" : "R";           // rotation about the parent (near)
        const char* f = left ? "R" : "L";           // rotation about the sibling (far)
        // W black, both children black: W red, X = parent (the root), parent black.
        ok &= run(left, 1, 0, 0, 0, "", 0, 1, 0, 0);
        // W red with black children: rotate about the parent (stub), then both black.
        ok &= run(left, 0, 1, 0, 0, (std::string(n) + "1;").c_str(), 0, 1, 0, 0);
        // Near child red and far child black: far rotation about W, then the final steps copy colour(P) into W.
        ok &= run(left, 1, 0, left ? 1 : 0, left ? 0 : 1, (std::string(f) + "3;" + n + "1;").c_str(), 0, 1, 0, 0);
        // Far child red (near child black): final steps only.
        ok &= run(left, 1, 0, left ? 0 : 1, left ? 1 : 0, (std::string(n) + "1;").c_str(), 0, 1, 0, 0);
        // Both children red: final steps only; the near child keeps its colour.
        ok &= run(left, 0, 0, 1, 1, (std::string(n) + "1;").c_str(), 0, 0, left ? 1 : 0, left ? 0 : 1);
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
