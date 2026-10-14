// Host check for vt_BlindingLight. Not an EE plan input; it includes the definition file.
#include "vt_BlindingLight.cpp"
int main() {
    if (sizeof vt_BlindingLight != 0x30) return 1;
    for (unsigned char byte : vt_BlindingLight) if (byte != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(vt_BlindingLight) % 4u != 0) return 3;
    return 0;
}
