// Host check only; not a build input. Build as x86_64 with statics below 4 GiB. Checks that Find
// gets the child by reference, its result reaches Erase, and the child's parent word is cleared.
#include "remove_child.cpp"
#include <cstdio>
#include <cstring>
#include <string>

static unsigned char self_object[64], child_object[64];
static std::string trace;
extern "C" {
int Find__Ct5VISet1ZP5VIWndRCP5VIWnd(void* set, void* const* key) {
    trace += (set == self_object ? "find:self," : "find:?,") + std::string(*key == child_object ? "child;" : "?;");
    return 9;
}
void Erase__t5VISet1ZP5VIWndi(void* set, int index) {
    trace += (set == self_object ? "erase:self," : "erase:?,") + std::to_string(index) + ";";
}
}

int main() {
    std::memset(child_object, 0x77, sizeof child_object);
    RemoveChild__5VIWndP5VIWnd(self_object, child_object);
    unsigned parent;
    std::memcpy(&parent, child_object + 20, 4);
    bool ok = trace == "find:self,child;erase:self,9;" && parent == 0 && child_object[19] == 0x77 &&
              child_object[24] == 0x77;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
