// Host contract for msg_moveCollision__6GoblinPP10GameObject; not an EE input.
#define W10_ACTOR_G_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10g_host_base;
static std::string calls;
static void *entrySeenObj, *entrySeenSelf; static int entrySeenInt; static float entrySeenFloat;
static void entry(void* o, void* s, int i, float f) { calls += "entry;"; entrySeenObj = o; entrySeenSelf = s; entrySeenInt = i; entrySeenFloat = f; }
static unsigned char nodeA[4], nodeB[4];
static int typeCalls; static bool eqResults[4];
static void* nodeFn() { calls += "type;"; return nodeB; }
void* w10g_host_fn(unsigned w) { return w == 1 ? reinterpret_cast<void*>(nodeFn) : reinterpret_cast<void*>(entry); }
#include "msg_move_collision.cpp"
extern "C" void* __tf9Container() { calls += "tf;"; return nodeA; }
extern "C" int __eq__C9type_infoRC9type_info(const void* a, const void* b) {
    if (a != nodeB || b != nodeA) calls += "BADARGS;"; calls += "eq;"; return eqResults[typeCalls++];
}
static void *shotState, *shotHeader; static int shotFlags, shots; static float fa, fb;
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* s, void* h, float a, float b, int f) {
    calls += "shot;"; ++shots; shotState = s; shotHeader = h; fa = a; fb = b; shotFlags = f;
}
static unsigned char arena[0x4000] __attribute__((aligned(16)));
int main() {
    w10g_host_base = arena; int bad = 0;
    unsigned char* object = arena + 0x1000; unsigned char* o1 = arena + 0x2000; unsigned char* o2 = arena + 0x2400; unsigned char* o3 = arena + 0x2800;
    void* others[4];
    auto setup = [&](short state, unsigned short flags) {
        std::memset(arena, 0, sizeof arena); calls.clear(); shots = 0; typeCalls = 0;
        std::memcpy(object + 572, &state, 2); std::memcpy(object + 294, &flags, 2);
        unsigned table = 0x100, header = 0x777; std::memcpy(object + 36, &table, 4); std::memcpy(arena + table + 172, &header, 4);
        unsigned vt = 0x300, one = 1, two = 2; short adj = 12;
        for (unsigned char* o : {o1, o2, o3}) std::memcpy(o + 72, &vt, 4);
        std::memcpy(arena + vt + 4, &one, 4); std::memcpy(arena + vt + 16, &adj, 2); std::memcpy(arena + vt + 20, &two, 4);
        others[0] = o1; others[1] = o2; others[2] = o3; others[3] = nullptr;
        eqResults[0] = true; eqResults[1] = false; eqResults[2] = true;
    };
    setup(8, 0); msg_moveCollision__6GoblinPP10GameObject(object, others);
    unsigned bitsA, bitsB; std::memcpy(&bitsA, &fa, 4); std::memcpy(&bitsB, &fb, 4);
    if (calls != "shot;type;tf;eq;entry;type;tf;eq;type;tf;eq;entry;") { std::printf("fail: sequence %s\n", calls.c_str()); bad = 1; }
    if (shotState != object + 288 || shotHeader != arena + 0x777 || shotFlags != 32 || bitsA != 0x3dcccccc || bitsB != 0x3dcccccc) { std::puts("fail: one-shot arguments"); bad = 1; }
    if (entrySeenObj != o3 + 12 || entrySeenSelf != object || entrySeenInt != 1 || entrySeenFloat != 1.0f) { std::puts("fail: container entry arguments (last match)"); bad = 1; }
    setup(8, 0); others[0] = nullptr; msg_moveCollision__6GoblinPP10GameObject(object, others);
    if (calls != "shot;") { std::puts("fail: empty array still runs the one-shot"); bad = 1; }
    setup(8, 0x20); msg_moveCollision__6GoblinPP10GameObject(object, others);
    if (!calls.empty()) { std::puts("fail: bit 5 blocks everything"); bad = 1; }
    setup(9, 0); msg_moveCollision__6GoblinPP10GameObject(object, others);
    if (!calls.empty()) { std::puts("fail: state must be 8"); bad = 1; }
    if (!bad) std::puts("goblin msg_moveCollision ok");
    return bad;
}
