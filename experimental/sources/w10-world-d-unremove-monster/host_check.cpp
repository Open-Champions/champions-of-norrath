// Host contract for unremoveMonster__FP10GameObject. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct GameObject;
static int calls;
static int flags_at_call;
static unsigned char state_at_call;
static unsigned char* watched;
extern "C" void objectUpdateInGrid__FP10GameObject(GameObject*) {
    ++calls;
    flags_at_call = *reinterpret_cast<int*>(watched + 12);
    state_at_call = watched[570];
}
extern "C" void unremoveMonster__FP10GameObject(GameObject*);
alignas(8) static unsigned char obj[640];
static void run(int flags, unsigned char state) {
    std::memset(obj, 0, sizeof obj);
    watched = obj;
    *reinterpret_cast<int*>(obj + 12) = flags;
    obj[570] = state;
    unremoveMonster__FP10GameObject(reinterpret_cast<GameObject*>(obj));
}
int main() {
    run(-1, 4);
    if (calls != 1 || flags_at_call != static_cast<int>(0xfff77fff) || state_at_call != 0) return 1;
    if (*reinterpret_cast<int*>(obj + 12) != static_cast<int>(0xfff77fff) || obj[570] != 0) return 2;
    run(0x20, 5);                      // state is 5, not 4
    run(0x10, 4);                      // flag bit clear
    run(0x20, 0x84);                   // sign bit set: byte is not 4
    if (calls != 1) return 3;
    if (*reinterpret_cast<int*>(obj + 12) != 0x20 || obj[570] != 0x84) return 4;
    std::printf("unremove monster contract passed\n");
    return 0;
}
