#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

extern "C" int msg_load__8CatapultPc(void*, char*);
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
    for (int value : {1, -1, 2, 127, -128}) {
        trace.clear();
        std::memset(object, 0x42, sizeof object);
        char buffer[3] = {static_cast<char>(value), 9, 9};
        expect(msg_load__8CatapultPc(object, buffer) == 1, "returns 1");
        expect(trace.size() == 3 && trace[0] == "init" && trace[1] == "find:catapult.lmp:catapult.anm" &&
                   trace[2] == "add:state:header:1",
               "nonzero byte replays the trigger sequence");
        expect(buffer[1] == 9 && buffer[2] == 9, "buffer not written");
    }
    trace.clear();
    char zero[2] = {0, 9};
    expect(msg_load__8CatapultPc(object, zero) == 1, "zero byte still consumes one byte");
    expect(trace.empty(), "zero byte does nothing");
    bool untouched = true;
    for (unsigned char c : object) if (c != 0x42) untouched = false;
    expect(untouched, "object bytes untouched");
    return failures ? 1 : 0;
}
