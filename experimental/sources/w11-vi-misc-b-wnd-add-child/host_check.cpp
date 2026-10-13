// Host check only; not a build input. Build as x86_64 with statics below 4 GiB. Checks that the
// set receives the child by reference and that the child's parent word becomes this.
#include "add_child.cpp"
#include <cstdio>
#include <cstring>

static unsigned char self_object[64], child_object[64];
static void* seen_set;
static void* seen_key;
extern "C" void Insert__t5VISet1ZP5VIWndRCP5VIWnd(void* set, void* const* key) { seen_set = set; seen_key = *key; }

int main() {
    std::memset(child_object, 0x77, sizeof child_object);
    AddChild__5VIWndP5VIWnd(self_object, child_object);
    unsigned parent;
    std::memcpy(&parent, child_object + 20, 4);
    bool ok = seen_set == self_object && seen_key == child_object &&
              parent == static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(self_object)) &&
              child_object[19] == 0x77 && child_object[24] == 0x77;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
