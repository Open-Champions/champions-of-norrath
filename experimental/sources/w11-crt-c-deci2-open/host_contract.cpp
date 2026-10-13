// Host check only; not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -o /tmp/c host_contract.cpp deci2_open.cpp
// Pointers are compared as their low 32 bits, the width of a target pointer.
#include <cstdint>
extern "C" {
unsigned char norrath_deci2_user_area_view[4152];
static int calls, g_request, g_result;
static unsigned g_words[4];
int Deci2Call(int request, void* words) {
    ++calls; g_request = request;
    for (int i = 0; i < 4; ++i) g_words[i] = static_cast<const unsigned*>(words)[i];
    return g_result;
}
int sceDeci2Open(int protocol, void* option, void (*handler)());
}
static void handler() {}
static unsigned low(const void* p) { return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p)); }
int main() {
    static int option;
    g_result = 5;
    if (sceDeci2Open(528, &option, handler) != 5 || calls != 1 || g_request != 1) return 1;
    if (g_words[0] != 528 || g_words[1] != low(&option) || g_words[2] != low(reinterpret_cast<void*>(handler))) return 2;
    if (g_words[3] != (low(norrath_deci2_user_area_view) | 0x20000000u)) return 3;
    g_result = -4;
    if (sceDeci2Open(0x12345, nullptr, nullptr) != -4) return 4;
    if (g_words[0] != 0x2345 || g_words[1] != 0 || g_words[2] != 0) return 5;      // protocol masked to 16 bits
    sceDeci2Open(-1, &option, handler);
    if (g_words[0] != 0xffff) return 6;
    return 0;
}
