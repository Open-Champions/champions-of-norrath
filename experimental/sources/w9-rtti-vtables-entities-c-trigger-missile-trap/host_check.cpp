// Host check only. Not an EE build input.
#include "trigger.cpp"
#include <cstdio>
static int adds = 0, removes = 0;
static void* seen = nullptr;
extern "C" void objectAddToRunList__FP10GameObject(void* object) { ++adds; seen = object; }
extern "C" void objectRemoveFromRunList__FP10GameObject(void* object) { ++removes; seen = object; }
int main() {
    int bad = 0;
    int object;
    const int values[] = {0, 1, -1, 2, 0x10000};
    for (int v : values) {
        adds = removes = 0; seen = nullptr;
        msg_trigger__11MissileTrapi(&object, v);
        if (v != 0 && (adds != 1 || removes != 0)) { std::printf("on %d\n", v); ++bad; }
        if (v == 0 && (adds != 0 || removes != 1)) { std::printf("off\n"); ++bad; }
        if (seen != &object) { std::printf("object %d\n", v); ++bad; }
    }
    std::printf(bad ? "failed\n" : "msg_trigger__11MissileTrapi passed\n");
    return bad != 0;
}
