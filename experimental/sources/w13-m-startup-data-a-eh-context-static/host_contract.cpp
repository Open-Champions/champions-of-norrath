// Host contract for eh_context_static. Not a build-plan input.
//   c++ -std=c++17 -Wall -Wextra -o check host_contract.cpp && ./check
// The expected layout and first-call effects come from the observed routine. The
// block's pointer word is compared with the target inside the same storage.
#include <cstddef>
#include <cstdio>
#include <cstring>
#include "eh_context_static.cpp"

int main() {
    int bad = 0;
    static_assert(offsetof(EhContextStatic, initialized) == 224, "flag word must follow the 224-byte block");
    static_assert(offsetof(EhContextStatic, target) == 232, "pointer target must sit 232 bytes into the block");

    // First call: flag clear, so the block is zeroed, the flag is set, and the pointer word is stored.
    for (unsigned char& byte : eh_context_storage.block) byte = 0xa5;
    eh_context_storage.initialized = 0;
    void* first = eh_context_static();
    if (first != eh_context_storage.block) { std::puts("first return"); ++bad; }
    if (eh_context_storage.initialized != 1) { std::puts("flag not set"); ++bad; }
    for (unsigned index = 0; index < 224; ++index) {
        if (index >= 4 && index < 4 + sizeof(void*)) continue;  // pointer word is checked below
        if (eh_context_storage.block[index] != 0) { std::printf("byte %u not zeroed\n", index); ++bad; break; }
    }
    void* stored = nullptr;
    std::memcpy(&stored, eh_context_storage.block + 4, sizeof stored);
    if (stored != static_cast<void*>(eh_context_storage.target)) { std::puts("pointer word"); ++bad; }
    if (stored != static_cast<void*>(eh_context_storage.block + 232)) { std::puts("pointer offset"); ++bad; }

    // Later call: flag set, so only the address is returned and the block keeps its contents.
    eh_context_storage.block[100] = 0x5a;
    void* second = eh_context_static();
    if (second != first || eh_context_storage.block[100] != 0x5a) { std::puts("second call touched the block"); ++bad; }

    // A cleared flag makes the next call re-initialize.
    eh_context_storage.initialized = 0;
    eh_context_static();
    if (eh_context_storage.block[100] != 0) { std::puts("re-initialization did not zero"); ++bad; }

    if (bad == 0) std::puts("eh_context_static host contract passed");
    return bad;
}
