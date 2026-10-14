// Host check only; not a build input. The data pointer word is compared by its low 32 bits.
#include "seticon.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>

int main() {
    bool ok = true;
    unsigned char object[1024];
    std::memset(object, 0x6b, sizeof object);
    int marker = 0;
    SetIcon__16VIIconDefinitionPvi(object, &marker, 321);
    unsigned w964, w968;
    std::memcpy(&w964, object + 964, 4);
    std::memcpy(&w968, object + 968, 4);
    ok = ok && w964 == static_cast<unsigned>(reinterpret_cast<unsigned long>(&marker)) && w968 == 321;
    for (int slot : {260, 324, 388}) ok = ok && std::memcmp(object + slot, "static.ico", 11) == 0;
    for (int i = 0; i < 1024; ++i) {
        const bool written = (i >= 260 && i < 271) || (i >= 324 && i < 335) || (i >= 388 && i < 399) || (i >= 964 && i < 972);
        if (!written && object[i] != 0x6b) ok = false;
    }
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
