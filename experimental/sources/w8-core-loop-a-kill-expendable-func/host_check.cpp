// Host check for killExpendableFunc__FP10GameObject. Not an EE plan input.
#include "kill_expendable_func.cpp"
#include <cstring>

static void* seenSelf;
static int seenArg, calls;
static void slot(void* self, int arg) {
    seenSelf = self;
    seenArg = arg;
    ++calls;
}

// A fake object: 128 bytes with the vtable pointer at +72. Host pointers are 8 bytes wide, so the fake
// vtable is read through the same byte loaders the function uses.
struct Fake {
    alignas(16) unsigned char object[128];
    alignas(16) unsigned char vtable[32];
};

static void setup(Fake& f, unsigned flags, short adjust) {
    std::memset(&f, 0, sizeof f);
    std::memcpy(f.object + 12, &flags, 4);
    unsigned char* vt = f.vtable;
    std::memcpy(f.object + 72, &vt, sizeof vt);
    std::memcpy(f.vtable + 8, &adjust, 2);
    void (*fn)(void*, int) = slot;
    std::memcpy(f.vtable + 12, &fn, sizeof fn);
    seenSelf = nullptr;
    seenArg = 0;
    calls = 0;
}

int main() {
    Fake f;
    setup(f, 0x1000, 0);
    killExpendableFunc__FP10GameObject(f.object);
    if (calls != 1 || seenSelf != f.object || seenArg != 3) return 1;
    setup(f, 0x1000 | 0x7, 16);                // other bits are ignored; this-adjustment is applied
    killExpendableFunc__FP10GameObject(f.object);
    if (calls != 1 || seenSelf != f.object + 16) return 2;
    setup(f, 0x101000, 0);                      // bit 0x100000 set: skipped
    killExpendableFunc__FP10GameObject(f.object);
    if (calls != 0) return 3;
    setup(f, 0x100000, 0);                      // 0x1000 clear: skipped
    killExpendableFunc__FP10GameObject(f.object);
    if (calls != 0) return 4;
    setup(f, 0, 0);
    killExpendableFunc__FP10GameObject(f.object);
    if (calls != 0) return 5;
    setup(f, 0x1000, -8);                       // negative adjustment, object not at the start of its record
    killExpendableFunc__FP10GameObject(f.object);
    if (seenSelf != f.object - 8) return 6;
    return 0;
}
