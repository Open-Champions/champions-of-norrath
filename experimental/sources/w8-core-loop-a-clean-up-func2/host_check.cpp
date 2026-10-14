// Host check for cleanUpFunc2__FP10GameObject. Not an EE plan input; every callee is a mock.
#include "clean_up_func2.cpp"
#include <cstring>
#include <string>

extern "C" {
unsigned char player[21880];
}

static std::string trace;
static int tiGold, tiContainer, tiOther;
static void* typeToReturn;
static void* seenSelf[4];
static int seenWord[4], seenSixtyFour;
static float seenFloat;
static void (*iterateSeen)(unsigned char*);
static int queries;

extern "C" {
void* __tf4Gold() { return &tiGold; }
void* __tf9Container() { return &tiContainer; }
int __eq__C9type_infoRC9type_info(void* l, void* r) { trace += "="; return l == r; }
void __throw_bad_typeid() { trace += "T"; }
void objectIterateGrid__FPFP10GameObject_v(void (*v)(unsigned char*)) { trace += "G"; iterateSeen = v; }
void killExpendableFunc__FP10GameObject(unsigned char*) {}
}

static void* typeFn() { trace += "q"; ++queries; return typeToReturn; }
static void goldSlot(void* self, int word) { trace += "g"; seenSelf[0] = self; seenWord[0] = word; }
static void wideSlot(void* self, int word, int sf, float f) {
    trace += "w"; seenSelf[1] = self; seenWord[1] = word; seenSixtyFour = sf; seenFloat = f;
}
static void plainSlot(void* self) { trace += "p"; seenSelf[2 + (trace.find('p') != trace.rfind('p'))] = self; }

struct Fake {
    alignas(16) unsigned char object[128];
    alignas(16) unsigned char vtable[256];
};

static void setup(Fake& f, void* type, unsigned flags) {
    std::memset(&f, 0, sizeof f);
    typeToReturn = type;
    std::memcpy(f.object + 12, &flags, 4);
    unsigned char* vt = f.vtable;
    std::memcpy(f.object + 72, &vt, sizeof vt);
    void* (*tf)() = typeFn;
    std::memcpy(f.vtable + 4, &tf, sizeof tf);
    short a16 = 16, a32 = 32, a48 = 48;
    std::memcpy(f.vtable + 16, &a16, 2);
    std::memcpy(f.vtable + 32, &a32, 2);
    std::memcpy(f.vtable + 48, &a48, 2);
    void (*w)(void*, int, int, float) = wideSlot;
    void (*p)(void*) = plainSlot;
    void (*g)(void*, int) = goldSlot;
    std::memcpy(f.vtable + 20, &w, sizeof w);
    std::memcpy(f.vtable + 36, &p, sizeof p);
    std::memcpy(f.vtable + 52, &g, sizeof g);
    trace.clear();
    for (void*& s : seenSelf) s = nullptr;
    for (int& s : seenWord) s = -1;
    seenSixtyFour = 0;
    seenFloat = 0;
    iterateSeen = nullptr;
    queries = 0;
    unsigned word = 0x01020304u;
    std::memcpy(player + 21872, &word, 4);
}

int main() {
    Fake f;
    // flag 0x40: returns before any type query
    setup(f, &tiOther, 0x40);
    cleanUpFunc2__FP10GameObject(f.object);
    if (!trace.empty()) return 1;
    // Gold: slot 48 with (object + 48, player word); no grid walk
    setup(f, &tiGold, 0);
    cleanUpFunc2__FP10GameObject(f.object);
    if (trace != "q=g" || seenSelf[0] != f.object + 48 || seenWord[0] != 0x01020304) return 2;
    // Other type, no 0x20: wide slot, container query, grid walk
    setup(f, &tiOther, 0);
    cleanUpFunc2__FP10GameObject(f.object);
    if (trace != "q=wq=G") return 3;
    if (seenSelf[1] != f.object + 16 || seenWord[1] != 0x01020304 || seenSixtyFour != 64 || seenFloat != 300.0f) return 4;
    if (iterateSeen != killExpendableFunc__FP10GameObject) return 5;
    // bit 0x20: plain slot (adjust 32) before the container query
    setup(f, &tiOther, 0x20);
    cleanUpFunc2__FP10GameObject(f.object);
    if (trace != "q=wpq=G" || seenSelf[2] != f.object + 32) return 6;
    // Container without bit 0x20: plain slot after the second query
    setup(f, &tiContainer, 0);
    cleanUpFunc2__FP10GameObject(f.object);
    if (trace != "q=wq=pG" || seenSelf[2] != f.object + 32) return 7;
    // Container with bit 0x20: plain slot twice
    setup(f, &tiContainer, 0x20);
    cleanUpFunc2__FP10GameObject(f.object);
    if (trace != "q=wpq=pG" || seenSelf[3] != f.object + 32) return 8;
    return 0;
}
