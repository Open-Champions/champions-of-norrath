// Host contract for findCommander__3Orc; not an EE input. Includes the source.
#define W11_ACTOR_B_HOST 1
#include "find_commander.cpp"
#include <cstring>
#include <vector>
static unsigned char arena[0x8000];
unsigned char* w11_actor_b_host_base = arena;
unsigned w11_actor_b_host_call(unsigned, void*, int, unsigned long, unsigned long) { return 0; }
unsigned _3Orc_commanderList[20] asm("_3Orc$m_commanderList");
static std::vector<void*> left;
extern "C" void leaveCommander__3Orc(void* self) { left.push_back(self); }
static void setf(unsigned off, float x, float y, float z) {
    float v[3] = {x, y, z};
    std::memcpy(arena + off, v, 12);
}
static unsigned w(unsigned off) { unsigned v; std::memcpy(&v, arena + off, 4); return v; }
static void setw(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
int main() {
    const unsigned self = 0x100;
    // Candidate k: node at 0x1000+0x40*k whose word 12 points at the candidate object at 0x2000+0x80*k.
    auto node = [](int k) { return 0x1000u + 0x40u * k; };
    auto cand = [](int k) { return 0x2000u + 0x80u * k; };
    auto setup = [&](int count) {
        std::memset(arena, 0, sizeof arena);
        std::memset(_3Orc_commanderList, 0, sizeof _3Orc_commanderList);
        for (int k = 0; k < count; ++k) { setw(node(k) + 12, cand(k)); setw(cand(k) + 12, 0); }
    };
    // No entries: null result, no leave call when +1128 is zero.
    setup(0); left.clear(); setf(self + 16, 1, 2, 3);
    if (findCommander__3Orc(arena + self) != nullptr || !left.empty()) return 1;
    // +1128 non-zero: leave is called first.
    setw(self + 1128, 0x500);
    if (findCommander__3Orc(arena + self) != nullptr || left.size() != 1 || left[0] != arena + self) return 2;
    setw(self + 1128, 0);
    // Nearest of several, with gaps (zero entries) and a flagged candidate skipped.
    setup(5); setf(self + 16, 10, 20, 30);
    for (int k : {0, 1, 2, 3, 4}) _3Orc_commanderList[k * 3 + 1] = node(k);  // spread out, gaps between
    setf(cand(0) + 16, 10, 20, 130);   // 10000
    setf(cand(1) + 16, 10, 20, 31);    // 1  (flagged, skipped)
    setw(cand(1) + 12, 8);
    setf(cand(2) + 16, 12, 20, 30);    // 4
    setf(cand(3) + 16, 10, 22, 30);    // 4 (tie: first wins)
    setf(cand(4) + 16, 400, 0, 0);     // beyond 360000? 390^2 + ... = 152100+400+900 -> inside
    if (findCommander__3Orc(arena + self) != arena + cand(2)) return 3;
    // Only the last slot (19) is used.
    setup(1); setf(self + 16, 0, 0, 0); setf(cand(0) + 16, 100, 0, 0);
    _3Orc_commanderList[19] = node(0);
    if (findCommander__3Orc(arena + self) != arena + cand(0)) return 4;
    // Exactly at the limit (600^2 = 360000) is not strictly smaller; just inside is.
    setf(cand(0) + 16, 600, 0, 0);
    if (findCommander__3Orc(arena + self) != nullptr) return 5;
    setf(cand(0) + 16, 599, 0, 0);
    if (findCommander__3Orc(arena + self) != arena + cand(0)) return 6;
    // Flag bits other than 8 do not skip.
    setw(cand(0) + 12, 0xfffffff7u);
    if (findCommander__3Orc(arena + self) != arena + cand(0)) return 7;
    setw(cand(0) + 12, 8);
    if (findCommander__3Orc(arena + self) != nullptr) return 8;
    // Sum order x, y, z with distinct magnitudes picks the smaller squared sum.
    setup(2); setf(self + 16, 0, 0, 0);
    _3Orc_commanderList[0] = node(0); _3Orc_commanderList[1] = node(1);
    setf(cand(0) + 16, 3, 4, 0); setf(cand(1) + 16, 0, 0, 4.9f);
    if (findCommander__3Orc(arena + self) != arena + cand(1)) return 9;
    return 0;
}
