// Host check for coldSpeck. Not an EE plan input; it includes the definition file.
#include "coldSpeck.cpp"
int main() {
    if (sizeof coldSpeck != 80u) return 1;
    for (unsigned char byte : coldSpeck) if (byte != 0) return 2;
    return 0;
}
