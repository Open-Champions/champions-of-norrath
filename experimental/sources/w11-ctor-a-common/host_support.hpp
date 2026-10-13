// Host-test support for the w11-ctor-a constructors; not an EE input. Objects used by a test
// live in one static arena so the low 32 bits of a host address identify them, like a target
// pointer word. Function words stored in tables are small integers dispatched by the test.
#pragma once
#define W11_HOST 1
#include <cstring>
#include <string>
#include <functional>
#include <map>
#include "ctor.hpp"

namespace w11_host {
alignas(4096) static unsigned char arena[1 << 16];
inline unsigned char* at(unsigned offset) { return arena + offset; }
inline std::map<unsigned, std::function<int(void*, int, int, int)>>& functions() {
    static std::map<unsigned, std::function<int(void*, int, int, int)>> m;
    return m;
}
inline std::string& calls() { static std::string s; return s; }
}  // namespace w11_host

extern "C" unsigned w11_host_word(const void* pointer) {
    return static_cast<unsigned>(reinterpret_cast<unsigned long>(pointer));
}
extern "C" void* w11_host_pointer(unsigned address) {
    return reinterpret_cast<void*>((reinterpret_cast<unsigned long>(w11_host::arena) & ~0xffffffffUL) | address);
}
extern "C" int w11_host_call_int4(unsigned function, void* self, int a, int b, int c) {
    return w11_host::functions().at(function)(self, a, b, c);
}
