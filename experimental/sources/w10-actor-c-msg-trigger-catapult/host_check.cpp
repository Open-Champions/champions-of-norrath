#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" void msg_trigger__8Catapulti(void*, int);
static std::vector<std::string> trace;
static int failures = 0;
static unsigned char object[200];
static int header_marker;
extern "C" void animInitState__FP14AnimationState(void* state) {
    trace.push_back(state == object + 92 ? "init" : "init-wrong");
}
extern "C" void* lumpFindResource__FPcT0(const char* lump, const char* name) {
    trace.push_back(std::string("find:") + lump + ":" + name);
    return &header_marker;
}
extern "C" void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flag) {
    trace.push_back(std::string("add:") + (state == object + 92 ? "state" : "wrong") + ":" +
                  (header == &header_marker ? "header" : "wrong") + ":" + std::to_string(flag));
}
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
int main() {
    for (int arg : {0, 5, -1}) {
        trace.clear();
        std::memset(object, 0x42, sizeof object);
        msg_trigger__8Catapulti(object, arg);
        expect(trace.size() == 3, "three calls");
        if (trace.size() == 3) {
            expect(trace[0] == "init", "init first");
            expect(trace[1] == "find:catapult.lmp:catapult.anm", "lookup second with both names in order");
            expect(trace[2] == "add:state:header:1", "add last with state, found header, 1");
        }
        bool untouched = true;
        for (unsigned char c : object) if (c != 0x42) untouched = false;
        expect(untouched, "object bytes not written by the method itself");
    }
    return failures ? 1 : 0;
}
