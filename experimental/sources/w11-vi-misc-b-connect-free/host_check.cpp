// Host check only; not a build input. Checks the call order and the this+40 arguments.
#include "free.cpp"
#include <cstdio>
#include <string>

static unsigned char object[128];
static std::string trace;
extern "C" {
void RemoveChild__5VIWndP5VIWnd(void* self, void* child) {
    trace += std::string("rm:") + (self == object ? "self" : "?") + "," +
             std::to_string(static_cast<unsigned char*>(child) - object) + ";";
}
void Free__12VIWndMessage(void* self) { trace += "free:" + std::to_string(static_cast<unsigned char*>(self) - object) + ";"; }
}

int main() {
    Free__12VIWndConnect(object);
    bool ok = trace == "rm:self,40;free:40;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
