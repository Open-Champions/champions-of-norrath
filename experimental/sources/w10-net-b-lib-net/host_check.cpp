// Host check for the libnet storage. Not an EE plan input.
#include "lib_net.cpp"
int main() {
    if (net_buf != nullptr || sLibNetInitialized != 0) return 1;
    if (sizeof sLibNetInitialized != 4) return 2;
    return 0;
}
