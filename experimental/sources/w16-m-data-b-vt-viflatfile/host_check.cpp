// Host check for vt_VIFlatFile. Not an EE plan input; it includes the definition file.
#include "vt_VIFlatFile.cpp"
int main() {
    if (sizeof vt_VIFlatFile != 0x18) return 1;
    for (unsigned char byte : vt_VIFlatFile) if (byte != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(vt_VIFlatFile) % 4u != 0) return 3;
    return 0;
}
