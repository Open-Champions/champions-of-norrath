// Host contract for msg_run__10SpawnMummy; not an EE input. 32-bit words are offsets from a host base and the callees log. The
// authored function was also compared with the original in a private R5900 run (200 scenarios over the delay, the spawned flag,
// allocation success, the link, and the watched object's flag bit; every original word covered).
#define W12_SPELL_C_HOST 1
#include "msg_run.cpp"
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w12_spell_c_host_base;
static std::string calls;
static void* call_object;
unsigned w12_spell_c_host_call(unsigned fn, void* object, int count, unsigned long a1, unsigned long) {
    calls += "entry" + std::to_string(fn) + "(" + std::to_string(count) + "," + std::to_string(a1) + ");";
    call_object = object;
    return 0;
}
extern "C" {
int frameNm;
static bool alloc_ok;
static unsigned char block[1200] __attribute__((aligned(16)));
static short ctor_heading;
static Point3 ctor_position;
static char *ctor_b, *ctor_c;
static int roll;
void* blockAlloc__Fi(int size) { calls += "alloc" + std::to_string(size) + ";"; return alloc_ok ? block : nullptr; }
int eRand__10GameObject(void*) { calls += "rand;"; return roll; }
void __5MummyG6Point3sPPcPc(void*, Point3 position, short a, char** b, char* c) {
    calls += "mummy;"; ctor_position = position; ctor_heading = a; ctor_b = reinterpret_cast<char*>(b); ctor_c = c;
}
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* link, void* data) {
    calls += "link" + std::to_string(link == block && data == block ? 1 : 0) + ";"; (void)head;
}
void objectAddToDeleteList__FP10GameObject(void*) { calls += "del;"; }
void msg_run__9DustDevil(void*) { calls += "dust;"; }
}
static unsigned char arena[0x2000] __attribute__((aligned(16)));
static void put32(unsigned off, unsigned v) { std::memcpy(arena + off, &v, 4); }
static void putf(unsigned off, float v) { std::memcpy(arena + off, &v, 4); }
enum { O = 0x100, Y = 0xc00, L = 0x600, W = 0x700, X = 0x800, T = 0x900, NAME = 0xa00 };
static void reset(int delay, unsigned spawned) {
    w12_spell_c_host_base = arena;
    std::memset(arena, 0, sizeof arena); std::memset(block, 0, sizeof block);
    calls.clear(); call_object = nullptr; alloc_ok = true; roll = 0x12345; frameNm = 100;
    put32(O + 80, static_cast<unsigned>(delay)); put32(O + 92, spawned); put32(O + 88, NAME);
    putf(O + 16, 1.0f); putf(O + 20, 2.0f); putf(O + 24, 3.0f);
}
int main() {
    // too early: only the dust routine
    reset(100, 0);
    msg_run__10SpawnMummy(arena + O);
    if (calls != "dust;") return 1;
    // due: allocate, build, link and flag the mummy; the mocked link routine does not store the link, so the spawner then deletes itself
    reset(99, 0);
    msg_run__10SpawnMummy(arena + O);
    if (calls != "alloc1128;rand;mummy;link1;del;dust;") return 2;
    if (ctor_heading != 0x2345 || ctor_position.x != 1.0f || ctor_position.z != 3.0f || ctor_b != nullptr || ctor_c != reinterpret_cast<char*>(arena + NAME)) return 3;
    unsigned long long block160;
    std::memcpy(&block160, block + 160, 8);
    if (block160 != 0x01000000ull || arena[O + 92] != 1) return 4;
    // allocation failure still marks the spawner as done, and with no link it deletes itself
    reset(99, 0); alloc_ok = false;
    msg_run__10SpawnMummy(arena + O);
    if (calls != "alloc1128;del;dust;" || arena[O + 92] != 1) return 5;
    // already spawned with a live link whose watched word is zero: the link's object gets table entry 184 called with 10
    reset(0, 1);
    put32(O + 96, L); put32(L + 12, W); put32(W + 72, T); put32(T + 184 + 4, 0x77);
    const short adjust = 8;
    std::memcpy(arena + T + 184, &adjust, 2);
    msg_run__10SpawnMummy(arena + O);
    if (calls != "entry119(1,10);dust;" || call_object != arena + W + 8) return 6;
    // a watched object whose flags lack bit 8 suppresses the call; with it set the call happens
    reset(0, 1);
    put32(O + 96, L); put32(L + 12, W); put32(W + 72, T); put32(T + 184 + 4, 0x77); put32(O + 100, X); put32(X + 12, Y); put32(Y + 12, 0);
    msg_run__10SpawnMummy(arena + O);
    if (calls != "dust;") return 7;
    put32(Y + 12, 8);
    calls.clear();
    msg_run__10SpawnMummy(arena + O);
    if (calls != "entry119(1,10);dust;") { std::puts(calls.c_str()); return 8; }
    return 0;
}
