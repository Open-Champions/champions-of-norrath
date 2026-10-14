// Host check only; not a build input. Callees record their order and arguments.
#include "dtor.cpp"
#include <cstdio>
#include <string>

static std::string trace;
static unsigned char storage[64];
extern "C" void Clear__t6VIList1Z10VIMaterial(void* self) { trace += "clear@" + std::to_string(static_cast<unsigned char*>(self) - storage) + ";"; }
extern "C" void __builtin_delete(void* pointer) { trace += "delete@" + std::to_string(static_cast<unsigned char*>(pointer) - storage) + ";"; }

int main() {
    bool ok = true;
    const int flag_values[] = {0, 1, 2, 3, 4, 5, -1, -2};
    for (int flags : flag_values) {
        trace.clear();
        for (auto& b : storage) b = 0x5a;
        _$_t6VIList1Z10VIMaterial(storage, flags);
        const std::string want = std::string("clear@0;") + ((flags & 1) ? "delete@0;" : "");
        if (trace != want) { std::puts(trace.c_str()); ok = false; }
        for (auto b : storage) if (b != 0x5a) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
