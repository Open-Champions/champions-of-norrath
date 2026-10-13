// Host contract for Innoruuk updateEnemy; not an EE input. Words are offsets from a host base; the
// table's function word 1 selects the accept stub.
#define W10_ACTOR_H_HOST 1
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>
unsigned char* w10h_host_base;
static int acceptStub(void*, void*);
void* w10h_host_fn(unsigned w) { return w == 1 ? reinterpret_cast<void*>(&acceptStub) : nullptr; }
#include "update_enemy.cpp"

static unsigned char arena[0x8000] __attribute__((aligned(16)));
static std::map<void*, int> verdict; static std::string calls; static std::vector<void*> found;
static float boxLow[3], boxHigh[3]; static int boxCapacity; static void* destroyed; static void* linked[3];
static void* adjustedSeen;
static int acceptStub(void* s, void* c) { calls += "a"; adjustedSeen = s; auto it = verdict.find(c); return it == verdict.end() ? 0 : it->second; }
extern "C" {
void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* lo, const void* hi, void** out, int cap) {
    calls += "B"; std::memcpy(boxLow, lo, 12); std::memcpy(boxHigh, hi, 12); boxCapacity = cap;
    size_t i = 0; for (; i < found.size(); ++i) out[i] = found[i]; out[i] = nullptr; }
void listDestroy__FP8ListHead(void* h) { calls += "D"; destroyed = h; }
void* listOnewayLink__FP8ListHeadT0Pv(void* a, void* b, void* c) { calls += "L"; linked[0] = a; linked[1] = b; linked[2] = c; return nullptr; }
}
static unsigned char* const O = arena + 0x1000;
static unsigned char* obj(unsigned off) { return arena + off; }
static unsigned word(unsigned char* base) { return static_cast<unsigned>(base - arena); }
static unsigned headWord() { unsigned v; std::memcpy(&v, O + 576, 4); return v; }
static void put(unsigned char* where, unsigned v) { std::memcpy(where, &v, 4); }
static void putf(unsigned char* where, float v) { std::memcpy(where, &v, 4); }
static void reset() {
    std::memset(arena, 0, sizeof arena); verdict.clear(); calls.clear(); found.clear(); destroyed = nullptr; linked[0] = linked[1] = linked[2] = nullptr;
    put(O + 72, 0x100); short adj = 40; std::memcpy(arena + 0x100 + 160, &adj, 2); put(arena + 0x100 + 164, 1);
    putf(O + 788, 10.0f); put(O + 808, 3); O[569] = 0;       // (3 ^ 0) & 31 != 0: no search unless nothing chosen
    putf(O + 776, 0); putf(O + 780, 0); putf(O + 784, 0);
    putf(O + 16, 0); putf(O + 20, 0); putf(O + 24, 0);
}
static void setHead(unsigned headOffset, unsigned nodeOffset, unsigned char* target) { put(O + headOffset, nodeOffset); put(arena + nodeOffset + 12, word(target)); }
int main() {
    int bad = 0;
    w10h_host_base = arena;
    auto expect = [&](bool ok, const char* what) { if (!ok) { std::printf("fail: %s (calls %s)\n", what, calls.c_str()); bad = 1; } };
    unsigned char *X = obj(0x2000), *Y = obj(0x2100), *Z = obj(0x2200);
    { reset(); O[570] = 0xff; setHead(576, 0x400, X); verdict[X] = 1; updateEnemy__8Innoruuk(O); expect(calls.empty(), "byte 570 returns at once"); }
    { reset(); setHead(576, 0x400, X); verdict[X] = 1; updateEnemy__8Innoruuk(O);
      expect(calls == "a" && adjustedSeen == O + 40, "kept target: one accept call on the adjusted object, no search, no list change"); }
    { reset(); setHead(752, 0x500, Y); verdict[Y] = 1; putf(Y + 16, 20.0f);  // 400 > 100 * 0.707
      updateEnemy__8Innoruuk(O); expect(calls == "aL" && linked[0] == O + 576 && linked[1] == Y && linked[2] == Y, "preferred far target is linked"); }
    { reset(); setHead(752, 0x500, Y); verdict[Y] = 1; putf(Y + 16, 5.0f);  // 25 < 70.7: not chosen, then the search runs
      O[569] = 3; put(O + 808, 3);  // (3 ^ 3) & 31 == 0 -- but s is null anyway, so the search runs
      updateEnemy__8Innoruuk(O); expect(calls == "aBD" && headWord() == 0, "near preferred target is not chosen; empty search ends and the empty head is destroyed"); }
    { reset(); put(O + 808, 0x20); O[569] = 0; setHead(576, 0x400, X); verdict[X] = 0; found = {Y, Z}; verdict[Y] = 1; verdict[Z] = 1;
      putf(Y + 16, 1.0f); putf(Z + 16, 20.0f);
      updateEnemy__8Innoruuk(O);
      expect(boxCapacity == 128 && boxLow[0] < -7.0f && boxLow[0] > -7.1f && boxHigh[2] > 7.0f && boxHigh[2] < 7.1f, "box is the position plus or minus 10 * 0.707");
      expect(calls == "aBaaDL" && destroyed == O + 576 && linked[1] == Y && headWord() == 0, "nearer candidate wins (score 2 vs 1); old head destroyed; new one linked"); }
    { reset(); put(O + 808, 0x20); setHead(576, 0x400, X); verdict[X] = 1; found = {X, Y}; verdict[Y] = 1; putf(X + 16, 30.0f); putf(Y + 16, 30.0f);
      // X is accepted at the first step so s = X is non-null; (0x20 ^ 0) & 31 == 0 still triggers the search; X scores 1 + 1 (it is the head's target) and wins the tie-break on score
      updateEnemy__8Innoruuk(O); expect(linked[0] == nullptr && destroyed == nullptr, "winning candidate equal to the head target keeps the list"); }
    { reset(); put(O + 808, 0x20); setHead(576, 0x400, X); verdict[X] = 0; found = {}; updateEnemy__8Innoruuk(O);
      expect(calls == "aBD" && headWord() == 0 && linked[0] == nullptr, "no target: head destroyed and cleared, nothing linked"); }
    if (!bad) std::puts("innoruuk updateEnemy ok");
    return bad;
}
