// Host check only; not a build input. Records the call trace of the routine.
#include "onkeyup.cpp"
#include <cstdarg>
#include <cstdio>
#include <string>

static std::string trace;
static int self_object;
extern "C" int scePrintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    trace += "printf:" + std::string(format) + "/" + std::to_string(va_arg(args, int)) + ";";
    va_end(args);
    return 0;
}
extern "C" void ProcessScreen__10VIWndMcErriii(void* self, int a, int b, int c) {
    trace += std::string("screen:") + (self == &self_object ? "this" : "?") + "," + std::to_string(a) + "," +
             std::to_string(b) + "," + std::to_string(c) + ";";
}

int main() {
    bool ok = true;
    for (int key : {0, 13, -1, 142}) {
        trace.clear();
        OnKeyUp__10VIWndMcErri(&self_object, key);
        ok = ok && trace == "printf:VIWndMcErr::OnKeyUp(KeyCode = %d)\n/" + std::to_string(key) + ";screen:this,4," +
                                std::to_string(key) + ",0;";
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
