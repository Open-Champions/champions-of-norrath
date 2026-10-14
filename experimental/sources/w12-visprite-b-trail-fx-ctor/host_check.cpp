// Host check only; not a build input. Fills an object with a pattern, runs the routine with
// recording stubs for the callees, then compares the call trace (names, this-relative offsets,
// extra arguments) and every byte of the object against an expectation built from the observed
// stores. For destructors flags 0 to 3 and 0xffffffff are tried.
#include "ctor.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static unsigned char object[0x2000];
static std::string trace;
static std::string off(void* p) { return std::to_string(static_cast<unsigned char*>(p) - object); }
extern "C" {
void __7VILNode(void* self) { trace += "__7VILNode:" + off(self) + ";"; }
void __7VILList(void* self) { trace += "__7VILList:" + off(self) + ";"; }
}

static bool run(int flags) {
    std::memset(object, 0x5a, sizeof object);
    unsigned char expect[sizeof object];
    std::memset(expect, 0x5a, sizeof expect);
    std::string want;
    want += "__7VILNode:0;";
    want += "__7VILList:28;";
    want += "__7VILList:40;";
    { unsigned v = 0u; std::memcpy(expect + 52, &v, 4); }
    (void)flags;
    trace.clear();
    fn_9VITrailFx(object);
    bool ok = trace == want && std::memcmp(object, expect, sizeof object) == 0;
    if (!ok) std::printf("got  %s\nwant %s\n", trace.c_str(), want.c_str());
    return ok;
}

int main() {
    bool ok = true;
    for (int flags : {0, 1, 2, 3, -1}) ok = run(flags) && ok;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
