// Host check for harm2. Not an EE plan input; it includes the definition file.
#include "harm2.cpp"
int main() {
    if (sizeof harm2 != 80u) return 1;
    for (unsigned char byte : harm2) if (byte != 0) return 2;
    return 0;
}
