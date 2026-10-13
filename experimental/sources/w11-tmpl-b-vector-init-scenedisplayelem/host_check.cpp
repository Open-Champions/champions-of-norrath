// Host check only; not a build input. Callees return scripted results and record order; the
// DestroyPool stand-in can also clear the storage word, which the original re-reads.
#include "init.cpp"
#include <cstdio>
#include <string>

static std::string trace;
static int destroy_result, allocate_result;
static bool destroy_clears;
static unsigned char block[8];
static int seen_capacity;
extern "C" int DestroyPool__t8VIVector1Z18VISceneDisplayElem(Container* self) {
    trace += "destroy;";
    if (destroy_clears) self->storage = 0;
    return destroy_result;
}
extern "C" int Allocate__t8VIVector1Z18VISceneDisplayElem(Container* self) {
    trace += "alloc;"; seen_capacity = self->capacity;
    if (self->word0 != 0 || self->storage != 0) trace += "dirty;";
    return allocate_result;
}
extern "C" void __builtin_vec_delete(void* pointer) { trace += pointer == block ? "vecdel;" : "vecdel?;"; }

int main() {
    bool ok = true;
    const int results[] = {-3, -1, 0, 2};
    for (int has = 0; has < 2; ++has)
        for (int clears = 0; clears < 2; ++clears)
            for (int d : results)
                for (int a : results) {
                    trace.clear(); destroy_result = d; allocate_result = a; destroy_clears = clears;
                    Container box = {0x77, 0x88, has ? block : nullptr};
                    int r = Init__t8VIVector1Z18VISceneDisplayElemi(&box, 9);
                    std::string want_trace; int want;
                    if (has) {
                        want_trace = "destroy;";
                        if (d < 0) want = -1;
                        else {
                            if (!clears) want_trace += "vecdel;";
                            want_trace += "alloc;"; want = a < 0 ? -1 : 0;
                        }
                    } else { want_trace = "alloc;"; want = a < 0 ? -1 : 0; }
                    if (has && d < 0) {
                        if (box.word0 != 0x77 || box.capacity != 0x88 || (clears ? box.storage != nullptr : box.storage != block)) ok = false;
                    } else if (box.word0 != 0 || box.capacity != 9 || box.storage != nullptr || seen_capacity != 9) ok = false;
                    if (r != want || trace != want_trace) { std::puts(trace.c_str()); ok = false; }
                }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
