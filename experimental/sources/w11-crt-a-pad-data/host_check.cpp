// Host contract for PadData. Not an EE plan input.
#include <cstdio>
#include "pad_data.cpp"
int main() {
    if (sizeof(PadData) != 256 || (reinterpret_cast<__UINTPTR_TYPE__>(PadData) & 63) != 0) return 1;
    for (unsigned i = 0; i < sizeof(PadData); ++i) if (PadData[i] != 0) return 2;
    std::puts("PadData ok");
    return 0;
}
