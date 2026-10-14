// Host check for vt_RealmGatewayAPI. Not an EE plan input; it includes the definition file.
#include "vt_RealmGatewayAPI.cpp"
int main() {
    if (sizeof vt_RealmGatewayAPI != 0x240) return 1;
    for (unsigned char byte : vt_RealmGatewayAPI) if (byte != 0) return 2;
    if (reinterpret_cast<__UINTPTR_TYPE__>(vt_RealmGatewayAPI) % 4u != 0) return 3;
    return 0;
}
