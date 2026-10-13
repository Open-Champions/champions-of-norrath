// Host check only; not a build input. Checks the call order and arguments, the zeroed words and
// the untouched neighbours, and that the result is 0 whatever Init__12VIWndMessage returns.
#include "init.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static unsigned char object[2048];
static std::string trace;
static int message_result;
extern "C" {
int Init__12VIWndMessage(void* self) {
    trace += "init:" + std::to_string(static_cast<unsigned char*>(self) - object) + ";";
    return message_result;
}
void AddChild__5VIWndP5VIWnd(void* self, void* child) {
    trace += std::string("add:") + (self == object ? "self" : "?") + "," +
             std::to_string(static_cast<unsigned char*>(child) - object) + ";";
}
}

int main() {
    bool ok = true;
    for (int result : {0, 0x55}) {
        message_result = result;
        trace.clear();
        std::memset(object, 0x5a, sizeof object);
        static unsigned char expect[2048];
        std::memset(expect, 0x5a, sizeof expect);
        for (int off : {1916, 1920, 1924, 1912, 1956, 68, 1960, 1964}) std::memset(expect + off, 0, 4);
        const int r = Init__12VIWndConnect(object);
        ok = ok && r == 0 && trace == "init:40;add:self,40;" && std::memcmp(object, expect, sizeof object) == 0;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
