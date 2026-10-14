// Host check for packetshift.cpp; not an EE plan input.
#include "packetshift.cpp"

int main() {
    if (packetshift != 0) return 1;
    packetshift = 3;
    if (packetshift != 3) return 2;
    return 0;
}
