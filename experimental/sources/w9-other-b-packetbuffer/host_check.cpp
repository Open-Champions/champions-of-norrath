// Host check for packetbuffer.cpp; not an EE plan input.
#include "packetbuffer.cpp"

int main() {
    for (unsigned i = 0; i < sizeof(packetbuffer); ++i) if (packetbuffer[i] != 0) return 1;
    return 0;
}
