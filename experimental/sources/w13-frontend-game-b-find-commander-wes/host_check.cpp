// Host contract for findCommander__14WoodElfSoldier. Not an EE plan input.
// Build with -ffp-contract=off.
#include <cstdio>
#include <cstring>
struct WoodElfSoldier;
struct CommanderNode { unsigned char pad[12]; WoodElfSoldier* soldier; };
extern "C" {
CommanderNode* WoodElfSoldier_commanderList[10] __asm__("_14WoodElfSoldier$m_commanderList");
WoodElfSoldier* findCommander__14WoodElfSoldier(WoodElfSoldier*);
}
static int left_calls; static WoodElfSoldier* left_arg;
extern "C" void leaveCommander__14WoodElfSoldier(WoodElfSoldier* s) { ++left_calls; left_arg = s; }
struct Obj { alignas(8) unsigned char b[1024]; };
static void setpos(Obj& o, float x, float y, float z) { float v[3] = {x, y, z}; std::memcpy(o.b + 16, v, 12); }
int main() {
    int failures = 0;
    auto expect = [&](bool ok, const char* m) { if (!ok) { std::fprintf(stderr, "fail: %s\n", m); ++failures; } };
    Obj self, c[4]; CommanderNode n[4];
    std::memset(&self, 0, sizeof self); std::memset(c, 0, sizeof c); std::memset(n, 0, sizeof n);
    for (int k = 0; k < 4; ++k) n[k].soldier = reinterpret_cast<WoodElfSoldier*>(&c[k]);
    auto S = reinterpret_cast<WoodElfSoldier*>(&self);
    setpos(self, 10, 20, 30);
    expect(findCommander__14WoodElfSoldier(S) == 0 && left_calls == 0, "empty table gives zero, no leave");
    *reinterpret_cast<int*>(self.b + 960) = 5;
    findCommander__14WoodElfSoldier(S);
    expect(left_calls == 1 && left_arg == S, "leave is called first when +960 is set");
    *reinterpret_cast<int*>(self.b + 960) = 0;
    setpos(c[0], 10, 20, 130); setpos(c[1], 10, 20, 40); setpos(c[2], 10, 20, 31); setpos(c[3], 10, 20, 31);
    WoodElfSoldier_commanderList[1] = &n[0]; WoodElfSoldier_commanderList[3] = &n[1];
    WoodElfSoldier_commanderList[5] = &n[2]; WoodElfSoldier_commanderList[9] = &n[3];
    expect(findCommander__14WoodElfSoldier(S) == reinterpret_cast<WoodElfSoldier*>(&c[2]), "nearest wins, ties keep the first");
    *reinterpret_cast<unsigned*>(c[2].b + 12) = 8; 
    expect(findCommander__14WoodElfSoldier(S) == reinterpret_cast<WoodElfSoldier*>(&c[3]), "bit 3 candidate skipped");
    *reinterpret_cast<int*>(c[3].b + 960) = 1;
    expect(findCommander__14WoodElfSoldier(S) == reinterpret_cast<WoodElfSoldier*>(&c[1]), "led candidate skipped");
    setpos(c[0], 10, 20, 30 + 700); setpos(c[1], 10, 20, 30 + 601);
    expect(findCommander__14WoodElfSoldier(S) == 0, "nothing inside the squared radius gives zero");
    if (failures) return 1;
    std::printf("findCommander WoodElfSoldier contract passed\n");
    return 0;
}
