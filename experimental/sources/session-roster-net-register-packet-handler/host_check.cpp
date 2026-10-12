#include "register_packet_handler.cpp"
extern "C" NetHandler pktHandler[5] = {};
static void a(unsigned char*, int) {}
static void b(unsigned char*, int) {}
static void c(unsigned char*, int) {}
int main() {
    if (netRegisterPacketHandler__FPFPUci_v(a) != 0 || pktHandler[0] != a) return 1;
    if (netRegisterPacketHandler__FPFPUci_v(b) != 1 || pktHandler[1] != b) return 2;
    if (netRegisterPacketHandler__FPFPUci_v(a) != 0) return 3;
    if (netRegisterPacketHandler__FPFPUci_v(b) != 1) return 4;
    pktHandler[0] = 0;  // an earlier hole is filled first
    if (netRegisterPacketHandler__FPFPUci_v(c) != 0 || pktHandler[0] != c) return 5;
    pktHandler[0] = a; pktHandler[1] = b; pktHandler[2] = a; pktHandler[3] = b; pktHandler[4] = 0;
    if (netRegisterPacketHandler__FPFPUci_v(c) != 4 || pktHandler[4] != c) return 6;
    pktHandler[3] = 0; pktHandler[4] = 0;
    if (netRegisterPacketHandler__FPFPUci_v(c) != 3 || pktHandler[3] != c || pktHandler[4] != 0) return 7;
    if (netRegisterPacketHandler__FPFPUci_v(c) != 3) return 8;
    return 0;
}
