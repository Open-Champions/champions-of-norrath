// Host check only; not a build input.
#include "isopen.cpp"
#include <cstdio>
#include <string>

static unsigned char object[256];
static int opening_result, open_result;
static std::string trace;
extern "C" int IsOpening__C10VIUIWindow(const void* w) {
    trace += "opening:" + std::to_string(static_cast<const unsigned char*>(w) - object) + ";";
    return opening_result;
}
extern "C" int IsOpen__C10VIUIWindow(const void* w) {
    trace += "open:" + std::to_string(static_cast<const unsigned char*>(w) - object) + ";";
    return open_result;
}

int main() {
    bool ok = true;
    struct Case { int opening, open, want; const char* trace; };
    const Case cases[] = {{0, 0, 0, "opening:128;open:128;"}, {0, 7, 1, "opening:128;open:128;"},
                          {5, 0, 1, "opening:128;"}, {-1, 3, 1, "opening:128;"}};
    for (const Case& c : cases) {
        opening_result = c.opening; open_result = c.open; trace.clear();
        ok = ok && IsOpen__11VIGUIDialog(object) == c.want && trace == c.trace;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
