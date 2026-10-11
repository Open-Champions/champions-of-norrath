#include <cstdio>
#include <utility>
using Callback = void (*)();
extern "C" void norrath_test_ctors();
static unsigned trace[104], length;
template<unsigned Index> void callback() { trace[length++] = Index + 1; }
extern "C" {
Callback norrath_test_ctor_list[103];
unsigned char norrath_test_frames[4];
void norrath_test_dtors() {}
void norrath_test_register(const void* frames, void* object) {
    if (frames != norrath_test_frames) __builtin_trap();
    auto* words = static_cast<unsigned*>(object);
    for (unsigned i = 0; i < 6; ++i) words[i] = i;
    trace[length++] = 999;
}
int norrath_test_atexit(Callback function) {
    if (function != norrath_test_dtors) __builtin_trap();
    trace[length++] = 998;
    return -1; // Original caller does not retry or invent failure handling.
}
}
template<unsigned... Index> void fill(std::integer_sequence<unsigned, Index...>) {
    ((norrath_test_ctor_list[Index + 1] = callback<Index>), ...);
}
int main() {
    for (unsigned sentinel = 0; sentinel < 2; ++sentinel) {
        for (unsigned count = 0; count <= 101; ++count) {
            fill(std::make_integer_sequence<unsigned, 101>{});
            norrath_test_ctor_list[0] = reinterpret_cast<Callback>(
                static_cast<__UINTPTR_TYPE__>(sentinel ? ~0u : count));
            norrath_test_ctor_list[count + 1] = nullptr;
            length = 0;
            norrath_test_ctors();
            if (length != count + 2 || trace[0] != 999 || trace[count + 1] != 998) return 1;
            for (unsigned i = 0; i < count; ++i) if (trace[i + 1] != count - i) return 2;
        }
    }
    std::puts("204 constructor tables: frame registration, reverse callbacks, atexit order passed");
}
