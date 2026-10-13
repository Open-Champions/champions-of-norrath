// Host check only; not a build input. Callees record order and arguments; DestroyPool can
// leave the storage word alone or clear it, which are the two outcomes the original branches on.
#include "dtor.cpp"
#include <cstdio>
#include <string>

alignas(8) static unsigned char storage[64];
static unsigned char block[16];
static std::string trace;
static bool clear_word;
static Container* as_c() { return reinterpret_cast<Container*>(storage); }
extern "C" void DestroyPool__t6VIPool1ZP7VIRFont(Container* self) {
    trace += "destroy@" + std::to_string(reinterpret_cast<unsigned char*>(self) - storage) + ";";
    if (clear_word) self->storage = 0;
}
extern "C" void __builtin_vec_delete(void* pointer) { trace += pointer == block ? "vecdel;" : "vecdel?;"; }
extern "C" void __builtin_delete(void* pointer) { trace += "delete@" + std::to_string(static_cast<unsigned char*>(pointer) - storage) + ";"; }

int main() {
    bool ok = true;
    const int flag_values[] = {0, 1, 2, 3, -1, -2};
    for (int has_block = 0; has_block < 2; ++has_block)
    for (int clear = 0; clear < 2; ++clear)
    for (int flags : flag_values) {
        trace.clear();
        clear_word = clear;
        for (auto& b : storage) b = 0x5a;
        as_c()->storage = has_block ? block : nullptr;
        _$_t6VIPool1ZP7VIRFont(as_c(), flags);
        std::string want;
        if (has_block) { want += "destroy@0;"; if (!clear) want += "vecdel;"; }
        if (flags & 1) want += "delete@0;";
        if (trace != want) { std::puts(trace.c_str()); ok = false; }
        for (int i = 0; i < 64; ++i) {
            const int at = (int)__builtin_offsetof(Container, storage);  // 4 on the target, 8 where pointers are 64-bit
            const bool in_word = i >= at && i < at + (int)sizeof(void*);
            if (!in_word && storage[i] != 0x5a) ok = false;
        }
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
