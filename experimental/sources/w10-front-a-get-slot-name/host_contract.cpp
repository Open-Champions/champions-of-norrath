#include "get_slot_name.cpp"

#include <cstdarg>
#include <cstdio>
#include <cstring>

int MCA_Port, MCA_Slot;
static int padMax;
extern "C" int scePadGetSlotMax(int port) { return port == 1 ? padMax : 1; }
extern "C" int sprintf(char* out, const char* format, ...) {
    std::va_list ap;
    __builtin_va_start(ap, format);
    int n = std::vsnprintf(out, 64, format, ap);
    __builtin_va_end(ap);
    return n;
}

int main() {
    char slots[2] = {1, 4};
    MCA_Port = 1; MCA_Slot = 2; padMax = 4;
    if (std::strcmp(getSlotName__FiiPc(-1, -1, nullptr), "2-C") != 0) return 1;
    padMax = 1;
    if (std::strcmp(getSlotName__FiiPc(-1, -1, nullptr), "2") != 0) return 2;
    if (std::strcmp(getSlotName__FiiPc(0, 3, slots), "1") != 0) return 3;
    if (std::strcmp(getSlotName__FiiPc(1, 3, slots), "2-D") != 0) return 4;
    std::strcpy(getSlotName__FiiPc(0, 0, slots), "zz");
    if (getSlotName__FiiPc(0, -1, slots)[0] != 0) return 5;
    if (getSlotName__FiiPc(-1, 2, slots)[0] != 0) return 6;
    MCA_Port = -1;
    if (getSlotName__FiiPc(-1, -1, slots)[0] != 0) return 7;
    std::puts("get slot name passed");
    return 0;
}
