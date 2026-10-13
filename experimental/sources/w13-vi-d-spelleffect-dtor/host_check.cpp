// Host check only; not a build input. Stubs record the call order and the offsets of their arguments from the object.
#include "dtor.cpp"
#include <cstdio>
#include <string>

static unsigned char object[0x80];
static std::string trace;
static std::string off(void* p) { return std::to_string(static_cast<unsigned char*>(p) - object); }
extern "C" {
void Destroy__13VISpellEffect(void* self) { trace += "D:" + off(self) + ";"; }
void Destroy__16VISpellEventList(void* list) { trace += "L:" + off(list) + ";"; }
void __builtin_delete(void* pointer) { trace += "del:" + off(pointer) + ";"; }
}

int main() {
    bool ok = true;
    for (int flags : {0, 1, 2, 3, -1, -2}) {
        trace.clear();
        _$_13VISpellEffect(object, flags);
        std::string want = "D:0;L:36;L:24;L:12;";
        if (flags & 1) want += "del:0;";
        if (trace != want) { std::printf("flags %d got %s\n", flags, trace.c_str()); ok = false; }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
