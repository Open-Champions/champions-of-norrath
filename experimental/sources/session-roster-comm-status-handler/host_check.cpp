#include "comm_status_handler.cpp"
unsigned commSendmask;
int main() {
    unsigned char packet[1];
    commSendmask = 0x10;
    packet[0] = 0x83;
    commStatusHandler__FPUci(packet, 1);
    if (commSendmask != 0x13u) return 1;
    packet[0] = 0x85;
    commStatusHandler__FPUci(packet, 1);
    if (commSendmask != 0x17u) return 2;
    packet[0] = 0x07;  // clear bits 0..2
    commStatusHandler__FPUci(packet, 1);
    if (commSendmask != 0x10u) return 3;
    commSendmask = 0xffffffffu;
    packet[0] = 0xf8;  // set flag with empty nibble changes nothing
    commStatusHandler__FPUci(packet, 1);
    if (commSendmask != 0xffffffffu) return 4;
    packet[0] = 0x7a;  // clear flag, nibble 0xa; high nibble ignored
    commStatusHandler__FPUci(packet, 1);
    if (commSendmask != 0xfffffff5u) return 5;
    return 0;
}
