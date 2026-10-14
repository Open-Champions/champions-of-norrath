#include "comm_handler.cpp"
static int calls;
static int seen_source;
static unsigned char* seen_packet;
extern "C" void recievePacket__FiPUc(int source, unsigned char* packet) {
    ++calls;
    seen_source = source;
    seen_packet = packet;
}
int main() {
    unsigned char packet[160] = {0};
    packet[132] = 0xc3;
    commHandler__FPUci(packet, 7);
    if (calls != 1) return 1;
    if (seen_source != 0xc3) return 2;
    if (seen_packet != packet) return 3;
    packet[132] = 2;
    commHandler__FPUci(packet, 0);
    if (calls != 2 || seen_source != 2) return 4;
    return 0;
}
