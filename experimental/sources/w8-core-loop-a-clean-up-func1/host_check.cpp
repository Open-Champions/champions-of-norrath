// Host check for cleanUpFunc1__FP10GameObject. Not an EE plan input; every callee is a mock.
#include "clean_up_func1.cpp"
#include <cstring>
#include <string>

static std::string trace;
static int tiPush, tiSave, tiNpc, tiOther;
static void* selfSeen;
static int argSeen;
static bool threw;
static void (*iterateSeen)(unsigned char*);

extern "C" {
void* __tf11PushTrigger() { return &tiPush; }
void* __tf9Savepoint() { return &tiSave; }
void* __tf3NPC() { return &tiNpc; }
int __eq__C9type_infoRC9type_info(void* l, void* r) { trace += "="; return l == r; }
void __throw_bad_typeid() { threw = true; trace += "T"; }
void objectIterateGrid__FPFP10GameObject_v(void (*v)(unsigned char*)) { trace += "G"; iterateSeen = v; }
void killExpendableFunc__FP10GameObject(unsigned char*) {}
}

static void* typeToReturn;
static void* typeFn() { trace += "q"; return typeToReturn; }
static void slotFn(void* self, int arg) { trace += "S"; selfSeen = self; argSeen = arg; }

struct Fake {
    alignas(16) unsigned char object[128];
    alignas(16) unsigned char vtable[256];
};

static void setup(Fake& f, void* type, unsigned flags, short adjust) {
    std::memset(&f, 0, sizeof f);
    typeToReturn = type;
    std::memcpy(f.object + 12, &flags, 4);
    unsigned char* vt = f.vtable;
    std::memcpy(f.object + 72, &vt, sizeof vt);
    void* (*tf)() = typeFn;
    std::memcpy(f.vtable + 4, &tf, sizeof tf);
    std::memcpy(f.vtable + 112, &adjust, 2);
    void (*sf)(void*, int) = slotFn;
    std::memcpy(f.vtable + 116, &sf, sizeof sf);
    trace.clear();
    selfSeen = nullptr;
    argSeen = -1;
    threw = false;
    iterateSeen = nullptr;
}

int main() {
    Fake f;
    // The three excluded types stop after their own comparison (and the ones before it).
    setup(f, &tiPush, 0x100000, 0);
    cleanUpFunc1__FP10GameObject(f.object);
    if (trace != "q=") return 1;
    setup(f, &tiSave, 0x100000, 0);
    cleanUpFunc1__FP10GameObject(f.object);
    if (trace != "q=q=") return 2;
    setup(f, &tiNpc, 0x100000, 0);
    cleanUpFunc1__FP10GameObject(f.object);
    if (trace != "q=q=q=") return 3;
    // Another type with bit 0x100000: slot at 112 with (object + adjust, 0), then the grid walk.
    setup(f, &tiOther, 0x100000, 24);
    cleanUpFunc1__FP10GameObject(f.object);
    if (trace != "q=q=q=SG") return 4;
    if (selfSeen != f.object + 24 || argSeen != 0 || iterateSeen != killExpendableFunc__FP10GameObject) return 5;
    // Without the bit: three comparisons, nothing else; other bits do not substitute.
    setup(f, &tiOther, ~0x00100000u, 0);
    cleanUpFunc1__FP10GameObject(f.object);
    if (trace != "q=q=q=") return 6;
    return 0;
}
