#include <cstring>
extern "C" {
unsigned char player[23984 * 4 + 512];
void inventorySetAnim__FiP14AnimationStatei(int, void*, int);
}

static int n;
static char lumps[8][32], resources[8][32];
static int findCalls, getCalls, addCalls;
static void* gotState; static void* gotHeader; static int existing;
static void* addState; static void* addHeader; static float addRate; static int addFlags;
static int tag;

extern "C" void* lumpFindResource__FPcT0(char* l, char* r) {
    std::strcpy(lumps[findCalls], l); std::strcpy(resources[findCalls], r);
    return (void*)(long)(0x1000 + findCalls++);
}
extern "C" void* animGetAnimComponent__FP14AnimationStateP15AnimationHeader(void* s, void* h) { ++getCalls; gotState = s; gotHeader = h; return existing ? (void*)1 : nullptr; }
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* s, void* h, float r, int f) { ++addCalls; addState = s; addHeader = h; addRate = r; addFlags = f; }

static void reset() { findCalls = getCalls = addCalls = 0; existing = 0; std::memset(player, 0, sizeof player); }
static void setup(int p, int type, int state, int word) {
    unsigned char* r = player + p * 23984;
    *(int*)(r + 21872) = word; *(int*)(r + 84) = type; r[23582] = (unsigned char)state;
}

int main() {
    static const char* const lump[10] = {"ragnar2.lmp","sonya2.lmp","elf_mwo2.lmp","elf_fwo2.lmp","hem_high.lmp","elf_fhi2.lmp","eru_male.lmp","eru_fem.lmp","elf_mdk.lmp","elf_fdk2.lmp"};
    int dummy;
    for (int t = 0; t < 10; ++t) {
        reset(); setup(2, t, 1, 5);
        inventorySetAnim__FiP14AnimationStatei(2, &dummy, 0);
        if (findCalls != 2 || std::strcmp(lumps[0], lump[t]) || std::strcmp(lumps[1], lump[t])) return 1 + t;
    }
    static const struct { int s; const char* r; } res[] = {{1,"dhs_idleb.anm"},{2,"dha_idleb.anm"},{3,"l_idleb.anm"},{4,"b_idleb.anm"},{5,"nw_idleb.anm"},{6,"ohw_idleb.anm"},{7,"dw_idleb.anm"},{0,"ohw_idleb.anm"},{8,"ohw_idleb.anm"},{-1,"ohw_idleb.anm"}};
    for (auto& e : res) {
        reset(); setup(1, 3, e.s, 1);
        inventorySetAnim__FiP14AnimationStatei(1, &dummy, 0);
        if (findCalls != 2 || std::strcmp(resources[0], e.r) || std::strcmp(resources[1], e.r)) return 20 + e.s;
    }
    // adds the transition with the second lookup's header, rate 0.3, flags 67
    reset(); setup(0, 4, 2, 9);
    inventorySetAnim__FiP14AnimationStatei(0, &dummy, 0);
    if (getCalls != 1 || gotState != &dummy || gotHeader != (void*)0x1000) return 40;
    if (addCalls != 1 || addState != &dummy || addHeader != (void*)0x1001 || addRate != 0.3f || addFlags != 67) return 41;
    // flag nonzero: rate 0
    reset(); setup(3, 4, 2, 9);
    inventorySetAnim__FiP14AnimationStatei(3, &dummy, 5);
    if (addCalls != 1 || addRate != 0.0f) return 42;
    // already has the component: one lookup only, no transition
    reset(); setup(3, 4, 2, 9); existing = 1;
    inventorySetAnim__FiP14AnimationStatei(3, &dummy, 0);
    if (findCalls != 1 || getCalls != 1 || addCalls != 0) return 43;
    // word at +21872 zero: nothing
    reset(); setup(3, 4, 2, 0);
    inventorySetAnim__FiP14AnimationStatei(3, &dummy, 0);
    if (findCalls || getCalls || addCalls) return 44;
    // type 10 and above: nothing
    reset(); setup(3, 10, 2, 1);
    inventorySetAnim__FiP14AnimationStatei(3, &dummy, 0);
    if (findCalls || getCalls || addCalls) return 45;
    reset(); setup(3, -1, 2, 1);
    inventorySetAnim__FiP14AnimationStatei(3, &dummy, 0);
    if (findCalls || getCalls || addCalls) return 46;
    return 0;
}
