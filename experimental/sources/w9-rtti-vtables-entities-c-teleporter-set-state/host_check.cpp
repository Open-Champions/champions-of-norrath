// Host check only. Not an EE build input.
#include "set_state.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
int main() {
    unsigned char object[64];
    unsigned char copy[64];
    for (unsigned i = 0; i < sizeof object; ++i) copy[i] = object[i] = static_cast<unsigned char>(i * 3 + 1);
    for (int state : {0, 1, -1, 99}) setState__10Teleporteri(object, state);
    const bool ok = std::memcmp(object, copy, sizeof object) == 0;
    std::printf(ok ? "setState__10Teleporteri passed\n" : "failed\n");
    return ok ? 0 : 1;
}
