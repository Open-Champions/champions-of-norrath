// Host check only; not a build input. Checks the call order (timer read, then SetFocus) and the
// SetFocus arguments (this, no window, force = true).
#include "ondrawself.cpp"
#include <cstdio>
#include <string>

static std::string trace;
extern "C" long long GetTime__7VITimer() { trace += "T;"; return 1234567890123LL; }
extern "C" void SetFocus__5VIWndP5VIWndb(void* self, void* window, bool force) {
    trace += std::string("F:") + (self ? "self" : "null") + "," + (window ? "win" : "null") + "," + (force ? "1" : "0") + ";";
}
int main() {
    int object;
    OnDrawSelf__10VIWndLegal(&object);
    const bool ok = trace == "T;F:self,null,1;";
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
