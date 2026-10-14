#include "stub.cpp"
int main() {
    unsigned char obj[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    notifyRemoveInviteRoom__Q212RealmService15RealmGatewayAPIPCUsN21(&obj, (const unsigned short*)0x4321, (const unsigned short*)0x4321, (const unsigned short*)0x4321);
    for (int i = 0; i < 16; ++i) if (obj[i] != i + 1) return 1;
    return 0;
}
